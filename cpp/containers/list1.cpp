#include<iostream>
#include<list>
#include<iterator>
#include<algorithm>

std::ostream& operator<<(std::ostream& out, const std::list<int>& li){
  for(auto &e: li){
    out<<e<<" ";
  }
  return out;
}

int  main(int argc, char *argv[])
{
  std::list<int> li1{1,2,3,4,5};
  std::list<int> li2{10,20,30,40,50};
  
  li1.splice(li1.begin(), li2);
  std::cout<<li1<<"\n-----\n";
  std::cout<<li2<<"\n";
  std::cout<<"li1.size() == "<<li1.size()<<"\n";
  std::cout<<"li2.size() == "<<li2.size()<<"\n";

  std::list<int> li3{11,9,20,18,31,7,4,6,15,5,42,6,34,17,12,85,1,99};
  std::cout<<"list origin: "<<li3<<"\n";
  int in;
  std::cout<<"input integer for devided list on two partition: ";
  std::cin>>in;
  std::cout<<std::endl;
  auto it3 = std::partition(li3.begin(), li3.end(), [&](int &el){ return el<=in; });
  std::cout<<"list after partition: "<<li3<<"\n";
  std::cout<<"partition return is "<<*it3<<"\n";
  
  return 0;
}

