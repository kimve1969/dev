#include<iostream>
#include<atomic>
#include<string>

int main(int argc, char *argv[])
{
  std::atomic<int> i1{0};

  // std::atomic<T>::is_always_lock_free is member defined in c++17
  std::cout<<std::boolalpha<<std::atomic<int>::is_always_lock_free<<"\n";

  struct A{
    int a;
    int b[10];
    //A(const A&) = delete;
    // ...
    //
    //
    //
    //A& operator=(const A&) = delete;
  };
  std::cout<<std::boolalpha<<std::atomic<A>::is_always_lock_free<<"\n";
  
  return 0;
}
