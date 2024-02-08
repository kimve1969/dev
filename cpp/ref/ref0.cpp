#include<iostream>

int main(){
  std::cout<<"Start\n";

  int a = 1;
  int b = 10;
  std::cout<<a<<", "<<b<<"\n"; // 1, 10

  int& lva = a;
  lva += 1;
  std::cout<<a<<", "<<b<<"\n"; // 2, 10

  lva = b;
  lva += 2;
  std::cout<<a<<", "<<b<<"\n"; // 12, 10 

  int&& rva = a + 1;
  b = rva;
  std::cout<<a<<", "<<b<<"\n"; // 12, 13

  std::cout<<"End\n";
  return 0;
}
