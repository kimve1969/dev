#include<atomic>
#include<thread>
#include<assert.h>


std::atomic<bool> x(false), y(false);
std::atomic<int> z(0);

void write_x(){
  x.store(true, std::memory_order_relaxed);
}

void write_y(){
  y.store(true, std::memory_order_relaxed);
}

void read_x_then_y(){
  while(!x.load(std::memory_order_relaxed));
  if(y.load(std::memory_order_relaxed)) ++z;
}

void read_y_then_x(){
  while(!y.load(std::memory_order_relaxed));
  if(x.load(std::memory_order_relaxed)) ++z;
}

int main(){
  for(int i=0; i<1000; ++i){
    std::thread th1(write_x);
    std::thread th2(write_y);
    std::thread th3(read_x_then_y);
    std::thread th4(read_y_then_x);
    th1.join();
    th2.join();
    th3.join();
    th4.join();

    assert(z.load()!=0);

    x = false;
    y = false;
    z = 0;
  }
  return 0;
}
