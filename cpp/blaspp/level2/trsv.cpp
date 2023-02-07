#include<iostream>
#include<blas.hh>
#include<algorithm>

/*
  The equation Ax=b I have checked in Octave programm as:

  A=[1,2,3; 0,4,5; 0,0,6]
  b=[1;2;3]
  x=A\b   # or x=inv(A)*b
  x = 
     -0.2500
     -0.1250
      0.5000
 */

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";

  int64_t incx{1};
  
  const int N = 3;
  double UA[N * N]{1, 2, 3,
                   0, 4, 5,
		   0, 0, 6},
        U1A[N * N]{1, 2, 3,
                   0, 1, 4,
	           0, 0, 1},
         LA[N * N]{1, 0, 0,
                   2, 3, 0,
	           4, 5, 6},
        L1A[N * N]{1, 0, 0,
                   2, 1, 0,
	           3, 4, 1};
  const double x[N]{1, 2, 3};
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
  
  std::cout<<"blas::trsv() - Solve the triangl matrix-vector equation\n";
  std::cout<<"\t\top(A)x=b,\n";
  std::cout<<"\t\twhere op(A) is one of op(A)=A, op(A)=A^T, or op(A)=A^H, x and b are vectors, and A is an n-by-n, unit or non-unit, upper or lower triangular matrix.\n";
  std::cout<<"\t\tNo test for singularity or near-singularity is included in this routine. Such tests must be performed before calling this routine.\n";
  
  prnm("matrix UA", UA, N, N);

  std::copy(std::begin(x), std::end(x), std::begin(tmp));
  prnv("vector x", tmp, N);

  
  blas::trsv(blas::Layout::RowMajor, // ColMajor, RowMajor
	     blas::Uplo::Upper, // Lower triangular, Upper
	     blas::Op::NoTrans, // NoTrans - Ax=b, Trans - A^T*x=b, ConjTrans - A^H*x=b
	     blas::Diag::NonUnit, // Unit, NonUnit
	     N, // Number of rows or columns of matrix A
	     UA,
 	     N, // Leading dimension of A. lda >= max(1, n)
	     tmp,
	     incx
	     );

  prnv("vector x after blas::trsv()", tmp, N);
  std::cout<<"\n";
  
  prnm("matrix U1A", U1A, N, N);
  std::copy(std::begin(x), std::end(x), std::begin(tmp));
  prnv("vector x", tmp, N);

  blas::trsv(blas::Layout::RowMajor, // ColMajor, RowMajor
	     blas::Uplo::Upper, // Lower triangular, Upper
	     blas::Op::NoTrans, // NoTrans - Ax=b, Trans - A^T*x=b, ConjTrans - A^H*x=b
	     blas::Diag::Unit, // Unit, NonUnit
	     N, // Number of rows or columns of matrix A
	     U1A,
 	     N, // Leading dimension of A. lda >= max(1, n)
	     tmp,
	     incx
	     );
  
  prnv("vector x after blas::trsv()", tmp, N);
  std::cout<<"\n";
  
  prnm("matrix LA", LA, N, N);
  std::copy(std::begin(x), std::end(x), std::begin(tmp));
  prnv("vector x", tmp, N);

  blas::trsv(blas::Layout::RowMajor, // ColMajor, RowMajor
	     blas::Uplo::Lower, // Lower triangular, Upper
	     blas::Op::NoTrans, // NoTrans - Ax=b, Trans - A^T*x=b, ConjTrans - A^H*x=b
	     blas::Diag::NonUnit, // Unit, NonUnit
	     N, // Number of rows or columns of matrix A
	     LA,
 	     N, // Leading dimension of A. lda >= max(1, n)
	     tmp,
	     incx
	     );
  
  prnv("vector x after blas::trsv()", tmp, N);
  std::cout<<"\n";
  
  prnm("matrix L1A", L1A, N, N);
  std::copy(std::begin(x), std::end(x), std::begin(tmp));
  prnv("vector x", tmp, N);

  blas::trsv(blas::Layout::RowMajor, // ColMajor, RowMajor
	     blas::Uplo::Lower, // Lower triangular, Upper
	     blas::Op::NoTrans, // NoTrans - Ax=b, Trans - A^T*x=b, ConjTrans - A^H*x=b
	     blas::Diag::Unit, // Unit, NonUnit
	     N, // Number of rows or columns of matrix A
	     L1A,
 	     N, // Leading dimension of A. lda >= max(1, n)
	     tmp,
	     incx
	     );
  
  prnv("vector x after blas::trsv()", tmp, N);
  std::cout<<"\n";
  
  std::cout<<"\nEnd\n";
  return 0;
}
