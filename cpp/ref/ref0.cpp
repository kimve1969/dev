#include<iostream>
#include<cassert>

int main(){
  std::cout<<"Start\n";

  int a = 1;
  int b = 10;
  std::cout<<a<<", "<<b<<"\n"; // 1, 10

  int& lva = a;
  lva += 1;
  assert( a == 2 && b == 10);
  std::cout<<a<<", "<<b<<"\n"; // 2, 10

  lva = b;
  assert( a == 10 && b == 10);
  lva += 2;
  assert( a == 12 && b == 10);
  std::cout<<a<<", "<<b<<"\n"; // 12, 10 

  int&& rva = a + 1;
  assert( rva == 13);
  b = rva;
  assert( a == 12 && b == 13);
  std::cout<<a<<", "<<b<<"\n"; // 12, 13

  std::cout<<"End\n";
  return 0;
}
