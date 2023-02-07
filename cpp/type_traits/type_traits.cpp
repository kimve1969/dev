#include<iostream>
#include<type_traits>

int main(int argc, char *argv[])
{
  struct A{
    A() = default;
    A(const A&) = delete;
    A(A&&) = delete;
    A& operator=(const A&) = default;
    A& operator=(A&&) = delete;
  };
  
  std::cout<<std::boolalpha;
  std::cout<<"is_constructible = "<<std::is_constructible<A>::value<<"\n";
  std::cout<<"is_copy_constructible = "<<std::is_copy_constructible<A>::value<<"\n";
  // call operator()
  std::cout<<"is_move_constructible = "<<std::is_move_constructible<A>()()<<"\n";
  // call implisit opertor bool
  std::cout<<"is_copy_assignable = "<<std::is_copy_assignable<A>()<<"\n";
  // call explisit operator bool
  std::cout<<"is_copy_assignable = "<<std::is_move_assignable<A>().operator bool()<<"\n";
  
  return 0;
}
