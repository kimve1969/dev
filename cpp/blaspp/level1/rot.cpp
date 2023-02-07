#include<iostream>
#include<blas.hh>

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";

  int64_t n = 5, incx{1}, incy{1}; 
  double x[]{1.0, 2.0, 3.0, 4.0, 5.0};
  double y[]{6.0, 7.0, 8.0, 9.0, 10.0};
  blas::real_type<double> c{0.5};
  blas::scalar_type<double> s{0.3};

  auto prn = [](const char* str, double p[], int n){
    std::cout<<str<<": ";
    for(int i=0; i<n; ++i){
      std::cout<<p[i]<<", ";
    }
    std::cout<<"\n";
  };

  std::cout<<"blas::rot() y <=> x\n";
  prn("vector x", x, sizeof(x)/sizeof(double));
  prn("vector y", y, sizeof(y)/sizeof(double));
  
  blas::rot(n, x, incx, y, incy, c, s);

  prn("vector x after rot()", x, sizeof(x)/sizeof(double));
  prn("vector y after rot()", y, sizeof(y)/sizeof(double));
  
  std::cout<<"\nEnd\n";
  return 0;
}
