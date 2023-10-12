/************************************************************************
Copyright:      "Surgutneftegas" PJSC
Autors:         Created by Kim V.E. 2023/09/06
Target:         For education
		bases on cpu05.cpp + optimization by used sub-matrixes multiplier
Annotation:     CPU Matrix |C|=|A|+|B| and |C|=|A|*|B|

*************************************************************************/
#include<iostream>
#include<string>
#include<assert.h>
#include<stdexcept>
#include<omp.h>
#include<thread>
#include<cmath>

#include<emmintrin.h>
#include<immintrin.h>

#ifdef __GNUC__
        #define ALIGN(N) __attribute__((aligned(N)))    // Linux
#else
	#define ALIGN(N) __declspec(align(N))           // Windows
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

enum opt_t
{
	NO,
	SSE2,
	AVX,
	AVX512F,
	TRANS,
	TRANS_AND_RED,
	TRANS_AND_SSE2,
	TRANS_AND_AVX,
	TRANS_AND_AVX512F
} _opt_t;

// L1-cash is 32678 byte = 4096 double
// IF 3 sub-matrixes * 1365 double, each of 3 matrix is 36x36 double
// IF 2 sub-matrixes * 2048 double, each of 2 matrix is 45x45 double
// IF 1 sub-matrixes * 4096 double, 1 matrix is 64x64 double
const int CNST_DIM_OF_BLOCK = 45;

