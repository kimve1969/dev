#include<iostream>

template<typename T>
struct A;

template<unsigned int n>
struct fact;

template<unsigned int n>
struct fact{
  constexpr static unsigned int res = n*fact<n-1>::res;
};

template<>
struct fact<0>{
  constexpr static unsigned int res = 1;
};

static_assert(fact<5>::res == 120);

int main(){
  std::cout<<fact<5>::res<<std::endl;
  return 0;
}
