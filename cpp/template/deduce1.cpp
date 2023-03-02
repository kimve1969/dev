#include<iostream>

/*************************
template<typename T>
void f(ParamType param);

f(expr); // from expr compiler deduces T - first time and param - second time
*************************/
template<typename T>
void f(T param);
// function should pass param by value, param is copy of passing argument,
// T deduce from expr by ignored cv and references

int g(int){ return 0; };

int main(){
  int x = 0;
  const int cx = x;
  const int &rx = x;
  const int* const px = &x;

  f(x); // expr - int => T - int, param - int, f<int>(int) 
  f(cx); // expr - const int  => T - int, param - int, f<int>(int)
  f(rx); // expr - const int&  => T - int, param - int, f<int>(int)
  f(41); // expr - int&&  => T - int, param - int, f<int>(int)
  f(&x); // expr - int* => T - int*, param - int*, f<int>(int*), pointer on value !!!
  f(px); // expr - int const* const => T - int const*, param - int const*, f<int const*>(int const*), pointer on const value !!!

  int a[13];
  int *pa = a;

  f(a); // expr - int[13] (array passing as pointer) => T - int*, param - int*, f<int*>(int*)
  f(pa); // expr - int* => T - int*, param - int*, f<int*>(int*)

  f(g); // expr - int (*)(int) => T - int(*)(int), param - int(*)(int), f<int (*)(int)>(int (*)(int)) 
  
  return 0;
}
