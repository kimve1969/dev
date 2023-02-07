#include<iostream>

void print(const char* pstr){
  std::cout<<"\x1b[31;1m"<<pstr<<"\x1b[0m\n";
}

int main(int argc, char *argv[])
{
  struct A{
    char m_a{'A'};
    int  m_b{1};
    // default cnt, dst, oper=
  };

  print("default cnt:");
  A a1;
  std::cout<<"a1.m_a="<<a1.m_a<<", a1.m_b="<<a1.m_b<<"\n";
  
  print("default cnt and list initialize:");
  A a2{'A',2};
  std::cout<<"a2.m_a="<<a2.m_a<<", a2.m_b="<<a2.m_b<<"\n";

  print("default copy assignment:");
  a1=a2;
  std::cout<<"a1.m_a="<<a1.m_a<<", a1.m_b="<<a1.m_b<<"\n";

  print("default move assignment:");
  a1=A{'A',3};
  std::cout<<"a1.m_a="<<a1.m_a<<", a1.m_b="<<a1.m_b<<"\n";

  struct B{
    char m_a{'B'};
    int  m_b{1};
    B(A a):m_a(a.m_a), m_b(a.m_b){}; // implicit copy cnt
  };

  print("implicit copy:");
  B b1=a2;
  std::cout<<"b1.m_a="<<b1.m_a<<", b1.m_b="<<b1.m_b<<"\n";
  
  return 0;
}

