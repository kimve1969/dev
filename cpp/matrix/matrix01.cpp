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
  
  const int K{999}, L{999}, M{999};
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
  zero();
  t = t_now();
  
  for(int k=0; k<K; ++k)
    for(int m=0; m<M; ++m)
      for(int l=0; l<L; ++l)
	C[k][m] += A[k][l]*B[l][m];
	
  std::cout<<"(K,M,L) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  
  for(int l=0; l<L; ++l)
    for(int k=0; k<K; ++k)
      for(int m=0; m<M; ++m)
	C[k][m] += A[k][l]*B[l][m];
  	
  std::cout<<"(L,K,M) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  for(int l=0; l<L; ++l)
    for(int m=0; m<M; ++m)
      for(int k=0; k<K; ++k)
	C[k][m] += A[k][l]*B[l][m];
  	
  std::cout<<"(L,M,K) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  for(int m=0; m<M; ++m)
    for(int k=0; k<K; ++k)
      for(int l=0; l<L; ++l)
	C[k][m] += A[k][l]*B[l][m];
	
  std::cout<<"(M,K,L) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  for(int m=0; m<M; ++m)
    for(int l=0; l<L; ++l)
      for(int k=0; k<K; ++k)
	C[k][m] += A[k][l]*B[l][m];
	
  std::cout<<"(M,L,K) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  // B[L][M] -> RB[M][L]
  auto RB = new double[M][L];

  for(int m=0; m<M; ++m)
    for(int l=0; l<L; ++l)  
      RB[m][l] = B[l][m];
  
  //***************************************
  std::cout<<"rotate matrix B"<<std::endl;
  zero();
  t = t_now();

  for(int k=0; k<K; ++k)
    for(int m=0; m<M; ++m)
      for(int l=0; l<L; ++l)
	C[k][m] += A[k][l]*RB[m][l];
  	
  std::cout<<"(K,M,L: A * rot B) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;
  
  std::cout<<"End prorgam.\n";
  
  return 0;

}
