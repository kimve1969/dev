#include<iostream>

/*************************
template<typename T>
void f(ParamType param);

f(expr); // from expr compiler deduces T - first time and param - second time
*************************/
template<typename T>
void f(T* param);
// function should pass param by pointer T
// T deduce from expr is ignored references * and not ignored cv

int g(int) { return 0; }

int main(){
  int x = 0;
  int const* cx = &x;
  int &rx = x;
  
  //f(x); // expr - int => T - int, param - int*, f<int>(int*): error mismatch type int* and int
  f(&x); // expr - int* => T - int, param - int*, f<int>(int*)
  f(cx); // expr - const int* => T - const int, param - const int*, f<const int>(const int*)
  //f(rx); // expr - int& => error: mismatch type T* and int&

  int a[13];
  int const *pa = &a[0];
  int **ppa = (int**)(&pa);

  f(a); // expr - int [13] => T - int, param - int*, f<int>(int)
  f(pa); // expr - int const* => T - int const, param - int const*, f<int const>(int const*)
  f(
    ppa); // expr - int** => T - int*, param - int**, f<int*>(int**)
  f(g); // expr - int (int) => T - int (int), param - int (*)(int), f<int (int)>(int (*)(int)
  
  return 0;
}
