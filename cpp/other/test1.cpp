#include<iostream>
#include<fstream>
#include<cstring>

int main(){
  std::fstream f("1.txt",std::ios_base::in);
  if(!f.is_open()){
    std::cout<<__PRETTY_FUNCTION__<<", "<<strerror(errno)<<"\n";
  }
  else{
  }
  
  std::cout<<errno;
  std::cout<<"End\n";
  std::cout<<__cplusplus<<", "<<__FILE__<<", "<<__LINE__<<", "<<__DATE__<<", "<<__TIME__<<"\n";
  return 0;
}
