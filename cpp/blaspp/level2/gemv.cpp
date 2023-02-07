#include<iostream>
#include<blas.hh>
#include<algorithm>

/*
  y = alpha * A * x + beta * y 
  
*/

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";

  int64_t incx{1}, incy{1};
  
  const int M = 4, N = 4;
  const double alpha = 1.0;
  const double A[M * N]{
      1, 2, 3, 4,
      5, 6, 7, 8,
      9, 10, 11, 12,
      13, 14, 15, 16};
  const double x[N]{1, 2, 3, 4};

  const double beta = 1.0;
  double y[N]{0};
  double tmp[N];

  auto prnv = [](const char* str, const double v[], int n){
    std::cout<<str<<": ";
    for(int i=0; i<n; ++i){
      std::cout<<v[i]<<", ";
    }
    std::cout<<"\n";
  };

  
  auto prnm = [](const char*str, const double m[], int n, int lda, blas::Layout layout = blas::Layout::RowMajor /*Layout::ColMajor or Layout::RowMajor*/){
    std::cout<<str<<":\n";
   
    for(int i=0; i<(n*lda); ++i){
      (i % lda == 0) ? std::cout<<"\n" : std::cout;
      std::cout<<"\t"<<m[i];
    }
    std::cout<<"\n";
  };
  
  std::cout<<"blas::gemv()() - Solve the general matrix-vector multipy as:  alpha * A * x + beta *y\n";
  std::cout<<"alpha = "<<alpha<<"\n";
  prnm("matrix A = ", A, M, N);
  prnv("vector x = ", x, N);
  std::cout<<"beta = "<<beta<<"\n";
  prnv("vector y = ", y, N);
 
  std::copy(std::begin(x), std::end(x), std::begin(tmp));
  
  blas::gemv(blas::Layout::RowMajor, // ColMajor, RowMajor
	     blas::Op::NoTrans, // NoTrans - alpha A x + beta y, Trans - alpfa A^T x + beta y, ConjTrans - alpha A^H x + beta y
	     M, // number of rows matrix A, m>0
	     N, // number of cols matrix A, n>=
	     alpha, // scalar alpha
	     A, // matrix A MxN
	     N, // lda, leading dimention of A
	     x, // vector
	     incx, //
	     beta,
	     y,
	     incy
	     );

  prnv("vector y after blas::gemv()", y, N);
  std::cout<<"\n";

  std::cout<<"End\n";
  return 0;
}