// C = Sum ( A * B )
void Asub_mul_Bsub( const double* const * A, const double* const * B, double* const * C, int row_max_C, int col_max_C, int rc_max_AB )
{
	for(int i=0; i < row_max_C ; ++i)
	{
		for(int k=0; k < rc_max_AB ; ++k)
		{
			for(int j=0; j < col_max_C ; ++j)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void Asub_mul_Bsub_opt( const double* const * A, const double* const * B, double* const * C, int row_max_C, int col_max_C, int rc_max_AB, opt_t opt = SSE2 )
{
	for(int i=0; i < row_max_C ; ++i)
	{
		for(int k=0; k < rc_max_AB ; ++k)
		{
			if( opt == SSE2 )
			{
				// SSE vectorization
				const int VECTOR_SIZE = 2;

				ALIGN(16) __m128d a2d = _mm_set1_pd( A[i][k] );
				//ALIGN(16) __m128d b2d;
				//ALIGN(16) __m128d c2d;

				// from 1 to pre-last steps vectorization
				int j = 0;
				for( /* j see below */ ; j < col_max_C ; j += VECTOR_SIZE)
				{
					// B[k][j+1], B[k][j+0] -> b2d 
					//b2d = _mm_loadu_pd( & B[k][j] );
					// C[i][j+1], C[i][j+0] -> c2d 
					//c2d = _mm_loadu_pd( & C[i][j] );
					// FMA, gcc -mfma
					//c2d = _mm_fmadd_pd( a2d, b2d, c2d );
					// c2d -> C[i][j+1], C[i][j+0] 
					//_mm_storeu_pd( & C[i][j], c2d );
					_mm_storeu_pd( & C[i][j], _mm_fmadd_pd( a2d, 
								  _mm_loadu_pd( & B[k][j] ), 
					       		          _mm_loadu_pd( & C[i][j] )));

				}

				// last step vectorization with check bounds rc_max_AB

			}
			else if( opt == AVX )
			{
				// AVX vectorization
				const int VECTOR_SIZE = 4;

				ALIGN(32) __m256d a4d = _mm256_set1_pd( A[i][k] );
				//ALIGN(32) __m256d b4d;
				//ALIGN(32) __m256d c4d;

				// from 1 to pre-last steps vectorization
				int j = 0;
				for( /* j see below */ ; j < col_max_C ; j += VECTOR_SIZE)
				{
					// B[k][j+1], B[k][j+0] -> b2d 
					//b4d = _mm256_loadu_pd( & B[k][j] );
					// C[i][j+1], C[i][j+0] -> c2d 
					//c4d = _mm256_loadu_pd( & C[i][j] );
					// FMA, gcc -mfma
					//c4d = _mm256_fmadd_pd( a4d, b4d, c4d );
					//c2d -> C[i][j+1], C[i][j+0] 
					//_mm256_storeu_pd( & C[i][j], c4d );
					_mm256_storeu_pd( & C[i][j], _mm256_fmadd_pd( a4d, 
								     _mm256_loadu_pd( & B[k][j] ), 
								      _mm256_loadu_pd( & C[i][j] )));
					//_mm256_storeu_pd( & C[i][j+1], _mm256_fmadd_pd( a4d, 
					//				 _mm256_loadu_pd( & B[k][j+1] ), 
					//				 _mm256_loadu_pd( & C[i][j+1] )));

				}
				// last step vectorization with check bounds rc_max_AB
			}
			else if( opt == AVX512F )
			{
				// AVX512F vectorization
				const int VECTOR_SIZE = 8;

				ALIGN(64) __m512d a8d = _mm512_set1_pd( A[i][k] );
				//ALIGN(64) __m512d b8d;
				//ALIGN(64) __m512d c8d;

				// from 1 to pre-last steps vectorization
				int j = 0;
				for( /* j see below */ ; j < col_max_C ; j += VECTOR_SIZE)
				{
					// B[k][j+1], B[k][j+0] -> b2d 
					//b8d = _mm512_loadu_pd( & B[k][j] );
					// C[i][j+1], C[i][j+0] -> c2d 
					//c8d = _mm512_loadu_pd( & C[i][j] );
					// FMA, gcc -mfma
					//c8d = _mm512_fmadd_pd( a8d, b8d, c8d );
					// c2d -> C[i][j+1], C[i][j+0] 
					//_mm512_storeu_pd( & C[i][j], c8d );
					_mm512_storeu_pd( & C[i][j], _mm512_fmadd_pd( a8d, 
								     _mm512_loadu_pd( & B[k][j] ), 
								     _mm512_loadu_pd( & C[i][j] )));

				}

				// last step vectorization with check bounds rc_max_AB
			}
			else
			{
				assert(false);
			}

		}
	}
}

void Asub_mul_transBsub( const double* const * A, const double* const * B, double* const* C, 
			 int row_max_C, int col_max_C, int rc_max_AB, opt_t opt, double* const * B2D /* transpon(B) */ )
{
	// copy from B[k][j] to B2D[j][k], B = trans( B2D )
	for(int k = 0; k < rc_max_AB; ++k)
	{
		for(int j = 0; j < col_max_C; ++j)
		{
			B2D[j][k] = B[k][j];
		}
	}

	// C[i][j] = Sum ( A[i][k] * B[k][j] ) = Sum( A[i][k] * B2D[j][k] )
	for(int i=0; i < row_max_C ; ++i)
	{
		for(int j=0; j < col_max_C ; ++j)
		{
			if( opt == TRANS )
			{
				ALIGN(64) double sum{0};
				for(int k=0; k < rc_max_AB ; ++k)
				{
					sum += A[i][k] * B2D[j][k];
				}
				C[i][j] += sum;
			}
			else if( opt == TRANS_AND_RED )
			{
				// optimization for out-of-order pipepline
				const int PIPELINE = 4;
				ALIGN(64) double sum[PIPELINE]{0.0, 0.0, 0.0, 0.0};

				// reduction from 1 to pre-last step
				int k = 0;
				for( /*k see above*/ ; k < (rc_max_AB - PIPELINE) ; k += PIPELINE)
				{
					// out-of-order pipeline
					for(int p=0; p<PIPELINE; ++p)
					{
						sum[p] += A[i][k+p] * B2D[j][k+p];
					}
				}

				// last step, out-of-order pipeline
				for(int p=0; p < PIPELINE; ++p)
				{
					(k+p < rc_max_AB) ? sum[p] += A[i][k+p] * B2D[j][k+p] : 0.0;
				}

				double agr_sum{0};
				for(int p=0; p < PIPELINE; ++p)
				{
					agr_sum += sum[p];
				}
				C[i][j] += agr_sum;
			}
			else if ( opt == TRANS_AND_SSE2 )
			{
				// SSE vectorization
				const int VECTOR_SIZE = 2;
                        
				ALIGN(64) __m128d c2d = _mm_set_pd(0.0, 0.0);
				ALIGN(64) __m128d a2d;
				ALIGN(64) __m128d b2d;
			
				// from 1 to pre-last steps vectorization
				int k = 0;
				for( /*k see above*/ ; k < (rc_max_AB - VECTOR_SIZE) ; k += VECTOR_SIZE)
				{
					// __m128d _mm_set_pd( double e1, double e0 )
					a2d = _mm_set_pd( A[i][k+1], A[i][k+0] );
					b2d = _mm_set_pd( B2D[j][k+1], B2D[j][k+0] );
					// FMA, gcc -mfma
					c2d = _mm_fmadd_pd( a2d, b2d, c2d );
				}

				// last step vectorization with check bounds rc_max_AB
				a2d = _mm_set_pd( (k+1 < rc_max_AB ? A[i][k+1] : 0) , (k+0 < rc_max_AB ? A[i][k+0] : 0) );
				b2d = _mm_set_pd( (k+1 < rc_max_AB ? B2D[j][k+1] : 0) , (k+0 < rc_max_AB ? B2D[j][k+0] : 0) );
				c2d = _mm_fmadd_pd( a2d, b2d, c2d );
 
				C[i][j] += ((double*)(&c2d))[1] + ((double*)(&c2d))[0];
			}
			else if( opt == TRANS_AND_AVX )
			{
				// AVX, gcc -mavx
				const int VECTOR_SIZE = 4;

				ALIGN(64) __m256d c4d = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
				ALIGN(64) __m256d a4d;
				ALIGN(64) __m256d b4d;

				// from 1 to pre-last steps vectorization
				int k = 0;
				for( /*k see above*/ ; k < (rc_max_AB - VECTOR_SIZE) ; k += VECTOR_SIZE)
				{
					// _m256d _mm256_set_pd( double e3, double e2, double e1, double e0 )
					a4d = _mm256_set_pd( A[i][k+3], A[i][k+2], A[i][k+1], A[i][k+0] );
                                	b4d = _mm256_set_pd( B2D[j][k+3], B2D[j][k+2], B2D[j][k+1], B2D[j][k+0] );
					c4d = _mm256_fmadd_pd( a4d, b4d, c4d );
				}

				// last step vectorization with check bounds rc_max_AB
				a4d = _mm256_set_pd(    (k+3 < rc_max_AB ? A[i][k+3] : 0),
							(k+2 < rc_max_AB ? A[i][k+2] : 0),
							(k+1 < rc_max_AB ? A[i][k+1] : 0),
							(k+0 < rc_max_AB ? A[i][k+0] : 0));

				b4d = _mm256_set_pd(    (k+3 < rc_max_AB ? B2D[j][k+3] : 0),
							(k+2 < rc_max_AB ? B2D[j][k+2] : 0),
							(k+1 < rc_max_AB ? B2D[j][k+1] : 0),
							(k+0 < rc_max_AB ? B2D[j][k+0] : 0) );

				c4d = _mm256_fmadd_pd( a4d, b4d, c4d );

				C[i][j] += ((double*)(&c4d))[3] +  ((double*)(&c4d))[2] +  ((double*)(&c4d))[1] +  ((double*)(&c4d))[0];
			}
			else if( opt == TRANS_AND_AVX512F )
			{
				// AVX512F, gcc -mavx512f
				const int VECTOR_SIZE = 8;

				ALIGN(64) __m512d c8d = _mm512_set_pd(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
				ALIGN(64) __m512d a8d;
				ALIGN(64) __m512d b8d;

				// from 1 to pre-last steps vectorization
				int k = 0;
				for( /*k see above*/ ; k < (rc_max_AB - VECTOR_SIZE) ; k += VECTOR_SIZE)
				{
					// _m512d _mm512_set_pd( double e7, double e6, ... , double e0 )
					a8d = _mm512_set_pd( A[i][k+7], A[i][k+6], A[i][k+5], A[i][k+4], 
							     A[i][k+3], A[i][k+2], A[i][k+1], A[i][k+0] );
                                	b8d = _mm512_set_pd( B2D[j][k+7], B2D[j][k+6], B2D[j][k+5], B2D[j][k+4],
						       	     B2D[j][k+3], B2D[j][k+2], B2D[j][k+1], B2D[j][k+0] );
					c8d = _mm512_fmadd_pd( a8d, b8d, c8d );
				}

				// last step vectorization with check bounds rc_max_AB
				a8d = _mm512_set_pd(    (k+7 < rc_max_AB ? A[i][k+7] : 0),
							(k+6 < rc_max_AB ? A[i][k+6] : 0),
							(k+5 < rc_max_AB ? A[i][k+5] : 0),
							(k+4 < rc_max_AB ? A[i][k+4] : 0),
							(k+3 < rc_max_AB ? A[i][k+3] : 0),
							(k+2 < rc_max_AB ? A[i][k+2] : 0),
							(k+1 < rc_max_AB ? A[i][k+1] : 0),
							(k+0 < rc_max_AB ? A[i][k+0] : 0));

				b8d = _mm512_set_pd(    (k+7 < rc_max_AB ? B2D[j][k+7] : 0),
							(k+6 < rc_max_AB ? B2D[j][k+6] : 0),
							(k+5 < rc_max_AB ? B2D[j][k+5] : 0),
							(k+4 < rc_max_AB ? B2D[j][k+4] : 0),
							(k+3 < rc_max_AB ? B2D[j][k+3] : 0),
							(k+2 < rc_max_AB ? B2D[j][k+2] : 0),
							(k+1 < rc_max_AB ? B2D[j][k+1] : 0),
							(k+0 < rc_max_AB ? B2D[j][k+0] : 0) );

				c8d = _mm512_fmadd_pd( a8d, b8d, c8d );

				C[i][j] += ((double*)(&c8d))[7] +  ((double*)(&c8d))[6] +  ((double*)(&c8d))[5] +  ((double*)(&c8d))[4] +
					   ((double*)(&c8d))[3] +  ((double*)(&c8d))[2] +  ((double*)(&c8d))[1] +  ((double*)(&c8d))[0];
			}
			else
			{
				assert( false);
			}
		}
	}
}

void A_oper_B(const double* const * A, const double* const* B, double* const * C, int N, oper_t op = ADD, int num_omp_threads = 1, opt_t opt = NO){

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
		int dimBlock = N < CNST_DIM_OF_BLOCK ? N : CNST_DIM_OF_BLOCK;
		int dimGrid = N / dimBlock + ( N % dimBlock ? 1 : 0 );

		#pragma omp parallel for num_threads( num_omp_threads ) shared(dimGrid, dimBlock)
		for( int I = 0; I < dimGrid; ++I )
		{
			ALIGN(64) double **Asub = new double*[dimBlock]; 
			ALIGN(64) double **Bsub = new double*[dimBlock];
	       		ALIGN(64) double **Csub = new double*[dimBlock];

			// dublicate for tranponate Bsub
			ALIGN(64) double  *B1D = new double[ CNST_DIM_OF_BLOCK * CNST_DIM_OF_BLOCK ];
			ALIGN(64) double **B2D = new double*[ CNST_DIM_OF_BLOCK ];
	
			for(int row = 0; row < CNST_DIM_OF_BLOCK; ++row)
			{
				B2D[ row ] = & B1D[ row * CNST_DIM_OF_BLOCK ];
			}

			#pragma omp private(Asub, Bsub, Csub, row_max, col_max)
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

				auto rc_max = [&dimGrid, &dimBlock, &N](int Indx, int Bound = CNST_DIM_OF_BLOCK)
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
						Asub[ row ] = const_cast<double *>( &A[ I * dimBlock + row ][ K * dimBlock ] );  
					}

					for(int row = 0; row < dimBlock; ++row )
					{
						Bsub[ row ] = const_cast<double *>( &B[ K * dimBlock + row ][ J * dimBlock ] );
					}
					
					if( opt == NO )
					{
						// Csub[I][J] = Asub[I][K] * Bsub[K][J]
						Asub_mul_Bsub( Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB);
					}
					else if( opt == SSE2 | opt == AVX | opt == AVX512F )
					{
						Asub_mul_Bsub_opt( Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB, opt);
					}
					else
					{
						// Csub[I][J] = Asub[I][K] * trans (Bsub[K][J])
						Asub_mul_transBsub( Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB, opt, B2D);

					}
				}

			}

			delete[] Asub;
			delete[] Bsub;
			delete[] Csub;

			delete[] B1D;
			delete[] B2D;
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
            (argc >= 5 && ! is_int( argv[4] ) /* number of omp threads */) ||
	    (argc >= 6 && std::string(argv[5]) != "no" && 
	     		  std::string(argv[5]) != "trans" &&
			  std::string(argv[5]) != "sse2" && 
			  std::string(argv[5]) != "avx" && 
			  std::string(argv[5]) != "avx512f" && 
	     		  std::string(argv[5]) != "trans+red" &&  
			  std::string(argv[5]) != "trans+sse2" && 
			  std::string(argv[5]) != "trans+avx" &&
			  std::string(argv[5]) != "trans+avx512f" ) 
          )
        {
                // help
                std::cout<<"CPU Matrix operation: |C|=|A|+|B| or |C|=|A|*|B|\n";
                std::cout<<"Run: "<<argv[0]<<" arg1 arg2 [arg3] [arg4] [arg5]\n\
Where:\n\
\targ1 - dimention of matrix A,B,C, is mandatoty\n\
\targ2 - [ add | mul ] operation by martix, is mandatory\n\
\targ3 - [ print | noprint ], is optional, default \"print\"\n\
\targ4 - number of threads, default 1 (you should see available hardware threads)\n\
\targ5 - [ no | sse2 | avx | avx512f | trans | trans+red | trnas+sse2 | trans+avx | trans+avx512f ] optimization, is optional, default \"no\"\n\
Example:\n"
<<argv[0]<<" 4 add\n"
<<argv[0]<<" 5 mul print\n"
<<argv[0]<<" 1000 mul noprint\n"
<<argv[0]<<" 1000 mul noprint 2\n"
<<argv[0]<<" 1000 mul noprint 8 trans\n"
<<argv[0]<<" 1000 mul noprint 8 trans+sse2\n"
<<"\nEnd\n";
                return -1;
        };

        const int    arg_N{std::stoi(argv[1])};
        const oper_t arg_operation{ std::string( argv[2] ) == "add" ? ADD : MUL };
        const prn_t  arg_print{ argc < 4 ? NOPRINT : std::string( argv[3] ) == "print" ? PRINT : NOPRINT /* default print */};
        const int    arg_num_threads{ argc < 5 ? 1 : std::stoi( argv[4] ) };
	const opt_t  arg_optimization{ argc < 6 ? NO : std::string( argv[5] ) == "trans" ? TRANS :
						       std::string( argv[5] ) == "trans+red" ? TRANS_AND_RED :
						       std::string( argv[5] ) == "sse2" ? SSE2 :
						       std::string( argv[5] ) == "avx" ? AVX :
						       std::string( argv[5] ) == "avx512f" ? AVX512F :
						       std::string( argv[5] ) == "trans+sse2" ? TRANS_AND_SSE2 :
						       std::string( argv[5] ) == "trans+avx" ? TRANS_AND_AVX :
						       std::string( argv[5] ) == "trans+avx512f" ? TRANS_AND_AVX512F : NO /* defaulte no optimization */ };

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
        std::cout<<"\ncomputation on CPU...\n";

        t[1] = omp_get_wtime();

        A_oper_B( const_cast<const double **>( h_matrix_A ), 
		  const_cast<const double **>( h_matrix_B ), 
		  h_matrix_C, 
		  arg_N, 
		  arg_operation, 
		  arg_num_threads, 
		  arg_optimization);

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

	double gflops = ( 2 * (double)arg_N * (double)arg_N * (double)arg_N ) / ( t[2] - t[1] ) / 1'000'000'000.0;
        std::cout<<"\nOMP calculation time: "<<(t[2]-t[1])<<" sec., GFLOPS = "<<gflops<<"\n";

        std::cout<<"End\n"<<std::endl;

        return 0;
}
