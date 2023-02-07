#include<iostream>
#include<memory>

struct A{
  A(){ std::cout<<"A::A()\n"; }
  virtual ~A(){ std::cout<<"A::~A()\n"; }
  virtual void print(){ std::cout<<"A::print()\n"; }
};

struct B: A{
  B(){ std::cout<<"B::B()\n"; }
  ~B(){ std::cout<<"B::~B()\n";  }
  virtual void print() override { std::cout<<"B::print()\n"; }
};

int main(int argc, char* argv[]){

  A* ptr1 = new A();
  std::unique_ptr<A> uptr1(new A());

  std::cout<<"size of pointer of A* = "<<sizeof(ptr1)<<std::endl;
  std::cout<<"size of smart pointer of std::unique_ptr<> = "<<sizeof(uptr1)<<std::endl;
  
  delete ptr1;

  auto uptr2 = std::move(uptr1);
  std::cout<<"pointer 1 = "<<uptr1.get()<<std::endl;
  std::cout<<"pointer 2 = "<<uptr2.get()<<std::endl;

  auto deleter = [](A* ptr){
    std::cout<<"begin of deleter\n";
    delete ptr;
    std::cout<<"end of deleter\n";
  };

  std::unique_ptr<A, decltype(deleter)> uptr3(new A(), deleter);
  
  return 0;
}
