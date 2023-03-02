#include<iostream>
#include<type_traits>
#include<vector>
#include<utility>

template<typename Container, typename Index>
  auto get_elem1(Container &c, Index i){
    // any to do
    return c[i]; // c[i] -> T&, T is type of elem container, auto cast reference -> return T 
}

// C++11
template<typename Container, typename Index>
auto get_elem2(Container &c, Index i) ->decltype(c[i]) {
    // any to do
    return c[i]; // c[i] -> T&, T is type of elem container, decltype (c[i]) -> return T&
}

// C++14
template<typename Container, typename Index>
decltype(auto) get_elem3(Container &c, Index i) {
    // any to do
    return c[i]; // c[i] -> T&, T is type of elem container, decltype(auto) -> return T&
}

std::vector<int> make_vector(){
  std::vector<int> v{6,7,8,9,10};
  return v; // RVO
}

template<typename Container, typename Index>
decltype(auto) get_elem4(Container &&c, Index i) {
    // any to do
  return std::forward<Container>(c)[i]; // c[i] -> T&, T is type of elem container, decltype(auto) -> return T&
}

int main(){
  const int x = 0;
  decltype(x) y = 1;

  static_assert( std::is_same<const int, decltype(y)>::value );
  std::cout<<"typeid(y) == "<<typeid(y).name()<<std::endl;


  std::vector<int> v{1,2,3,4,5};
  static_assert( std::is_same<int, decltype( get_elem1(v,0))>::value );
  // get_elem1(v,0) = 1; error: get_elem1() return by value, when it on left is rvalue!
  
  static_assert( std::is_same<int&, decltype( get_elem2(v,0))>::value );
  get_elem2(v,0) = 1; // get_elem2() return lvalue!
  
  static_assert( std::is_same<int&, decltype( get_elem3(v,0))>::value );
  get_elem3(v,0) = 0; // get_elem3() return lvalue!

  // get_elem3( std::vector<int>{6,7,8,9,10}, 0); error: can't bind non-const lvalue with rvalue
  // get_elem3( make_vector(), 0) error: too..
  
  get_elem4(v,0) = 1; // pass lvalue vector
  get_elem4(std::vector<int>{6,7,8,9,10}, 0); // pass rvalue vector
  
  return 0;
}
