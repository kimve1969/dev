#include<iostream>
#include<iomanip>
#include<omp.h>
#include<cmath>
/*
  see page 66 of book
  
  Character of processor of my Samsung Galaxy Tab S7, SM-T875
  processor Qualcomm Snapdragon 865 Plus, 8 core:
     1 core - ARM Cortex-A77 2.84 GHz, 3 core - ARM Cortex-A77 2.42 GHz, 4 core - ARM Cortex-A55 1.8 GHz
  ARM Cortex-A77:
     CPU 3.3 GHz, L1 128KB, L2 256-512KB, L3 1-4MB
     Instruction set ARMv8-A, extention ARMv8.2A
  ARM Cortex-A55: 
     CPU 1.25-1.6 GHz, L1 32-128KB, L2 64-256 KB, L3 512KB-4MB 
     Instruction set ARMv8.2A
  RAM 6144 Mb, LPDDR4, Band Width = 25,6 GB/s
  L1 - 512 Kb, L2 - 1 Mb, L3 - 4 Mb

*/

int main(int argc, char* argv[]){

  std::cout<<"*********************** memory bound  *************************\n";
  
  int N=66'000'123;
  double *x=new double[N], *y=new double[N];
  double result = 0.0, t = 0.0;
  /*
    AI (arifmetic intensivity) ADD op = 1/24 (FLOP/bytes), 
    z = x+y, 2 double read, 1 double write = 24 bytes transfer between RAM & CPU
    for LPDDR4:
    BW (GB/s) * AI (FLOP/bytes) = 25,5 GB/s * 1/24 (FLOP/bytes) = 1,06 FLOPS speed of transfer RAM & CPU

    TEST (ARM):
    0.78 GFLOPS
  */
  {
    for(int i=0; i<N; ++i) x[i]=i%2, y[i]=i%3;
    t = omp_get_wtime();
          
    for(int i=0; i<N; ++i) x[i] += y[i];
      
    t = omp_get_wtime()-t;
    std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
  
  result = 0.0;
  {
    for(int i=0; i<N; ++i) x[i]=i%2, y[i]=i%3;
    t = omp_get_wtime();

    int N4=(N/4)*4;
    for(int i=0; i<N4; ++i){
      x[i] += y[i]; // unrolling loop
      x[i+1] += y[i+1];
      x[i+2] += y[i+2];
      x[i+3] += y[i+3];
    }
      
    for(int i=N4; i<N; ++i) x[i] += y[i];
          
    t = omp_get_wtime()-t;
    std::cout<<"4. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  result = 0.0;
  {
    for(int i=0; i<N; ++i) x[i]=i%2, y[i]=i%3;
    t = omp_get_wtime();

    int N4=(N/4)*4;
    #pragma omp parallel for num_threads(2)
    for(int i=0; i<N4; ++i){
      x[i] += y[i]; // unrolling loop
      x[i+1] += y[i+1];
      x[i+2] += y[i+2];
      x[i+3] += y[i+3];
    }
      
    for(int i=N4; i<N; ++i) x[i] += y[i];
          
    t = omp_get_wtime()-t;
    std::cout<<"4. (2 threads) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  {
    for(int i=0; i<N; ++i) x[i]=i%2, y[i]=i%3;
    t = omp_get_wtime();
          
    for(int i=0; i<N; ++i) x[i] += sin(y[i]);
      
    t = omp_get_wtime()-t;
    std::cout<<"1. sin(y) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GSINS: "<<N/(t*1E9)<<"\n";
  }

  result = 0.0;
  {
    for(int i=0; i<N; ++i) x[i]=i%2, y[i]=i%3;
    t = omp_get_wtime();

    int N4=(N/4)*4;
    for(int i=0; i<N4; ++i){
      x[i] += y[i]; // unrolling loop
      x[i+1] += y[i+1];
      x[i+2] += y[i+2];
      x[i+3] += y[i+3];
    }
      
    for(int i=N4; i<N; ++i) x[i] += sin(y[i]);
          
    t = omp_get_wtime()-t;
    std::cout<<"4. sin(y) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GSINS: "<<N/(t*1E9)<<"\n";
  }
  
  return 0;
}
