#include<iostream>
#include<iomanip>
#include<omp.h>
/*
  see page 61 of book

  Character of processor of my notebook Lenovo is: 
  proccessor Intel Core i5 frequance is 1.19 GHz, Turbo mode is 3.6 GHz (4 core)
  TPP (Theoretical pick perfomance) = D (number of bytes) * F (frequance) = 1 bytes * 1.19E9 Hz = 1.19 GFLOPS
  This example perfomance = 0.38 GFLOPS, only 32% from TPP!

  Character of processor of my Samsung Galaxy Tab S7, SM-T875
  processor Qualcomm Snapdragon 865 Plus, 8 core, 3.1 GHz (1 core - 2.84 GHz, 3 core - 2.42 GHz, 4 core - 1.8 GHz)
  TPP (for 1.8 GHz) = 1.8 GFLOPS
  RAM 6144 Mb, LPDDR4 / 128 Gb
*/
int main(int argc, char* argv[]){
  int N=330'000'123, K =0;
  double a = 1.0, result = 0.0, t = 0.0;

  auto add = [&](const int K) constexpr{
    result = 0.0;
    t = omp_get_wtime();
    {
      double result_tmp[K]={0};
      for(int i=0; i<N/K; ++i){
	for(int j=0; j<K; ++j) result_tmp[j] += a; // compile with option -O3 unroll loop
      }
      
      for(int i=0; i<N%K; ++i) result += a;
      
      for(int j=0; j<K; ++j) result += result_tmp[j];
    }
    t = omp_get_wtime()-t;
    std::cout<<K<<". result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  };

  add(1);
  add(2);
  add(4);
  add(8);
  add(16);
  add(32);
  add(48);
  add(54);
  add(58);
  add(64);
  add(128);
  add(256);
  std::cout<<"***********************\n";
  
  result = 0.0;
  t = omp_get_wtime();
  for(int i=0; i<N; ++i){
    result += a;
  }
  t = omp_get_wtime()-t;
  std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  t = omp_get_wtime();
  {
    double result_tmp[2]={0,0};
    for(int i=0; i<N/2; ++i){
      //result_tmp[0] += a;
      //result_tmp[1] += a;
      for(int j=0; j<2; ++j) result_tmp[j] += a; // compile with option -O3 unroll loop
    }
    for(int i=0; i<N%2; ++i) result += a;
      
    for(int j=0; j<2; ++j) result += result_tmp[j];
  }
  t = omp_get_wtime()-t;
  std::cout<<"2. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  
  return 0;
}
