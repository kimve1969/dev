#include<iostream>
#include<blas.hh>
#include<cmath>

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";

  int64_t n = 5, incx{1}; 
  double x[]{1.0, 2.0, 3.0, 4.0, 5.0};

  auto prn = [](const char* str, double p[], int n){
    std::cout<<str<<": ";
    for(int i=0; i<n; ++i){
      std::cout<<p[i]<<", ";
    }
    std::cout<<"\n";
  };

  prn("vector x", x, sizeof(x)/sizeof(double));
  
  double norm2 = blas::nrm2(n, x, incx);
  
  std::cout<<"blas::nrm2() return 2-norm of vector x == "<<norm2<<"\n";
 
  std::cout<<"sqrt(";
  bool b{false};
  for(auto &e: x){
    b ? std::cout<<" + ": std::cout, b=true;
    std::cout<<e<<"^2";
  }
  std::cout<<") == sqrt(";
  
  b = false;
  double sum{0}, pw{0};
  for(auto &e: x){
    b ? std::cout<<" + ": std::cout, b=true;
    pw = std::pow(e,2);
    std::cout<<pw;
    sum += pw;
  }
  std::cout<<") == sqrt ("<<sum<<") == "<<norm2<<"\n";
  
  std::cout<<"\nEnd\n";
  return 0;
}
