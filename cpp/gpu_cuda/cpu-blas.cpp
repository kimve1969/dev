/************************************************************************
Copyright:      "Surgutneftegas" PJSC
Autors:         Created by Kim V.E. 2023/09/06
Target:         For education
Annotation:     CPU Matrix |C|=|A|+|B| and |C|=|A|*|B|

*************************************************************************/
#include<iostream>
#include<string>
#include<assert.h>
#include<stdexcept>
#include<omp.h>
#include<thread>
#include<blas.hh>
#include<iomanip>
#include<cstdlib>

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

        double* h_A = new double[nelements];
        double* h_B = new double[nelements];
        double* h_C = new double[nelements];

        for(int i=0; i<nelements; ++i){
                h_A[i] = std::rand() / ( RAND_MAX + 1.0); //1.0;//i*2.1;
                h_B[i] = std::rand() / ( RAND_MAX + 1.0); //1.0;//i*3.1;
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

        std::cout<<"\ncomputation on CPU BLAS...\n";

        t[1] = omp_get_wtime();
	// C = alpha * op(A) * op(B) + beta * C
	// C[m][n], A[m][k], B[k][m]
	blas::gemm(	blas::Layout::RowMajor, // layout	[ ColMajor | RowMajor ]
			blas::Op::NoTrans,	// transA	[ Op::NoTrans | Op::Trans | Op::ConjTrans ]
			blas::Op::NoTrans,	// transB
			arg_N,			// m 
			arg_N,			// n
			arg_N,			// k
			1.0,			// alpha
			h_A,			// if transA = NoTrans: matrix A[m][k]
			arg_N,			// lda, for RowMajor: lda = max(1,k)
			h_B,			// if transB = NoTrans: matrix B[k][n]
			arg_N,			// ldb, for RowMajor: ldb = max(1, n)
			0,			// beta, if beta is zero, C need not be set on input
			h_C,			// C[m][n]
			arg_N			// ldc, for RowMajor: ldc = max(1, n)
		   );

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
        std::cout<<std::setprecision(3)<<"\nOMP calculation time: "<<(t[2]-t[1])<<" sec., GFLOPS = "<<gflops<<"\n";

        std::cout<<"End\n"<<std::endl;

        return 0;
}
