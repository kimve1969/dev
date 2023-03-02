#include<iostream>
#include<string>
  
int main(){
  std::cout<<"********* rvalue and const lvalue can extend lifetime of temprory objects ***********\n";

  std::string s1 = "Test";
  //std::string &&r1 = s1; // error: can't bind rvalue with lvalue
  const std::string &r1 = s1;
  //r1 += " is modified!"; // error: can't modify

  std::string &&r2 = s1 + " is "; // okay: extended lifitime temprory object
  r2 += " modified"; // okay: can modify
  std::cout<<r2<<"\n";
  
  std::cout<<"**************************** End ******************************\n";
  return 0;
}
