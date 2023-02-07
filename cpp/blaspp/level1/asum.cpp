#include<iostream>
#include<blas.hh>

int main(int argc, char* argv[]){
  std::cout<<"Start\n";

  int64_t n = 5, incx = 1;
  double m[] = {1.1, 2.0, 3.0, 4.0, 5.0};

  double norm = blas::asum(n, m, incx);

  std::cout<<"m[]= {";
  for(auto &e: m){
    std::cout<<e<<", ";
  }
  std::cout<<"};\n";

  std::cout<<"blas::asum() == "<<norm<<"\n";;
    
  std::cout<<"End\n";
  return 0;
}
