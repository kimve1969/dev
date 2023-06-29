#include<iostream>


int main(int argc, char* argv[]){
  // see in linux: cat /proc/cpuinfo
  std::cout<<"sse: "<<__builtin_cpu_supports("sse")<<std::endl;
  std::cout<<"sse2: "<<__builtin_cpu_supports("sse2")<<std::endl;
  std::cout<<"avx: "<<__builtin_cpu_supports("avx")<<std::endl;
  std::cout<<"avx2: "<<__builtin_cpu_supports("avx2")<<std::endl;
  std::cout<<"avx512f: "<<__builtin_cpu_supports("avx512f")<<std::endl;
  
  return 0;
}
