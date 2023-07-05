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
  {
    t2 = omp_get_wtime();
    
    ALIGN(64) __m128d result_tmp[2];
    result_tmp[0]  = _mm_set_pd(0.0, 0.0);
    result_tmp[1]  = _mm_set_pd(0.0, 0.0);
    
    ALIGN(64) __m128d aa = _mm_set_pd(a,a);
    
    for(int i=0; i<N/4; ++i){
      // N/2 unrolling loop + N/2 SIMD SSE2
      result_tmp[0] = _mm_add_pd( result_tmp[0], aa);
      result_tmp[1] = _mm_add_pd( result_tmp[1], aa);
    }
      
    for(int i=0; i<N%4; ++i) result += a;
     
    result += ((double*)(&result_tmp[0]))[0] + ((double*)(&result_tmp[0]))[1] +
              ((double*)(&result_tmp[1]))[0] + ((double*)(&result_tmp[1]))[1];
    
    t2 = omp_get_wtime()-t2;
    std::cout<<"2. (2 UNROLL + 2 SIMD SSE2) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t2<<" s, GFLOPS: "<<N/(t2*1E9)<<", accelerate = "<<std::setprecision(3)<<t1/t2<<std::endl;
  }

  result = 0.0;
  {
    t2 = omp_get_wtime();
    
    ALIGN(64) __m128d result_tmp[4];
    result_tmp[0]  = _mm_set_pd(0.0, 0.0);
    result_tmp[1]  = _mm_set_pd(0.0, 0.0);
    result_tmp[2]  = _mm_set_pd(0.0, 0.0);
    result_tmp[3]  = _mm_set_pd(0.0, 0.0);
    
    ALIGN(64) __m128d aa = _mm_set_pd(a,a);

   for(int i=0; i<N/8; ++i){
      // N/4 unrolling loop + N/2 SIMD SSE2
      result_tmp[0] = _mm_add_pd( result_tmp[0], aa);
      result_tmp[1] = _mm_add_pd( result_tmp[1], aa);
      result_tmp[2] = _mm_add_pd( result_tmp[2], aa);
      result_tmp[3] = _mm_add_pd( result_tmp[3], aa);
    }
      
    for(int i=0; i<N%8; ++i) result += a;
     
    result += ((double*)(&result_tmp[0]))[0] + ((double*)(&result_tmp[0]))[1] +
              ((double*)(&result_tmp[1]))[0] + ((double*)(&result_tmp[1]))[1] +
              ((double*)(&result_tmp[2]))[0] + ((double*)(&result_tmp[2]))[1] +
              ((double*)(&result_tmp[3]))[0] + ((double*)(&result_tmp[3]))[1];
    
    t2 = omp_get_wtime()-t2;
    std::cout<<"2. (4 UNROLL + 2 SIMD SSE2) result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t2<<" s, GFLOPS: "<<N/(t2*1E9)<<", accelerate = "<<std::setprecision(3)<<t1/t2<<std::endl;
  }
  
  return 0;
}
