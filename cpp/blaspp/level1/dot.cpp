#include<iostream>
#include<blas.hh>

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";

  int64_t n = 5, incx{1}, incy{1}; 
  double x[]{1.0, 2.0, 3.0, 4.0, 5.0};
  double y[]{6.0, 7.0, 8.0, 9.0, 10.0};

  auto prn = [](const char* str, double p[], int n){
    std::cout<<str<<": ";
    for(int i=0; i<n; ++i){
      std::cout<<p[i]<<", ";
    }
    std::cout<<"\n";
  };

  std::cout<<"blas::dot() xH*y, blas::dotu() xT*y\n";
  
  prn("vector x", x, sizeof(x)/sizeof(double));
  prn("vector y", y, sizeof(y)/sizeof(double));
  
  double retdot = blas::dot(n, x, incx, y, incy);
  double retdotu = blas::dotu(n, x, incx, y, incy);

  double sum{0};
  for(int i=0; i<5; ++i){
    sum += x[i]*y[i];
  }

  std::cout<<"blas::dot() return == "<<retdot<<"\n";
  std::cout<<"blas::dotu() return == "<<retdotu<<"\n";
  std::cout<<"SUM(x[i]*y[i]) == "<<sum<<"\n";
  
  std::cout<<"\nEnd\n";
  return 0;
}
