#include<iostream>
#include<type_traits>


int main(){
  std::cout<<"*****************  Reference collapsing *******************************\n";

  //int& a[3]; // error: array of references
  //int&* a;   // error: pointer to reference
  //int& &a;   // error: reference of reference
  
  typedef int & lref;
  typedef int && rref;

  int n{0};
  lref&  r1 = n; // int&& -> int, value
  lref&& r2 = n; // int &&& -> int &, reference lvalue
  rref&  r3 = n; // int &&& -> int &, reference lvalue
  rref&& r4 = 1; // int && && -> int &&, reference rvalue

  std::cout<<std::boolalpha;
  std::cout<<"lref is lvalue - "<<std::is_lvalue_reference<lref>::value<<std::endl;
  std::cout<<"lref& is lvalue - "<<std::is_lvalue_reference<lref&>::value<<std::endl;
  std::cout<<"lref&& is lvalue - "<<std::is_lvalue_reference<lref&&>::value<<std::endl;
  std::cout<<"rref is rvalue - "<<std::is_rvalue_reference<rref>::value<<std::endl;
  std::cout<<"rref& is lvalue - "<<std::is_lvalue_reference<rref&>::value<<std::endl;
  std::cout<<"rref&& is rvalue - "<<std::is_rvalue_reference<rref&&>::value<<" !!! "<<std::endl;

  std::cout<<"********************* End *************************************\n";
  return 0;
}
