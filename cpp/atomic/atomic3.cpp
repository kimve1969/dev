#include<iostream>
#include<atomic>
#include<thread>
/********************

 3 threads creates snapshots of atomic x,y,z and modify its
 2 threads creates snapshots of atomic x,y,x only read
 every snapshot has 10 snap
 print() output 5 snaphots  

 *******************/

std::atomic<int> x(0), y(0), z(0);
std::atomic<bool> b(false);
//std::memory_order order = std::memory_order_relaxed;
std::memory_order order = std::memory_order_seq_cst;
//std::memory_order order = std::memory_order_acquire;
struct s{
  int x;
  int y;
  int z;
};
const int cnst_size = 10;
s arr1[cnst_size], arr2[cnst_size], arr3[cnst_size], arr4[cnst_size], arr5[cnst_size];

void incriment(s* ps, std::atomic<int>* par){
  while(!b) std::this_thread::yield();
  for(int i=0; i<cnst_size; ++i){
    ps[i].x = x.load(order);
    ps[i].y = y.load(order);
    ps[i].z = z.load(order);
    par->store(i+1, order);
    std::this_thread::yield();
  }
}

void read(s* ps){
  while(!b) std::this_thread::yield();
  for(int i=0; i<cnst_size; ++i){
    ps[i].x = x.load(order);
    ps[i].y = y.load(order);
    ps[i].z = z.load(order);
    std::this_thread::yield();
  }
}

void print(s* ps){
  for(int i=0; i<cnst_size; ++i){
    if(i!=0) std::cout<<",";
    std::cout<<"( "<<ps[i].x<<", "<<ps[i].y<<", "<<ps[i].z<<" )";
  }
  std::cout<<"\n\n";
}

int main(){
  std::thread th1(incriment, arr1, &x);
  std::thread th2(incriment, arr2, &y);
  std::thread th3(incriment, arr3, &z);
  std::thread th4(read, arr4);
  std::thread th5(read, arr5);

  b = true;
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  th5.join();

  print(arr1);
  print(arr2);
  print(arr3);
  print(arr4);
  print(arr5);
  
  return 0;
}
