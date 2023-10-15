#include<iostream>
#include<immintrin.h>

int main(int argc, char* argv[])
{
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
	const int PSIZE = 2;


	int Ksteps = N / PSIZE;
	int Jsteps = N / VSIZE;
	int Ktail  = N % PSIZE;
	int Jtail  = N % VSIZE;

	for(int k = 0; k < Ksteps; ++k)
	{
		for(int j = 0; j < Jsteps; ++j)
		{
			// load
			a0 = _mm_loadu_pd( & A[ j * PSIZE + 0][ k * VSIZE ] );
			a1 = _mm_loadu_pd( & A[ j * PSIZE + 1][ k * VSIZE ] );
			// transponation
			// _mm_set_pd(double e2, double e0)
			ta0 = _mm_set_pd( ((double*)(&a1))[0], ((double*)(&a0))[0] );
			ta1 = _mm_set_pd( ((double*)(&a1))[1], ((double*)(&a0))[1] );
			// store
			_mm_storeu_pd( & B[ k * VSIZE + 0][ j * VSIZE ], ta0 );
			_mm_storeu_pd( & B[ k * VSIZE + 1][ j * VSIZE ], ta1 );
		}
	}

	for(int k = Ksteps * PSIZE; Ktail && (k < N) ; ++k)
	{
		// processing tail
		for(int j = 0; j < Jsteps * VSIZE ; ++j)
		{
			B[j][k] = A[k][j];
		}
	}

	for(int j = Jsteps * VSIZE; Jtail && (j < N) ; ++j)
	{

		for(int k = 0; k < Ksteps * PSIZE ; ++k)
		{
			// processing tail
			B[j][k] = A[k][j];
		}
	}

	for(int k = Ksteps * PSIZE; Ktail && (k < N); ++k)
	{
		for (int j = Jsteps * VSIZE; Jtail && (k < N); ++k )
		{
			B[j][k] = A[k][j];
		}
	}


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
