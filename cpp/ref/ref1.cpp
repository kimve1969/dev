#include<iostream>

struct A{
  A(int x):m_a(x){ std::cout<<"cnt A("<<m_a<<")\n"; }
  ~A(){ std::cout<<"dst A("<<m_a<<")\n"; }

  int m_a;
};

int main(){
  std::cout<<"Start\n";

  A a{1}; // cnt A
  A &lva = a;
  lva = A{2}; // cnt A(2),  a.m_a = 2, dst A(2)
  A &&rva = A{3}; // cnt A(3)
  // initialization rvalue reference extend liftime temprary object...

  std::cout<<"End\n";
  return 0; // dst A(3) dst A(2)
}
