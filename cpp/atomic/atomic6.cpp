#include<iostream>
#include<atomic>
#include<thread>
#include<chrono>


using namespace std::chrono_literals;
std::atomic<bool> b{false};
int arr[100'000'000]{0};

long sum(int *p){
  long s{0};
  /*for(long i=0; i<100'000'000; ++i){
    s+=p[i];
    }*/
  s = p[0]+p[100]+p[1'000]+p[10'000]+p[10'000'000]+p[100'000'000-1];
  return s;
}

void write(){
  arr[100]=1;
  arr[1'000]=1;
  arr[10'000]=1;
  arr[10'000'000]=1;
  arr[100'000'000-1]=1;
  //b.store(true, std::memory_order_relaxed);
  b.store(true, std::memory_order_release);
}

void read(){
  //while(!b.load(std::memory_order_relaxed));
  while(!b.load(std::memory_order_acquire));
  std::cout<<"from read() sum = "<<sum(arr)<<"\n";
  arr[0]=1;
}

int main(){
  std::cout<<"start\n";

  auto start = std::chrono::high_resolution_clock::now();

  std::thread th1(write);
  std::thread th2(read);
  th1.join();
  th2.join();
  
  auto end = std::chrono::high_resolution_clock::now();
  std::cout<<"from main() sum = "<<sum(arr)<<"\n";
  
  std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<<" ms\n";
  
  std::cout<<"end\n";

}
