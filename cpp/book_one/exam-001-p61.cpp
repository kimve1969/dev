#include<iostream>
#include<iomanip>
#include<omp.h>
/*
  see page 61 of book
*/

#ifdef __GNUC__
#define ALIGN(N) __attribute__((aligned(N))) // Linux
#else
#define ALIGN(N) __declspec(align(N)) // Windows
#endif

ALIGN(64) int N=330'000'123;

template<typename T> void tadd();
template<int K> void tadd(){  
    ALIGN(64) double a = 1.0;
    ALIGN(64) double result = 0.0;
    ALIGN(64) double t = 0.0;
    
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[K];
    for(int i=0; i<K; ++i) result_tmp[i]=0;
      
    for(int i=0; i<N/K; ++i){
      for(int j=0; j<K; ++j) result_tmp[j] += a; // unrolling loop
    }
      
    for(int i=0; i<N%K; ++i) result += a;
      
    for(int j=0; j<K; ++j) result += result_tmp[j];
    
    t = omp_get_wtime()-t;
    std::cout<<K<<". result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
}

template<typename T> void tmul();
template<int K> void tmul(){
    ALIGN(64) double a = 1.000'000'1;
    ALIGN(64) double result = 1.0;
    ALIGN(64) double t = 0.0;
  
    t = omp_get_wtime();
 
    ALIGN(64) double result_tmp[K];
    for(int i=0; i<K; ++i) result_tmp[i]=1.0;
      
    for(int i=0; i<N/K; ++i){
      for(int j=0; j<K; ++j) result_tmp[j] /= a; // unrolling loop
    }
      
    for(int i=0; i<N%K; ++i) result /= a;
      
    for(int j=0; j<K; ++j) result *= result_tmp[j];
    
    t = omp_get_wtime()-t;
    std::cout<<K<<". result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
}

int main(int argc, char* argv[]){

  std::cout<<"*********************** ADD optimization using unrolling loop *************************\n";
  
  tadd<1>();
  tadd<8>();
  tadd<64>();
  tadd<256>();
  
  std::cout<<"*********************** MULT optimization using unrolling loop ************************\n";

  tmul<1>();
  tmul<8>();
  tmul<64>();
  tmul<256>();
  
  return 0;
}
