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
    for(int i=0; i<M; ++i)
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
  auto t = t_now();
  
  for(int i=0; i<L; ++i)
    for(int j=0; j<M; ++j)
      for(int k=0; k<N; ++k)
	C[i][k] += A[i][j]*B[j][k];
	
  std::cout<<"(L,M,N) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();
  
  for(int i=0; i<L; ++i)
    for(int k=0; k<N; ++k)
      for(int j=0; j<M; ++j)
	C[i][k] += A[i][j]*B[j][k];
	
  std::cout<<"(L,N,M) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  for(int j=0; j<M; ++j)
    for(int i=0; i<L; ++i)
      for(int k=0; k<N; ++k)
	C[i][k] += A[i][j]*B[j][k];
	
  std::cout<<"(M,L,N) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  for(int j=0; j<M; ++j)
    for(int k=0; k<N; ++k)
      for(int i=0; i<L; ++i)
	C[i][k] += A[i][j]*B[j][k];
	
  std::cout<<"(M,N,L) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  for(int k=0; k<N; ++k)
    for(int j=0; j<M; ++j)
      for(int i=0; i<L; ++i)
	C[i][k] += A[i][j]*B[j][k];
	
  std::cout<<"(N,M,L) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  //***************************************
  zero();
  t = t_now();

  for(int k=0; k<N; ++k)
    for(int i=0; i<L; ++i)
      for(int j=0; j<M; ++j)
	C[i][k] += A[i][j]*B[j][k];
	
  std::cout<<"(N,L,M) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;

  // B[M][N] -> BT[N][M]
  auto BT = new double[N][M];
  
  for(int j=0; j<M; ++j)
    for(int k=0; k<N; ++k)
      BT[k][j] = B[j][k];
  
  //***************************************
  std::cout<<"rotate matrix B"<<std::endl;
  zero();
  t = t_now();
  
  for(int i=0; i<L; ++i)
    for(int k=0; k<N; ++k)
      for(int j=0; j<M; ++j)
	C[i][k] += A[i][j]*BT[k][j];
	
  std::cout<<"(L,N,M) t = "<<t_diff(t, t_now())<<", norm C = "<<norm()<<std::endl;
  
  std::cout<<"End prorgam.\n";
  
  return 0;

}
