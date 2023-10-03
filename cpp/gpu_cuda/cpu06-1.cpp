/************************************************************************
Copyright:      "Surgutneftegas" PJSC
Autors:         Created by Kim V.E. 2023/09/06
Target:         For education
		used optimization cpu06.cpp + ALIGN(64) + unrooling loop
Annotation:     CPU Matrix |C|=|A|+|B| and |C|=|A|*|B|

*************************************************************************/
#include<iostream>
#include<string>
#include<assert.h>
#include<stdexcept>
#include<omp.h>
#include<thread>
#include<cmath>

#ifdef __GNUC__
	#define ALIGN(N) __attribute__((aligned(N))) 	// Linux
#else
	#define ALIGN(N) __declspec(align(N)) 		// Windows
#endif

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

// L1-cash is 32678 byte = 4096 double = 3 sub-matrixes * 1365 byte, each matrix is 36x36 doubles
const int CNST_SIZE_OF_BLOCK = 36;

void Asub_mul_Bsub( double** A, double** B, double** C, int row_max_C, int col_max_C, int rc_max_AB )
{
	for(int i=0; i < row_max_C ; ++i)
	{	
		for( int k=0; k < rc_max_AB ; ++k )
		{
			const int PIPELINE = 4;

			for(int j=0; j < col_max_C ; j+=PIPELINE)
			{
				(j+0 < col_max_C) ? C[i][j + 0] += A[i][k] * B[k][j + 0] : 0;
				(j+1 < col_max_C) ? C[i][j + 1] += A[i][k] * B[k][j + 1] : 0;
				(j+2 < col_max_C) ? C[i][j + 2] += A[i][k] * B[k][j + 2] : 0;
				(j+3 < col_max_C) ? C[i][j + 3] += A[i][k] * B[k][j + 3] : 0;
				//C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void A_oper_B(double** A, double** B, double** C, int N, oper_t op = ADD, int num_omp_threads = 1){

        // |C| = |A|+|B|
        if(op == ADD){
                #pragma omp parallel for num_threads( num_omp_threads )
                for(int i=0; i<N; ++i){
                        for(int j=0; j<N; ++j){
                                C[i][j] = A[i][j] + B[i][j];
                        }
                }
        }

        // |C| = |A|*|B|
        if(op == MUL)
        {
		int dimBlock = N < CNST_SIZE_OF_BLOCK ? N : CNST_SIZE_OF_BLOCK;
		int dimGrid = N / dimBlock + ( N % dimBlock ? 1 : 0 );

		#pragma omp parallel for num_threads( num_omp_threads ) shared(dimGrid, dimBlock) 		
		for( int I = 0; I < dimGrid; ++I )
		{
			double **Asub = new double*[dimBlock]; 
			double **Bsub = new double*[dimBlock];
	       		double **Csub = new double*[dimBlock];
			
			#pragma omp private(Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB)
			for( int J = 0; J < dimGrid; ++J  )
			{
				
				/*
		 		 See above Asub_mul_Bsub
		 								     col max |
											     V
		
				| 1 1 1 |					| 1 1 |  | 1 X |
				| 1 1 1 | if dimBlock = 2, we have 4 sub-matrix | 1 1 |  | 1 X |
				| 1 1 1 |
										| 1 1 |	 | 1 X |
								row max -->	| X X |	 | X X |
		 		
				Csub[N-1, N-1] 	limited C row_max & col_max
				Asub[N-1, J] 	limited A row_max
				Bsub[I, N-1] 	limited B col_max

				Example: row_max = 37%36 = 1 => see Asub_mul_Bsub(..., row_max) => for(int i=0; i<row_max; ++i) 
				*/

				auto rc_max = [&dimGrid, &dimBlock, &N](int Indx, int Bound = CNST_SIZE_OF_BLOCK)
				{
					return (N % Bound) && (Indx == (dimGrid - 1) ) ? (N % Bound) : dimBlock; 
				};

				int row_max_C = rc_max(I);
				int col_max_C = rc_max(J);

				// Rebuild Csub-array of pointers to rows C-array
				for(int row = 0; row < row_max_C ; ++row )
				{
					Csub[ row ] = &C[ I * dimBlock + row ][ J * dimBlock ];
				}

				// Calculate Csub[I][J]
				for( int K = 0; K < dimGrid; ++K )
				{
					int rc_max_AB = rc_max(K);

					// Rebuild Asub, Bsub arrays of pointers to rows A,B
					for(int row = 0; row < rc_max_AB ; ++row )
					{
						Asub[ row ] = &A[ I * dimBlock + row ][ K * dimBlock ];  
					}

					for(int row = 0; row < dimBlock; ++row )
					{
						Bsub[ row ] = &B[ K * dimBlock + row ][ J * dimBlock ];
					}
					
					// Csub[I][J] = Asub[I][K] * Bsub[K][J];
					Asub_mul_Bsub( Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB);
				}

			}

			delete[] Asub;
			delete[] Bsub;
			delete[] Csub;

		}

	}
}

bool is_int(std::string arg)
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

 int main(int argc, char* argv[]){
        std::cout<<"\nStart\n\n";

        // check arguments
        if( (argc < 3 /* mandatory number arguments  */) ||
            (argc >= 2 && ! is_int( argv[1] ) ) ||
            (argc >= 3 && std::string(argv[2]) != "add" && std::string(argv[2]) != "mul") ||
            (argc >= 4 && std::string(argv[3]) != "print" && std::string(argv[3]) != "noprint" ) ||
            (argc >= 5 && ! is_int( argv[4] ) /* number of omp threads */)
          )
        {
                // help
                std::cout<<"CPU Matrix operation: |C|=|A|+|B| or |C|=|A|*|B|\n";
                std::cout<<"Run: "<<argv[0]<<" arg1 arg2 [arg3] [arg4]\n\
Where:\n\
\targ1 - dimention of matrix A,B,C, is mandatoty\n\
\targ2 - [ add | mul ] operation by martix, is mandatory\n\
\targ3 - [ print | noprint ], is optional, default print\n\
\targ4 - number of threads, default 1 (you should see available hardware threads)\n\
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
        const int    arg_num_threads{ argc>=5 && is_int( argv[4] ) ? std::stoi( argv[4] ) : 1  };

        double t[10];

        t[0] = omp_get_wtime();

		long nelements = arg_N * arg_N;
        std::cout<<"number elements: "<<nelements<<std::endl;

        // 2-х мерный массив в виде массива указателей на одномерный массив, т.к.
        // потом в пакетах BLAS или cuBLAS потребуется передавать именно НЕПРЕРЫВНЫЕ массивы
        // Есть два способа:
        //      1-ый - высчитывать псеводо-индексы 2-х мерного массива
        //      2-ой - создать доплнительный массив указателей
        //      скорее всего 2-ой будет работать быстрее, т.к. исключаются вычисления псевдо-индексов
        ALIGN(64) double* h_A = new double[nelements];
        ALIGN(64) double* h_B = new double[nelements];
        ALIGN(64) double* h_C = new double[nelements];

        for(int i=0; i<nelements; ++i){
                h_A[i] = i*2.1;
                h_B[i] = i*3.1;
                h_C[i] = 0.0;
        }

        ALIGN(64) double** h_matrix_A = new double*[arg_N];
        ALIGN(64) double** h_matrix_B = new double*[arg_N];
        ALIGN(64) double** h_matrix_C = new double*[arg_N];

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

        std::cout<<"number available threads is "<<std::thread::hardware_concurrency()<<"\n";
        std::cout<<"\ncomputation on GPU...\n";

        t[1] = omp_get_wtime();
        A_oper_B(h_matrix_A, h_matrix_B, h_matrix_C, arg_N, arg_operation, arg_num_threads);
        t[2] = omp_get_wtime();

        std::cout<<"\ndone\n";

        prn("\n\n|C|:\n\n", h_matrix_C, arg_N);

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

        t[3] = omp_get_wtime();

        std::cout<<"\nOMP common time: "<<(t[2]-t[1])<<" sec.\n";

        std::cout<<"End\n"<<std::endl;

        return 0;
}
