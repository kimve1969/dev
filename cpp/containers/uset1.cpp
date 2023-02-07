#include<iostream>
#include<unordered_set>

std::ostream& operator<< (std::ostream &os, std::unordered_set<std::string> &us){
  for(os<<"uset["<<us.size()<<"] {"; auto &e: us)
    os<<e<<", ";
  return os <<"}\n";
}

int main(int argc, char* argv[]){
  std::cout<<"Start\n";
  std::unordered_set<std::string> uset;

  auto prn_size = [&uset](){
    std::cout<<"uset.size() == "<<uset.size()<<"\n";
    std::cout<<"uset.max_size() == "<<uset.max_size()<<"\n";
  };

  prn_size();
  uset.insert("Albert");
  uset.insert("Bob");
  uset.insert("Dilan");
  prn_size();

  std::cout<<uset;
  
  std::cout<<"End\n";
  return 0;
}
