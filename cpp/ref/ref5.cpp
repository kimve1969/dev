#include<iostream>

/*template<typename T>
void f(T){    // error: ambiquos overload whith f(T&)
  std::cout<<"f(T)\n";
};*/

template<typename T>
void f(T&, int i){
  std::cout<<i<<". f(T&)\n";
};

template<typename T>
void f(const T&, int i){
  std::cout<<i<<". f(const T&)\n";
};

template<typename T>
void f(T&&, int i){
  std::cout<<i<<". f(T&&)\n";
};

int main(){
  std::cout<<"******************** overload lvalue and rvalue ******************\n";

  int i = 1;
  const int ci = 2;
  
  f(i, 1);  // f(int&, int)
  f(ci, 2); // f(const int&, int)
  f(1, 3);  // f(int&&, int)

  int &&rvi = 1;
  f(rvi, 4); // f(int&, int)
  f(std::move(rvi), 5); // f(int&&, int)
  
  return 0;
}
