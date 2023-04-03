#include<iostream>
#include<iomanip>
#include<omp.h>
/*
  see page 61 of book

  Character of processor of my Samsung Galaxy Tab S7, SM-T875
  processor Qualcomm Snapdragon 865 Plus, 8 core:
     1 core - ARM Cortex-A77 2.84 GHz, 3 core - ARM Cortex-A77 2.42 GHz, 4 core - ARM Cortex-A55 1.8 GHz
  ARM Cortex-A77:
     CPU 3.3 GHz, L1 128KB, L2 256-512KB, L3 1-4MB
     Instruction set ARMv8-A, extention ARMv8.2A
  ARM Cortex-A55: 
     CPU 1.25-1.6 GHz, L1 32-128KB, L2 64-256 KB, L3 512KB-4MB 
     Instruction set ARMv8.2A
  RAM 6144 Mb, LPDDR4, Band Width = 25,6 GB/s
  L1 - 512 Kb, L2 - 1 Mb, L3 - 4 Mb

  TPP (Theorety Pick Perfomance) = N * CPU (Hz) * IPC (instr/Hz) * FPI (FLOP/instr) * WR * NFPU
  N - number of cores
  CPU - clock, Hz
  IPC - instruction per cycle, instr/Hz, for superscalar computer IPC > 1
  FPI - FLOP per instruction, FLOP/instr, FMA4 (Fuised Multiply Added) a = b + c*d (4 double), if FMA has 512 bits register, then FPI = 4
  NFPU - number of FMA

  TPP(1) = TPP/N - Theorerty Pick Perfomance for one core

  TPP(1) = 1.8 GHz * 1 (IPC)* 4 * 2 = 14.4 GFLOPS
  
  TEST:  
  CPU use out-of-order execute code and pipeline FMA unit
  P1 (Perfomance) = CPU (Hz) / CPI (1/FPLOP) = 1.25 (GHz) / 3 (1/FLOP, ADD has 3 cycle) = 0.402 GFLOPS 
  P2 = P1 *2 = 0.804 GFLOPS 
  P4 = P2 *2 = 1.608 GFLOPS

*/

#ifdef __GNUC__
#define ALIGN(N) __attribute__((aligned(N))) // Linux
#else
#define ALIGN(N) __declspec(align(N)) // Windows
#endif

