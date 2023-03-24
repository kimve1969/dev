#include<iostream>
#include<iomanip>
#include<omp.h>
#include<emmintrin.h>
/*
  see page 65 of book, use intrinsic compile
  2.5.2 SIMD extention

  Character of processor of my notebook Lenovo is: 
  proccessor Intel Core i5 frequance is 1.19 GHz, Turbo mode is 3.6 GHz (4 core)
  TPP (Theoretical pick perfomance) = D (number of bytes) * F (frequance) = 1 bytes * 1.19E9 Hz = 1.19 GFLOPS
  This example perfomance = 0.38 GFLOPS, only 32% from TPP!

  Character of processor of my Samsung Galaxy Tab S7, SM-T875
  processor Qualcomm Snapdragon 865 Plus, 8 core, 3.1 GHz (1 core - 2.84 GHz, 3 core - 2.42 GHz, 4 core - 1.8 GHz)
  TPP (for 1.8 GHz) = 1.8 GFLOPS
  RAM 6144 Mb, LPDDR4 / 128 Gb
*/
int N=330'000'123;

template<int K> void tadd(){  
    double a = 1.0, result = 0.0, t = 0.0;
    double result_tmp[K];
    
    t = omp_get_wtime();
    
    for(int i=0; i<K; ++i) result_tmp[i]=0;
      
    for(int i=0; i<N/K; ++i){
      for(int j=0; j<K; ++j) result_tmp[j] += a; // compile with option -O3 unroll loop
    }
      
    for(int i=0; i<N%K; ++i) result += a;
      
    for(int j=0; j<K; ++j) result += result_tmp[j];
    
    t = omp_get_wtime()-t;
    std::cout<<K<<". result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
}

template<int K> void tadd_m128d(){  
    double a = 1.0, result = 0.0, t = 0.0;
    
    __m128d result_tmp[K];
    for(int i=0; i<K; ++i) result_tmp[i] = _mm_set_pd(0.0, 0.0);

    __m128d aa = _mm_set_pd(a,a);
    
    t = omp_get_wtime();
    
    for(int i=0; i<N/(K*2); ++i){
      for(int j=0; j<K; ++j) _mm_add_pd( result_tmp[j], aa); // using SIMD (single instruction multiple data) on FMA unit
    }
      
    for(int i=0; i<N%(K*2); ++i) result += a;
      
    for(int j=0; j<K; ++j) result += ((double*)(&result_tmp[j]))[0] + ((double*)(&result_tmp[j]))[1];
    
    t = omp_get_wtime()-t;
    std::cout<<K<<". result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
}

int main(int argc, char* argv[]){
  
  std::cout<<"************************* ADD optimization by using unrolling loop **********************************\n";
  tadd<1>();
  tadd<3>();
  
  std::cout<<"************************* ADD optimization by using instrinsics compiler as SIMD extension FMA unit **********************************\n";
  tadd_m128d<1>();
  tadd_m128d<3>();
  
  return 0;
}
