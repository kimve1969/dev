#include<iostream>

/*************************
template<typename T>
void f(ParamType param);

f(expr); // from expr compiler deduces T - first time and param - second time
*************************/
template<typename T>
void f(T const& param);
// function should  pass param by reference on const
// T deduce from expr is ignored references, but not ignored cv !

int g(int){ return 0; }

int main(){
  int x = 0;
  int const cx = x;
  int const &rx = x;
  
  f(x); // expr - int => T - int, param - int const&, f<int>(int const&) 
  f(cx); // expr - int const => T - int const, param - int const&, f<int const>(int const&)
  f(rx); // expr - int const&  => T - int const, param - int const&, f<int const>(int const&)
  f(41); // expr - int&&  => T - int, param - int const&, f<int>(int const&)
  
  int a[13];
  int *pa = a;

  f(a); // expr - int [13] => T - int [13], param - int const (&)[13] f<int [13]>(int const (&)[13])
  f(pa); // expr - int* => T - int*, param - int* const&, f<int*>(int* const&)

  f(g); // expr - int (int) => T - int (int), param - int (const &)(int), f<int (int)>(int (const &)(int))
  
  return 0;
}
