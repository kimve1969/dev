#include<iostream>
#include<thread>
#include<chrono>

using namespace std::chrono_literals;

void f(int i){
  std::cout<<"I am from function: f( "<<i<<" )\n";
  std::this_thread::sleep_for(1s);
  std::cout<<"exit from function: f( "<<i<<" )\n";
}

struct g{
  void operator()(int i){
    std::cout<<"I am from class: g.operator( "<<i<<" )\n";
    std::this_thread::sleep_for(1ms);
    std::cout<<"exit from class: g.operator( "<<i<<" )\n";
  }
};

auto h = [](int i){
    std::cout<<"I am from lambda: []( "<<i<<" )\n";
    std::this_thread::sleep_for(1ms);
    std::cout<<"exit from lambda: []( "<<i<<" )\n";
 };
  
int main(int argc, char* argv[]){
  std::cout<<"Start\n";
  
  std::thread th1(f,1), th2(g(),2), th3(h,3);
  
  th1.join(), std::cout<<"th1.join()\n", th2.join(), std::cout<<"th2.join()\n", th3.join(), std::cout<<"th3.join()\n"; // sync
  //th1.detach(), th2.detach(), th3.detach(); //async
  
  std::cout<<"End\n";
  return 0;
}
