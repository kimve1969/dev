#include<iostream>
#include<iomanip>
#include<omp.h>
/*
  see page 61 of book

  Character of processor of my notebook Lenovo is: 
  proccessor Intel Core i5 frequance is 1.19 GHz
  TPP (Theoretical pick perfomance) = D (number of bytes) * F (frequance) = 1 bytes * 1.19E9 Hz = 1.19 GFLOPS
  This example perfomance = 0.38 GFLOPS, only 32% from TPP!
*/
int main(int argc, char* argv[]){
  int N=330'000'123;
  double a = 1.0, result = 0.0, t;
  t = omp_get_wtime();
  for(int i=0; i<N; ++i){
    result += a;
  }
  t = omp_get_wtime()-t;
  std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  result = 0.0;
  t = omp_get_wtime();
  {
    double result_tmp[2]={0,0};
    for(int i=0; i<N/2; ++i){
      result_tmp[0] += a;
      result_tmp[1] += a;
    }
    for(int i=0; i<N%2; ++i) result += a;
    result += result_tmp[0] + result_tmp[1];
  }
  t = omp_get_wtime()-t;
  std::cout<<"2. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  result = 0.0;
  t = omp_get_wtime();
  {
    double result_tmp[3]={0,0};
    for(int i=0; i<N/3; ++i){
      result_tmp[0] += a;
      result_tmp[1] += a;
      result_tmp[2] += a;
    }
    for(int i=0; i<N%3; ++i) result += a;
    result += result_tmp[0] + result_tmp[1] + result_tmp[2];
  }
  t = omp_get_wtime()-t;
  std::cout<<"3. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  result = 0.0;
  t = omp_get_wtime();
  {
    double result_tmp[4]={0,0};
    for(int i=0; i<N/4; ++i){
      result_tmp[0] += a;
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
    }
    for(int i=0; i<N%4; ++i) result += a;
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3];
  }
  t = omp_get_wtime()-t;
  std::cout<<"4. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  result = 0.0;
  t = omp_get_wtime();
  {
    double result_tmp[5]={0,0};
    for(int i=0; i<N/5; ++i){
      result_tmp[0] += a;
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
    }
    for(int i=0; i<N%5; ++i) result += a;
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4];
  }
  t = omp_get_wtime()-t;
  std::cout<<"5. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  result = 0.0;
  t = omp_get_wtime();
  {
    double result_tmp[6]={0,0};
    for(int i=0; i<N/6; ++i){
      result_tmp[0] += a;
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
      result_tmp[5] += a;
    }
    for(int i=0; i<N%6; ++i) result += a;
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5];
  }
  t = omp_get_wtime()-t;
  std::cout<<"6. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  result = 0.0;
  t = omp_get_wtime();
  {
    double result_tmp[7]={0,0};
    for(int i=0; i<N/7; ++i){
      result_tmp[0] += a;
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
      result_tmp[5] += a;
      result_tmp[6] += a;
    }
    for(int i=0; i<N%7; ++i) result += a;
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6];
  }
  t = omp_get_wtime()-t;
  std::cout<<"7. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";

  
  return 0;
}
