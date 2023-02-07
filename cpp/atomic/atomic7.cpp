#include<iostream>
#include<thread>
#include<atomic>


int gint = 0; // concurrent is bad
//std::atomic<int> gint(0); // concurrent is ok 

void incr(){
  for(int i=0; i<1'000'000; ++i) ++gint;
} 

int main(int argc, char *argv[])
{
  std::thread th[10];

  for(auto &e: th){
    e = std::thread(incr);
  }

  for(auto &e: th){
    e.join();
  }

  std::cout<<"gint = "<<gint<<"\n";
  
  return 0;
}

