#include<atomic>
#include<iostream>
#include<thread>
#include<type_traits>


struct A{
  char a;
  int i1: 4;
  int i2: 4;
  int i3: 8;
  int i4: 16;
  int i5: 1;
};

std::atomic<A> atomic_A;

int main(int argc, char* arv[]){
  std::cout<<"Start\n";

  std::cout<<"size of A = "<<sizeof(A)<<std::endl;
  std::cout<<"A.is_lock_free() = "<<std::boolalpha<<atomic_A.is_lock_free()<<std::endl;
  std::cout<<"std::is_trivial<A>::value = "<<std::is_trivial<A>::value<<"\n";
  
  std::cout<<"Stop\n";
  return 0;
}
