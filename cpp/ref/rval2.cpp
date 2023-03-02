#include<iostream>

void f(int &lv){
  std::cout<<"f(int&)\n";
}

void f(int &&rv){
  std::cout<<"f(int &&)\n";
}

int main(){
  
  int x{0};
  f(x);
  f(1);
  
  return 0;
}
