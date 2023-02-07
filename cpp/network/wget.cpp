#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

int main(int argc, char* argv[]){
  std::cout<<"Start\n";

  std::vector<std::string> vec;

  std::string s;

  while( std::cin>>s) vec.push_back(std::move(s));

  std::cout<<"vec.size() == "<<vec.size()<<"\n";

  std::sort(vec.begin(), vec.end());

  for(auto &e: vec) std::cout<<e<<"\n";

  std::cout<<std::endl;
  
  std::cout<<"End\n";
  return 0;
}
