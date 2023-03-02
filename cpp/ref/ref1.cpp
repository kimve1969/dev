#include<iostream>

struct A{
  A(int x):m_x(x){ std::cout<<"cnt A("<<x<<")\n"; }
  ~A(){ std::cout<<"dst ~A("<<m_x<<")\n"; }

  int m_x;
};

int main(){
  A a1(1);
  A &ra = a1;
  
  {
    A a2(2);
    ra = a2;
  }
  
  A a3{3};
  return 0;
}
