#include<iostream>
#include<thread>
#include<atomic>
#include<chrono>
#include<mutex>


int gint = 0;
std::mutex mtx;

void incr_mutex(){
  for(int i=0; i<1'000'000; ++i){
    mtx.lock();
    ++gint;
    mtx.unlock();
  }
} 

// simple spinlock 
class spin{
public:
  spin():b_state(false){}
  void lock(){
    bool expected = false;
    bool desired = true;
    while(!b_state.compare_exchange_strong(expected, desired,
	  std::memory_order_acq_rel /*if compare success*/,
	  std::memory_order_relaxed /*if compare failure*/)){
      //1. if b_state == false == expected
      //   then b_state = desired = true
      //   then while(!true) and exit from circle
      //2. if b_state != false == expected
      //   then expected = b_state = true
      //   enter to  circle
      //   expected = false and repeate circle
      //   STEP 2 is spin looping!
      //   IF b_state set true in other thread
      expected = false;
      std::this_thread::yield();
    }
  }
  void unlock(){
    b_state = false;
  }
private:
  std::atomic<bool> b_state;
};

spin s;

void incr_atomic_spin(){
  for(int i=0; i<1'000'000; ++i){
    s.lock();
    ++gint;
    s.unlock();
  }
}

std::atomic<int> aint(0);

void incr_atomic_var(){
  ++aint;
}

int main(int argc, char *argv[])
{
  std::thread th[10];

  //------------------------------------
  auto t1 = std::chrono::high_resolution_clock::now();
  for(auto &e: th){
    e = std::thread(incr_mutex);
  }

  for(auto &e: th){
    e.join();
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::micro> d1 = (t2-t1);
  std::cout<<"use mutex d1 = "<<d1.count()<<" mcsec."<<std::endl;
  //-----------------------------------
  std::cout<<"gint = "<<gint<<"\n";
  //-----------------------------------
  gint = 0;
  auto t3 = std::chrono::high_resolution_clock::now();
  for(auto &e: th){
    e = std::thread(incr_atomic_spin);
  }

  for(auto &e: th){
    e.join();
  }
  auto t4 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::micro> d2 = (t4-t3);
  std::cout<<"use atomic spin d2 = "<<d2.count()<<" mcsec."<<std::endl;
  //-----------------------------------
  std::cout<<"gint = "<<gint<<"\n";
  //-----------------------------------
  auto t5 = std::chrono::high_resolution_clock::now();
  for(auto &e: th){
    e = std::thread(incr_atomic_var);
  }

  for(auto &e: th){
    e.join();
  }
  auto t6 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::micro> d3 = (t6-t5);
  std::cout<<"use atomic variable d3 = "<<d3.count()<<" mcsec."<<std::endl;
  //-----------------------------------
  std::cout<<"aint = "<<gint<<"\n";
  std::cout<<"d1/d2="<<d1/d2<<", d1/d3="<<d1/d3<<std::endl;
  
  return 0;
}

