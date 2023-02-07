#include<iostream>
#include<blas.hh>

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";

  blas::scalar_type<double, double> a{3.1};

  int64_t n = 5, incx{1}, incy{1}; 
  double x[]{1.0, 2.0, 3.0, 4.0, 5.0};
  double y[]{2.0, 2.0, 2.0, 2.0, 2.0};

  auto prn = [](const char* str, double p[], int n){
    std::cout<<str<<": ";
    for(int i=0; i<n; ++i){
      std::cout<<p[i]<<", ";
    }
    std::cout<<"\n";
  };

  std::cout<<"blas::axpy() y = ax+y\n";
  std::cout<<"a == "<<a<<"\n";
  prn("vector x", x, sizeof(x)/sizeof(double));
  prn("vector y", y, sizeof(y)/sizeof(double));
  
  blas::axpy(n, a, x, incx, y, incy);

  prn("vector y after axpy()", y, sizeof(y)/sizeof(double));
  
  std::cout<<"\nEnd\n";
  return 0;
}
