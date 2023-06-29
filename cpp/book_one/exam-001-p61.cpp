#include<iostream>
#include<iomanip>
#include<omp.h>
/*
  see page 61 of book

  Character of processor of my notebook Lenovo is: 
  proccessor Intel Core i5 frequance is 1.19 GHz, Turbo mode is 3.6 GHz (4 core)
  TPP (Theoretical pick perfomance) = D (number of bytes) * F (frequance) = 1 bytes * 1.19E9 Hz = 1.19 GFLOPS
  This example perfomance = 0.38 GFLOPS, only 32% from TPP!
  K = 24, perfomance = 12.8 GFLOPS

  Character of processor of my Samsung Galaxy Tab S7, SM-T875
  processor Qualcomm Snapdragon 865 Plus, 8 core, 3.1 GHz (1 core - 2.84 GHz, 3 core - 2.42 GHz, 4 core - 1.8 GHz)
  TPP (for 1.8 GHz) = 1.8 GFLOPS
  RAM 6144 Mb, LPDDR4 2750 MHz, BW =   44 Gb/s or 5,5 GB/s
  L1 - 512 Kb, L2 - 1 Mb, L3 - 4 Mb
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
  tadd<2>();
  tadd<4>();
  tadd<8>();
  tadd<16>();
  tadd<32>();
  tadd<48>();
  tadd<54>();
  tadd<58>();
  tadd<64>();
  tadd<128>();
  tadd<256>();
  
  std::cout<<"*********************** MULT optimization using unrolling loop *****************************\n";

  tmul<1>();
  tmul<2>();
  tmul<4>();
  tmul<8>();
  tmul<16>();
  tmul<32>();
  tmul<48>();
  tmul<54>();
  tmul<58>();
  tmul<64>();
  tmul<128>();
  tmul<256>();
  
  return 0;
}
