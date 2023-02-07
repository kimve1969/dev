#include<iostream>
#include<vector>

std::ostream& operator<< (std::ostream &os, std::vector<int> &v){
  os<<"size, capacity ["<<v.size()<<", "<<v.capacity()<<"], T* == ["<<v.data()<<"] {";
  for(auto &e: v) os<<e<<", ";
  return os<<"}\n";
}

int main(int argc, char* argv[]){
  std::cout<<"Start\n";
  std::vector<int> v1, v2(10), v3{1,2,3};

  auto prn = [&](){ std::cout<<"v1 == "<<v1<<"v2 =="<<v2<<"v3 == "<<v3; };

  prn();

  v1.push_back(1);
  v2.push_back(1);
  v3.push_back(4);

  prn();
  

  std::cout<<"End\n";
  return 0;
}
