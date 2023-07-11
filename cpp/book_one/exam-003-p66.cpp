#include<iostream>
#include<iomanip>
#include<omp.h>
#include<cmath>
/*
  see page 66 of book
*/

int main(int argc, char* argv[]){

  std::cout<<"*********************** band width  *************************\n";
  
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
    std::cout<<"4. (4 UNROLL) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
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
    std::cout<<"4+2. (4 UNROOL + 2 THREADS) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n\n";
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
      x[i] += sin(y[i]); // unrolling loop
      x[i+1] += sin(y[i+1]);
      x[i+2] += sin(y[i+2]);
      x[i+3] += sin(y[i+3]);
    }
      
    for(int i=N4; i<N; ++i) x[i] += sin(y[i]);
          
    t = omp_get_wtime()-t;
    std::cout<<"4. (4 UNROLL) sin(y) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GSINS: "<<N/(t*1E9)<<"\n";
  }
  
  return 0;
}
