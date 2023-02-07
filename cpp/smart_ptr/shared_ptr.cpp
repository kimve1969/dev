#include<iostream>
#include<memory>

struct A{
  A(){ std::cout<<"A::A()\n"; }
  ~A(){ std::cout<<"~A::A()\n"; }
};

template<class T>
struct Deleter{
  void operator()(T* p){
    std::cout<<"begin Deleter::operator()(T*)\n";
    delete p;
    std::cout<<"end Deleter::operator()(T*)\n";
  }
};

int main(int argc, char* argv[]){

  auto prn = [](const char* s){ std::cout<<s<<"\n"; };
  std::weak_ptr<A> wptr1;
  
  prn("1.{");
  {
    std::shared_ptr<A> sptr1;
    std::cout<<"size of smart pointer std::shared_ptr<>= "<<sizeof(sptr1)<<"\n";

    prn("2.{");
    {
      //std::shared_ptr<A> sptr2(new A(), Deleter<A>());
      auto sptr2 = std::make_shared<A>();
      wptr1 = sptr2;
  
      std::cout<<"sptr2.use_count() == "<<sptr2.use_count()<<"\n";
      std::cout<<"wptr1.use_count() == "<<wptr1.use_count()<<"\n";
      std::cout<<std::boolalpha<<"wptr1.expired() == "<<wptr1.expired()<<"\n";
      sptr1 = sptr2;
      
      std::cout<<"sptr2.use_count() == "<<sptr2.use_count()<<"\n";
      std::cout<<"sptr1.use_count() == "<<sptr1.use_count()<<"\n";
      std::cout<<"wptr1.use_count() == "<<wptr1.use_count()<<"\n";
      std::cout<<std::boolalpha<<"wptr1.expired() == "<<wptr1.expired()<<"\n";
      prn("2.}");
    }
    std::cout<<"sptr1.use_count() == "<<sptr1.use_count()<<"\n";
    std::cout<<"wptr1.use_count() == "<<wptr1.use_count()<<"\n";
    std::cout<<std::boolalpha<<"wptr1.expired() == "<<wptr1.expired()<<"\n";
  prn("1.}");  
  }
  std::cout<<std::boolalpha<<"wptr1.expired() == "<<wptr1.expired()<<"\n";
 
  return 0;

}