int main(int argc, char* argv[]){

  std::cout<<"*********************** ADD optimization using unrolling loop *************************\n";
    
  ALIGN(64) double a = 1.0;
  ALIGN(64) double result = 0.0;
  ALIGN(64) double t = 0.0;
  ALIGN(64) int N=330'000'123;

  {
    t = omp_get_wtime();

    for(int i=0; i<N; ++i) result += a;
      
    t = omp_get_wtime()-t;
    std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[2]={0.0, 0.0};
      
    for(int i=0; i<N/2; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
    }
      
    for(int i=0; i<N%2; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1];
    
    t = omp_get_wtime()-t;
    std::cout<<"2. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
  
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[4]={0.0, 0.0, 0.0, 0.0};
      
    for(int i=0; i<N/4; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
    }
      
    for(int i=0; i<N%4; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3];
    
    t = omp_get_wtime()-t;
    std::cout<<"4. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[8]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      
    for(int i=0; i<N/8; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
      result_tmp[5] += a;
      result_tmp[6] += a;
      result_tmp[7] += a;
    }
      
    for(int i=0; i<N%8; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7];
    
    t = omp_get_wtime()-t;
    std::cout<<"8. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
  
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[16]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      
    for(int i=0; i<N/16; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
      result_tmp[5] += a;
      result_tmp[6] += a;
      result_tmp[7] += a;
      result_tmp[8] += a; // unrolling loop
      result_tmp[9] += a;
      result_tmp[10] += a;
      result_tmp[11] += a;
      result_tmp[12] += a;
      result_tmp[13] += a;
      result_tmp[14] += a;
      result_tmp[15] += a;
    }
      
    for(int i=0; i<N%16; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15];
    
    t = omp_get_wtime()-t;
    std::cout<<"16. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[32]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      
    for(int i=0; i<N/32; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
      result_tmp[5] += a;
      result_tmp[6] += a;
      result_tmp[7] += a;
      result_tmp[8] += a; // unrolling loop
      result_tmp[9] += a;
      result_tmp[10] += a;
      result_tmp[11] += a;
      result_tmp[12] += a;
      result_tmp[13] += a;
      result_tmp[14] += a;
      result_tmp[15] += a;
      result_tmp[16] += a; // unrolling loop
      result_tmp[17] += a;
      result_tmp[18] += a;
      result_tmp[19] += a;
      result_tmp[20] += a;
      result_tmp[21] += a;
      result_tmp[22] += a;
      result_tmp[23] += a;
      result_tmp[24] += a; // unrolling loop
      result_tmp[25] += a;
      result_tmp[26] += a;
      result_tmp[27] += a;
      result_tmp[28] += a;
      result_tmp[29] += a;
      result_tmp[30] += a;
      result_tmp[31] += a;
    }
      
    for(int i=0; i<N%32; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31];
    
    t = omp_get_wtime()-t;
    std::cout<<"32. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[64]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    for(int i=0; i<N/64; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
      result_tmp[5] += a;
      result_tmp[6] += a;
      result_tmp[7] += a;
      result_tmp[8] += a; // unrolling loop
      result_tmp[9] += a;
      result_tmp[10] += a;
      result_tmp[11] += a;
      result_tmp[12] += a;
      result_tmp[13] += a;
      result_tmp[14] += a;
      result_tmp[15] += a;
      result_tmp[16] += a; // unrolling loop
      result_tmp[17] += a;
      result_tmp[18] += a;
      result_tmp[19] += a;
      result_tmp[20] += a;
      result_tmp[21] += a;
      result_tmp[22] += a;
      result_tmp[23] += a;
      result_tmp[24] += a; // unrolling loop
      result_tmp[25] += a;
      result_tmp[26] += a;
      result_tmp[27] += a;
      result_tmp[28] += a;
      result_tmp[29] += a;
      result_tmp[30] += a;
      result_tmp[31] += a;
      result_tmp[32] += a; // unrolling loop
      result_tmp[33] += a;
      result_tmp[34] += a;
      result_tmp[35] += a;
      result_tmp[36] += a;
      result_tmp[37] += a;
      result_tmp[38] += a;
      result_tmp[39] += a;
      result_tmp[40] += a; // unrolling loop
      result_tmp[41] += a;
      result_tmp[42] += a;
      result_tmp[43] += a;
      result_tmp[44] += a;
      result_tmp[45] += a;
      result_tmp[46] += a;
      result_tmp[47] += a;
      result_tmp[48] += a; // unrolling loop
      result_tmp[49] += a;
      result_tmp[50] += a;
      result_tmp[51] += a;
      result_tmp[52] += a;
      result_tmp[53] += a;
      result_tmp[54] += a;
      result_tmp[55] += a;
      result_tmp[56] += a; // unrolling loop
      result_tmp[57] += a;
      result_tmp[58] += a;
      result_tmp[59] += a;
      result_tmp[60] += a;
      result_tmp[61] += a;
      result_tmp[62] += a;
      result_tmp[63] += a;
    }
      
    for(int i=0; i<N%64; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31] + result_tmp[32] + result_tmp[33] + result_tmp[34] + result_tmp[35] + result_tmp[36] + result_tmp[37] + result_tmp[38] + result_tmp[39] + result_tmp[40] + result_tmp[41] + result_tmp[42] + result_tmp[43] + result_tmp[44] + result_tmp[45] + result_tmp[46] + result_tmp[47] + result_tmp[48] + result_tmp[49] + result_tmp[50] + result_tmp[51] + result_tmp[52] + result_tmp[53] + result_tmp[54] + result_tmp[55] + result_tmp[56] + result_tmp[57] + result_tmp[58] + result_tmp[59] + result_tmp[60] + result_tmp[61] + result_tmp[62] + result_tmp[63];
    
    t = omp_get_wtime()-t;
    std::cout<<"64. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }

  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[128]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    for(int i=0; i<N/128; ++i){
      result_tmp[0] += a; // unrolling loop
      result_tmp[1] += a;
      result_tmp[2] += a;
      result_tmp[3] += a;
      result_tmp[4] += a;
      result_tmp[5] += a;
      result_tmp[6] += a;
      result_tmp[7] += a;
      result_tmp[8] += a; // unrolling loop
      result_tmp[9] += a;
      result_tmp[10] += a;
      result_tmp[11] += a;
      result_tmp[12] += a;
      result_tmp[13] += a;
      result_tmp[14] += a;
      result_tmp[15] += a;
      result_tmp[16] += a; // unrolling loop
      result_tmp[17] += a;
      result_tmp[18] += a;
      result_tmp[19] += a;
      result_tmp[20] += a;
      result_tmp[21] += a;
      result_tmp[22] += a;
      result_tmp[23] += a;
      result_tmp[24] += a; // unrolling loop
      result_tmp[25] += a;
      result_tmp[26] += a;
      result_tmp[27] += a;
      result_tmp[28] += a;
      result_tmp[29] += a;
      result_tmp[30] += a;
      result_tmp[31] += a;
      result_tmp[32] += a; // unrolling loop
      result_tmp[33] += a;
      result_tmp[34] += a;
      result_tmp[35] += a;
      result_tmp[36] += a;
      result_tmp[37] += a;
      result_tmp[38] += a;
      result_tmp[39] += a;
      result_tmp[40] += a; // unrolling loop
      result_tmp[41] += a;
      result_tmp[42] += a;
      result_tmp[43] += a;
      result_tmp[44] += a;
      result_tmp[45] += a;
      result_tmp[46] += a;
      result_tmp[47] += a;
      result_tmp[48] += a; // unrolling loop
      result_tmp[49] += a;
      result_tmp[50] += a;
      result_tmp[51] += a;
      result_tmp[52] += a;
      result_tmp[53] += a;
      result_tmp[54] += a;
      result_tmp[55] += a;
      result_tmp[56] += a; // unrolling loop
      result_tmp[57] += a;
      result_tmp[58] += a;
      result_tmp[59] += a;
      result_tmp[60] += a;
      result_tmp[61] += a;
      result_tmp[62] += a;
      result_tmp[63] += a;

      result_tmp[64] += a; // unrolling loop
      result_tmp[65] += a;
      result_tmp[66] += a;
      result_tmp[67] += a;
      result_tmp[68] += a;
      result_tmp[69] += a;
      result_tmp[70] += a;
      result_tmp[71] += a;
      result_tmp[72] += a; // unrolling loop
      result_tmp[73] += a;
      result_tmp[74] += a;
      result_tmp[75] += a;
      result_tmp[76] += a;
      result_tmp[77] += a;
      result_tmp[78] += a;
      result_tmp[79] += a;
      result_tmp[80] += a; // unrolling loop
      result_tmp[81] += a;
      result_tmp[82] += a;
      result_tmp[83] += a;
      result_tmp[84] += a;
      result_tmp[85] += a;
      result_tmp[86] += a;
      result_tmp[87] += a;
      result_tmp[88] += a; // unrolling loop
      result_tmp[89] += a;
      result_tmp[90] += a;
      result_tmp[91] += a;
      result_tmp[92] += a;
      result_tmp[93] += a;
      result_tmp[94] += a;
      result_tmp[95] += a;
      result_tmp[96] += a; // unrolling loop
      result_tmp[97] += a;
      result_tmp[98] += a;
      result_tmp[99] += a;
      result_tmp[100] += a;
      result_tmp[101] += a;
      result_tmp[102] += a;
      result_tmp[103] += a;
      result_tmp[104] += a; // unrolling loop
      result_tmp[105] += a;
      result_tmp[106] += a;
      result_tmp[107] += a;
      result_tmp[108] += a;
      result_tmp[109] += a;
      result_tmp[110] += a;
      result_tmp[111] += a;
      result_tmp[112] += a; // unrolling loop
      result_tmp[113] += a;
      result_tmp[114] += a;
      result_tmp[115] += a;
      result_tmp[116] += a;
      result_tmp[117] += a;
      result_tmp[118] += a;
      result_tmp[119] += a;
      result_tmp[120] += a; // unrolling loop
      result_tmp[121] += a;
      result_tmp[122] += a;
      result_tmp[123] += a;
      result_tmp[124] += a;
      result_tmp[125] += a;
      result_tmp[126] += a;
      result_tmp[127] += a;
    }
      
    for(int i=0; i<N%128; ++i) result += a;
      
    result += result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31] + result_tmp[32] + result_tmp[33] + result_tmp[34] + result_tmp[35] + result_tmp[36] + result_tmp[37] + result_tmp[38] + result_tmp[39] + result_tmp[40] + result_tmp[41] + result_tmp[42] + result_tmp[43] + result_tmp[44] + result_tmp[45] + result_tmp[46] + result_tmp[47] + result_tmp[48] + result_tmp[49] + result_tmp[50] + result_tmp[51] + result_tmp[52] + result_tmp[53] + result_tmp[54] + result_tmp[55] + result_tmp[56] + result_tmp[57] + result_tmp[58] + result_tmp[59] + result_tmp[60] + result_tmp[61] + result_tmp[62] + result_tmp[63];
    
    t = omp_get_wtime()-t;
    std::cout<<"128. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
  
  return 0;
}
