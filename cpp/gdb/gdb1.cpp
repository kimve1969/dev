#include<iostream>

int *f(){
  int *p = nullptr;
  return p;
}

void print(int *ptr_i){
  std::cout<<*ptr_i<<"\n";
}

int main(int argc, char* argv[]){
  std::cout<<"Start\n";

  print(f());
  
  std::cout<<"End\n";
  return 0;
}
