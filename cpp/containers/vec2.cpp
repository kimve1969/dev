#include<iostream>
#include<functional>
#include<algorithm>

template<class U, class W>
class MyCompType
{
public:
  MyCompType(int x):xcount(x){}
  bool operator()(U &l, W &r)
  {
    xcount++;
    return l < r;
  }
  int get_count(){ return xcount; }
private:
  int xcount;
};

int main(int argc, char* argv[])
{
  std::cout<<"Start\n";
  
  int i = 5;
  std::reference_wrapper<int> irw(i);
  (int&)irw = 3; // explicit call opertor T&()

  std::cout<<"i = "<<i<<", irw = "<<irw<<"\n";

  auto prn = [](auto &s, auto &c)
  {
    int size = c.size();
    std::cout<<s<<"{";
    for(auto &e : c) std::cout<<e<<(--size ? "," : "");
    std::cout<<"}\n";
  };
  
  std::vector<int> v{4,3,2,1,5,6,8,7};
  prn("v = ", v);

  std::vector< std::reference_wrapper<int> > vrw;
  for(auto &e: v) vrw.push_back(e);

  MyCompType<int, int> comparator{0};
  std::reference_wrapper< MyCompType<int, int> > comp_rw(comparator);
  std::sort(v.begin(), v.end(), comp_rw); // if will pass comparator, then get_count()=0!

  std::cout<<"number step of sort() is "<<comparator.get_count()<<"\n";    
  prn("vrw = ", vrw);
  prn("v = ", v);
  
  return EXIT_SUCCESS;
}
