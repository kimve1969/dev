#include<iostream>
#include<iomanip>
#include<omp.h>

//#ifdef __intel__
#include<emmintrin.h> // __m128d
#include<immintrin.h> // __m256d
//#endif

#ifdef __arm__
   #include<arm_neon.h> // under ARM CPU
#endif
/*
  see page 65 of book, use intrinsic compile
  2.5.2 SIMD extention

  Character of processor of my notebook Lenovo is: 
  proccessor Intel Core i5 frequance is 1.19 GHz, Turbo mode is 3.6 GHz (4 core)
  TPP (Theoretical pick perfomance) =  GFLOPS

  Character of processor of my Samsung Galaxy Tab S7, SM-T875
  processor Qualcomm Snapdragon 865 Plus, 8 core, 3.1 GHz (1 core - 2.84 GHz, 3 core - 2.42 GHz, 4 core - 1.8 GHz)
  TPP (for 1.8 GHz) = 1.8 GFLOPS
  RAM 6144 Mb, LPDDR4 / 128 Gb

  See CPU flags intrinsics in Linux: cat /proc/cpuinfo
*/

int main(int argc, char* argv[]){
  int N=330'000'123;

  double a = 1.0, result = 0.0, t = 0.0;

  { 
    t = omp_get_wtime();
    
    for(int i=0; i<N; ++i) result += a;
   
    t = omp_get_wtime()-t;
    std::cout<<"1. (no SIMD) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  result = 0;
  {
    // CPU flags: SSE2
    // __mm_set_pd(double e1, double e0) 
    __m128d result_tmp  = _mm_set_pd(0.0, 0.0);
    __m128d aa = _mm_set_pd(a,a);
    
    t = omp_get_wtime();
    
    for(int i=0; i<N/2; ++i){
       result_tmp = _mm_add_pd( result_tmp, aa); // using SIMD (single instruction multiple data) on FMA unit
    }
    result += ((double*)(&result_tmp))[0] + ((double*)(&result_tmp))[1];
    
    for(int i=0; i<N%2; ++i) result += a;
 
    t = omp_get_wtime()-t;
    std::cout<<"2. (SIMD 128 bit) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  // CPU flags: AVX
  // __set256_set_pd(double e3, double e2, double e1, double e0)
    result = 0;
  {
    __m256d result_tmp  = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
    __m256d aa = _mm256_set_pd(a, a, a, a);
    
    t = omp_get_wtime();
    
    for(int i=0; i<N/4; ++i){
       result_tmp = _mm256_add_pd( result_tmp, aa); // using SIMD (single instruction multiple data) on FMA unit
    }
    result += ((double*)(&result_tmp))[0] + ((double*)(&result_tmp))[1] + ((double*)(&result_tmp))[2] + ((double*)(&result_tmp))[3];
    
    for(int i=0; i<N%4; ++i) result += a;
 
    t = omp_get_wtime()-t;
    std::cout<<"2. (SIMD 256 bit) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
  
  return 0;
}
