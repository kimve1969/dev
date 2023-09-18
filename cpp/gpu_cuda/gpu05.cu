/************************************************************************
Copyright:      "Surgutneftegas" PJSC
Autors:         Created by Kim V.E. 2023/08/30
Target:         For education
Annotation:     Matrix |C|=|A|+|B| and |C|=|A|*|B|

*************************************************************************/
#include<iostream>
#include<string>
#include<assert.h>
#include<stdexcept>
#include<omp.h>

enum oper_t
{
        ADD,
        MUL
} _oper_t;

enum prn_t
{
        PRINT,
        NOPRINT
} _prn_t;

__global__ void Create2D(void *p2D, int N, double* p1D)
{
        // index of thread by block
        int ind_tb = threadIdx.x + blockDim.x*threadIdx.y + blockDim.x*blockDim.y*threadIdx.z;
        // index of block by grid
        int ind_bg = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
        // index of tread by grid
        int ind_tg = ind_tb + blockDim.x*blockDim.y*blockDim.z*ind_bg;

        if(ind_tg > N-1)
        {
                return;
        }
		
		 //
        double **p = (double**)p2D;

        p[ind_tg] = &p1D[ind_tg*N];
}

__global__ void A_oper_B(void* p2D_A, void* p2D_B, void* p2D_C, int N, oper_t op = ADD){

        double **A = (double**)p2D_A;
        double **B = (double**)p2D_B;
        double **C = (double**)p2D_C;

        int ind_tb = threadIdx.x + blockDim.x*threadIdx.y + blockDim.x*blockDim.y*threadIdx.z;
        int ind_bg = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
        int ind_tg = ind_tb + blockDim.x*blockDim.y*blockDim.z*ind_bg;

        // recalculate global threads index to i-row an d j-col threads
        int i = ind_tg / N;
        int j = ind_tg % N;

        // check bounders of i & j
        if(i > (N-1) || j > (N-1))
        {
                return;
        }

        // |C| = |A|+|B|
        if(op == ADD){
                C[i][j] = A[i][j] + B[i][j];
        }

        // |C| = |A|*|B|
        if(op == MUL)
        {
                //#pragma unroll
                for(int k=0; k<N; ++k)
                {
                        C[i][j] += A[i][k] * B[k][j];
				}
        }
}

__host__ bool is_int(std::string arg)
{
        try
        {
                const int test{ std::stoi(arg) };
        }
        catch(std::invalid_argument const& ex)
        {
                return false;
        }
        catch(std::out_of_range const& ex)
        {
                return false;
        }

        return true;
}

