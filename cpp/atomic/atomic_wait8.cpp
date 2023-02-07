#include<iostream>
#include<atomic>
#include<thread>
#include<chrono>


std::atomic<bool> b(false);
int arr[3]={1,2,3};
void f1(){
  arr[0]=3;
  b = true;
  b.notify_one();
  std::cout<<"data is prepared & notify one\n";
}

void f2(){
  std::cout<<"wait data..,\n";
  b.wait(false); // thread bloks on b.wait() and ready notify() from another thread
  arr[2]=1;
  std::cout<<"data is given\n";
}

int main(int argc, char** argv){
  using namespace std::chrono_literals;
  
  std::cout<<"Start\n";
  std::cout<<"data is begining\n";

  for(auto &e: arr){
    std::cout<<e<<", ";
  }
  std::cout<<"\n";
  
  std::thread t1(f1),t2(f2);
  
  t1.join();
  std::this_thread::sleep_for(1000ms);
  t2.join();
  
  for(auto &e: arr){
    std::cout<<e<<", ";
  }
  
  std::cout<<"\nEnd\n";
  
  return 0;
}
