#include<iostream>

/*************************
template<typename T>
void f(ParamType param);

f(expr); // from expr compiler deduces T - first time and param - second time
*************************/
template<typename T>
void f(T&& param);
// function should pass param as refefences
// 1) lvalue as lvalue, T as lvalue, param as lvalue
// 2) rvalue as rvalue

int main(){
  int x = 0;
  const int cx = x;
  const int &rx = x;
  int *px = &x;
  
  f(x);  // expr - int, lvalue => T - int&, lvalue, param - (int&)& -> int&,, f<int&>(int&) 
  f(cx); // expr - const int, lvalue => T - const int&, param - (const int&)& -> const int&, f<const int&>(const int&)
  f(rx); // expr - const int&, lvalue => T - const int&, param - (const int&)& -> const int&, f<const int&>(const int&)
  f(41); // expr - int&&, rvalue => T - int, param - int&&, f<int>(int&&)
  f(&x); // expr - int*, &x is rvalue (no pointer variable!) => T - int*, param - int*&&, f<int*>(int*&&)
  f(px); // expr - int*, px is lvalue (has pointer variable!) => T - int*&, param - (int*&)&->int*&, f<int*&>(int*&)
  
  return 0;
}
