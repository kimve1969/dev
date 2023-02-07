#include<iostream>

int main(int argc, char* argv[]){

  int N{0};
  int a[100'000];
  int ch;

  std::cout<<"a = "<<(int)'a'<<std::endl;
  std::cout<<"b = "<<(int)'b'<<std::endl;
  std::cout<<". = "<<(int)'.'<<std::endl;
  std::cout<<"# = "<<(int)'#'<<std::endl;

  std::cout<<"a+. = "<<(int)'a'+(int)'.'<<std::endl;
  std::cout<<"a+# = "<<(int)'a'+(int)'#'<<std::endl;

  std::cout<<"b+. = "<<(int)'b'+(int)'.'<<std::endl;
  std::cout<<"b+# = "<<(int)'b'+(int)'#'<<std::endl;

  std::cin>>N;
  for(int i=0; i<N; ++i){
    std::cin>>a[i];
  }
  for(int i=0; i<N; ++i){
    std::cin>>ch;
    a[i] += ch;
  }

  auto prn = [&a](){
  };

  
  return 0;
}
