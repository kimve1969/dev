#include<iostream>
#include<iomanip>
#include<omp.h>
#include<string>
/*
  example from video youtube - define size of cahce L1, L2 and L3  
*/

#ifdef __GNUC__
#define ALIGN(N) __attribute__((aligned(N))) // Linux
#else
#define ALIGN(N) __declspec(align(N)) // Windows
#endif

int main(int argc, char* argv[]){
  long N{100}, SIZE_LBLOCK{0}, result{0}, asum{0}, *lptr{nullptr};
  double t{0.0};

  if(argc < 2){
    std::cout<<"you must pass parameter - size of block bytes\n";
    return -1;
  }

  try{
    SIZE_LBLOCK = std::stoi(argv[1])/sizeof(long);
  }
  catch(std::invalid_argument){
    std::cout<<"parameter size of block bytes is invalid\n";
    return -1;
  }
  catch(std::out_of_range){
    std::cout<<"parameter size of block bytes is invalid: out of range\n";
    return -1;
  }
    
  lptr = new ALIGN(64) long [SIZE_LBLOCK];
  for(int j = 0; j < SIZE_LBLOCK; ++j) lptr[j] = j+1; // initialize 1,2,3,...
  
  t = omp_get_wtime();
  
  for(int k = 0; k < N; ++k){
    for(int OFFSET = 0; OFFSET < 32; ++OFFSET){ 
      for(int m = OFFSET; m < SIZE_LBLOCK; m += 32) {
	// unorder access memory: 0,32,64..., 1,33,65..., 2,34,66...  31,63,95
	// it need that CPU don't optimize arifmetic progression
	result += lptr[m]; // transfer between RAM & CPU      
      }
    }
  }
  
  t = omp_get_wtime()-t;

  asum = ((lptr[0]+lptr[SIZE_LBLOCK-1])/2)*SIZE_LBLOCK;
    
  std::cout<<"SIZE OF BLOCK =  "<<SIZE_LBLOCK*sizeof(long)<<" bytes"<<std::setw(10)<<std::setprecision(3)<<", result = "<<result/(asum*N)<<", time: "<<t<<" s, BW (GB\\s) = "<< N*SIZE_LBLOCK*sizeof(long)/(t*1e9)<<"\n";

  delete[] lptr;

  return 0;
}
