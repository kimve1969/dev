#include<iostream>

template<typename T, std::size_t N>
constexpr std::size_t get_size_array(T (&)[N]) noexcept {
  return N;
}

int main(){
  int arr1[]={1,2,3,4,5,6,7,8,9,10,11,12,13};
  std::cout<<"size of arr1 == "<<sizeof(arr1)/sizeof(int)<<std::endl;

  int arr2[get_size_array(arr1)]{0};
  std::cout<<"size of arr2 == "<<sizeof(arr2)/sizeof(int)<<std::endl;

  return 0;
}
