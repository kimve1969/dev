#include<iostream>
#include<immintrin.h>
#include<vector>

int main(int argc, char* argv[])
{
/*	const int N = 9;

	double A[N][N]{	{1,2,	3,4,	5,6,	7,8,	9},
			{10,11,	12,13,	14,15,	16,17,	18},
			{19,20,	21,22,	23,24,	25,26,	27},
			{28,29,	30,31,	32,33,	34,35,	36},
			{37,38,	39,40,	41,42,	43,44,	45},
			{46,47,	48,49,	50,51,	52,53,	54},
			{55,56,	57,58,	59,60,	61,62,	63},
			{64,65,	66,67,	68,69,	70,71,	72},
			{73,74,	75,76,	77,78,	79,80,	81}};
*/

	const int N = 5;

	double A[N][N]{	{1,2,	3,4,	5},
			{6,7,	8,9,	10},
			{11,12,	13,14,	15},
			{16,17,	18,19,	20},
			{21,22,	23,24,	25}};

/*	const int N = 4;

	double A[N][N]{	{1,2,	3,4},
			{5,6,	7,8},
			{9,10,	11,12},
			{13,14,	15,16}};
*/
	double B[N][N];
	
	__m128d a0, a1;
	__m128d ta0, ta1;

	const int VSIZE = 2;
	int steps = N / VSIZE;
	int tail  = N % VSIZE;

	__m128i vindex = _mm_set_epi64x(  N, 0 );

	const int VAR = 3;

	for(int k = 0; k < steps; ++k)
	{
		for(int j = 0; j < steps; ++j)
		{
			if( VAR == 1 )
			{
				// Version 1 SSE2
				// load
				a0 = _mm_loadu_pd( & A[ j * VSIZE + 0][ k * VSIZE ] );
				a1 = _mm_loadu_pd( & A[ j * VSIZE + 1][ k * VSIZE ] );
				// transponation
				// _mm_set_pd(double e2, double e0)
				ta0 = _mm_unpacklo_pd(a0, a1);	//_mm_set_pd( ((double*)(&a1))[0], ((double*)(&a0))[0] );
				ta1 = _mm_unpackhi_pd(a0, a1);	//_mm_set_pd( ((double*)(&a1))[1], ((double*)(&a0))[1] );
				// store
				_mm_storeu_pd( & B[ k * VSIZE + 0][ j * VSIZE ], ta0 );
				_mm_storeu_pd( & B[ k * VSIZE + 1][ j * VSIZE ], ta1 );
			}
			else if( VAR == 2 )
			{
				// Version 2 AVX512VL
				// load row
				a0 = _mm_loadu_pd( & A[ j * VSIZE + 0][ k * VSIZE ] );
				a1 = _mm_loadu_pd( & A[ j * VSIZE + 1][ k * VSIZE ] );
				// transponation
				_mm_i64scatter_pd( & B[ k * VSIZE ][ j * VSIZE + 0 ] , vindex , a0, 8);
				_mm_i64scatter_pd( & B[ k * VSIZE ][ j * VSIZE + 1 ] , vindex, a1, 8);
			}
			else if( VAR == 3)
			{
				// Version 3 AVX2
				// load by vindex
				a0 = _mm_i64gather_pd( & A[ k * VSIZE ][ j *VSIZE + 0 ], vindex , 8);
				a1 = _mm_i64gather_pd( & A[ k * VSIZE ][ j *VSIZE + 1 ], vindex , 8);
				// transponation
				_mm_storeu_pd( & B[ j * VSIZE + 0 ][ k * VSIZE ], a0);
				_mm_storeu_pd( & B[ j * VSIZE + 1 ][ k * VSIZE ], a1);
			}
		}
	}

	// processing tail
	for(int k = steps * VSIZE; tail && (k < N) ; ++k)
	{
		// processing tail
		for(int j = 0; j < steps * VSIZE ; ++j)
		{
			B[j][k] = A[k][j];
		}
	}
	for(int j = steps * VSIZE; tail && (j < N) ; ++j)
	{
		// processing tail
		for(int k = 0; k < steps * VSIZE ; ++k)
		{
			B[j][k] = A[k][j];
		}
	}
	for(int k = steps * VSIZE; tail && (k < N) ; ++k)
	{
		// processing tail
		for(int j = steps * VSIZE; tail && (j < N); ++j)
		{
			B[j][k] = A[k][j];
		}
	}

	// print
	for(int k=0; k<N; ++k){
		for(int j=0; j<N; ++j)
		{
			std::cout<<"A["<<k<<"]["<<j<<"] = "<<A[k][j]<<"\t";
		}
		std::cout<<"\n";
	}
	std::cout<<"\n";

	for(int k=0; k<N; ++k){
		for(int j=0; j<N; ++j)
		{
			std::cout<<"B["<<k<<"]["<<j<<"]= "<<B[k][j]<<"\t";
		}
		std::cout<<"\n";
	}
	std::cout<<"\n";

	return 0;
}
