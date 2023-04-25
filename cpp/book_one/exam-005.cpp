#include<iostream>
#include<omp.h>

int main(int argc, char* argv[]){

  
#pragma omp parallel
  {
    #pragma omp critical
    std::cout<< omp_get_thread_num()<<","<<omp_get_num_threads()<<"\n";
  }
  
  std::cout<<"****************************************************\n";
  
#pragma omp parallel num_threads(3)
  {
    #pragma omp critical
    std::cout<< omp_get_thread_num()<<","<<omp_get_num_threads()<<"\n";
  }
  return 0;
}
