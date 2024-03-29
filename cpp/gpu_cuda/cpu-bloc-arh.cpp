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
#include<iomanip>
#include<cstdlib>
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
	TRANS_AND_AVX512F,
	TRANS_AND_REG
} _opt_t;

// A[M][N]
double norma(const double* const * A, int M, int N)
{
	double sum{0};
	for(int m = 0; m < M; ++m)
	{
		for(int n = 0; n < N; ++n)
		{
			sum += A[m][n];
		}
	}
	return sum;
}

// L1-cash is 32678 byte = 4096 double
// IF 3 sub-matrixes * 1365 double, each of 3 matrix is 36x36 double
// IF 2 sub-matrixes * 2048 double, each of 2 matrix is 45x45 double
// IF 1 sub-matrixes * 4096 double, 1 matrix is 64x64 double
const int CNST_DIM_OF_BLOCK = 48;

// C = Sum ( A * B )
void Asub_mul_Bsub( const double* const * A, const double* const * B, double* const * C, int row_max_C, int col_max_C, int rc_max_AB, opt_t opt)
{
	//std::cout<<"row_max_C = "<<row_max_C<<", col_max_C = "<<col_max_C<<", rc_max_AB = "<<rc_max_AB<<"\n";

	for(int i=0; i < row_max_C ; ++i)
	{
		for(int k=0; k < rc_max_AB ; ++k)
		{
			if( NO == opt)
			{
				for(int j=0; j < col_max_C ; ++j)
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
			else if( SSE2 == opt )
			{
				const int PSIZE = 2; // each core has 2 FMA-units
				const int VSIZE = 2; // length of vector SSE2
				// _mm_set_pd(double e1, double e0)
				ALIGN(16) __m128d a2d = _mm_set_pd( A[i][k], A[i][k] );

				// stepa fma and vectorization
				int steps = col_max_C / ( PSIZE * VSIZE );
				int tail  = col_max_C % ( PSIZE * VSIZE );
		
				for( int j = 0 ; j < steps ; ++j)
				{
					// first FMA-unit
					int jone = (j * PSIZE + 0) * VSIZE;
					_mm_storeu_pd( & C[i][ jone ], _mm_fmadd_pd( a2d,
							          // load B[k][j+1], B[k][j+0]	
								  _mm_loadu_pd( & B[k][ jone  ] ), 
								  // load C[i][j+1], C[i][j+0]
					       		          _mm_loadu_pd( & C[i][ jone ] )));
					// second FMA-unit
					int jtwo = (j * PSIZE + 1) * VSIZE;
					_mm_storeu_pd( & C[i][ jtwo  ], _mm_fmadd_pd( a2d,
								  // load B[k][j+3], B[k][j+2]
								  _mm_loadu_pd( & B[k][ jtwo ] ), 
								  // load C[i][j+3], C[i][j+2]
  					       		          _mm_loadu_pd( & C[i][ jtwo ] )));
				}
				// processing tail
				for( int j = steps * PSIZE * VSIZE ; tail && (j < col_max_C) ; ++j )
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
			else if( AVX == opt )
			{
				const int PSIZE = 2; // each core has 2 FMA-units
				const int VSIZE = 4; // length of vector AVX
				// _mm_set_pd(double e1, double e0)
				ALIGN(32) __m256d a4d = _mm256_set_pd( A[i][k], A[i][k], A[i][k], A[i][k] );

				// steps fma and vectorization
				int steps = col_max_C / ( PSIZE * VSIZE );
				int tail  = col_max_C % ( PSIZE * VSIZE );
		
				for( int j = 0 ; j < steps ; ++j)
				{
					// first FMA-unit
					int jone = (j * PSIZE + 0) * VSIZE;
					_mm256_storeu_pd( & C[i][ jone ], _mm256_fmadd_pd( a4d,
							          	  // load B[k][j+1], B[k][j+0]	
								  	  _mm256_loadu_pd( & B[k][ jone  ] ), 
								  	  // load C[i][j+1], C[i][j+0]
					       		          	  _mm256_loadu_pd( & C[i][ jone ] )));
					// second FMA-unit
					int jtwo = (j * PSIZE + 1) * VSIZE;
					_mm256_storeu_pd( & C[i][ jtwo  ], _mm256_fmadd_pd( a4d,
								  	  // load B[k][j+3], B[k][j+2]
								  	  _mm256_loadu_pd( & B[k][ jtwo ] ), 
								  	  // load C[i][j+3], C[i][j+2]
  					       		          	  _mm256_loadu_pd( & C[i][ jtwo ] )));
				}
				// processing tail
				for( int j = steps * PSIZE * VSIZE ; tail && (j < col_max_C) ; ++j )
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
			else if( AVX512F == opt )
			{
				const int PSIZE = 2; // each core has 2 FMA-units
				const int VSIZE = 8; // length of vector AVX512F
				// _mm_set_pd(double e7, ..., double e0)
				ALIGN(64) __m512d a8d = _mm512_set_pd( A[i][k], A[i][k], A[i][k], A[i][k], A[i][k], A[i][k], A[i][k], A[i][k] );

				// steps fma and vectorization
				int steps = col_max_C / ( PSIZE * VSIZE );
				int tail  = col_max_C % ( PSIZE * VSIZE );
		
				for( int j = 0 ; j < steps ; ++j)
				{
					// first FMA-unit
					int jone = (j * PSIZE + 0) * VSIZE;
					_mm512_storeu_pd( & C[i][ jone ], _mm512_fmadd_pd( a8d,
							          	  // load B[k][j+1], B[k][j+0]	
								  	  _mm512_loadu_pd( & B[k][ jone  ] ), 
								  	  // load C[i][j+1], C[i][j+0]
					       		          	  _mm512_loadu_pd( & C[i][ jone ] )));
					// second FMA-unit
					int jtwo = (j * PSIZE + 1) * VSIZE;
					_mm512_storeu_pd( & C[i][ jtwo  ], _mm512_fmadd_pd( a8d,
								  	  // load B[k][j+3], B[k][j+2]
								  	  _mm512_loadu_pd( & B[k][ jtwo ] ), 
								  	  // load C[i][j+3], C[i][j+2]
  					       		          	  _mm512_loadu_pd( & C[i][ jtwo ] )));
				}
				// processing tail
				for( int j = steps * PSIZE * VSIZE ; tail && (j < col_max_C) ; ++j )
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
			else
			{
				assert( false && "Unknown type of optimization!" );
			}
		}
	}
}

void gemm_avx_trans(const double* const * A, const double * const * transB, double * const *C, int i, int j, int k)
{
	// Intel x86-64 has 16 256-bit registers YMM0-YMM15, each register can store 4 double
	// 16 reg * 4 double = 64 double
	// 64 / 3 matrixes = 21 double
	// sqrt(21) = 4 dimention of each A,B,C small-matrix
	// we will use AVX
	//  A 4x4, B 4x4, C 4x4 = 48 double, 48 / 4 = 12 registers A - 4 reg, B - 4 reg, C - reg
	//
	// Calc: 2*O(n^3) = 2*4^3 = 128 flop, _mm256_fmadd_pd() = 2 * 4 = 8 flop, 128 flop / 8 = 16 fma
	// 
	const int CNST_VEC_AVX = 4;
	/*for(int i=0; i < CNST_VEC_AVX; ++i)
	{
		for(int j=0; j < CNST_VEC_AVX; ++j)
		{
			for(int k=0; k < CNST_VEC_AVX; ++k)
			{
				C[i][j] += A[i][k] * transB[j][k];
			}
		}
	}
	return;*/

	__m256d a0 = _mm256_loadu_pd( & A[0][0] );
	__m256d a1 = _mm256_loadu_pd( & A[1][0] );
	__m256d a2 = _mm256_loadu_pd( & A[2][0] );
	__m256d a3 = _mm256_loadu_pd( & A[3][0] );
	__m256d b0 = _mm256_loadu_pd( & transB[0][0] );
	__m256d b1 = _mm256_loadu_pd( & transB[1][0] );
	__m256d b2 = _mm256_loadu_pd( & transB[2][0] );
	__m256d b3 = _mm256_loadu_pd( & transB[3][0] );

	__m256d c0, c1, c2, c3;

	// calc small-matrixes Creg[i][j] += Areg[i][k] * Breg[j][k], where Breg is transponated
	auto calc_and_set_Creg = [&]( int row )
	{
		double s0 = ((double*)(&c0))[3] + ((double*)(&c0))[2] + ((double*)(&c0))[1] + ((double*)(&c0))[0];
		double s1 = ((double*)(&c1))[3] + ((double*)(&c1))[2] + ((double*)(&c1))[1] + ((double*)(&c1))[0]; 
		double s2 = ((double*)(&c2))[3] + ((double*)(&c2))[2] + ((double*)(&c2))[1] + ((double*)(&c2))[0]; 
		double s3 = ((double*)(&c3))[3] + ((double*)(&c3))[2] + ((double*)(&c3))[1] + ((double*)(&c3))[0];

		__m256d sum = _mm256_add_pd(  _mm256_loadu_pd( & C[ row ][0] ) , _mm256_set_pd(s3, s2, s1, s0)  );	
		_mm256_storeu_pd( & C[ row ][0],  sum );
	};

	// 0-row Creg 
	c0 = _mm256_mul_pd( a0, b0 );
	c1 = _mm256_mul_pd( a0, b1 );
	c2 = _mm256_mul_pd( a0, b2 );
	c3 = _mm256_mul_pd( a0, b3 );
	calc_and_set_Creg(0);
	// 1-row Creg 
	c0 = _mm256_mul_pd( a1, b0 );
	c1 = _mm256_mul_pd( a1, b1 );
	c2 = _mm256_mul_pd( a1, b2 );
	c3 = _mm256_mul_pd( a1, b3 );
	calc_and_set_Creg(1);
	// 0-row Creg 
	c0 = _mm256_mul_pd( a2, b0 );
	c1 = _mm256_mul_pd( a2, b1 );
	c2 = _mm256_mul_pd( a2, b2 );
	c3 = _mm256_mul_pd( a2, b3 );
	calc_and_set_Creg(2);
	// 0-row Creg 
	c0 = _mm256_mul_pd( a3, b0 );
	c1 = _mm256_mul_pd( a3, b1 );
	c2 = _mm256_mul_pd( a3, b2 );
	c3 = _mm256_mul_pd( a3, b3 );
	calc_and_set_Creg(3);
}

void Asub_mul_transBsub( const double* const * A, const double* const * B, double* const* C, 
			 int row_max_C, int col_max_C, int rc_max_AB, opt_t opt, double* const * B2D /* transpon(B) */, int arg_N )
{
	//std::cout<<"row_max_C = "<<row_max_C<<", col_max_C = "<<col_max_C<<", rc_max_AB = "<<rc_max_AB<<"\n";
	// copy from B[k][j] to B2D[j][k], B = trans( B2D )
	const int CNST_VEC_AVX = 4;

	if( !(row_max_C == CNST_DIM_OF_BLOCK && col_max_C == CNST_DIM_OF_BLOCK && rc_max_AB == CNST_DIM_OF_BLOCK) /* small-size and tail-size A,B,C */ )
	{
		for(int k = 0; k < rc_max_AB; ++k)
		{
			for(int j = 0; j < col_max_C; ++j)
			{
				B2D[j][k] = B[k][j];
			}
		}
	}
	/*else // processing full size A,B,C
	{
		__m256d b0, b1, b2, b3;
		__m256d tb0, tb1, tb2, tb3;

		int steps = CNST_DIM_OF_BLOCK / CNST_VEC_AVX;

		for(int k = 0; k < steps; ++k)
		{
			int kindx = k * CNST_VEC_AVX;
			for(int j = 0; j < steps; ++j)
			{
				int jindx = j * CNST_VEC_AVX;
				// load
				b0 = _mm256_loadu_pd( & B[ jindx + 0][ kindx ] );
				b1 = _mm256_loadu_pd( & B[ jindx + 1][ kindx ] );
				b2 = _mm256_loadu_pd( & B[ jindx + 2][ kindx ] );
				b3 = _mm256_loadu_pd( & B[ jindx + 3][ kindx ] );
				// transponation
				// _mm256_set_pd(double e3, ..., double e0)
				tb0 = _mm256_set_pd( ((double*)(&b3))[0], ((double*)(&b2))[0], ((double*)(&b1))[0], ((double*)(&b0))[0] );
				tb1 = _mm256_set_pd( ((double*)(&b3))[1], ((double*)(&b2))[1], ((double*)(&b1))[1], ((double*)(&b0))[1] );
				tb2 = _mm256_set_pd( ((double*)(&b3))[2], ((double*)(&b2))[2], ((double*)(&b1))[2], ((double*)(&b0))[2] );
				tb3 = _mm256_set_pd( ((double*)(&b3))[3], ((double*)(&b2))[3], ((double*)(&b1))[3], ((double*)(&b0))[3] );
				// store
				_mm256_storeu_pd( & B2D[ kindx + 0][ jindx ], tb0 );
				_mm256_storeu_pd( & B2D[ kindx + 1][ jindx ], tb1 );
				_mm256_storeu_pd( & B2D[ kindx + 2][ jindx ], tb2 );
				_mm256_storeu_pd( & B2D[ kindx + 3][ jindx ], tb3 );
			}
		}
	}*/
	else
	{
		__m256d b0, b1, b2, b3;
		__m256i vindex = _mm256_set_epi64x( arg_N *3, arg_N * 2, arg_N, 0 );

		int steps = CNST_DIM_OF_BLOCK / CNST_VEC_AVX;

		for(int k = 0; k < steps; ++k)
		{
			int kindx = k * CNST_VEC_AVX;
			for(int j = 0; j < steps; ++j)
			{
				int jindx = j * CNST_VEC_AVX;

				// load by vindex
				b0 = _mm256_i64gather_pd( & B[ kindx ][ jindx + 0 ], vindex , 8);
				b1 = _mm256_i64gather_pd( & B[ kindx ][ jindx + 1 ], vindex , 8);
				b2 = _mm256_i64gather_pd( & B[ kindx ][ jindx + 2 ], vindex , 8);
				b3 = _mm256_i64gather_pd( & B[ kindx ][ jindx + 3 ], vindex , 8);
				// transponation
				_mm256_storeu_pd( & B2D[ jindx + 0 ][ kindx ], b0);
				_mm256_storeu_pd( & B2D[ jindx + 1 ][ kindx ], b1);
				_mm256_storeu_pd( & B2D[ jindx + 2 ][ kindx ], b2);
				_mm256_storeu_pd( & B2D[ jindx + 3 ][ kindx ], b3);
			}
		}

	}

	// Multiple on registers YMM0-YMM15
	if( opt == TRANS_AND_REG && ( row_max_C == CNST_DIM_OF_BLOCK && col_max_C == CNST_DIM_OF_BLOCK && rc_max_AB == CNST_DIM_OF_BLOCK ) )
	{
		assert( (CNST_DIM_OF_BLOCK % CNST_VEC_AVX) == 0 );

		const int steps = CNST_DIM_OF_BLOCK / CNST_VEC_AVX;

		ALIGN(32) double* Areg[ CNST_VEC_AVX ];
		ALIGN(32) double* Breg[ CNST_VEC_AVX ];
		ALIGN(32) double* Creg[ CNST_VEC_AVX ];

		for(int i=0; i < steps; ++i)
		{
			for(int j=0; j < steps; ++j)
			{
				// Rebuild Creg
				for(int row=0; row < CNST_VEC_AVX; ++row)
				{
					Creg[ row ] = & C[ i * CNST_VEC_AVX + row ][ j * CNST_VEC_AVX ];
				}
				// Creg[i][j] += Areg[i][k] * Breg[j][k], where Breg is transponated
				for(int k=0; k < steps; ++k)
				{
					// Rebuild Areg, Breg
					for(int row=0; row < CNST_VEC_AVX; ++row)
					{
						Areg[ row ] = const_cast<double *>( & A[ i * CNST_VEC_AVX + row ][ k * CNST_VEC_AVX ] );
						Breg[ row ] = const_cast<double *>( & B2D[ j * CNST_VEC_AVX + row ][ k * CNST_VEC_AVX ] );
					}	
					// calc Creg[i][j] 
					gemm_avx_trans( Areg, Breg, Creg , i, j, k);
				}
			}
		}

		return;
	}
	else if ( opt == TRANS_AND_REG && !( row_max_C == CNST_DIM_OF_BLOCK && col_max_C == CNST_DIM_OF_BLOCK && rc_max_AB == CNST_DIM_OF_BLOCK ) )
	{
		// change algoritm for no full-size matrix and continue ...
		opt = TRANS_AND_AVX;
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
			else if ( opt == TRANS_AND_SSE2 )
			{
				const int PSIZE = 2; // each core has 2 FMA-units
				const int VSIZE = 2; // length of vector SSE2
				ALIGN(16) __m128d c2d_one = _mm_setzero_pd();
				ALIGN(16) __m128d c2d_two = _mm_setzero_pd();

				// steps fma and vectorization
				int steps = rc_max_AB / ( PSIZE * VSIZE );
				int tail  = rc_max_AB % ( PSIZE * VSIZE );
		
				for( int k = 0 ; k < steps ; ++k)
				{
					// first FMA-unit
					int kone = (k * PSIZE + 0) * VSIZE;
					//			     load A[i][k+1], A[i][k+0]
					c2d_one =  _mm_fmadd_pd( _mm_loadu_pd( & A[i][ kone ] ),
							          // load B2D[j][k+1], B2D[j][k+0]	
								  _mm_loadu_pd( & B2D[j][ kone ] ), 
					       		          c2d_one );
					// second FMA-unit
					int ktwo = (k * PSIZE + 1) * VSIZE;
					//			    load A[i][k+3], A[i][k+2]
					c2d_two =  _mm_fmadd_pd( _mm_loadu_pd( &A[i][ ktwo ]  ),
						   	         // load B2D[j][k+3], B2D[j][k+2]
								 _mm_loadu_pd( & B2D[j][ ktwo ] ), 
  					       		         c2d_two);
				}
				// processing tail
				double sum{0};
				for( int k = steps * PSIZE * VSIZE ; tail && (k < rc_max_AB) ; ++k )
				{
					sum += A[i][k] * B2D[j][k];
				}
				// gather sum 
				sum += ((double*)(&c2d_one))[1] + ((double*)(&c2d_one))[0] +
				       ((double*)(&c2d_two))[1] + ((double*)(&c2d_two))[0];
				C[i][j] += sum;

			}
			else if( opt == TRANS_AND_AVX )
			{
				const int PSIZE = 2; // each core has 2 FMA-units
				const int VSIZE = 4; // length of vector AVX
				ALIGN(32) __m256d c4d_one = _mm256_setzero_pd();
				ALIGN(32) __m256d c4d_two = _mm256_setzero_pd();

				// steps fma and vectorization
				int steps = rc_max_AB / ( PSIZE * VSIZE );
				int tail  = rc_max_AB % ( PSIZE * VSIZE );
		
				for( int k = 0 ; k < steps ; ++k)
				{
					// first FMA-unit
					int kone = (k * PSIZE + 0) * VSIZE;
					//			     load A[i][k+1], A[i][k+0]
					c4d_one =  _mm256_fmadd_pd( _mm256_loadu_pd( & A[i][ kone ] ),
							            // load B2D[j][k+1], B2D[j][k+0]	
								    _mm256_loadu_pd( & B2D[j][ kone ] ), 
					       		            c4d_one );
					// second FMA-unit
					int ktwo = (k * PSIZE + 1) * VSIZE;
					//			    load A[i][k+3], A[i][k+2]
					c4d_two =  _mm256_fmadd_pd( _mm256_loadu_pd( &A[i][ ktwo ]  ),
						   	            // load B2D[j][k+3], B2D[j][k+2]
								    _mm256_loadu_pd( & B2D[j][ ktwo ] ), 
  					       		            c4d_two);
				}
				// processing tail
				double sum{0};
				for( int k = steps * PSIZE * VSIZE ; tail && (k < rc_max_AB) ; ++k )
				{
					sum += A[i][k] * B2D[j][k];
				}
				// gather sum 
				sum += ((double*)(&c4d_one))[3] + ((double*)(&c4d_one))[2] + ((double*)(&c4d_one))[1] + ((double*)(&c4d_one))[0] +
				       ((double*)(&c4d_two))[3] + ((double*)(&c4d_two))[2] + ((double*)(&c4d_two))[1] + ((double*)(&c4d_two))[0];

				C[i][j] += sum;
			}
			else if( opt == TRANS_AND_AVX512F )
			{
				const int PSIZE = 2; // each core has 2 FMA-units
				const int VSIZE = 8; // length of vector AVX512F
				// _mm_set_pd(double e1, double e0)
				ALIGN(64) __m512d c8d_one = _mm512_setzero_pd();
				ALIGN(64) __m512d c8d_two = _mm512_setzero_pd();

				// steps fma and vectorization
				int steps = rc_max_AB / ( PSIZE * VSIZE );
				int tail  = rc_max_AB % ( PSIZE * VSIZE );
		
				for( int k = 0 ; k < steps ; ++k)
				{
					// first FMA-unit
					int kone = (k * PSIZE + 0) * VSIZE;
					//			     load A[i][k+1], A[i][k+0]
					c8d_one =  _mm512_fmadd_pd( _mm512_loadu_pd( & A[i][ kone ] ),
							            // load B2D[j][k+1], B2D[j][k+0]	
								    _mm512_loadu_pd( & B2D[j][ kone ] ), 
					       		            c8d_one );
					// second FMA-unit
					int ktwo = (k * PSIZE + 1) * VSIZE;
					//			    load A[i][k+3], A[i][k+2]
					c8d_two =  _mm512_fmadd_pd( _mm512_loadu_pd( &A[i][ ktwo ]  ),
						   	            // load B2D[j][k+3], B2D[j][k+2]
								    _mm512_loadu_pd( & B2D[j][ ktwo ] ), 
  					       		            c8d_two);
				}
				// processing tail
				double sum{0};
				for( int k = steps * PSIZE * VSIZE ; tail && (k < rc_max_AB) ; ++k )
				{
					sum += A[i][k] * B2D[j][k];
				}
				// gather sum 
				sum += ((double*)(&c8d_one))[7] + ((double*)(&c8d_one))[6] + ((double*)(&c8d_one))[5] + ((double*)(&c8d_one))[4] +
				       ((double*)(&c8d_one))[3] + ((double*)(&c8d_one))[2] + ((double*)(&c8d_one))[1] + ((double*)(&c8d_one))[0] +
				       ((double*)(&c8d_two))[7] + ((double*)(&c8d_two))[6] + ((double*)(&c8d_two))[5] + ((double*)(&c8d_two))[4] + 
				       ((double*)(&c8d_two))[3] + ((double*)(&c8d_two))[2] + ((double*)(&c8d_two))[1] + ((double*)(&c8d_two))[0];

				C[i][j] += sum;
			}
			else
			{
				assert( false && "Unknown type of optimization!");
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
				
					// Asub_mul_Bsub( Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB);
					if( NO == opt || SSE2 == opt || AVX == opt || AVX512F == opt )
					{
						// Csub[I][J] = Asub[I][K] * Bsub[K][J]
						Asub_mul_Bsub( Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB, opt);
					}
					else if( TRANS == opt || TRANS_AND_RED == opt || TRANS_AND_SSE2 == opt || TRANS_AND_AVX == opt || TRANS_AND_AVX512F == opt ||
						 TRANS_AND_REG == opt )
					{
						// Csub[I][J] = Asub[I][K] * trans (Bsub[K][J])
						Asub_mul_transBsub( Asub, Bsub, Csub, row_max_C, col_max_C, rc_max_AB, opt, B2D, N);
					}
					else
					{
						assert( false && "Unknown type of optimization!"  );
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
  			  std::string(argv[5]) != "sse2" && 
			  std::string(argv[5]) != "avx" && 
			  std::string(argv[5]) != "avx512f" &&
			  std::string(argv[5]) != "trans" &&
			  std::string(argv[5]) != "trans+sse2" && 
			  std::string(argv[5]) != "trans+avx" &&
			  std::string(argv[5]) != "trans+avx512f" &&
			  std::string(argv[5]) != "trans+reg") 
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
\targ5 - [ no | sse2 | avx | avx512f | trans | trnas+sse2 | trans+avx | trans+avx512f ] optimization, is optional, default \"no\"\n\
Example:\n"
<<argv[0]<<" 4 add\n"
<<argv[0]<<" 5 mul print\n"
<<argv[0]<<" 1000 mul noprint\n"
<<argv[0]<<" 1000 mul noprint 2\n"
<<argv[0]<<" 1000 mul noprint 1 sse2\n"
<<argv[0]<<" 1000 mul noprint 8 trans\n"
<<argv[0]<<" 1000 mul noprint 8 trans+sse2\n"
<<"\nEnd\n";
                return -1;
        };

        const int    arg_N{std::stoi(argv[1])};
        const oper_t arg_operation{ std::string( argv[2] ) == "add" ? ADD : MUL };
        const prn_t  arg_print{ argc < 4 ? NOPRINT : std::string( argv[3] ) == "print" ? PRINT : NOPRINT /* default print */};
        const int    arg_num_threads{ argc < 5 ? 1 : std::stoi( argv[4] ) };
	const opt_t  arg_optimization{ argc < 6 ? NO : std::string( argv[5] ) == "no" ? NO :
 					 	       std::string( argv[5] ) == "sse2" ? SSE2 :
						       std::string( argv[5] ) == "avx" ? AVX :
						       std::string( argv[5] ) == "avx512f" ? AVX512F :
						       std::string( argv[5] ) == "trans" ? TRANS :
						       std::string( argv[5] ) == "trans+sse2" ? TRANS_AND_SSE2 :
						       std::string( argv[5] ) == "trans+avx" ? TRANS_AND_AVX :
						       std::string( argv[5] ) == "trans+avx512f" ? TRANS_AND_AVX512F :
						       std::string( argv[5] ) == "trans+reg" ? TRANS_AND_REG : NO /* defaulte no optimization */ };

        double t[10];

        t[0] = omp_get_wtime();

	long nelements = arg_N * arg_N;

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
                h_A[i] = std::rand() / ( RAND_MAX + 1.0 );//1.0;//i*2.1;
                h_B[i] = std::rand() / ( RAND_MAX + 1.0 );//1.0;//i*3.1;
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

	// check norma
	double norma_C = norma( h_matrix_C, arg_N, arg_N);
	std::cout<<"Norma |C| = "<<std::setprecision(15)<<norma_C<<"\n";
        // Free host memory
	delete [] h_matrix_A;
	delete [] h_matrix_B;
	delete [] h_matrix_C;

        delete [] h_A;
        delete [] h_B;
	delete [] h_C;

        t[3] = omp_get_wtime();

	double gflops = ( 2 * (double)arg_N * (double)arg_N * (double)arg_N ) / ( t[2] - t[1] ) / 1'000'000'000.0;
        std::cout<<std::setprecision(3)<<"\nCalculation time: "<<(t[2]-t[1])<<" sec., GFLOPS = "<<gflops<<"\n";

        std::cout<<"End\n"<<std::endl;

        return 0;
}
