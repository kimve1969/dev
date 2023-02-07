#include<iostream>
#include<type_traits>

// clang decltype.cpp -o decltype -lstdc++ -std=c++17
int g(int x){
  std::cout<<"g("<<x<<")\n";
  return x+5;
};

int main(int argc, char *argv[])
{
  int a{5};
  decltype(a) b = a;

  std::cout<<std::boolalpha<<"std::is_same<>: "<<std::is_same<decltype(a), decltype(b)>::value<<std::endl;
  std::cout<<std::boolalpha<<"std::is_same_v<>: "<<std::is_same_v<decltype(a), decltype(b)><<"\n";

  auto f = [](int x){ return x+3; };

  std::cout<<std::is_same_v<decltype(g), int(int)><<std::endl;

  decltype((g)) g1{g};
  std::cout<<g1(2)<<std::endl;

  // g(0) is not called
  decltype(g(0)) g2 = 3;

  struct HasCnt{
    HasCnt(){
      std::cout<<"HasCnt()\n";
    }
    int foo(){return 1;}
  };

  // HasCnt() is not called
  decltype(HasCnt().foo()) g3 = 2;
  
  return 0;
}

