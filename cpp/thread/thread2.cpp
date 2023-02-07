#include<iostream>
#include<thread>
#include<chrono>
#include<stdexcept>

using namespace std::chrono_literals;

auto h = [](int i){
    std::cout<<"I am from lambda: []( "<<i<<" )\n";
    std::this_thread::sleep_for(1ms);
    throw; // catch throw!
 };
  
int main(int argc, char* argv[]){
  std::cout<<"Start\n";
  std::cout<<"Hardware std::thread::concurrency == "<<std::thread::hardware_concurrency()<<"\n";

  try{
    std::thread th(h,1);
    //th.detach(); // programm is not terminated
    //th.join(); // programm is terminated
    // if didn't call join() programm also is terminated
  }
  catch(const std::exception &e){
    // here is not enter!
    std::cout<<"exception block\n";
    std::cout<<e.what()<<std::endl;
  }
  
  std::cout<<"End\n";
  return 0;
}
