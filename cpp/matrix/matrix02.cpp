#include<iostream>
#include<chrono>
#include<cstdlib>
#include<ctime>
#include<cmath>

auto t_now(){ return std::chrono::high_resolution_clock::now(); }
auto t_diff(const std::chrono::time_point<std::chrono::high_resolution_clock> &start,
	    const std::chrono::time_point<std::chrono::high_resolution_clock> &end){
  std::chrono::duration<double> diff = end - start;
  return diff.count();
}

int main(int argc, char** argv){
  std::cout<<"Start.\n";

  std::cout<<"|A|*|B|=|C|, C[k,m] = SUM( A[k,l]*B[l,m])\n";

  // size of cash 1 - level is 32 kB, 32'768 B / 8 B = 4'096 double elements,
  // 4'096 elem / 3 (matrix A,B,C) = 1365 size of each matrix,
  // sqrt(1365) = 37 elem. is size of side matrix
  const int N = 37;
  const int P = 27;
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
  
  for(int k=0; k<K; ++k)
    for(int l=0; l<L; ++l)
      for(int m=0; m<M; ++m)
	C[k][m] += A[k][l]*B[l][m];
	
  std::cout<<"(K,L,M) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

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
  for(int k = 0; k < P; ++k)
    for(int m = 0; m < P; ++m)
      for(int l = 0; l < P; ++l)
	ABpC(k, l /* A[k;l] */, l, m /* B[l;m] */, k, m /* C[k;m] */);
  
  std::cout<<"(block matrix multiplier) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;
  
  //***************************************
  std::cout<<"End prorgam.\n";
  
  return 0;

}
