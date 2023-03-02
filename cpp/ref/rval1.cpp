#include<iostream>

int main(){
  int v{0};
  
  int &lv1{v};
  //int &lv2{1}; // error compiler can't bind non-const lvalue  with rvalue
  
  const int &clv1{v}; // const lvalue <- lvalue
  const int &clv2{1}; // const lvalue <- rvalue
  
  //int &&rv{v}; // error compiler can't bind non-const rvalue with lvalue 
  const int &&crv{1};
  
  return 0;
}
