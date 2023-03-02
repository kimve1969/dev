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
  int &ri = i;
  const int& cri = i;
  
  f(ri, 1);
  f(cri, 2);
  f(1, 3);

  int &&rri = 1;
  f(rri, 4); // call f(int&)
  f(std::move(rri), 5); // call f(int&&)
  
  return 0;
}
