#include<iostream>
#include<iomanip>
#include<omp.h>

//#ifdef __intel__
#include<emmintrin.h> // __m128d
//#endif

#ifdef __arm__
   #include<arm_neon.h> // under ARM CPU
#endif
/*
  see page 65 of book, use intrinsic compile
  2.5.2 SIMD extention

  See CPU flags intrinsics in Linux: cat /proc/cpuinfo
  Help of intrinsics: https://www.laruence.com/sse/
*/

int main(int argc, char* argv[]){
  int N=330'000'123;

  double a = 1.0, result = 0.0, t1 = 0.0, t2 = 0.0;

  { 
    t1 = omp_get_wtime();
    
    for(int i=0; i<N; ++i) result += a;
   
    t1 = omp_get_wtime()-t1;
    std::cout<<"1. (no SIMD) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t1<<" s, GFLOPS: "<<N/(t1*1E9)<<"\n";
  }

  result = 0;
  {
    // CPU flags: SSE2
    // __mm_set_pd(double e1, double e0)
    __m128d result_tmp  = _mm_set_pd(0.0, 0.0);
    __m128d aa = _mm_set_pd(a,a);
    
    t2 = omp_get_wtime();
    
    for(int i=0; i<N/2; ++i){
       result_tmp = _mm_add_pd( result_tmp, aa); // SIMD SSE2
    }
    result += ((double*)(&result_tmp))[0] + ((double*)(&result_tmp))[1];
    
    for(int i=0; i<N%2; ++i) result += a;
 
    t2 = omp_get_wtime()-t2;
    std::cout<<"2. (SIMD 128 bit SSE2) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t2<<" s, GFLOPS: "<<N/(t2*1E9)<<", accelerate = "<<std::setprecision(3)<<t1/t2<<std::endl;
  }
  
  return 0;
}
