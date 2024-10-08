#include<iostream>

void f() noexcept { throw -1; }
void g() {};
void h() noexcept {};
struct S{};
struct G{
  G() noexcept(false);
};
struct H{
  H() noexcept{ f(); };
};
struct A{
  A(){}; // cnt user-defined is noexcept(false)
};
struct B{
  A b;
  B()= default; //
};

int main(){
  constexpr bool b1{ noexcept(f()) }; // true, as it has noexcept specification 
  constexpr bool b2{ noexcept(g()) }; // false, as default function is false noexception
  constexpr bool b3{ noexcept(h()) }; // true
  constexpr bool b4{ noexcept(S()) }; // true, as default cnt S is noexception
  constexpr bool b5{ noexcept(G()) }; // false, as explisit cnt G is noexcept(false)
  constexpr bool b6{ noexcept(H()) }; // true, but when H() thow exception, process call std::terminate()
  constexpr bool b7{ noexcept(B()) }; // false, as cnt A() is user-defined
  
  std::cout<<std::boolalpha;
  
  std::cout<<"b1 == "<<b1<<std::endl;
  std::cout<<"b2 == "<<b2<<std::endl;
  std::cout<<"b3 == "<<b3<<std::endl;
  std::cout<<"b4 == "<<b4<<std::endl;
  std::cout<<"b5 == "<<b5<<std::endl;
  std::cout<<"b6 == "<<b6<<std::endl;
  std::cout<<"b7 == "<<b7<<std::endl;
  
  return 0;
}
