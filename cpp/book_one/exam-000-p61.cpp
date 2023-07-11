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
  ALIGN(64) double t1 = 0.0, t2 = 0.0;
  ALIGN(64) int N=330'000'123;

  {
    t1 = omp_get_wtime();

    for(int i=0; i<N; ++i) result += a;
      
    t1 = omp_get_wtime()-t1;
    std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t1<<" s, GFLOPS: "<<N/(t1*1E9)<<"\n";
  }

  
  result = 0.0;
  {
    t2 = omp_get_wtime();
    
    ALIGN(64) double result_tmp[2]={0.0, 0.0};
      
    for(int i=0; i<N/2; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
    }
      
    for(int i=0; i<N%2; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1];
    
    t2 = omp_get_wtime()-t2;
    std::cout<<"2. (UNROLL 2) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t2<<" s, GFLOPS: "<<N/(t2*1E9)<<", accelerate = "<<std::setprecision(3)<<t1/t2<<std::endl;
  }
  
  result = 0.0;
  {
    t2 = omp_get_wtime();
    
    ALIGN(64) double result_tmp[4]={0.0, 0.0, 0.0, 0.0};
      
    for(int i=0; i<N/4; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
    }
      
    for(int i=0; i<N%4; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3];
    
    t2 = omp_get_wtime()-t2;
    std::cout<<"4. (UNROLL 4) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t2<<" s, GFLOPS: "<<N/(t2*1E9)<<", accelerate = "<<std::setprecision(3)<<t1/t2<<std::endl;
  }
  
  return 0;
}
