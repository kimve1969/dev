/************************************************************************
Copyright:      "Surgutneftegas" PJSC
Autors:         Created by Kim V.E. 2023/08/25
Target:         For education
Annotation:     Timing steps calulation & copy

*************************************************************************/
#include<iostream>
#include<string>
#include<assert.h>
#include<omp.h>


__global__ void VecAdd(int* pA, int* pB, int *pC){
        int id_b = (blockIdx.x + 1) * (blockIdx.y + 1) * (blockIdx.z + 1);
        int id_t = (threadIdx.x + 1) * (threadIdx.y + 1) * (threadIdx.z + 1);
        int indx = (id_b -1) * (blockDim.x * blockDim.y * blockDim.z) + (id_t - 1);
        //printf("%d, ",indx);
        pC[indx] = pA[indx] + pB[indx];
}

/*
   Ким В.Е.
   Аннотация:
*/
__host__ int main(int argc, char* argv[]){
        if(argc < 3){
                std::cout<<"You should run: "<<argv[0]<<" num_blocks num_threads\n\tnum_blocks\tnumber blocks in grid\n\tnum_threads\tnumber threads in block\n";
                return -1;
        }

        std::cout<<"Start"<<std::endl;

        double t0{0}, t1{0}, t2{0}, t3{0};
        const int B{std::stoi(argv[1])}, T{std::stoi(argv[2])};

        dim3 BpG(B,1,1), TpB(T,1,1);
        std::cout<<"grid dimention: "<<BpG.x<<"."<<BpG.y<<"."<<BpG.z<<std::endl;
        std::cout<<"block dimention: "<<TpB.x<<"."<<TpB.y<<"."<<TpB.z<<std::endl;

        long nelements = (BpG.x * BpG.y * BpG.z) * (TpB.x * TpB.y * TpB.z);
        std::cout<<"number elements: "<<nelements<<std::endl;

        int* h_A = new int[nelements];
        int* h_B = new int[nelements];
        int* h_C = new int[nelements];

        for(int i=0; i<nelements; ++i){
                h_A[i] = i*2;
                h_B[i] = i*3;
        }

        // Cuda initialization
        int* d_dummy;
        t0 = omp_get_wtime();
        cudaMalloc((void**)&d_dummy, sizeof(int));

        // Allocate A,B,C vectors in device memory
        size_t size = nelements * sizeof(int);

        t1 = omp_get_wtime();
		
        int* d_A;
        cudaMalloc((void**)&d_A, size);
        int* d_B;
        cudaMalloc(&d_B, size);
        int* d_C;
        cudaMalloc(&d_C, size);

        // Copy A,B vectors from host to device
        cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

        t2 = omp_get_wtime();

        cudaEvent_t start, stop;

        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        cudaEventRecord(start);

        // Call kernel
        std::cout<<"\ncomputation on GPU...\n";
        VecAdd<<< BpG /* blocks per grid */, TpB /* threads per block */>>>(d_A, d_B, d_C);

        cudaEventRecord(stop);
        // synchronize host & device
        cudaEventSynchronize(stop);

        std::cout<<"\ndone\n";

        t3 = omp_get_wtime();
        // Copy C vector from device to host
        cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

        // Free device resources
        float ms_cuda{0};
        cudaEventElapsedTime(&ms_cuda, start, stop);
		
		cudaEventDestroy(start);
        cudaEventDestroy(stop);

        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);

        // check last element ...
        assert((h_A[nelements-1] + h_B[nelements-1]) == h_C[nelements-1]);
        std::cout<<"Check last element A,B,C vectors: A[ "<<(nelements-1)<<" ] + B[ "<<(nelements-1)<<" ] = C[ "<<(nelements-1)<<" ] : "<<h_A[nelements-1]<<" + "<<h_B[nelements-1]<<" = "<<h_C[nelements-1]<<"\n";

        // Free host memory
        delete [] h_A;
        delete [] h_B;
        delete [] h_C;


        std::cout<<"Time initialization CUDA: "<<(t1-t0)<<" sec"<<std::endl;
        std::cout<<"Timing are common: "<<(t3-t1)<<" sec, calculate: "<<(t3-t2)<<" sec ( "<<(t3-t2)*100.0/(t3-t1)<<" % ), copy: "<<(t2-t1)<<" sec ("<<(t2-t1)*100.0/(t3-t1)<<"%)"<<std::endl;
        std::cout<<"\nCuda time calculate: "<<ms_cuda/1000.0<<" sec\n";

        std::cout<<"End"<<std::endl;
        return 0;
}