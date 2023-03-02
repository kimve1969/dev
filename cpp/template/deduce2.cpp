#include<iostream>

/*************************
template<typename T>
void f(ParamType param);

f(expr); // from expr compiler deduces T - first time and param - second time
*************************/
template<typename T>
void f(T& param);
// function should  pass param by reference
// T deduce from expr is ignored references, but not ignored cv !

int g(int){ return 0; }

int main(){
  int x = 0;
  const int cx = x;
  const int &rx = x;
  
  f(x); // expr - int => T - int, param - int&, f<int>(int&) 
  f(cx); // expr - const int => T - const int, param - const int&, f<const int>(const int&)
  f(rx); // expr - const int&  => T - const int, param - const int&, f<const int>(const int&)
  //f(41); // expr - int&&  => T - int, param - int&, f<int>(int&) error: can't bind lvalue with rvalue

  int a[13];
  int *pa = a;

  f(a); // expr - int [13] => T - int [13], param - int [13]& f<int [13]>(int (&)[13])
  f(pa); // expr - int* => T - int*, param - int*&, f<int*>(int*&)

  f(g); // expr - int (int) => T - int (int), param - int (&)(int), f<int (int)>(int (&)(int))
  
  return 0;
}
