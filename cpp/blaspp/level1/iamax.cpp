#include<iostream>
#include<blas.hh>

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";

  int64_t n = 5, incx{1}; 
  double x[]{1.0, 2.0, 7.0, 4.0, 5.0};

  auto prn = [](const char* str, double p[], int n){
    std::cout<<str<<": ";
    for(int i=0; i<n; ++i){
      std::cout<<p[i]<<", ";
    }
    std::cout<<"\n";
  };

  std::cout<<"blas::iamax() return index infinity-norm of vector x\n";
  prn("vector x", x, sizeof(x)/sizeof(double));
  
  int64_t inorm = blas::iamax(n, x, incx);

  std::cout<<"iamax() return index == "<<inorm<<", x["<<inorm<<"] == "<<x[inorm]<<"\n";

  std::cout<<"\nEnd\n";
  return 0;
}
