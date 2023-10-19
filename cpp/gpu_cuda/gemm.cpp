#include<iostream>
#include<emmintrin.h>
#include<immintrin.h>


int main(int argc, char* argv[])
{
	double A[4][4], transB[4][4], C[4][4];
	for(int i=0; i<4; ++i)
	{
		for(int j=0; j<4; ++j)
		{
			A[i][j] = 1.0;
			transB[i][j] = 1.0;
			C[i][j] = 0.0;
		}
	}

	auto prn = [](std::string sinfo, double M[4][4], int N){
                std::cout<<sinfo;
		for(int i=0; i<N; ++i)
		{
                        for(int j=0; j<N; ++j)
			{
                                std::cout<<M[i][j]<<"\t";
                        }
                        std::cout<<"\n";
                }
        };

	__m256d a0 = _mm256_loadu_pd( & A[0][0] );
	__m256d a1 = _mm256_loadu_pd( & A[1][0] );
	__m256d a2 = _mm256_loadu_pd( & A[2][0] );
	__m256d a3 = _mm256_loadu_pd( & A[3][0] );

	__m256d b0 = _mm256_loadu_pd( & transB[0][0] );
	__m256d b1 = _mm256_loadu_pd( & transB[1][0] );
	__m256d b2 = _mm256_loadu_pd( & transB[2][0] );
	__m256d b3 = _mm256_loadu_pd( & transB[3][0] );

	__m256d c0 = _mm256_loadu_pd( & C[0][0] );
	__m256d c1 = _mm256_loadu_pd( & C[1][0] );
	__m256d c2 = _mm256_loadu_pd( & C[2][0] );
	__m256d c3 = _mm256_loadu_pd( & C[3][0] );


	c0 = _mm256_fmadd_pd( a0, b0, c0 );
	__m256d c01 = 

	prn("|A|:\n", A, 4);
	prn("|B|:\n", transB, 4);
	prn("|C|:\n", C, 4);	
	
	return 0;
}
