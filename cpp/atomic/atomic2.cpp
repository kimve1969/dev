#include<iostream>
#include<thread>
#include<atomic>

const int SIZE = 10;
std::atomic<int> x[SIZE];
std::atomic<int> y[SIZE];
std::atomic<int> z[SIZE];
std::atomic<bool> b(false);

void fill(std::atomic<int> a[]){
  while(!b.load(std::memory_order_relaxed) );
  for(int i=0; i<SIZE; ++i){
    a[i].store(i, std::memory_order_relaxed);
  }
}
void print(){
  while(!b.load( std::memory_order_relaxed) );
  for(int i=0; i<SIZE; ++i){
    std::cout<<"( "<<x[i].load( std::memory_order_acquire )<<", " \
	     <<y[i].load( std::memory_order_relaxed ) <<", " \
	     <<z[i].load( std::memory_order_relaxed ) <<" ), ";
  }
  std::cout<<std::endl;
}

int main(){
  //b.store( false, std::memory_order_relaxed );  
  std::thread th1(fill, x);
  std::thread th2(fill, y);
  std::thread th3(fill, z);
  std::thread th4(print);

  b.store(true, std::memory_order_relaxed);
  
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  
  return 0;
}
