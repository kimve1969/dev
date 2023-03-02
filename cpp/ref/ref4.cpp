#include<iostream>
#include<string>

char& char_number(std::string &s, int n){
  return s.at(n); // if function return type is lvalue, then value (expresion 's.at(n)') becomes lvalue
}

int main(){
  std::cout<<"*********** If function return type is lvalue, then value (expresion 's.at(n)') becomes lvalue *************\n ";
  std::string str("Test!");

  char_number(str,1)='a';
  std::cout<<str<<std::endl;
  
  return 0;
}
