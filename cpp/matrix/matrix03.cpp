#include<iostream>
#include<chrono>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<omp.h>

auto t_now(){ return std::chrono::high_resolution_clock::now(); }
auto t_diff(const std::chrono::time_point<std::chrono::high_resolution_clock> &start,
	    const std::chrono::time_point<std::chrono::high_resolution_clock> &end){
  std::chrono::duration<double> diff = end - start;
  return diff.count();
}

#ifdef __GNUC__
  #define ALIGN(N) __attribute__((aligned(N))) // Linux
#else
  #define ALIGN(N) __declspec(align(N)) // Windows
#endif

int main(int argc, char** argv){
  std::cout<<"Start.\n";

  std::cout<<"|A|*|B|=|C|, C[k,m] = SUM( A[k,l]*B[l,m])\n";
  // Intel
  // size of cash 1 - level is 32 kB, 32'768 B / 8 B = 4'096 double elements,
  // 4'096 elem / 3 (matrix A,B,C) = 1'365 size of each matrix,
  // sqrt(1'365) = 37 elem. is size of side matrix

  // ARM
  // size of cash 1 - level is 64 kB, 65'536 B / 8 B = 8'192 double elements,
  // 8'192 elem / 3 (matrix A,B,C) = 2'730 size of each matrix,
  // sqrt(2'730) = 52 elem. is size of side matrix
  
  // Intel   N = 37, P = 27
  // ARM     N = 52, P = 19
  
#ifdef __x86_64__ // Intel x86
  const int N = 37;
  const int P = 27;
#elif __aarch64__ // ARM 64
  const int N = 52;
  const int P = 19;
#else
  // here is compilation error - undefined variables N,P
#endif
  
  constexpr int n{P*N}; // 27 * 37 = 999
  const int K{n}, L{n}, M{n};
  
  auto A = new double[K][L];
  auto B = new double[L][M];
  auto C = new double[K][M];
    
  std::srand(std::time(nullptr));

  for(int k=0; k<K; ++k){
    for(int l=0; l<L; ++l){
      A[k][l] = std::sin(k*K+l);
    }
  }
  
  for(int l=0; l<L; ++l){
    for(int m=0; m<M; ++m){
      B[l][m] = std::sin(l*L+m);
    }
  }

  auto norm = [&C](){
    double res{0};
    for(int i=0; i<L; ++i)
      for(int k=0; k<M; ++k)
	res += C[i][k];
	
    return res;
  };

  auto zero = [&C](){
    for(int k=0; k<K; ++k)
      for(int m=0; m<M; ++m)
	C[k][m] = 0;
  };

  //**************************************
  zero();
  auto t = t_now();

  // K,L,M - is the best variant
  for(int k=0; k<K; ++k)
    for(int l=0; l<L; ++l)
      for(int m=0; m<M; ++m)
	C[k][m] += A[k][l]*B[l][m];
 
  auto tsec1 = t_diff(t, t_now());
  std::cout<<"(K,L,M) t1 = "<<tsec1<<", norm C = "<<norm()<<std::endl;

  //***************************************
  
  auto ABpC = [&A, &B, &C, &N](int ra, int ca, int rb, int cb, int rc, int cc){
		    for(int k=0; k<N; ++k)
		      for(int m=0; m<N; ++m)
			for(int l=0; l<N; ++l)
			  C[rc*N + k][cc*N + m] += A[ra*N + k][ca*N + l]*B[rb*N + l][cb*N + m];
  		  };
  
  zero();
  t = t_now();

  // C[k;m] = SUM( A[k;l]*B[l;m] )
  #pragma omp parallel for num_threads(4)
  //#pragma omp critical
  for(int k = 0; k < P; ++k)
    for(int m = 0; m < P; ++m)
      for(int l = 0; l < P; ++l)
	//ABpC(k, l /* A[k;l] */, l, m /* B[l;m] */, k, m /* C[k;m] */);
	//call ABpC() P^3 times add overheads about 10% common time
	for(int kk=0; kk<N; ++kk)
	  for(int mm=0; mm<N; ++mm)
	    for(int ll=0; ll<N; ++ll)
	      //calculate index C,A,B matrixs add overhead about 100% common time!!!
	      C[k*N + kk][m*N + mm] += A[k*N + kk][l*N + ll]*B[l*N + ll][m*N + mm];

  auto tsec2 = t_diff(t, t_now());
  std::cout<<"(block matrix multiplier) t2 = "<<tsec2<<", norm C = "<<norm()<<", t2/t1 = "<<tsec2/tsec1<<std::endl;
  
  //***************************************
  std::cout<<"End prorgam.\n";
  
  return 0;

}
