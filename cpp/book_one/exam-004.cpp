#include<iostream>
#include<iomanip>
#include<omp.h>
/*
  example from video youtube - define size of cahce L1, L2 and L3

  Character of Samsung Galaxy Tab S7, SM-T875
  L1 - 512 KB (32 16-KB blocks), L2 - 1 MB (64 16-KB), L3 - 4 MB (256 16-KB)
*/

int main(int argc, char* argv[]){
  long N{100}, MAX_BLOCKS{128}, result{0}, asum{0}, *lptr{nullptr};
  constexpr long cnst_lblock = 16*1024/sizeof(long); // 2048 long in 16KB block
  double t{0.0};

  lptr = new long [MAX_BLOCKS * cnst_lblock];
  for(int j = 0; j < MAX_BLOCKS * cnst_lblock; ++j) lptr[j] = j+1; // initialize 0
  
  for(int i = 0; i < MAX_BLOCKS /* 512 blocks of 16KB is equal 8MB */; ++i){
    result = 0, asum = 0;

    t = omp_get_wtime();
    //#pragma omp parallel for num_threads(1)
    for(int k = 0; k < N; ++k){
      for(int m = 0; m < (i+1)*cnst_lblock; ++m) result += lptr[m]; // transfer between RAM & CPU
    }
    t = omp_get_wtime()-t;

    asum = ((lptr[0]+lptr[(i+1)*cnst_lblock-1])*(i+1)/2)*cnst_lblock;
    
    std::cout<<"block "<<(i+1)<<std::setw(10)<<std::setprecision(3)<<", result = "<<result/(asum*N)<<", time: "<<t<<" s, BW (GB\\s) = "<< N*(i+1)*cnst_lblock*sizeof(long)/(t*1e9)<<"\n";
    
  }

  delete[] lptr;

  return 0;
}
