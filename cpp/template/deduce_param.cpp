#include<iostream>
#include<type_traits>
#include<typeinfo>

template<class T>
void f(/*param_type*/T& arg) {
  std::cout<<std::boolalpha;
  std::cout<<"T is - "<<typeid(T).name()<<", is reference - "<<std::is_reference<T>().value<<", is const - "<<std::is_const<T>().value<<std::endl;
  std::cout<<"param_type is reference - "<<std::is_reference<decltype(arg)>().value<<", is const - "<<std::is_const<decltype((arg))>().value<<std::endl;
}

int main(int argc, char* argv[]){
  std::cout<<"void f(T& arg)"<<std::endl;
  
  int x(0);
  const int cx = x;
  const int& rx = x;

  f(x);
  f(cx);
  f(rx);
  
  return 0;
}