__host__ int main(int argc, char* argv[]){
        std::cout<<"\nStart\n\n";

        // check arguments
        if( (argc < 3 /* mandatory number arguments  */) ||
            (argc >= 2 && ! is_int( argv[1] ) ) ||
            (argc >= 3 && std::string(argv[2]) != "add" && std::string(argv[2]) != "mul") ||
            (argc >= 4 && std::string(argv[3]) != "print" && std::string(argv[3]) != "noprint" ) ||
            (argc >= 5 && ! is_int( argv[4]) )
          )
        {
                // help
                std::cout<<"Matrix operation: |C|=|A|+|B| or |C|=|A|*|B|\n";
                std::cout<<"Run: "<<argv[0]<<" arg1 arg2 [arg3] [arg4]\n\
Where:\n\
\targ1 - dimention of matrix A,B,C, is mandatoty\n\
\targ2 - [ add | mul ] operation by martix, is mandatory\n\
\targ3 - [ print | noprint ], is optional, default print\n\
\targ4 - number of device (GPU) from 0....(N-1) where N - last number GPU, is optional, default 0 (if use docker, the number begin from 0)\n\
Example:\n"
<<argv[0]<<" 4 add\n"
<<argv[0]<<" 1000 mul noprint\n"
<<argv[0]<<" 3000 mul noprint 2\n";

                std::cout<<"\nEnd\n";

                return -1;
        }

        const int    arg_N{std::stoi(argv[1])};
        const oper_t arg_operation{ std::string( argv[2] ) == "add" ? ADD : MUL };
        const prn_t  arg_print{ argc >= 4 && std::string( argv[3] ) == "print" ? PRINT : NOPRINT /* default print */};
        const int    arg_device{ argc >= 5 ? std::stoi( argv[4] )  : 0 /* default device is 0 */};

        cudaSetDevice( arg_device );

        double t1{0}, t2{0};

        t1 = omp_get_wtime();

        long nelements = arg_N * arg_N;
        std::cout<<"number elements: "<<nelements<<std::endl;

        // 2-х мерный массив в виде массива указателей на одномерный массив, т.к.
        // потом в пакетах BLAS или cuBLAS потребуется передавать именно НЕПРЕРЫВНЫЕ массивы
        // Есть два способа:
        //      1-ый - высчитывать псеводо-индексы 2-х мерного массива
        //      2-ой - создать доплнительный массив указателей
        //      скорее всего 2-ой будет работать быстрее, т.к. исключаются вычисления псевдо-индексов
        double* h_A = new double[nelements];
        double* h_B = new double[nelements];
        double* h_C = new double[nelements];

        for(int i=0; i<nelements; ++i){
                h_A[i] = i*2.1;
                h_B[i] = i*3.1;
                h_C[i] = 0.0;
        }

        double** h_matrix_A = new double*[arg_N];
        double** h_matrix_B = new double*[arg_N];
        double** h_matrix_C = new double*[arg_N];

        for(int i=0; i<arg_N; ++i){
                h_matrix_A[i] = &h_A[i*arg_N];
                h_matrix_B[i] = &h_B[i*arg_N];
                h_matrix_C[i] = &h_C[i*arg_N];
        }

        auto prn = [ &arg_print ](std::string sinfo, double** M, int N){
                if(arg_print != PRINT)
                {
                        return;
                }

                std::cout<<sinfo;

                for(int i=0; i<N; ++i){
                        for(int j=0; j<N; ++j){
                                std::cout<<M[i][j]<<"\t";
                        }
                        std::cout<<"\n";
                }
        };

        prn("\n\n|A|:\n\n", h_matrix_A, arg_N);
        prn("\n\n|B|:\n\n", h_matrix_B, arg_N);

        // CUDA timing
        float ms[10]{0};
		cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        cudaEventRecord(start);
        // Cuda initialization
        int* d_dummy;
        cudaMalloc((void**)&d_dummy, sizeof(double));

        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&ms[0], start, stop);

        // Allocate A,B,C vectors in device memory
        size_t size = nelements * sizeof(double);

        // Create arrays of int
        double* d_A;
        cudaMalloc((void**)&d_A, size);
        double* d_B;
        cudaMalloc(&d_B, size);
        double* d_C;
        cudaMalloc(&d_C, size);

        // Create arrays of pointer on int
        void* d_matrix_A;
        cudaMalloc( (void**)&d_matrix_A, arg_N*sizeof(double*));
        void* d_matrix_B;
        cudaMalloc( (void**)&d_matrix_B, arg_N*sizeof(double*));
        void* d_matrix_C;
        cudaMalloc( (void**)&d_matrix_C, arg_N*sizeof(double*));

        // Link arrays of pointer with array of double
        Create2D<<<dim3((arg_N/1024)+1,1,1), dim3(1024,1,1) >>>(d_matrix_A, arg_N, d_A);
        Create2D<<<dim3((arg_N/1024)+1,1,1), dim3(1024,1,1) >>>(d_matrix_B, arg_N, d_B);
        Create2D<<<dim3((arg_N/1024)+1,1,1), dim3(1024,1,1) >>>(d_matrix_C, arg_N, d_C);

        // Copy A,B vectors from host to device
		cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&ms[1], start, stop);

        // Call kernel
        std::cout<<"\ncomputation on GPU...\n";
        A_oper_B<<< dim3( arg_N*arg_N/1024+1, 1, 1) /* blocks per grid */, dim3(1024,1,1) /* threads per block */>>>(d_matrix_A, d_matrix_B, d_matrix_C, arg_N, arg_operation);

        // synchronize host & device
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&ms[2], start, stop);

        std::cout<<"\ndone\n";

        // Copy C vector from device to host
        cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

        prn("\n\n|C|:\n\n", h_matrix_C, arg_N);

        // Free device resources
        cudaEventDestroy(start);
        cudaEventDestroy(stop);

        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);

        cudaFree(d_matrix_A);
        cudaFree(d_matrix_B);
        cudaFree(d_matrix_C);

        // check last element ...
        auto prn_check = [&](int i, int j)
		{
                std::cout<<"C[ "<<i<<" ][ "<<j<<" ] = "<<h_matrix_C[i][j]<<"\n";
        };

        std::cout<<"\nCheck results:\n";
        prn_check(0, 1);
        prn_check(arg_N-1, arg_N-1);

        // Free host memory
        delete [] h_A;
        delete [] h_B;
        delete [] h_C;

        t2 = omp_get_wtime();

        std::cout<<"\nOMP common time: "<<(t2-t1)<<" sec.\n";

        std::cout<<"CUDA common time: "<<ms[2]<<" msec. (100%),  init: "<<ms[0]<<" msec. ( "<<ms[0]*100.0/ms[2]<<" % ), copy: "<<ms[1]-ms[0]<<" msec. ( "<<(ms[1]-ms[0])*100.0/ms[2]<<" % ), calc: "<<ms[2]-ms[1]<<" msec. ( "<<(ms[2]-ms[1])*100.0/ms[2] <<" % )\n";

        std::cout<<"End\n"<<std::endl;

        return 0;
}
