#include<iostream>
#include<type_traits>

template<class T>
void f(/*param_type*/ T&& arg) {
  std::cout<<std::boolalpha;
  std::cout<<"T is - "<<typeid(T).name()<<", is const - "<<std::is_const<T>().value<<std::endl;
  std::cout<<"param_type is lvalue  reference - "<<std::is_lvalue_reference<decltype(arg)>().value<<", is rvalue reference - "<<std::is_rvalue_reference<decltype(arg)>().value<<", is const - "<<std::is_const<decltype(arg)>().value<<std::endl;
}

int main(int argc, char* argv[]){
  std::cout<<"void f(T&& arg)"<<std::endl;
  
  int x(0);
  const int cx = x;
  const int& rx = x;

  f(x);
  f(cx);
  f(rx);
  f(27); // 27 - rvalue
  
  return 0;
}
