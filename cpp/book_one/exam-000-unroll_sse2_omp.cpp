#include<iostream>
#include<iomanip>
#include<omp.h>
#include<cmath>

//#ifdef __intel__
#include<emmintrin.h> // __m128d
//#endif
#ifdef __arm__
   #include<arm_neon.h> // under ARM CPU
#endif

#ifdef __GNUC__
#define ALIGN(N) __attribute__((aligned(N))) // Linux
#else
#define ALIGN(N) __declspec(align(N)) // Windows
#endif

int main(int argc, char* argv[]){

  std::cout<<"*********************** ADD optimization using unrolling loop *************************\n";
    
  ALIGN(64) double a = 1.0;
  ALIGN(64) double result = 0.0;
  ALIGN(64) double t1 = 0.0, t2 = 0.0;
  ALIGN(64) int N=330'000'123;

  {
    t1 = omp_get_wtime();

    for(int i=0; i<N; ++i) result += a;
      
    t1 = omp_get_wtime()-t1;
    std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t1<<" s, GFLOPS: "<<N/(t1*1E9)<<"\n";
  }
 
  result = 0.0;
  double tomp = omp_get_wtime();

  #pragma omp parallel num_threads(2)
  {
    
    ALIGN(64) __m128d result_tmp[2];
    result_tmp[0]  = _mm_set_pd(0.0, 0.0);
    result_tmp[1]  = _mm_set_pd(0.0, 0.0);
    
    ALIGN(64) __m128d aa = _mm_set_pd(a,a);

    for(int i=0; i<N/8; ++i){
      // N/2 unrolling loop + N/2 SIMD SSE2 + N/2 THREADS
      result_tmp[0] = _mm_add_pd( result_tmp[0], aa);
      result_tmp[1] = _mm_add_pd( result_tmp[1], aa);
    }

    #pragma omp atomic update
    result += ((double*)(&result_tmp[0]))[0] + ((double*)(&result_tmp[0]))[1] +
              ((double*)(&result_tmp[1]))[0] + ((double*)(&result_tmp[1]))[1];

    #pragma omp single // another may be release in outer block
    for(int i=0; i<N%8; ++i){
      result += a;
    }
  }

  tomp = omp_get_wtime()-tomp;  
  std::cout<<"2. (2 UNROLL + 2 SIMD SSE2 + 2 THREADS) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<tomp<<" s, GFLOPS: "<<N/(tomp*1E9)<<", accelerate = "<<std::setprecision(3)<<t1/tomp<<std::endl;




  result = 0.0;
  tomp = omp_get_wtime();

  #pragma omp parallel num_threads(2)
  { 
    ALIGN(64) __m128d result_tmp[4];
    result_tmp[0]  = _mm_set_pd(0.0, 0.0);
    result_tmp[1]  = _mm_set_pd(0.0, 0.0);
    result_tmp[2]  = _mm_set_pd(0.0, 0.0);
    result_tmp[3]  = _mm_set_pd(0.0, 0.0);
    
    ALIGN(64) __m128d aa = _mm_set_pd(a,a);

    for(int i=0; i<N/16; ++i){
      // N/4 unrolling loop + N/2 SIMD SSE2 + N/2 THREADS
      result_tmp[0] = _mm_add_pd( result_tmp[0], aa);
      result_tmp[1] = _mm_add_pd( result_tmp[1], aa);
      result_tmp[2] = _mm_add_pd( result_tmp[2], aa);
      result_tmp[3] = _mm_add_pd( result_tmp[3], aa);
    }
       
    result += ((double*)(&result_tmp[0]))[0] + ((double*)(&result_tmp[0]))[1] +
              ((double*)(&result_tmp[1]))[0] + ((double*)(&result_tmp[1]))[1] +
              ((double*)(&result_tmp[2]))[0] + ((double*)(&result_tmp[2]))[1] +
              ((double*)(&result_tmp[3]))[0] + ((double*)(&result_tmp[3]))[1];

    #pragma omp single // another may be release in outer block
    for(int i=0; i<N%16; ++i){
      result += a;
    }
  }   

  tomp = omp_get_wtime()-tomp;
  std::cout<<"2. (4 UNROLL + 2 SIMD SSE2 + 2 THREADS) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<tomp<<" s, GFLOPS: "<<N/(tomp*1E9)<<", accelerate = "<<std::setprecision(3)<<t1/tomp<<std::endl;
  
  return 0;
}
