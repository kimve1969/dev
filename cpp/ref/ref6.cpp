#include<iostream>
#include<type_traits>
template<typename T>
void f(T&& arg, const char* s){
  std::cout<<std::boolalpha;
  std::cout<<s<<"\t\t is prvalue \t"<<std::is_rvalue_reference<decltype(arg)>::value<<"\n";
}

int main(){
  std::cout<<"********************* prvalue expressions ***********************\n";
  std::cout<<"*********** see https://en.cppreference.com/w/cpp/language/value_category ************\n";

  
  f(42, "literal 42");
  f(true, "boolean true");
  f(nullptr, "nullptr");
  f("my string", "const char*");
  int x=1, y=1;
  f(x++, "x++");
  f(x--, "x--");
  f(++x, "++x");
  f(--x, "--x");
  f(x+y, "all built-in arithmetic expressions");
  bool a{true},b{false};
  f(a && b, "all built-in logical expressions");

  struct S{
    enum enum_t{A, B} e{B};
    void f(int);
  } s;
  
  f(s.e, "s.e the member of object expression");
  f(s.e, "s.f the member of object expression");

  enum enum_t{A, B, D, E} e{D};
  f(e, "e is enumeration");
  
  return 0;
}
