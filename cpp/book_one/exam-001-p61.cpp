#include<iostream>
#include<iomanip>
#include<omp.h>
/*
  see page 61 of book
  Character of processor of my notebook Lenovo is: 
  proccessor Intel Core i5 frequance is 1.19 GHz

*/
int main(int argc, char* argv[]){
  int N=330'000'123;
  double a = 1.0, result = 0.0, t;
  t = omp_get_wtime();
  for(int i=0; i<N; ++i){
    result += a;
  }
  t = omp_get_wtime()-t;
  std::cout<<"result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  
  return 0;
}
