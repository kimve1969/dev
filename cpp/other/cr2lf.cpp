#include<iostream>
#include<string>

int main(){
  std::string s;
  char ch;
  while(std::cin.get(ch)){
    
    if(ch == (char)0x0D) std::cout<<(char)0x0A;
    else std::cout<<ch;
    //std::cout<<s<<(char)0x0D<<(char)0x0A;
  }
  return 0;
}
