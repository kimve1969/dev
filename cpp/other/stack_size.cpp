/*
  command shell 'ulimit -a' print limits of resources
  default stack size 8192 kb
 */
#include<iostream>

int main(){
  std::cout<<"Start\n";
  char a[8'000*1024];
  return 0;
}
