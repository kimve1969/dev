#include<iostream>
#include<chrono>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<blas.hh>

int main(int argc, char** argv){
  std::cout<<"Start.\n";
  
  const int L{1000}, M{1000}, N{1000};
  auto A = new double[L][M];
  auto B = new double[M][N];
  auto C = new double[L][N];
    
  std::srand(std::time(nullptr));

  for(int i=0; i<L; ++i){
    for(int j=0; j<M; ++j){
      A[i][j] = std::sin(i*L+j);
    }
  }

  for(int i=0; i<M; ++i){
    for(int j=0; j<N; ++j){
      B[i][j] = std::sin(i*M+j);
    }
  }

  auto norm = [&C](){
    double res{0};

    for(int i=0; i<L; ++i)
      for(int k=0; k<N; ++k)
	res += C[i][k];
	
    return res;
  };

  auto zero = [&C](){
    for(int i=0; i<L; ++i)
      for(int k=0; k<N; ++k)
	C[i][k] = 0;
  };

  //**************************************
  zero();
  auto t1 = std::chrono::high_resolution_clock::now();
  
  for(int i=0; i<L; ++i)
    for(int j=0; j<M; ++j)
      for(int k=0; k<N; ++k)
	C[i][k] += A[i][j]*B[j][k];
	
  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> t3 = t2-t1;
  std::cout<<"(L,M,N) t = "<<t3.count()<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t1 = std::chrono::high_resolution_clock::now();
  
  for(int i=0; i<L; ++i)
    for(int k=0; k<N; ++k)
      for(int j=0; j<M; ++j)
	C[i][k] += A[i][j]*B[j][k];
	
  t2 = std::chrono::high_resolution_clock::now();
  t3 = t2-t1;
  std::cout<<"(L,N,M) t = "<<t3.count()<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t1 = std::chrono::high_resolution_clock::now();

  for(int j=0; j<M; ++j)
    for(int i=0; i<L; ++i)
      for(int k=0; k<N; ++k)
	C[i][k] += A[i][j]*B[j][k];
	
  t2 = std::chrono::high_resolution_clock::now();
  t3 = t2-t1;
  std::cout<<"(M,L,N) t = "<<t3.count()<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t1 = std::chrono::high_resolution_clock::now();

  for(int j=0; j<M; ++j)
    for(int k=0; k<N; ++k)
      for(int i=0; i<L; ++i)
	C[i][k] += A[i][j]*B[j][k];
	
  t2 = std::chrono::high_resolution_clock::now();
  t3 = t2-t1;
  std::cout<<"(M,N,L) t = "<<t3.count()<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t1 = std::chrono::high_resolution_clock::now();

  for(int k=0; k<N; ++k)
    for(int j=0; j<M; ++j)
      for(int i=0; i<L; ++i)
	C[i][k] += A[i][j]*B[j][k];
	
  t2 = std::chrono::high_resolution_clock::now();
  t3 = t2-t1;
  std::cout<<"(N,M,L) t = "<<t3.count()<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t1 = std::chrono::high_resolution_clock::now();

  for(int k=0; k<N; ++k)
    for(int i=0; i<L; ++i)
      for(int j=0; j<M; ++j)
	C[i][k] += A[i][j]*B[j][k];
	
  t2 = std::chrono::high_resolution_clock::now();
  t3 = t2-t1;
  std::cout<<"(N,L,M) t = "<<t3.count()<<", norm C = "<<norm()<<std::endl;

  // B[M][N] -> BT[N][M]
  auto BT = new double[N][M];
  
  for(int j=0; j<M; ++j)
    for(int k=0; k<N; ++k)
      BT[k][j] = B[j][k];
  
  //***************************************
  std::cout<<"rotate matrix B"<<std::endl;
  zero();
  t1 = std::chrono::high_resolution_clock::now();
  
  for(int i=0; i<L; ++i)
    for(int k=0; k<N; ++k)
      for(int j=0; j<M; ++j)
	C[i][k] += A[i][j]*BT[k][j];
	
  t2 = std::chrono::high_resolution_clock::now();
  t3 = t2-t1;
  std::cout<<"(L,N,M) t = "<<t3.count()<<", norm C = "<<norm()<<std::endl;
  
  std::cout<<"End prorgam.\n";
  return 0;

}
