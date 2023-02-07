#include<iostream>
#include<memory>

struct A{
  A(){ std::cout<<"A()\n"; }
  A(const A&){ std::cout<<"A(const &A)\n"; }
  A(A&&){ std::cout<<"A(A&&)\n"; }
  ~A(){ std::cout<<"~A()\n"; }
  void print(){ std::cout<<"I am struct A\n"; }
};
// is OK
A f1(){
  A a;
  return a;
}
// is BAD
A&& f2(){
  A a;
  return std::move(a);
}
// is OK
std::unique_ptr<A>&& f3(){
  std::unique_ptr<A> ptr_a(new A);
  return std::move(ptr_a);
}

int main(){
  std::cout<<"start\n";
  std::cout<<"1. A f1() RVO\n";
  {
    A a = f1(); // 1. cnt A()
    std::cout<<"befor }\n";
  } // 2. dst ~A()
  std::cout<<"after }\n";
  std::cout<<"-----------------------\n";
  std::cout<<"2. A&& f2() move semantic\n";
  {
    A a = f2(); // 1. cnt A() 2. A(A&&) 3. dst ~A()
    a.print();
    std::cout<<"befor }\n";
  } // 4. dst ~A()
  std::cout<<"after }\n";
  std::cout<<"-----------------------\n";
  std::cout<<"3. f3() unique_ptr<A>\n";
  {
    std::unique_ptr<A> ptr_a = f3(); // 1. cnt A() 2. cnt unique_ptr<> 3. move cnt unique_ptr<>
    (*ptr_a).print();
    std::cout<<"befor }\n";
  } // 3. dst unique_ptr<> 4. dst ~A()
  std::cout<<"after }\n";
  return 0;
}
