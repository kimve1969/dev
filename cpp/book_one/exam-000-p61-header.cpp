#include<iostream>
#include<iomanip>
#include<omp.h>
#include<cmath>
/*
  see page 61 of book
*/

#ifdef __GNUC__
#define ALIGN(N) __attribute__((aligned(N))) // Linux
#else
#define ALIGN(N) __declspec(align(N)) // Windows
#endif

int main(int argc, char* argv[]){

  std::cout<<"*********************** ADD optimization using unrolling loop *************************\n";
    
  ALIGN(64) double a = 1.0;
  ALIGN(64) double result = 0.0;
  ALIGN(64) double t = 0.0;
  ALIGN(64) int N=330'000'123;

  /*{
    t = omp_get_wtime();

    for(int i=0; i<N; ++i) result += a;
      
    t = omp_get_wtime()-t;
    std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
    }*/
  
