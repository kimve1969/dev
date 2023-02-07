#include<atomic>
#include<thread>
#include<assert.h>

std::atomic<bool> x(false), y(false);
std::atomic<int> z(0);

void write_x_then_y(){
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x(){
  while(!y.load(std::memory_order_relaxed));
  if(x.load(std::memory_order_relaxed)) ++z;
}


int main(){
  std::thread th1(write_x_then_y);
  std::thread th2(read_y_then_x);
  th1.join();
  th2.join();

  assert(z.load()!=0);
  return 0;
}
