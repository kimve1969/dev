#include<iostream>
#include<iomanip>
#include<omp.h>
#include<cmath>
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

Notebook Lenovo
  CPU Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz   3.6 GHz (Turbo) = 0,28 ns/cicle
  Skylake (client) - Microarchitectures - Intel
     https://en.wikichip.org/wiki/intel/microarchitectures/skylake_(client)#Pipeline
Pipeline
     Type: Superscalar, Superpipeline
     OoOE (out-of-order):	Yes
     Speculative:	Yes
     Stages:	14-19
Memory Hierarchy
  Cache
     L1I Cache	32 KiB/core 8-way set associative
         64 sets, 64 B line size
	 shared by the two threads, per core
     L1D Cache	32 KiB/core 8-way set associative, 
         64 sets, 64 B line size, 
	 4 cycles for fastest load-to-use (simple pointer accesses), 5 cycles for complex addresses
	 64 B/cycle load bandwidth
	 32 B/cycle store bandwidth
     L2 Cache	256 KiB/core 4-way set associative
         1024 sets, 64 B line size
	 12 cycles for fastest load-to-use
     L3 Cache	2 MiB/core Up to 16-way set associative
         Up to 2 MiB Per core, shared across all cores
	 64 B line size
	 Read: 32 B/cycle (@ ring clock)
	 Write: 32 B/cycle (@ ring clock)
	 42 cycles for fastest load-to-use    
     Side Cache	
         64 MiB & 128 MiB eDRAM
	 Per package
         128 MiB/package
	 on Iris Pro GPUs only
	 Read: 32 B/cycle (@ eDRAM clock)
	 Write: 32 B/cycle (@ eDRAM clock)
     System DRAM
         2 Channels
	 8 B/cycle/channel (@ memory clock)
	 42 cycles + 51 ns latency ( 184 cicles of processor )

  4 core
  Band Width = 58,3 GB/s

  GPU 	Intel UHD Graphics (Ice Lake G1)
  execute bloks 64, quantity shreiders 512, quantity of monitors 3

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

  /*{
    t = omp_get_wtime();

    for(int i=0; i<N; ++i) result += a;
      
    t = omp_get_wtime()-t;
    std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
    }*/
  


//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[1]={0.0};

    for(int i=0; i<N/1; ++i ){
		result_tmp[0] += a;
	}

    for(int i=0; i<N%1; ++i) result += a;

    result +=result_tmp[0];

    t = omp_get_wtime()-t;
    std::cout<<"1. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[2]={0.0,0.0};

    for(int i=0; i<N/2; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
	}

    for(int i=0; i<N%2; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1];

    t = omp_get_wtime()-t;
    std::cout<<"2. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[4]={0.0,0.0,0.0,0.0};

    for(int i=0; i<N/4; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
	}

    for(int i=0; i<N%4; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3];

    t = omp_get_wtime()-t;
    std::cout<<"4. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[8]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/8; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
	}

    for(int i=0; i<N%8; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7];

    t = omp_get_wtime()-t;
    std::cout<<"8. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[16]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/16; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
		result_tmp[8] += a;
		result_tmp[9] += a;
		result_tmp[10] += a;
		result_tmp[11] += a;
		result_tmp[12] += a;
		result_tmp[13] += a;
		result_tmp[14] += a;
		result_tmp[15] += a;
	}

    for(int i=0; i<N%16; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15];

    t = omp_get_wtime()-t;
    std::cout<<"16. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[32]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/32; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
		result_tmp[8] += a;
		result_tmp[9] += a;
		result_tmp[10] += a;
		result_tmp[11] += a;
		result_tmp[12] += a;
		result_tmp[13] += a;
		result_tmp[14] += a;
		result_tmp[15] += a;
		result_tmp[16] += a;
		result_tmp[17] += a;
		result_tmp[18] += a;
		result_tmp[19] += a;
		result_tmp[20] += a;
		result_tmp[21] += a;
		result_tmp[22] += a;
		result_tmp[23] += a;
		result_tmp[24] += a;
		result_tmp[25] += a;
		result_tmp[26] += a;
		result_tmp[27] += a;
		result_tmp[28] += a;
		result_tmp[29] += a;
		result_tmp[30] += a;
		result_tmp[31] += a;
	}

    for(int i=0; i<N%32; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31];

    t = omp_get_wtime()-t;
    std::cout<<"32. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[64]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/64; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
		result_tmp[8] += a;
		result_tmp[9] += a;
		result_tmp[10] += a;
		result_tmp[11] += a;
		result_tmp[12] += a;
		result_tmp[13] += a;
		result_tmp[14] += a;
		result_tmp[15] += a;
		result_tmp[16] += a;
		result_tmp[17] += a;
		result_tmp[18] += a;
		result_tmp[19] += a;
		result_tmp[20] += a;
		result_tmp[21] += a;
		result_tmp[22] += a;
		result_tmp[23] += a;
		result_tmp[24] += a;
		result_tmp[25] += a;
		result_tmp[26] += a;
		result_tmp[27] += a;
		result_tmp[28] += a;
		result_tmp[29] += a;
		result_tmp[30] += a;
		result_tmp[31] += a;
		result_tmp[32] += a;
		result_tmp[33] += a;
		result_tmp[34] += a;
		result_tmp[35] += a;
		result_tmp[36] += a;
		result_tmp[37] += a;
		result_tmp[38] += a;
		result_tmp[39] += a;
		result_tmp[40] += a;
		result_tmp[41] += a;
		result_tmp[42] += a;
		result_tmp[43] += a;
		result_tmp[44] += a;
		result_tmp[45] += a;
		result_tmp[46] += a;
		result_tmp[47] += a;
		result_tmp[48] += a;
		result_tmp[49] += a;
		result_tmp[50] += a;
		result_tmp[51] += a;
		result_tmp[52] += a;
		result_tmp[53] += a;
		result_tmp[54] += a;
		result_tmp[55] += a;
		result_tmp[56] += a;
		result_tmp[57] += a;
		result_tmp[58] += a;
		result_tmp[59] += a;
		result_tmp[60] += a;
		result_tmp[61] += a;
		result_tmp[62] += a;
		result_tmp[63] += a;
	}

    for(int i=0; i<N%64; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31] + result_tmp[32] + result_tmp[33] + result_tmp[34] + result_tmp[35] + result_tmp[36] + result_tmp[37] + result_tmp[38] + result_tmp[39] + result_tmp[40] + result_tmp[41] + result_tmp[42] + result_tmp[43] + result_tmp[44] + result_tmp[45] + result_tmp[46] + result_tmp[47] + result_tmp[48] + result_tmp[49] + result_tmp[50] + result_tmp[51] + result_tmp[52] + result_tmp[53] + result_tmp[54] + result_tmp[55] + result_tmp[56] + result_tmp[57] + result_tmp[58] + result_tmp[59] + result_tmp[60] + result_tmp[61] + result_tmp[62] + result_tmp[63];

    t = omp_get_wtime()-t;
    std::cout<<"64. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[128]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/128; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
		result_tmp[8] += a;
		result_tmp[9] += a;
		result_tmp[10] += a;
		result_tmp[11] += a;
		result_tmp[12] += a;
		result_tmp[13] += a;
		result_tmp[14] += a;
		result_tmp[15] += a;
		result_tmp[16] += a;
		result_tmp[17] += a;
		result_tmp[18] += a;
		result_tmp[19] += a;
		result_tmp[20] += a;
		result_tmp[21] += a;
		result_tmp[22] += a;
		result_tmp[23] += a;
		result_tmp[24] += a;
		result_tmp[25] += a;
		result_tmp[26] += a;
		result_tmp[27] += a;
		result_tmp[28] += a;
		result_tmp[29] += a;
		result_tmp[30] += a;
		result_tmp[31] += a;
		result_tmp[32] += a;
		result_tmp[33] += a;
		result_tmp[34] += a;
		result_tmp[35] += a;
		result_tmp[36] += a;
		result_tmp[37] += a;
		result_tmp[38] += a;
		result_tmp[39] += a;
		result_tmp[40] += a;
		result_tmp[41] += a;
		result_tmp[42] += a;
		result_tmp[43] += a;
		result_tmp[44] += a;
		result_tmp[45] += a;
		result_tmp[46] += a;
		result_tmp[47] += a;
		result_tmp[48] += a;
		result_tmp[49] += a;
		result_tmp[50] += a;
		result_tmp[51] += a;
		result_tmp[52] += a;
		result_tmp[53] += a;
		result_tmp[54] += a;
		result_tmp[55] += a;
		result_tmp[56] += a;
		result_tmp[57] += a;
		result_tmp[58] += a;
		result_tmp[59] += a;
		result_tmp[60] += a;
		result_tmp[61] += a;
		result_tmp[62] += a;
		result_tmp[63] += a;
		result_tmp[64] += a;
		result_tmp[65] += a;
		result_tmp[66] += a;
		result_tmp[67] += a;
		result_tmp[68] += a;
		result_tmp[69] += a;
		result_tmp[70] += a;
		result_tmp[71] += a;
		result_tmp[72] += a;
		result_tmp[73] += a;
		result_tmp[74] += a;
		result_tmp[75] += a;
		result_tmp[76] += a;
		result_tmp[77] += a;
		result_tmp[78] += a;
		result_tmp[79] += a;
		result_tmp[80] += a;
		result_tmp[81] += a;
		result_tmp[82] += a;
		result_tmp[83] += a;
		result_tmp[84] += a;
		result_tmp[85] += a;
		result_tmp[86] += a;
		result_tmp[87] += a;
		result_tmp[88] += a;
		result_tmp[89] += a;
		result_tmp[90] += a;
		result_tmp[91] += a;
		result_tmp[92] += a;
		result_tmp[93] += a;
		result_tmp[94] += a;
		result_tmp[95] += a;
		result_tmp[96] += a;
		result_tmp[97] += a;
		result_tmp[98] += a;
		result_tmp[99] += a;
		result_tmp[100] += a;
		result_tmp[101] += a;
		result_tmp[102] += a;
		result_tmp[103] += a;
		result_tmp[104] += a;
		result_tmp[105] += a;
		result_tmp[106] += a;
		result_tmp[107] += a;
		result_tmp[108] += a;
		result_tmp[109] += a;
		result_tmp[110] += a;
		result_tmp[111] += a;
		result_tmp[112] += a;
		result_tmp[113] += a;
		result_tmp[114] += a;
		result_tmp[115] += a;
		result_tmp[116] += a;
		result_tmp[117] += a;
		result_tmp[118] += a;
		result_tmp[119] += a;
		result_tmp[120] += a;
		result_tmp[121] += a;
		result_tmp[122] += a;
		result_tmp[123] += a;
		result_tmp[124] += a;
		result_tmp[125] += a;
		result_tmp[126] += a;
		result_tmp[127] += a;
	}

    for(int i=0; i<N%128; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31] + result_tmp[32] + result_tmp[33] + result_tmp[34] + result_tmp[35] + result_tmp[36] + result_tmp[37] + result_tmp[38] + result_tmp[39] + result_tmp[40] + result_tmp[41] + result_tmp[42] + result_tmp[43] + result_tmp[44] + result_tmp[45] + result_tmp[46] + result_tmp[47] + result_tmp[48] + result_tmp[49] + result_tmp[50] + result_tmp[51] + result_tmp[52] + result_tmp[53] + result_tmp[54] + result_tmp[55] + result_tmp[56] + result_tmp[57] + result_tmp[58] + result_tmp[59] + result_tmp[60] + result_tmp[61] + result_tmp[62] + result_tmp[63] + result_tmp[64] + result_tmp[65] + result_tmp[66] + result_tmp[67] + result_tmp[68] + result_tmp[69] + result_tmp[70] + result_tmp[71] + result_tmp[72] + result_tmp[73] + result_tmp[74] + result_tmp[75] + result_tmp[76] + result_tmp[77] + result_tmp[78] + result_tmp[79] + result_tmp[80] + result_tmp[81] + result_tmp[82] + result_tmp[83] + result_tmp[84] + result_tmp[85] + result_tmp[86] + result_tmp[87] + result_tmp[88] + result_tmp[89] + result_tmp[90] + result_tmp[91] + result_tmp[92] + result_tmp[93] + result_tmp[94] + result_tmp[95] + result_tmp[96] + result_tmp[97] + result_tmp[98] + result_tmp[99] + result_tmp[100] + result_tmp[101] + result_tmp[102] + result_tmp[103] + result_tmp[104] + result_tmp[105] + result_tmp[106] + result_tmp[107] + result_tmp[108] + result_tmp[109] + result_tmp[110] + result_tmp[111] + result_tmp[112] + result_tmp[113] + result_tmp[114] + result_tmp[115] + result_tmp[116] + result_tmp[117] + result_tmp[118] + result_tmp[119] + result_tmp[120] + result_tmp[121] + result_tmp[122] + result_tmp[123] + result_tmp[124] + result_tmp[125] + result_tmp[126] + result_tmp[127];

    t = omp_get_wtime()-t;
    std::cout<<"128. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[256]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/256; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
		result_tmp[8] += a;
		result_tmp[9] += a;
		result_tmp[10] += a;
		result_tmp[11] += a;
		result_tmp[12] += a;
		result_tmp[13] += a;
		result_tmp[14] += a;
		result_tmp[15] += a;
		result_tmp[16] += a;
		result_tmp[17] += a;
		result_tmp[18] += a;
		result_tmp[19] += a;
		result_tmp[20] += a;
		result_tmp[21] += a;
		result_tmp[22] += a;
		result_tmp[23] += a;
		result_tmp[24] += a;
		result_tmp[25] += a;
		result_tmp[26] += a;
		result_tmp[27] += a;
		result_tmp[28] += a;
		result_tmp[29] += a;
		result_tmp[30] += a;
		result_tmp[31] += a;
		result_tmp[32] += a;
		result_tmp[33] += a;
		result_tmp[34] += a;
		result_tmp[35] += a;
		result_tmp[36] += a;
		result_tmp[37] += a;
		result_tmp[38] += a;
		result_tmp[39] += a;
		result_tmp[40] += a;
		result_tmp[41] += a;
		result_tmp[42] += a;
		result_tmp[43] += a;
		result_tmp[44] += a;
		result_tmp[45] += a;
		result_tmp[46] += a;
		result_tmp[47] += a;
		result_tmp[48] += a;
		result_tmp[49] += a;
		result_tmp[50] += a;
		result_tmp[51] += a;
		result_tmp[52] += a;
		result_tmp[53] += a;
		result_tmp[54] += a;
		result_tmp[55] += a;
		result_tmp[56] += a;
		result_tmp[57] += a;
		result_tmp[58] += a;
		result_tmp[59] += a;
		result_tmp[60] += a;
		result_tmp[61] += a;
		result_tmp[62] += a;
		result_tmp[63] += a;
		result_tmp[64] += a;
		result_tmp[65] += a;
		result_tmp[66] += a;
		result_tmp[67] += a;
		result_tmp[68] += a;
		result_tmp[69] += a;
		result_tmp[70] += a;
		result_tmp[71] += a;
		result_tmp[72] += a;
		result_tmp[73] += a;
		result_tmp[74] += a;
		result_tmp[75] += a;
		result_tmp[76] += a;
		result_tmp[77] += a;
		result_tmp[78] += a;
		result_tmp[79] += a;
		result_tmp[80] += a;
		result_tmp[81] += a;
		result_tmp[82] += a;
		result_tmp[83] += a;
		result_tmp[84] += a;
		result_tmp[85] += a;
		result_tmp[86] += a;
		result_tmp[87] += a;
		result_tmp[88] += a;
		result_tmp[89] += a;
		result_tmp[90] += a;
		result_tmp[91] += a;
		result_tmp[92] += a;
		result_tmp[93] += a;
		result_tmp[94] += a;
		result_tmp[95] += a;
		result_tmp[96] += a;
		result_tmp[97] += a;
		result_tmp[98] += a;
		result_tmp[99] += a;
		result_tmp[100] += a;
		result_tmp[101] += a;
		result_tmp[102] += a;
		result_tmp[103] += a;
		result_tmp[104] += a;
		result_tmp[105] += a;
		result_tmp[106] += a;
		result_tmp[107] += a;
		result_tmp[108] += a;
		result_tmp[109] += a;
		result_tmp[110] += a;
		result_tmp[111] += a;
		result_tmp[112] += a;
		result_tmp[113] += a;
		result_tmp[114] += a;
		result_tmp[115] += a;
		result_tmp[116] += a;
		result_tmp[117] += a;
		result_tmp[118] += a;
		result_tmp[119] += a;
		result_tmp[120] += a;
		result_tmp[121] += a;
		result_tmp[122] += a;
		result_tmp[123] += a;
		result_tmp[124] += a;
		result_tmp[125] += a;
		result_tmp[126] += a;
		result_tmp[127] += a;
		result_tmp[128] += a;
		result_tmp[129] += a;
		result_tmp[130] += a;
		result_tmp[131] += a;
		result_tmp[132] += a;
		result_tmp[133] += a;
		result_tmp[134] += a;
		result_tmp[135] += a;
		result_tmp[136] += a;
		result_tmp[137] += a;
		result_tmp[138] += a;
		result_tmp[139] += a;
		result_tmp[140] += a;
		result_tmp[141] += a;
		result_tmp[142] += a;
		result_tmp[143] += a;
		result_tmp[144] += a;
		result_tmp[145] += a;
		result_tmp[146] += a;
		result_tmp[147] += a;
		result_tmp[148] += a;
		result_tmp[149] += a;
		result_tmp[150] += a;
		result_tmp[151] += a;
		result_tmp[152] += a;
		result_tmp[153] += a;
		result_tmp[154] += a;
		result_tmp[155] += a;
		result_tmp[156] += a;
		result_tmp[157] += a;
		result_tmp[158] += a;
		result_tmp[159] += a;
		result_tmp[160] += a;
		result_tmp[161] += a;
		result_tmp[162] += a;
		result_tmp[163] += a;
		result_tmp[164] += a;
		result_tmp[165] += a;
		result_tmp[166] += a;
		result_tmp[167] += a;
		result_tmp[168] += a;
		result_tmp[169] += a;
		result_tmp[170] += a;
		result_tmp[171] += a;
		result_tmp[172] += a;
		result_tmp[173] += a;
		result_tmp[174] += a;
		result_tmp[175] += a;
		result_tmp[176] += a;
		result_tmp[177] += a;
		result_tmp[178] += a;
		result_tmp[179] += a;
		result_tmp[180] += a;
		result_tmp[181] += a;
		result_tmp[182] += a;
		result_tmp[183] += a;
		result_tmp[184] += a;
		result_tmp[185] += a;
		result_tmp[186] += a;
		result_tmp[187] += a;
		result_tmp[188] += a;
		result_tmp[189] += a;
		result_tmp[190] += a;
		result_tmp[191] += a;
		result_tmp[192] += a;
		result_tmp[193] += a;
		result_tmp[194] += a;
		result_tmp[195] += a;
		result_tmp[196] += a;
		result_tmp[197] += a;
		result_tmp[198] += a;
		result_tmp[199] += a;
		result_tmp[200] += a;
		result_tmp[201] += a;
		result_tmp[202] += a;
		result_tmp[203] += a;
		result_tmp[204] += a;
		result_tmp[205] += a;
		result_tmp[206] += a;
		result_tmp[207] += a;
		result_tmp[208] += a;
		result_tmp[209] += a;
		result_tmp[210] += a;
		result_tmp[211] += a;
		result_tmp[212] += a;
		result_tmp[213] += a;
		result_tmp[214] += a;
		result_tmp[215] += a;
		result_tmp[216] += a;
		result_tmp[217] += a;
		result_tmp[218] += a;
		result_tmp[219] += a;
		result_tmp[220] += a;
		result_tmp[221] += a;
		result_tmp[222] += a;
		result_tmp[223] += a;
		result_tmp[224] += a;
		result_tmp[225] += a;
		result_tmp[226] += a;
		result_tmp[227] += a;
		result_tmp[228] += a;
		result_tmp[229] += a;
		result_tmp[230] += a;
		result_tmp[231] += a;
		result_tmp[232] += a;
		result_tmp[233] += a;
		result_tmp[234] += a;
		result_tmp[235] += a;
		result_tmp[236] += a;
		result_tmp[237] += a;
		result_tmp[238] += a;
		result_tmp[239] += a;
		result_tmp[240] += a;
		result_tmp[241] += a;
		result_tmp[242] += a;
		result_tmp[243] += a;
		result_tmp[244] += a;
		result_tmp[245] += a;
		result_tmp[246] += a;
		result_tmp[247] += a;
		result_tmp[248] += a;
		result_tmp[249] += a;
		result_tmp[250] += a;
		result_tmp[251] += a;
		result_tmp[252] += a;
		result_tmp[253] += a;
		result_tmp[254] += a;
		result_tmp[255] += a;
	}

    for(int i=0; i<N%256; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31] + result_tmp[32] + result_tmp[33] + result_tmp[34] + result_tmp[35] + result_tmp[36] + result_tmp[37] + result_tmp[38] + result_tmp[39] + result_tmp[40] + result_tmp[41] + result_tmp[42] + result_tmp[43] + result_tmp[44] + result_tmp[45] + result_tmp[46] + result_tmp[47] + result_tmp[48] + result_tmp[49] + result_tmp[50] + result_tmp[51] + result_tmp[52] + result_tmp[53] + result_tmp[54] + result_tmp[55] + result_tmp[56] + result_tmp[57] + result_tmp[58] + result_tmp[59] + result_tmp[60] + result_tmp[61] + result_tmp[62] + result_tmp[63] + result_tmp[64] + result_tmp[65] + result_tmp[66] + result_tmp[67] + result_tmp[68] + result_tmp[69] + result_tmp[70] + result_tmp[71] + result_tmp[72] + result_tmp[73] + result_tmp[74] + result_tmp[75] + result_tmp[76] + result_tmp[77] + result_tmp[78] + result_tmp[79] + result_tmp[80] + result_tmp[81] + result_tmp[82] + result_tmp[83] + result_tmp[84] + result_tmp[85] + result_tmp[86] + result_tmp[87] + result_tmp[88] + result_tmp[89] + result_tmp[90] + result_tmp[91] + result_tmp[92] + result_tmp[93] + result_tmp[94] + result_tmp[95] + result_tmp[96] + result_tmp[97] + result_tmp[98] + result_tmp[99] + result_tmp[100] + result_tmp[101] + result_tmp[102] + result_tmp[103] + result_tmp[104] + result_tmp[105] + result_tmp[106] + result_tmp[107] + result_tmp[108] + result_tmp[109] + result_tmp[110] + result_tmp[111] + result_tmp[112] + result_tmp[113] + result_tmp[114] + result_tmp[115] + result_tmp[116] + result_tmp[117] + result_tmp[118] + result_tmp[119] + result_tmp[120] + result_tmp[121] + result_tmp[122] + result_tmp[123] + result_tmp[124] + result_tmp[125] + result_tmp[126] + result_tmp[127] + result_tmp[128] + result_tmp[129] + result_tmp[130] + result_tmp[131] + result_tmp[132] + result_tmp[133] + result_tmp[134] + result_tmp[135] + result_tmp[136] + result_tmp[137] + result_tmp[138] + result_tmp[139] + result_tmp[140] + result_tmp[141] + result_tmp[142] + result_tmp[143] + result_tmp[144] + result_tmp[145] + result_tmp[146] + result_tmp[147] + result_tmp[148] + result_tmp[149] + result_tmp[150] + result_tmp[151] + result_tmp[152] + result_tmp[153] + result_tmp[154] + result_tmp[155] + result_tmp[156] + result_tmp[157] + result_tmp[158] + result_tmp[159] + result_tmp[160] + result_tmp[161] + result_tmp[162] + result_tmp[163] + result_tmp[164] + result_tmp[165] + result_tmp[166] + result_tmp[167] + result_tmp[168] + result_tmp[169] + result_tmp[170] + result_tmp[171] + result_tmp[172] + result_tmp[173] + result_tmp[174] + result_tmp[175] + result_tmp[176] + result_tmp[177] + result_tmp[178] + result_tmp[179] + result_tmp[180] + result_tmp[181] + result_tmp[182] + result_tmp[183] + result_tmp[184] + result_tmp[185] + result_tmp[186] + result_tmp[187] + result_tmp[188] + result_tmp[189] + result_tmp[190] + result_tmp[191] + result_tmp[192] + result_tmp[193] + result_tmp[194] + result_tmp[195] + result_tmp[196] + result_tmp[197] + result_tmp[198] + result_tmp[199] + result_tmp[200] + result_tmp[201] + result_tmp[202] + result_tmp[203] + result_tmp[204] + result_tmp[205] + result_tmp[206] + result_tmp[207] + result_tmp[208] + result_tmp[209] + result_tmp[210] + result_tmp[211] + result_tmp[212] + result_tmp[213] + result_tmp[214] + result_tmp[215] + result_tmp[216] + result_tmp[217] + result_tmp[218] + result_tmp[219] + result_tmp[220] + result_tmp[221] + result_tmp[222] + result_tmp[223] + result_tmp[224] + result_tmp[225] + result_tmp[226] + result_tmp[227] + result_tmp[228] + result_tmp[229] + result_tmp[230] + result_tmp[231] + result_tmp[232] + result_tmp[233] + result_tmp[234] + result_tmp[235] + result_tmp[236] + result_tmp[237] + result_tmp[238] + result_tmp[239] + result_tmp[240] + result_tmp[241] + result_tmp[242] + result_tmp[243] + result_tmp[244] + result_tmp[245] + result_tmp[246] + result_tmp[247] + result_tmp[248] + result_tmp[249] + result_tmp[250] + result_tmp[251] + result_tmp[252] + result_tmp[253] + result_tmp[254] + result_tmp[255];

    t = omp_get_wtime()-t;
    std::cout<<"256. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[512]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/512; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
		result_tmp[8] += a;
		result_tmp[9] += a;
		result_tmp[10] += a;
		result_tmp[11] += a;
		result_tmp[12] += a;
		result_tmp[13] += a;
		result_tmp[14] += a;
		result_tmp[15] += a;
		result_tmp[16] += a;
		result_tmp[17] += a;
		result_tmp[18] += a;
		result_tmp[19] += a;
		result_tmp[20] += a;
		result_tmp[21] += a;
		result_tmp[22] += a;
		result_tmp[23] += a;
		result_tmp[24] += a;
		result_tmp[25] += a;
		result_tmp[26] += a;
		result_tmp[27] += a;
		result_tmp[28] += a;
		result_tmp[29] += a;
		result_tmp[30] += a;
		result_tmp[31] += a;
		result_tmp[32] += a;
		result_tmp[33] += a;
		result_tmp[34] += a;
		result_tmp[35] += a;
		result_tmp[36] += a;
		result_tmp[37] += a;
		result_tmp[38] += a;
		result_tmp[39] += a;
		result_tmp[40] += a;
		result_tmp[41] += a;
		result_tmp[42] += a;
		result_tmp[43] += a;
		result_tmp[44] += a;
		result_tmp[45] += a;
		result_tmp[46] += a;
		result_tmp[47] += a;
		result_tmp[48] += a;
		result_tmp[49] += a;
		result_tmp[50] += a;
		result_tmp[51] += a;
		result_tmp[52] += a;
		result_tmp[53] += a;
		result_tmp[54] += a;
		result_tmp[55] += a;
		result_tmp[56] += a;
		result_tmp[57] += a;
		result_tmp[58] += a;
		result_tmp[59] += a;
		result_tmp[60] += a;
		result_tmp[61] += a;
		result_tmp[62] += a;
		result_tmp[63] += a;
		result_tmp[64] += a;
		result_tmp[65] += a;
		result_tmp[66] += a;
		result_tmp[67] += a;
		result_tmp[68] += a;
		result_tmp[69] += a;
		result_tmp[70] += a;
		result_tmp[71] += a;
		result_tmp[72] += a;
		result_tmp[73] += a;
		result_tmp[74] += a;
		result_tmp[75] += a;
		result_tmp[76] += a;
		result_tmp[77] += a;
		result_tmp[78] += a;
		result_tmp[79] += a;
		result_tmp[80] += a;
		result_tmp[81] += a;
		result_tmp[82] += a;
		result_tmp[83] += a;
		result_tmp[84] += a;
		result_tmp[85] += a;
		result_tmp[86] += a;
		result_tmp[87] += a;
		result_tmp[88] += a;
		result_tmp[89] += a;
		result_tmp[90] += a;
		result_tmp[91] += a;
		result_tmp[92] += a;
		result_tmp[93] += a;
		result_tmp[94] += a;
		result_tmp[95] += a;
		result_tmp[96] += a;
		result_tmp[97] += a;
		result_tmp[98] += a;
		result_tmp[99] += a;
		result_tmp[100] += a;
		result_tmp[101] += a;
		result_tmp[102] += a;
		result_tmp[103] += a;
		result_tmp[104] += a;
		result_tmp[105] += a;
		result_tmp[106] += a;
		result_tmp[107] += a;
		result_tmp[108] += a;
		result_tmp[109] += a;
		result_tmp[110] += a;
		result_tmp[111] += a;
		result_tmp[112] += a;
		result_tmp[113] += a;
		result_tmp[114] += a;
		result_tmp[115] += a;
		result_tmp[116] += a;
		result_tmp[117] += a;
		result_tmp[118] += a;
		result_tmp[119] += a;
		result_tmp[120] += a;
		result_tmp[121] += a;
		result_tmp[122] += a;
		result_tmp[123] += a;
		result_tmp[124] += a;
		result_tmp[125] += a;
		result_tmp[126] += a;
		result_tmp[127] += a;
		result_tmp[128] += a;
		result_tmp[129] += a;
		result_tmp[130] += a;
		result_tmp[131] += a;
		result_tmp[132] += a;
		result_tmp[133] += a;
		result_tmp[134] += a;
		result_tmp[135] += a;
		result_tmp[136] += a;
		result_tmp[137] += a;
		result_tmp[138] += a;
		result_tmp[139] += a;
		result_tmp[140] += a;
		result_tmp[141] += a;
		result_tmp[142] += a;
		result_tmp[143] += a;
		result_tmp[144] += a;
		result_tmp[145] += a;
		result_tmp[146] += a;
		result_tmp[147] += a;
		result_tmp[148] += a;
		result_tmp[149] += a;
		result_tmp[150] += a;
		result_tmp[151] += a;
		result_tmp[152] += a;
		result_tmp[153] += a;
		result_tmp[154] += a;
		result_tmp[155] += a;
		result_tmp[156] += a;
		result_tmp[157] += a;
		result_tmp[158] += a;
		result_tmp[159] += a;
		result_tmp[160] += a;
		result_tmp[161] += a;
		result_tmp[162] += a;
		result_tmp[163] += a;
		result_tmp[164] += a;
		result_tmp[165] += a;
		result_tmp[166] += a;
		result_tmp[167] += a;
		result_tmp[168] += a;
		result_tmp[169] += a;
		result_tmp[170] += a;
		result_tmp[171] += a;
		result_tmp[172] += a;
		result_tmp[173] += a;
		result_tmp[174] += a;
		result_tmp[175] += a;
		result_tmp[176] += a;
		result_tmp[177] += a;
		result_tmp[178] += a;
		result_tmp[179] += a;
		result_tmp[180] += a;
		result_tmp[181] += a;
		result_tmp[182] += a;
		result_tmp[183] += a;
		result_tmp[184] += a;
		result_tmp[185] += a;
		result_tmp[186] += a;
		result_tmp[187] += a;
		result_tmp[188] += a;
		result_tmp[189] += a;
		result_tmp[190] += a;
		result_tmp[191] += a;
		result_tmp[192] += a;
		result_tmp[193] += a;
		result_tmp[194] += a;
		result_tmp[195] += a;
		result_tmp[196] += a;
		result_tmp[197] += a;
		result_tmp[198] += a;
		result_tmp[199] += a;
		result_tmp[200] += a;
		result_tmp[201] += a;
		result_tmp[202] += a;
		result_tmp[203] += a;
		result_tmp[204] += a;
		result_tmp[205] += a;
		result_tmp[206] += a;
		result_tmp[207] += a;
		result_tmp[208] += a;
		result_tmp[209] += a;
		result_tmp[210] += a;
		result_tmp[211] += a;
		result_tmp[212] += a;
		result_tmp[213] += a;
		result_tmp[214] += a;
		result_tmp[215] += a;
		result_tmp[216] += a;
		result_tmp[217] += a;
		result_tmp[218] += a;
		result_tmp[219] += a;
		result_tmp[220] += a;
		result_tmp[221] += a;
		result_tmp[222] += a;
		result_tmp[223] += a;
		result_tmp[224] += a;
		result_tmp[225] += a;
		result_tmp[226] += a;
		result_tmp[227] += a;
		result_tmp[228] += a;
		result_tmp[229] += a;
		result_tmp[230] += a;
		result_tmp[231] += a;
		result_tmp[232] += a;
		result_tmp[233] += a;
		result_tmp[234] += a;
		result_tmp[235] += a;
		result_tmp[236] += a;
		result_tmp[237] += a;
		result_tmp[238] += a;
		result_tmp[239] += a;
		result_tmp[240] += a;
		result_tmp[241] += a;
		result_tmp[242] += a;
		result_tmp[243] += a;
		result_tmp[244] += a;
		result_tmp[245] += a;
		result_tmp[246] += a;
		result_tmp[247] += a;
		result_tmp[248] += a;
		result_tmp[249] += a;
		result_tmp[250] += a;
		result_tmp[251] += a;
		result_tmp[252] += a;
		result_tmp[253] += a;
		result_tmp[254] += a;
		result_tmp[255] += a;
		result_tmp[256] += a;
		result_tmp[257] += a;
		result_tmp[258] += a;
		result_tmp[259] += a;
		result_tmp[260] += a;
		result_tmp[261] += a;
		result_tmp[262] += a;
		result_tmp[263] += a;
		result_tmp[264] += a;
		result_tmp[265] += a;
		result_tmp[266] += a;
		result_tmp[267] += a;
		result_tmp[268] += a;
		result_tmp[269] += a;
		result_tmp[270] += a;
		result_tmp[271] += a;
		result_tmp[272] += a;
		result_tmp[273] += a;
		result_tmp[274] += a;
		result_tmp[275] += a;
		result_tmp[276] += a;
		result_tmp[277] += a;
		result_tmp[278] += a;
		result_tmp[279] += a;
		result_tmp[280] += a;
		result_tmp[281] += a;
		result_tmp[282] += a;
		result_tmp[283] += a;
		result_tmp[284] += a;
		result_tmp[285] += a;
		result_tmp[286] += a;
		result_tmp[287] += a;
		result_tmp[288] += a;
		result_tmp[289] += a;
		result_tmp[290] += a;
		result_tmp[291] += a;
		result_tmp[292] += a;
		result_tmp[293] += a;
		result_tmp[294] += a;
		result_tmp[295] += a;
		result_tmp[296] += a;
		result_tmp[297] += a;
		result_tmp[298] += a;
		result_tmp[299] += a;
		result_tmp[300] += a;
		result_tmp[301] += a;
		result_tmp[302] += a;
		result_tmp[303] += a;
		result_tmp[304] += a;
		result_tmp[305] += a;
		result_tmp[306] += a;
		result_tmp[307] += a;
		result_tmp[308] += a;
		result_tmp[309] += a;
		result_tmp[310] += a;
		result_tmp[311] += a;
		result_tmp[312] += a;
		result_tmp[313] += a;
		result_tmp[314] += a;
		result_tmp[315] += a;
		result_tmp[316] += a;
		result_tmp[317] += a;
		result_tmp[318] += a;
		result_tmp[319] += a;
		result_tmp[320] += a;
		result_tmp[321] += a;
		result_tmp[322] += a;
		result_tmp[323] += a;
		result_tmp[324] += a;
		result_tmp[325] += a;
		result_tmp[326] += a;
		result_tmp[327] += a;
		result_tmp[328] += a;
		result_tmp[329] += a;
		result_tmp[330] += a;
		result_tmp[331] += a;
		result_tmp[332] += a;
		result_tmp[333] += a;
		result_tmp[334] += a;
		result_tmp[335] += a;
		result_tmp[336] += a;
		result_tmp[337] += a;
		result_tmp[338] += a;
		result_tmp[339] += a;
		result_tmp[340] += a;
		result_tmp[341] += a;
		result_tmp[342] += a;
		result_tmp[343] += a;
		result_tmp[344] += a;
		result_tmp[345] += a;
		result_tmp[346] += a;
		result_tmp[347] += a;
		result_tmp[348] += a;
		result_tmp[349] += a;
		result_tmp[350] += a;
		result_tmp[351] += a;
		result_tmp[352] += a;
		result_tmp[353] += a;
		result_tmp[354] += a;
		result_tmp[355] += a;
		result_tmp[356] += a;
		result_tmp[357] += a;
		result_tmp[358] += a;
		result_tmp[359] += a;
		result_tmp[360] += a;
		result_tmp[361] += a;
		result_tmp[362] += a;
		result_tmp[363] += a;
		result_tmp[364] += a;
		result_tmp[365] += a;
		result_tmp[366] += a;
		result_tmp[367] += a;
		result_tmp[368] += a;
		result_tmp[369] += a;
		result_tmp[370] += a;
		result_tmp[371] += a;
		result_tmp[372] += a;
		result_tmp[373] += a;
		result_tmp[374] += a;
		result_tmp[375] += a;
		result_tmp[376] += a;
		result_tmp[377] += a;
		result_tmp[378] += a;
		result_tmp[379] += a;
		result_tmp[380] += a;
		result_tmp[381] += a;
		result_tmp[382] += a;
		result_tmp[383] += a;
		result_tmp[384] += a;
		result_tmp[385] += a;
		result_tmp[386] += a;
		result_tmp[387] += a;
		result_tmp[388] += a;
		result_tmp[389] += a;
		result_tmp[390] += a;
		result_tmp[391] += a;
		result_tmp[392] += a;
		result_tmp[393] += a;
		result_tmp[394] += a;
		result_tmp[395] += a;
		result_tmp[396] += a;
		result_tmp[397] += a;
		result_tmp[398] += a;
		result_tmp[399] += a;
		result_tmp[400] += a;
		result_tmp[401] += a;
		result_tmp[402] += a;
		result_tmp[403] += a;
		result_tmp[404] += a;
		result_tmp[405] += a;
		result_tmp[406] += a;
		result_tmp[407] += a;
		result_tmp[408] += a;
		result_tmp[409] += a;
		result_tmp[410] += a;
		result_tmp[411] += a;
		result_tmp[412] += a;
		result_tmp[413] += a;
		result_tmp[414] += a;
		result_tmp[415] += a;
		result_tmp[416] += a;
		result_tmp[417] += a;
		result_tmp[418] += a;
		result_tmp[419] += a;
		result_tmp[420] += a;
		result_tmp[421] += a;
		result_tmp[422] += a;
		result_tmp[423] += a;
		result_tmp[424] += a;
		result_tmp[425] += a;
		result_tmp[426] += a;
		result_tmp[427] += a;
		result_tmp[428] += a;
		result_tmp[429] += a;
		result_tmp[430] += a;
		result_tmp[431] += a;
		result_tmp[432] += a;
		result_tmp[433] += a;
		result_tmp[434] += a;
		result_tmp[435] += a;
		result_tmp[436] += a;
		result_tmp[437] += a;
		result_tmp[438] += a;
		result_tmp[439] += a;
		result_tmp[440] += a;
		result_tmp[441] += a;
		result_tmp[442] += a;
		result_tmp[443] += a;
		result_tmp[444] += a;
		result_tmp[445] += a;
		result_tmp[446] += a;
		result_tmp[447] += a;
		result_tmp[448] += a;
		result_tmp[449] += a;
		result_tmp[450] += a;
		result_tmp[451] += a;
		result_tmp[452] += a;
		result_tmp[453] += a;
		result_tmp[454] += a;
		result_tmp[455] += a;
		result_tmp[456] += a;
		result_tmp[457] += a;
		result_tmp[458] += a;
		result_tmp[459] += a;
		result_tmp[460] += a;
		result_tmp[461] += a;
		result_tmp[462] += a;
		result_tmp[463] += a;
		result_tmp[464] += a;
		result_tmp[465] += a;
		result_tmp[466] += a;
		result_tmp[467] += a;
		result_tmp[468] += a;
		result_tmp[469] += a;
		result_tmp[470] += a;
		result_tmp[471] += a;
		result_tmp[472] += a;
		result_tmp[473] += a;
		result_tmp[474] += a;
		result_tmp[475] += a;
		result_tmp[476] += a;
		result_tmp[477] += a;
		result_tmp[478] += a;
		result_tmp[479] += a;
		result_tmp[480] += a;
		result_tmp[481] += a;
		result_tmp[482] += a;
		result_tmp[483] += a;
		result_tmp[484] += a;
		result_tmp[485] += a;
		result_tmp[486] += a;
		result_tmp[487] += a;
		result_tmp[488] += a;
		result_tmp[489] += a;
		result_tmp[490] += a;
		result_tmp[491] += a;
		result_tmp[492] += a;
		result_tmp[493] += a;
		result_tmp[494] += a;
		result_tmp[495] += a;
		result_tmp[496] += a;
		result_tmp[497] += a;
		result_tmp[498] += a;
		result_tmp[499] += a;
		result_tmp[500] += a;
		result_tmp[501] += a;
		result_tmp[502] += a;
		result_tmp[503] += a;
		result_tmp[504] += a;
		result_tmp[505] += a;
		result_tmp[506] += a;
		result_tmp[507] += a;
		result_tmp[508] += a;
		result_tmp[509] += a;
		result_tmp[510] += a;
		result_tmp[511] += a;
	}

    for(int i=0; i<N%512; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31] + result_tmp[32] + result_tmp[33] + result_tmp[34] + result_tmp[35] + result_tmp[36] + result_tmp[37] + result_tmp[38] + result_tmp[39] + result_tmp[40] + result_tmp[41] + result_tmp[42] + result_tmp[43] + result_tmp[44] + result_tmp[45] + result_tmp[46] + result_tmp[47] + result_tmp[48] + result_tmp[49] + result_tmp[50] + result_tmp[51] + result_tmp[52] + result_tmp[53] + result_tmp[54] + result_tmp[55] + result_tmp[56] + result_tmp[57] + result_tmp[58] + result_tmp[59] + result_tmp[60] + result_tmp[61] + result_tmp[62] + result_tmp[63] + result_tmp[64] + result_tmp[65] + result_tmp[66] + result_tmp[67] + result_tmp[68] + result_tmp[69] + result_tmp[70] + result_tmp[71] + result_tmp[72] + result_tmp[73] + result_tmp[74] + result_tmp[75] + result_tmp[76] + result_tmp[77] + result_tmp[78] + result_tmp[79] + result_tmp[80] + result_tmp[81] + result_tmp[82] + result_tmp[83] + result_tmp[84] + result_tmp[85] + result_tmp[86] + result_tmp[87] + result_tmp[88] + result_tmp[89] + result_tmp[90] + result_tmp[91] + result_tmp[92] + result_tmp[93] + result_tmp[94] + result_tmp[95] + result_tmp[96] + result_tmp[97] + result_tmp[98] + result_tmp[99] + result_tmp[100] + result_tmp[101] + result_tmp[102] + result_tmp[103] + result_tmp[104] + result_tmp[105] + result_tmp[106] + result_tmp[107] + result_tmp[108] + result_tmp[109] + result_tmp[110] + result_tmp[111] + result_tmp[112] + result_tmp[113] + result_tmp[114] + result_tmp[115] + result_tmp[116] + result_tmp[117] + result_tmp[118] + result_tmp[119] + result_tmp[120] + result_tmp[121] + result_tmp[122] + result_tmp[123] + result_tmp[124] + result_tmp[125] + result_tmp[126] + result_tmp[127] + result_tmp[128] + result_tmp[129] + result_tmp[130] + result_tmp[131] + result_tmp[132] + result_tmp[133] + result_tmp[134] + result_tmp[135] + result_tmp[136] + result_tmp[137] + result_tmp[138] + result_tmp[139] + result_tmp[140] + result_tmp[141] + result_tmp[142] + result_tmp[143] + result_tmp[144] + result_tmp[145] + result_tmp[146] + result_tmp[147] + result_tmp[148] + result_tmp[149] + result_tmp[150] + result_tmp[151] + result_tmp[152] + result_tmp[153] + result_tmp[154] + result_tmp[155] + result_tmp[156] + result_tmp[157] + result_tmp[158] + result_tmp[159] + result_tmp[160] + result_tmp[161] + result_tmp[162] + result_tmp[163] + result_tmp[164] + result_tmp[165] + result_tmp[166] + result_tmp[167] + result_tmp[168] + result_tmp[169] + result_tmp[170] + result_tmp[171] + result_tmp[172] + result_tmp[173] + result_tmp[174] + result_tmp[175] + result_tmp[176] + result_tmp[177] + result_tmp[178] + result_tmp[179] + result_tmp[180] + result_tmp[181] + result_tmp[182] + result_tmp[183] + result_tmp[184] + result_tmp[185] + result_tmp[186] + result_tmp[187] + result_tmp[188] + result_tmp[189] + result_tmp[190] + result_tmp[191] + result_tmp[192] + result_tmp[193] + result_tmp[194] + result_tmp[195] + result_tmp[196] + result_tmp[197] + result_tmp[198] + result_tmp[199] + result_tmp[200] + result_tmp[201] + result_tmp[202] + result_tmp[203] + result_tmp[204] + result_tmp[205] + result_tmp[206] + result_tmp[207] + result_tmp[208] + result_tmp[209] + result_tmp[210] + result_tmp[211] + result_tmp[212] + result_tmp[213] + result_tmp[214] + result_tmp[215] + result_tmp[216] + result_tmp[217] + result_tmp[218] + result_tmp[219] + result_tmp[220] + result_tmp[221] + result_tmp[222] + result_tmp[223] + result_tmp[224] + result_tmp[225] + result_tmp[226] + result_tmp[227] + result_tmp[228] + result_tmp[229] + result_tmp[230] + result_tmp[231] + result_tmp[232] + result_tmp[233] + result_tmp[234] + result_tmp[235] + result_tmp[236] + result_tmp[237] + result_tmp[238] + result_tmp[239] + result_tmp[240] + result_tmp[241] + result_tmp[242] + result_tmp[243] + result_tmp[244] + result_tmp[245] + result_tmp[246] + result_tmp[247] + result_tmp[248] + result_tmp[249] + result_tmp[250] + result_tmp[251] + result_tmp[252] + result_tmp[253] + result_tmp[254] + result_tmp[255] + result_tmp[256] + result_tmp[257] + result_tmp[258] + result_tmp[259] + result_tmp[260] + result_tmp[261] + result_tmp[262] + result_tmp[263] + result_tmp[264] + result_tmp[265] + result_tmp[266] + result_tmp[267] + result_tmp[268] + result_tmp[269] + result_tmp[270] + result_tmp[271] + result_tmp[272] + result_tmp[273] + result_tmp[274] + result_tmp[275] + result_tmp[276] + result_tmp[277] + result_tmp[278] + result_tmp[279] + result_tmp[280] + result_tmp[281] + result_tmp[282] + result_tmp[283] + result_tmp[284] + result_tmp[285] + result_tmp[286] + result_tmp[287] + result_tmp[288] + result_tmp[289] + result_tmp[290] + result_tmp[291] + result_tmp[292] + result_tmp[293] + result_tmp[294] + result_tmp[295] + result_tmp[296] + result_tmp[297] + result_tmp[298] + result_tmp[299] + result_tmp[300] + result_tmp[301] + result_tmp[302] + result_tmp[303] + result_tmp[304] + result_tmp[305] + result_tmp[306] + result_tmp[307] + result_tmp[308] + result_tmp[309] + result_tmp[310] + result_tmp[311] + result_tmp[312] + result_tmp[313] + result_tmp[314] + result_tmp[315] + result_tmp[316] + result_tmp[317] + result_tmp[318] + result_tmp[319] + result_tmp[320] + result_tmp[321] + result_tmp[322] + result_tmp[323] + result_tmp[324] + result_tmp[325] + result_tmp[326] + result_tmp[327] + result_tmp[328] + result_tmp[329] + result_tmp[330] + result_tmp[331] + result_tmp[332] + result_tmp[333] + result_tmp[334] + result_tmp[335] + result_tmp[336] + result_tmp[337] + result_tmp[338] + result_tmp[339] + result_tmp[340] + result_tmp[341] + result_tmp[342] + result_tmp[343] + result_tmp[344] + result_tmp[345] + result_tmp[346] + result_tmp[347] + result_tmp[348] + result_tmp[349] + result_tmp[350] + result_tmp[351] + result_tmp[352] + result_tmp[353] + result_tmp[354] + result_tmp[355] + result_tmp[356] + result_tmp[357] + result_tmp[358] + result_tmp[359] + result_tmp[360] + result_tmp[361] + result_tmp[362] + result_tmp[363] + result_tmp[364] + result_tmp[365] + result_tmp[366] + result_tmp[367] + result_tmp[368] + result_tmp[369] + result_tmp[370] + result_tmp[371] + result_tmp[372] + result_tmp[373] + result_tmp[374] + result_tmp[375] + result_tmp[376] + result_tmp[377] + result_tmp[378] + result_tmp[379] + result_tmp[380] + result_tmp[381] + result_tmp[382] + result_tmp[383] + result_tmp[384] + result_tmp[385] + result_tmp[386] + result_tmp[387] + result_tmp[388] + result_tmp[389] + result_tmp[390] + result_tmp[391] + result_tmp[392] + result_tmp[393] + result_tmp[394] + result_tmp[395] + result_tmp[396] + result_tmp[397] + result_tmp[398] + result_tmp[399] + result_tmp[400] + result_tmp[401] + result_tmp[402] + result_tmp[403] + result_tmp[404] + result_tmp[405] + result_tmp[406] + result_tmp[407] + result_tmp[408] + result_tmp[409] + result_tmp[410] + result_tmp[411] + result_tmp[412] + result_tmp[413] + result_tmp[414] + result_tmp[415] + result_tmp[416] + result_tmp[417] + result_tmp[418] + result_tmp[419] + result_tmp[420] + result_tmp[421] + result_tmp[422] + result_tmp[423] + result_tmp[424] + result_tmp[425] + result_tmp[426] + result_tmp[427] + result_tmp[428] + result_tmp[429] + result_tmp[430] + result_tmp[431] + result_tmp[432] + result_tmp[433] + result_tmp[434] + result_tmp[435] + result_tmp[436] + result_tmp[437] + result_tmp[438] + result_tmp[439] + result_tmp[440] + result_tmp[441] + result_tmp[442] + result_tmp[443] + result_tmp[444] + result_tmp[445] + result_tmp[446] + result_tmp[447] + result_tmp[448] + result_tmp[449] + result_tmp[450] + result_tmp[451] + result_tmp[452] + result_tmp[453] + result_tmp[454] + result_tmp[455] + result_tmp[456] + result_tmp[457] + result_tmp[458] + result_tmp[459] + result_tmp[460] + result_tmp[461] + result_tmp[462] + result_tmp[463] + result_tmp[464] + result_tmp[465] + result_tmp[466] + result_tmp[467] + result_tmp[468] + result_tmp[469] + result_tmp[470] + result_tmp[471] + result_tmp[472] + result_tmp[473] + result_tmp[474] + result_tmp[475] + result_tmp[476] + result_tmp[477] + result_tmp[478] + result_tmp[479] + result_tmp[480] + result_tmp[481] + result_tmp[482] + result_tmp[483] + result_tmp[484] + result_tmp[485] + result_tmp[486] + result_tmp[487] + result_tmp[488] + result_tmp[489] + result_tmp[490] + result_tmp[491] + result_tmp[492] + result_tmp[493] + result_tmp[494] + result_tmp[495] + result_tmp[496] + result_tmp[497] + result_tmp[498] + result_tmp[499] + result_tmp[500] + result_tmp[501] + result_tmp[502] + result_tmp[503] + result_tmp[504] + result_tmp[505] + result_tmp[506] + result_tmp[507] + result_tmp[508] + result_tmp[509] + result_tmp[510] + result_tmp[511];

    t = omp_get_wtime()-t;
    std::cout<<"512. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END



//THIS CODE AUTOGENETARE BEGIN
  result = 0.0;
  {
    t = omp_get_wtime();
    
    ALIGN(64) double result_tmp[1024]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    for(int i=0; i<N/1024; ++i ){
		result_tmp[0] += a;
		result_tmp[1] += a;
		result_tmp[2] += a;
		result_tmp[3] += a;
		result_tmp[4] += a;
		result_tmp[5] += a;
		result_tmp[6] += a;
		result_tmp[7] += a;
		result_tmp[8] += a;
		result_tmp[9] += a;
		result_tmp[10] += a;
		result_tmp[11] += a;
		result_tmp[12] += a;
		result_tmp[13] += a;
		result_tmp[14] += a;
		result_tmp[15] += a;
		result_tmp[16] += a;
		result_tmp[17] += a;
		result_tmp[18] += a;
		result_tmp[19] += a;
		result_tmp[20] += a;
		result_tmp[21] += a;
		result_tmp[22] += a;
		result_tmp[23] += a;
		result_tmp[24] += a;
		result_tmp[25] += a;
		result_tmp[26] += a;
		result_tmp[27] += a;
		result_tmp[28] += a;
		result_tmp[29] += a;
		result_tmp[30] += a;
		result_tmp[31] += a;
		result_tmp[32] += a;
		result_tmp[33] += a;
		result_tmp[34] += a;
		result_tmp[35] += a;
		result_tmp[36] += a;
		result_tmp[37] += a;
		result_tmp[38] += a;
		result_tmp[39] += a;
		result_tmp[40] += a;
		result_tmp[41] += a;
		result_tmp[42] += a;
		result_tmp[43] += a;
		result_tmp[44] += a;
		result_tmp[45] += a;
		result_tmp[46] += a;
		result_tmp[47] += a;
		result_tmp[48] += a;
		result_tmp[49] += a;
		result_tmp[50] += a;
		result_tmp[51] += a;
		result_tmp[52] += a;
		result_tmp[53] += a;
		result_tmp[54] += a;
		result_tmp[55] += a;
		result_tmp[56] += a;
		result_tmp[57] += a;
		result_tmp[58] += a;
		result_tmp[59] += a;
		result_tmp[60] += a;
		result_tmp[61] += a;
		result_tmp[62] += a;
		result_tmp[63] += a;
		result_tmp[64] += a;
		result_tmp[65] += a;
		result_tmp[66] += a;
		result_tmp[67] += a;
		result_tmp[68] += a;
		result_tmp[69] += a;
		result_tmp[70] += a;
		result_tmp[71] += a;
		result_tmp[72] += a;
		result_tmp[73] += a;
		result_tmp[74] += a;
		result_tmp[75] += a;
		result_tmp[76] += a;
		result_tmp[77] += a;
		result_tmp[78] += a;
		result_tmp[79] += a;
		result_tmp[80] += a;
		result_tmp[81] += a;
		result_tmp[82] += a;
		result_tmp[83] += a;
		result_tmp[84] += a;
		result_tmp[85] += a;
		result_tmp[86] += a;
		result_tmp[87] += a;
		result_tmp[88] += a;
		result_tmp[89] += a;
		result_tmp[90] += a;
		result_tmp[91] += a;
		result_tmp[92] += a;
		result_tmp[93] += a;
		result_tmp[94] += a;
		result_tmp[95] += a;
		result_tmp[96] += a;
		result_tmp[97] += a;
		result_tmp[98] += a;
		result_tmp[99] += a;
		result_tmp[100] += a;
		result_tmp[101] += a;
		result_tmp[102] += a;
		result_tmp[103] += a;
		result_tmp[104] += a;
		result_tmp[105] += a;
		result_tmp[106] += a;
		result_tmp[107] += a;
		result_tmp[108] += a;
		result_tmp[109] += a;
		result_tmp[110] += a;
		result_tmp[111] += a;
		result_tmp[112] += a;
		result_tmp[113] += a;
		result_tmp[114] += a;
		result_tmp[115] += a;
		result_tmp[116] += a;
		result_tmp[117] += a;
		result_tmp[118] += a;
		result_tmp[119] += a;
		result_tmp[120] += a;
		result_tmp[121] += a;
		result_tmp[122] += a;
		result_tmp[123] += a;
		result_tmp[124] += a;
		result_tmp[125] += a;
		result_tmp[126] += a;
		result_tmp[127] += a;
		result_tmp[128] += a;
		result_tmp[129] += a;
		result_tmp[130] += a;
		result_tmp[131] += a;
		result_tmp[132] += a;
		result_tmp[133] += a;
		result_tmp[134] += a;
		result_tmp[135] += a;
		result_tmp[136] += a;
		result_tmp[137] += a;
		result_tmp[138] += a;
		result_tmp[139] += a;
		result_tmp[140] += a;
		result_tmp[141] += a;
		result_tmp[142] += a;
		result_tmp[143] += a;
		result_tmp[144] += a;
		result_tmp[145] += a;
		result_tmp[146] += a;
		result_tmp[147] += a;
		result_tmp[148] += a;
		result_tmp[149] += a;
		result_tmp[150] += a;
		result_tmp[151] += a;
		result_tmp[152] += a;
		result_tmp[153] += a;
		result_tmp[154] += a;
		result_tmp[155] += a;
		result_tmp[156] += a;
		result_tmp[157] += a;
		result_tmp[158] += a;
		result_tmp[159] += a;
		result_tmp[160] += a;
		result_tmp[161] += a;
		result_tmp[162] += a;
		result_tmp[163] += a;
		result_tmp[164] += a;
		result_tmp[165] += a;
		result_tmp[166] += a;
		result_tmp[167] += a;
		result_tmp[168] += a;
		result_tmp[169] += a;
		result_tmp[170] += a;
		result_tmp[171] += a;
		result_tmp[172] += a;
		result_tmp[173] += a;
		result_tmp[174] += a;
		result_tmp[175] += a;
		result_tmp[176] += a;
		result_tmp[177] += a;
		result_tmp[178] += a;
		result_tmp[179] += a;
		result_tmp[180] += a;
		result_tmp[181] += a;
		result_tmp[182] += a;
		result_tmp[183] += a;
		result_tmp[184] += a;
		result_tmp[185] += a;
		result_tmp[186] += a;
		result_tmp[187] += a;
		result_tmp[188] += a;
		result_tmp[189] += a;
		result_tmp[190] += a;
		result_tmp[191] += a;
		result_tmp[192] += a;
		result_tmp[193] += a;
		result_tmp[194] += a;
		result_tmp[195] += a;
		result_tmp[196] += a;
		result_tmp[197] += a;
		result_tmp[198] += a;
		result_tmp[199] += a;
		result_tmp[200] += a;
		result_tmp[201] += a;
		result_tmp[202] += a;
		result_tmp[203] += a;
		result_tmp[204] += a;
		result_tmp[205] += a;
		result_tmp[206] += a;
		result_tmp[207] += a;
		result_tmp[208] += a;
		result_tmp[209] += a;
		result_tmp[210] += a;
		result_tmp[211] += a;
		result_tmp[212] += a;
		result_tmp[213] += a;
		result_tmp[214] += a;
		result_tmp[215] += a;
		result_tmp[216] += a;
		result_tmp[217] += a;
		result_tmp[218] += a;
		result_tmp[219] += a;
		result_tmp[220] += a;
		result_tmp[221] += a;
		result_tmp[222] += a;
		result_tmp[223] += a;
		result_tmp[224] += a;
		result_tmp[225] += a;
		result_tmp[226] += a;
		result_tmp[227] += a;
		result_tmp[228] += a;
		result_tmp[229] += a;
		result_tmp[230] += a;
		result_tmp[231] += a;
		result_tmp[232] += a;
		result_tmp[233] += a;
		result_tmp[234] += a;
		result_tmp[235] += a;
		result_tmp[236] += a;
		result_tmp[237] += a;
		result_tmp[238] += a;
		result_tmp[239] += a;
		result_tmp[240] += a;
		result_tmp[241] += a;
		result_tmp[242] += a;
		result_tmp[243] += a;
		result_tmp[244] += a;
		result_tmp[245] += a;
		result_tmp[246] += a;
		result_tmp[247] += a;
		result_tmp[248] += a;
		result_tmp[249] += a;
		result_tmp[250] += a;
		result_tmp[251] += a;
		result_tmp[252] += a;
		result_tmp[253] += a;
		result_tmp[254] += a;
		result_tmp[255] += a;
		result_tmp[256] += a;
		result_tmp[257] += a;
		result_tmp[258] += a;
		result_tmp[259] += a;
		result_tmp[260] += a;
		result_tmp[261] += a;
		result_tmp[262] += a;
		result_tmp[263] += a;
		result_tmp[264] += a;
		result_tmp[265] += a;
		result_tmp[266] += a;
		result_tmp[267] += a;
		result_tmp[268] += a;
		result_tmp[269] += a;
		result_tmp[270] += a;
		result_tmp[271] += a;
		result_tmp[272] += a;
		result_tmp[273] += a;
		result_tmp[274] += a;
		result_tmp[275] += a;
		result_tmp[276] += a;
		result_tmp[277] += a;
		result_tmp[278] += a;
		result_tmp[279] += a;
		result_tmp[280] += a;
		result_tmp[281] += a;
		result_tmp[282] += a;
		result_tmp[283] += a;
		result_tmp[284] += a;
		result_tmp[285] += a;
		result_tmp[286] += a;
		result_tmp[287] += a;
		result_tmp[288] += a;
		result_tmp[289] += a;
		result_tmp[290] += a;
		result_tmp[291] += a;
		result_tmp[292] += a;
		result_tmp[293] += a;
		result_tmp[294] += a;
		result_tmp[295] += a;
		result_tmp[296] += a;
		result_tmp[297] += a;
		result_tmp[298] += a;
		result_tmp[299] += a;
		result_tmp[300] += a;
		result_tmp[301] += a;
		result_tmp[302] += a;
		result_tmp[303] += a;
		result_tmp[304] += a;
		result_tmp[305] += a;
		result_tmp[306] += a;
		result_tmp[307] += a;
		result_tmp[308] += a;
		result_tmp[309] += a;
		result_tmp[310] += a;
		result_tmp[311] += a;
		result_tmp[312] += a;
		result_tmp[313] += a;
		result_tmp[314] += a;
		result_tmp[315] += a;
		result_tmp[316] += a;
		result_tmp[317] += a;
		result_tmp[318] += a;
		result_tmp[319] += a;
		result_tmp[320] += a;
		result_tmp[321] += a;
		result_tmp[322] += a;
		result_tmp[323] += a;
		result_tmp[324] += a;
		result_tmp[325] += a;
		result_tmp[326] += a;
		result_tmp[327] += a;
		result_tmp[328] += a;
		result_tmp[329] += a;
		result_tmp[330] += a;
		result_tmp[331] += a;
		result_tmp[332] += a;
		result_tmp[333] += a;
		result_tmp[334] += a;
		result_tmp[335] += a;
		result_tmp[336] += a;
		result_tmp[337] += a;
		result_tmp[338] += a;
		result_tmp[339] += a;
		result_tmp[340] += a;
		result_tmp[341] += a;
		result_tmp[342] += a;
		result_tmp[343] += a;
		result_tmp[344] += a;
		result_tmp[345] += a;
		result_tmp[346] += a;
		result_tmp[347] += a;
		result_tmp[348] += a;
		result_tmp[349] += a;
		result_tmp[350] += a;
		result_tmp[351] += a;
		result_tmp[352] += a;
		result_tmp[353] += a;
		result_tmp[354] += a;
		result_tmp[355] += a;
		result_tmp[356] += a;
		result_tmp[357] += a;
		result_tmp[358] += a;
		result_tmp[359] += a;
		result_tmp[360] += a;
		result_tmp[361] += a;
		result_tmp[362] += a;
		result_tmp[363] += a;
		result_tmp[364] += a;
		result_tmp[365] += a;
		result_tmp[366] += a;
		result_tmp[367] += a;
		result_tmp[368] += a;
		result_tmp[369] += a;
		result_tmp[370] += a;
		result_tmp[371] += a;
		result_tmp[372] += a;
		result_tmp[373] += a;
		result_tmp[374] += a;
		result_tmp[375] += a;
		result_tmp[376] += a;
		result_tmp[377] += a;
		result_tmp[378] += a;
		result_tmp[379] += a;
		result_tmp[380] += a;
		result_tmp[381] += a;
		result_tmp[382] += a;
		result_tmp[383] += a;
		result_tmp[384] += a;
		result_tmp[385] += a;
		result_tmp[386] += a;
		result_tmp[387] += a;
		result_tmp[388] += a;
		result_tmp[389] += a;
		result_tmp[390] += a;
		result_tmp[391] += a;
		result_tmp[392] += a;
		result_tmp[393] += a;
		result_tmp[394] += a;
		result_tmp[395] += a;
		result_tmp[396] += a;
		result_tmp[397] += a;
		result_tmp[398] += a;
		result_tmp[399] += a;
		result_tmp[400] += a;
		result_tmp[401] += a;
		result_tmp[402] += a;
		result_tmp[403] += a;
		result_tmp[404] += a;
		result_tmp[405] += a;
		result_tmp[406] += a;
		result_tmp[407] += a;
		result_tmp[408] += a;
		result_tmp[409] += a;
		result_tmp[410] += a;
		result_tmp[411] += a;
		result_tmp[412] += a;
		result_tmp[413] += a;
		result_tmp[414] += a;
		result_tmp[415] += a;
		result_tmp[416] += a;
		result_tmp[417] += a;
		result_tmp[418] += a;
		result_tmp[419] += a;
		result_tmp[420] += a;
		result_tmp[421] += a;
		result_tmp[422] += a;
		result_tmp[423] += a;
		result_tmp[424] += a;
		result_tmp[425] += a;
		result_tmp[426] += a;
		result_tmp[427] += a;
		result_tmp[428] += a;
		result_tmp[429] += a;
		result_tmp[430] += a;
		result_tmp[431] += a;
		result_tmp[432] += a;
		result_tmp[433] += a;
		result_tmp[434] += a;
		result_tmp[435] += a;
		result_tmp[436] += a;
		result_tmp[437] += a;
		result_tmp[438] += a;
		result_tmp[439] += a;
		result_tmp[440] += a;
		result_tmp[441] += a;
		result_tmp[442] += a;
		result_tmp[443] += a;
		result_tmp[444] += a;
		result_tmp[445] += a;
		result_tmp[446] += a;
		result_tmp[447] += a;
		result_tmp[448] += a;
		result_tmp[449] += a;
		result_tmp[450] += a;
		result_tmp[451] += a;
		result_tmp[452] += a;
		result_tmp[453] += a;
		result_tmp[454] += a;
		result_tmp[455] += a;
		result_tmp[456] += a;
		result_tmp[457] += a;
		result_tmp[458] += a;
		result_tmp[459] += a;
		result_tmp[460] += a;
		result_tmp[461] += a;
		result_tmp[462] += a;
		result_tmp[463] += a;
		result_tmp[464] += a;
		result_tmp[465] += a;
		result_tmp[466] += a;
		result_tmp[467] += a;
		result_tmp[468] += a;
		result_tmp[469] += a;
		result_tmp[470] += a;
		result_tmp[471] += a;
		result_tmp[472] += a;
		result_tmp[473] += a;
		result_tmp[474] += a;
		result_tmp[475] += a;
		result_tmp[476] += a;
		result_tmp[477] += a;
		result_tmp[478] += a;
		result_tmp[479] += a;
		result_tmp[480] += a;
		result_tmp[481] += a;
		result_tmp[482] += a;
		result_tmp[483] += a;
		result_tmp[484] += a;
		result_tmp[485] += a;
		result_tmp[486] += a;
		result_tmp[487] += a;
		result_tmp[488] += a;
		result_tmp[489] += a;
		result_tmp[490] += a;
		result_tmp[491] += a;
		result_tmp[492] += a;
		result_tmp[493] += a;
		result_tmp[494] += a;
		result_tmp[495] += a;
		result_tmp[496] += a;
		result_tmp[497] += a;
		result_tmp[498] += a;
		result_tmp[499] += a;
		result_tmp[500] += a;
		result_tmp[501] += a;
		result_tmp[502] += a;
		result_tmp[503] += a;
		result_tmp[504] += a;
		result_tmp[505] += a;
		result_tmp[506] += a;
		result_tmp[507] += a;
		result_tmp[508] += a;
		result_tmp[509] += a;
		result_tmp[510] += a;
		result_tmp[511] += a;
		result_tmp[512] += a;
		result_tmp[513] += a;
		result_tmp[514] += a;
		result_tmp[515] += a;
		result_tmp[516] += a;
		result_tmp[517] += a;
		result_tmp[518] += a;
		result_tmp[519] += a;
		result_tmp[520] += a;
		result_tmp[521] += a;
		result_tmp[522] += a;
		result_tmp[523] += a;
		result_tmp[524] += a;
		result_tmp[525] += a;
		result_tmp[526] += a;
		result_tmp[527] += a;
		result_tmp[528] += a;
		result_tmp[529] += a;
		result_tmp[530] += a;
		result_tmp[531] += a;
		result_tmp[532] += a;
		result_tmp[533] += a;
		result_tmp[534] += a;
		result_tmp[535] += a;
		result_tmp[536] += a;
		result_tmp[537] += a;
		result_tmp[538] += a;
		result_tmp[539] += a;
		result_tmp[540] += a;
		result_tmp[541] += a;
		result_tmp[542] += a;
		result_tmp[543] += a;
		result_tmp[544] += a;
		result_tmp[545] += a;
		result_tmp[546] += a;
		result_tmp[547] += a;
		result_tmp[548] += a;
		result_tmp[549] += a;
		result_tmp[550] += a;
		result_tmp[551] += a;
		result_tmp[552] += a;
		result_tmp[553] += a;
		result_tmp[554] += a;
		result_tmp[555] += a;
		result_tmp[556] += a;
		result_tmp[557] += a;
		result_tmp[558] += a;
		result_tmp[559] += a;
		result_tmp[560] += a;
		result_tmp[561] += a;
		result_tmp[562] += a;
		result_tmp[563] += a;
		result_tmp[564] += a;
		result_tmp[565] += a;
		result_tmp[566] += a;
		result_tmp[567] += a;
		result_tmp[568] += a;
		result_tmp[569] += a;
		result_tmp[570] += a;
		result_tmp[571] += a;
		result_tmp[572] += a;
		result_tmp[573] += a;
		result_tmp[574] += a;
		result_tmp[575] += a;
		result_tmp[576] += a;
		result_tmp[577] += a;
		result_tmp[578] += a;
		result_tmp[579] += a;
		result_tmp[580] += a;
		result_tmp[581] += a;
		result_tmp[582] += a;
		result_tmp[583] += a;
		result_tmp[584] += a;
		result_tmp[585] += a;
		result_tmp[586] += a;
		result_tmp[587] += a;
		result_tmp[588] += a;
		result_tmp[589] += a;
		result_tmp[590] += a;
		result_tmp[591] += a;
		result_tmp[592] += a;
		result_tmp[593] += a;
		result_tmp[594] += a;
		result_tmp[595] += a;
		result_tmp[596] += a;
		result_tmp[597] += a;
		result_tmp[598] += a;
		result_tmp[599] += a;
		result_tmp[600] += a;
		result_tmp[601] += a;
		result_tmp[602] += a;
		result_tmp[603] += a;
		result_tmp[604] += a;
		result_tmp[605] += a;
		result_tmp[606] += a;
		result_tmp[607] += a;
		result_tmp[608] += a;
		result_tmp[609] += a;
		result_tmp[610] += a;
		result_tmp[611] += a;
		result_tmp[612] += a;
		result_tmp[613] += a;
		result_tmp[614] += a;
		result_tmp[615] += a;
		result_tmp[616] += a;
		result_tmp[617] += a;
		result_tmp[618] += a;
		result_tmp[619] += a;
		result_tmp[620] += a;
		result_tmp[621] += a;
		result_tmp[622] += a;
		result_tmp[623] += a;
		result_tmp[624] += a;
		result_tmp[625] += a;
		result_tmp[626] += a;
		result_tmp[627] += a;
		result_tmp[628] += a;
		result_tmp[629] += a;
		result_tmp[630] += a;
		result_tmp[631] += a;
		result_tmp[632] += a;
		result_tmp[633] += a;
		result_tmp[634] += a;
		result_tmp[635] += a;
		result_tmp[636] += a;
		result_tmp[637] += a;
		result_tmp[638] += a;
		result_tmp[639] += a;
		result_tmp[640] += a;
		result_tmp[641] += a;
		result_tmp[642] += a;
		result_tmp[643] += a;
		result_tmp[644] += a;
		result_tmp[645] += a;
		result_tmp[646] += a;
		result_tmp[647] += a;
		result_tmp[648] += a;
		result_tmp[649] += a;
		result_tmp[650] += a;
		result_tmp[651] += a;
		result_tmp[652] += a;
		result_tmp[653] += a;
		result_tmp[654] += a;
		result_tmp[655] += a;
		result_tmp[656] += a;
		result_tmp[657] += a;
		result_tmp[658] += a;
		result_tmp[659] += a;
		result_tmp[660] += a;
		result_tmp[661] += a;
		result_tmp[662] += a;
		result_tmp[663] += a;
		result_tmp[664] += a;
		result_tmp[665] += a;
		result_tmp[666] += a;
		result_tmp[667] += a;
		result_tmp[668] += a;
		result_tmp[669] += a;
		result_tmp[670] += a;
		result_tmp[671] += a;
		result_tmp[672] += a;
		result_tmp[673] += a;
		result_tmp[674] += a;
		result_tmp[675] += a;
		result_tmp[676] += a;
		result_tmp[677] += a;
		result_tmp[678] += a;
		result_tmp[679] += a;
		result_tmp[680] += a;
		result_tmp[681] += a;
		result_tmp[682] += a;
		result_tmp[683] += a;
		result_tmp[684] += a;
		result_tmp[685] += a;
		result_tmp[686] += a;
		result_tmp[687] += a;
		result_tmp[688] += a;
		result_tmp[689] += a;
		result_tmp[690] += a;
		result_tmp[691] += a;
		result_tmp[692] += a;
		result_tmp[693] += a;
		result_tmp[694] += a;
		result_tmp[695] += a;
		result_tmp[696] += a;
		result_tmp[697] += a;
		result_tmp[698] += a;
		result_tmp[699] += a;
		result_tmp[700] += a;
		result_tmp[701] += a;
		result_tmp[702] += a;
		result_tmp[703] += a;
		result_tmp[704] += a;
		result_tmp[705] += a;
		result_tmp[706] += a;
		result_tmp[707] += a;
		result_tmp[708] += a;
		result_tmp[709] += a;
		result_tmp[710] += a;
		result_tmp[711] += a;
		result_tmp[712] += a;
		result_tmp[713] += a;
		result_tmp[714] += a;
		result_tmp[715] += a;
		result_tmp[716] += a;
		result_tmp[717] += a;
		result_tmp[718] += a;
		result_tmp[719] += a;
		result_tmp[720] += a;
		result_tmp[721] += a;
		result_tmp[722] += a;
		result_tmp[723] += a;
		result_tmp[724] += a;
		result_tmp[725] += a;
		result_tmp[726] += a;
		result_tmp[727] += a;
		result_tmp[728] += a;
		result_tmp[729] += a;
		result_tmp[730] += a;
		result_tmp[731] += a;
		result_tmp[732] += a;
		result_tmp[733] += a;
		result_tmp[734] += a;
		result_tmp[735] += a;
		result_tmp[736] += a;
		result_tmp[737] += a;
		result_tmp[738] += a;
		result_tmp[739] += a;
		result_tmp[740] += a;
		result_tmp[741] += a;
		result_tmp[742] += a;
		result_tmp[743] += a;
		result_tmp[744] += a;
		result_tmp[745] += a;
		result_tmp[746] += a;
		result_tmp[747] += a;
		result_tmp[748] += a;
		result_tmp[749] += a;
		result_tmp[750] += a;
		result_tmp[751] += a;
		result_tmp[752] += a;
		result_tmp[753] += a;
		result_tmp[754] += a;
		result_tmp[755] += a;
		result_tmp[756] += a;
		result_tmp[757] += a;
		result_tmp[758] += a;
		result_tmp[759] += a;
		result_tmp[760] += a;
		result_tmp[761] += a;
		result_tmp[762] += a;
		result_tmp[763] += a;
		result_tmp[764] += a;
		result_tmp[765] += a;
		result_tmp[766] += a;
		result_tmp[767] += a;
		result_tmp[768] += a;
		result_tmp[769] += a;
		result_tmp[770] += a;
		result_tmp[771] += a;
		result_tmp[772] += a;
		result_tmp[773] += a;
		result_tmp[774] += a;
		result_tmp[775] += a;
		result_tmp[776] += a;
		result_tmp[777] += a;
		result_tmp[778] += a;
		result_tmp[779] += a;
		result_tmp[780] += a;
		result_tmp[781] += a;
		result_tmp[782] += a;
		result_tmp[783] += a;
		result_tmp[784] += a;
		result_tmp[785] += a;
		result_tmp[786] += a;
		result_tmp[787] += a;
		result_tmp[788] += a;
		result_tmp[789] += a;
		result_tmp[790] += a;
		result_tmp[791] += a;
		result_tmp[792] += a;
		result_tmp[793] += a;
		result_tmp[794] += a;
		result_tmp[795] += a;
		result_tmp[796] += a;
		result_tmp[797] += a;
		result_tmp[798] += a;
		result_tmp[799] += a;
		result_tmp[800] += a;
		result_tmp[801] += a;
		result_tmp[802] += a;
		result_tmp[803] += a;
		result_tmp[804] += a;
		result_tmp[805] += a;
		result_tmp[806] += a;
		result_tmp[807] += a;
		result_tmp[808] += a;
		result_tmp[809] += a;
		result_tmp[810] += a;
		result_tmp[811] += a;
		result_tmp[812] += a;
		result_tmp[813] += a;
		result_tmp[814] += a;
		result_tmp[815] += a;
		result_tmp[816] += a;
		result_tmp[817] += a;
		result_tmp[818] += a;
		result_tmp[819] += a;
		result_tmp[820] += a;
		result_tmp[821] += a;
		result_tmp[822] += a;
		result_tmp[823] += a;
		result_tmp[824] += a;
		result_tmp[825] += a;
		result_tmp[826] += a;
		result_tmp[827] += a;
		result_tmp[828] += a;
		result_tmp[829] += a;
		result_tmp[830] += a;
		result_tmp[831] += a;
		result_tmp[832] += a;
		result_tmp[833] += a;
		result_tmp[834] += a;
		result_tmp[835] += a;
		result_tmp[836] += a;
		result_tmp[837] += a;
		result_tmp[838] += a;
		result_tmp[839] += a;
		result_tmp[840] += a;
		result_tmp[841] += a;
		result_tmp[842] += a;
		result_tmp[843] += a;
		result_tmp[844] += a;
		result_tmp[845] += a;
		result_tmp[846] += a;
		result_tmp[847] += a;
		result_tmp[848] += a;
		result_tmp[849] += a;
		result_tmp[850] += a;
		result_tmp[851] += a;
		result_tmp[852] += a;
		result_tmp[853] += a;
		result_tmp[854] += a;
		result_tmp[855] += a;
		result_tmp[856] += a;
		result_tmp[857] += a;
		result_tmp[858] += a;
		result_tmp[859] += a;
		result_tmp[860] += a;
		result_tmp[861] += a;
		result_tmp[862] += a;
		result_tmp[863] += a;
		result_tmp[864] += a;
		result_tmp[865] += a;
		result_tmp[866] += a;
		result_tmp[867] += a;
		result_tmp[868] += a;
		result_tmp[869] += a;
		result_tmp[870] += a;
		result_tmp[871] += a;
		result_tmp[872] += a;
		result_tmp[873] += a;
		result_tmp[874] += a;
		result_tmp[875] += a;
		result_tmp[876] += a;
		result_tmp[877] += a;
		result_tmp[878] += a;
		result_tmp[879] += a;
		result_tmp[880] += a;
		result_tmp[881] += a;
		result_tmp[882] += a;
		result_tmp[883] += a;
		result_tmp[884] += a;
		result_tmp[885] += a;
		result_tmp[886] += a;
		result_tmp[887] += a;
		result_tmp[888] += a;
		result_tmp[889] += a;
		result_tmp[890] += a;
		result_tmp[891] += a;
		result_tmp[892] += a;
		result_tmp[893] += a;
		result_tmp[894] += a;
		result_tmp[895] += a;
		result_tmp[896] += a;
		result_tmp[897] += a;
		result_tmp[898] += a;
		result_tmp[899] += a;
		result_tmp[900] += a;
		result_tmp[901] += a;
		result_tmp[902] += a;
		result_tmp[903] += a;
		result_tmp[904] += a;
		result_tmp[905] += a;
		result_tmp[906] += a;
		result_tmp[907] += a;
		result_tmp[908] += a;
		result_tmp[909] += a;
		result_tmp[910] += a;
		result_tmp[911] += a;
		result_tmp[912] += a;
		result_tmp[913] += a;
		result_tmp[914] += a;
		result_tmp[915] += a;
		result_tmp[916] += a;
		result_tmp[917] += a;
		result_tmp[918] += a;
		result_tmp[919] += a;
		result_tmp[920] += a;
		result_tmp[921] += a;
		result_tmp[922] += a;
		result_tmp[923] += a;
		result_tmp[924] += a;
		result_tmp[925] += a;
		result_tmp[926] += a;
		result_tmp[927] += a;
		result_tmp[928] += a;
		result_tmp[929] += a;
		result_tmp[930] += a;
		result_tmp[931] += a;
		result_tmp[932] += a;
		result_tmp[933] += a;
		result_tmp[934] += a;
		result_tmp[935] += a;
		result_tmp[936] += a;
		result_tmp[937] += a;
		result_tmp[938] += a;
		result_tmp[939] += a;
		result_tmp[940] += a;
		result_tmp[941] += a;
		result_tmp[942] += a;
		result_tmp[943] += a;
		result_tmp[944] += a;
		result_tmp[945] += a;
		result_tmp[946] += a;
		result_tmp[947] += a;
		result_tmp[948] += a;
		result_tmp[949] += a;
		result_tmp[950] += a;
		result_tmp[951] += a;
		result_tmp[952] += a;
		result_tmp[953] += a;
		result_tmp[954] += a;
		result_tmp[955] += a;
		result_tmp[956] += a;
		result_tmp[957] += a;
		result_tmp[958] += a;
		result_tmp[959] += a;
		result_tmp[960] += a;
		result_tmp[961] += a;
		result_tmp[962] += a;
		result_tmp[963] += a;
		result_tmp[964] += a;
		result_tmp[965] += a;
		result_tmp[966] += a;
		result_tmp[967] += a;
		result_tmp[968] += a;
		result_tmp[969] += a;
		result_tmp[970] += a;
		result_tmp[971] += a;
		result_tmp[972] += a;
		result_tmp[973] += a;
		result_tmp[974] += a;
		result_tmp[975] += a;
		result_tmp[976] += a;
		result_tmp[977] += a;
		result_tmp[978] += a;
		result_tmp[979] += a;
		result_tmp[980] += a;
		result_tmp[981] += a;
		result_tmp[982] += a;
		result_tmp[983] += a;
		result_tmp[984] += a;
		result_tmp[985] += a;
		result_tmp[986] += a;
		result_tmp[987] += a;
		result_tmp[988] += a;
		result_tmp[989] += a;
		result_tmp[990] += a;
		result_tmp[991] += a;
		result_tmp[992] += a;
		result_tmp[993] += a;
		result_tmp[994] += a;
		result_tmp[995] += a;
		result_tmp[996] += a;
		result_tmp[997] += a;
		result_tmp[998] += a;
		result_tmp[999] += a;
		result_tmp[1000] += a;
		result_tmp[1001] += a;
		result_tmp[1002] += a;
		result_tmp[1003] += a;
		result_tmp[1004] += a;
		result_tmp[1005] += a;
		result_tmp[1006] += a;
		result_tmp[1007] += a;
		result_tmp[1008] += a;
		result_tmp[1009] += a;
		result_tmp[1010] += a;
		result_tmp[1011] += a;
		result_tmp[1012] += a;
		result_tmp[1013] += a;
		result_tmp[1014] += a;
		result_tmp[1015] += a;
		result_tmp[1016] += a;
		result_tmp[1017] += a;
		result_tmp[1018] += a;
		result_tmp[1019] += a;
		result_tmp[1020] += a;
		result_tmp[1021] += a;
		result_tmp[1022] += a;
		result_tmp[1023] += a;
	}

    for(int i=0; i<N%1024; ++i) result += a;

    result +=result_tmp[0] + result_tmp[1] + result_tmp[2] + result_tmp[3] + result_tmp[4] + result_tmp[5] + result_tmp[6] + result_tmp[7] + result_tmp[8] + result_tmp[9] + result_tmp[10] + result_tmp[11] + result_tmp[12] + result_tmp[13] + result_tmp[14] + result_tmp[15] + result_tmp[16] + result_tmp[17] + result_tmp[18] + result_tmp[19] + result_tmp[20] + result_tmp[21] + result_tmp[22] + result_tmp[23] + result_tmp[24] + result_tmp[25] + result_tmp[26] + result_tmp[27] + result_tmp[28] + result_tmp[29] + result_tmp[30] + result_tmp[31] + result_tmp[32] + result_tmp[33] + result_tmp[34] + result_tmp[35] + result_tmp[36] + result_tmp[37] + result_tmp[38] + result_tmp[39] + result_tmp[40] + result_tmp[41] + result_tmp[42] + result_tmp[43] + result_tmp[44] + result_tmp[45] + result_tmp[46] + result_tmp[47] + result_tmp[48] + result_tmp[49] + result_tmp[50] + result_tmp[51] + result_tmp[52] + result_tmp[53] + result_tmp[54] + result_tmp[55] + result_tmp[56] + result_tmp[57] + result_tmp[58] + result_tmp[59] + result_tmp[60] + result_tmp[61] + result_tmp[62] + result_tmp[63] + result_tmp[64] + result_tmp[65] + result_tmp[66] + result_tmp[67] + result_tmp[68] + result_tmp[69] + result_tmp[70] + result_tmp[71] + result_tmp[72] + result_tmp[73] + result_tmp[74] + result_tmp[75] + result_tmp[76] + result_tmp[77] + result_tmp[78] + result_tmp[79] + result_tmp[80] + result_tmp[81] + result_tmp[82] + result_tmp[83] + result_tmp[84] + result_tmp[85] + result_tmp[86] + result_tmp[87] + result_tmp[88] + result_tmp[89] + result_tmp[90] + result_tmp[91] + result_tmp[92] + result_tmp[93] + result_tmp[94] + result_tmp[95] + result_tmp[96] + result_tmp[97] + result_tmp[98] + result_tmp[99] + result_tmp[100] + result_tmp[101] + result_tmp[102] + result_tmp[103] + result_tmp[104] + result_tmp[105] + result_tmp[106] + result_tmp[107] + result_tmp[108] + result_tmp[109] + result_tmp[110] + result_tmp[111] + result_tmp[112] + result_tmp[113] + result_tmp[114] + result_tmp[115] + result_tmp[116] + result_tmp[117] + result_tmp[118] + result_tmp[119] + result_tmp[120] + result_tmp[121] + result_tmp[122] + result_tmp[123] + result_tmp[124] + result_tmp[125] + result_tmp[126] + result_tmp[127] + result_tmp[128] + result_tmp[129] + result_tmp[130] + result_tmp[131] + result_tmp[132] + result_tmp[133] + result_tmp[134] + result_tmp[135] + result_tmp[136] + result_tmp[137] + result_tmp[138] + result_tmp[139] + result_tmp[140] + result_tmp[141] + result_tmp[142] + result_tmp[143] + result_tmp[144] + result_tmp[145] + result_tmp[146] + result_tmp[147] + result_tmp[148] + result_tmp[149] + result_tmp[150] + result_tmp[151] + result_tmp[152] + result_tmp[153] + result_tmp[154] + result_tmp[155] + result_tmp[156] + result_tmp[157] + result_tmp[158] + result_tmp[159] + result_tmp[160] + result_tmp[161] + result_tmp[162] + result_tmp[163] + result_tmp[164] + result_tmp[165] + result_tmp[166] + result_tmp[167] + result_tmp[168] + result_tmp[169] + result_tmp[170] + result_tmp[171] + result_tmp[172] + result_tmp[173] + result_tmp[174] + result_tmp[175] + result_tmp[176] + result_tmp[177] + result_tmp[178] + result_tmp[179] + result_tmp[180] + result_tmp[181] + result_tmp[182] + result_tmp[183] + result_tmp[184] + result_tmp[185] + result_tmp[186] + result_tmp[187] + result_tmp[188] + result_tmp[189] + result_tmp[190] + result_tmp[191] + result_tmp[192] + result_tmp[193] + result_tmp[194] + result_tmp[195] + result_tmp[196] + result_tmp[197] + result_tmp[198] + result_tmp[199] + result_tmp[200] + result_tmp[201] + result_tmp[202] + result_tmp[203] + result_tmp[204] + result_tmp[205] + result_tmp[206] + result_tmp[207] + result_tmp[208] + result_tmp[209] + result_tmp[210] + result_tmp[211] + result_tmp[212] + result_tmp[213] + result_tmp[214] + result_tmp[215] + result_tmp[216] + result_tmp[217] + result_tmp[218] + result_tmp[219] + result_tmp[220] + result_tmp[221] + result_tmp[222] + result_tmp[223] + result_tmp[224] + result_tmp[225] + result_tmp[226] + result_tmp[227] + result_tmp[228] + result_tmp[229] + result_tmp[230] + result_tmp[231] + result_tmp[232] + result_tmp[233] + result_tmp[234] + result_tmp[235] + result_tmp[236] + result_tmp[237] + result_tmp[238] + result_tmp[239] + result_tmp[240] + result_tmp[241] + result_tmp[242] + result_tmp[243] + result_tmp[244] + result_tmp[245] + result_tmp[246] + result_tmp[247] + result_tmp[248] + result_tmp[249] + result_tmp[250] + result_tmp[251] + result_tmp[252] + result_tmp[253] + result_tmp[254] + result_tmp[255] + result_tmp[256] + result_tmp[257] + result_tmp[258] + result_tmp[259] + result_tmp[260] + result_tmp[261] + result_tmp[262] + result_tmp[263] + result_tmp[264] + result_tmp[265] + result_tmp[266] + result_tmp[267] + result_tmp[268] + result_tmp[269] + result_tmp[270] + result_tmp[271] + result_tmp[272] + result_tmp[273] + result_tmp[274] + result_tmp[275] + result_tmp[276] + result_tmp[277] + result_tmp[278] + result_tmp[279] + result_tmp[280] + result_tmp[281] + result_tmp[282] + result_tmp[283] + result_tmp[284] + result_tmp[285] + result_tmp[286] + result_tmp[287] + result_tmp[288] + result_tmp[289] + result_tmp[290] + result_tmp[291] + result_tmp[292] + result_tmp[293] + result_tmp[294] + result_tmp[295] + result_tmp[296] + result_tmp[297] + result_tmp[298] + result_tmp[299] + result_tmp[300] + result_tmp[301] + result_tmp[302] + result_tmp[303] + result_tmp[304] + result_tmp[305] + result_tmp[306] + result_tmp[307] + result_tmp[308] + result_tmp[309] + result_tmp[310] + result_tmp[311] + result_tmp[312] + result_tmp[313] + result_tmp[314] + result_tmp[315] + result_tmp[316] + result_tmp[317] + result_tmp[318] + result_tmp[319] + result_tmp[320] + result_tmp[321] + result_tmp[322] + result_tmp[323] + result_tmp[324] + result_tmp[325] + result_tmp[326] + result_tmp[327] + result_tmp[328] + result_tmp[329] + result_tmp[330] + result_tmp[331] + result_tmp[332] + result_tmp[333] + result_tmp[334] + result_tmp[335] + result_tmp[336] + result_tmp[337] + result_tmp[338] + result_tmp[339] + result_tmp[340] + result_tmp[341] + result_tmp[342] + result_tmp[343] + result_tmp[344] + result_tmp[345] + result_tmp[346] + result_tmp[347] + result_tmp[348] + result_tmp[349] + result_tmp[350] + result_tmp[351] + result_tmp[352] + result_tmp[353] + result_tmp[354] + result_tmp[355] + result_tmp[356] + result_tmp[357] + result_tmp[358] + result_tmp[359] + result_tmp[360] + result_tmp[361] + result_tmp[362] + result_tmp[363] + result_tmp[364] + result_tmp[365] + result_tmp[366] + result_tmp[367] + result_tmp[368] + result_tmp[369] + result_tmp[370] + result_tmp[371] + result_tmp[372] + result_tmp[373] + result_tmp[374] + result_tmp[375] + result_tmp[376] + result_tmp[377] + result_tmp[378] + result_tmp[379] + result_tmp[380] + result_tmp[381] + result_tmp[382] + result_tmp[383] + result_tmp[384] + result_tmp[385] + result_tmp[386] + result_tmp[387] + result_tmp[388] + result_tmp[389] + result_tmp[390] + result_tmp[391] + result_tmp[392] + result_tmp[393] + result_tmp[394] + result_tmp[395] + result_tmp[396] + result_tmp[397] + result_tmp[398] + result_tmp[399] + result_tmp[400] + result_tmp[401] + result_tmp[402] + result_tmp[403] + result_tmp[404] + result_tmp[405] + result_tmp[406] + result_tmp[407] + result_tmp[408] + result_tmp[409] + result_tmp[410] + result_tmp[411] + result_tmp[412] + result_tmp[413] + result_tmp[414] + result_tmp[415] + result_tmp[416] + result_tmp[417] + result_tmp[418] + result_tmp[419] + result_tmp[420] + result_tmp[421] + result_tmp[422] + result_tmp[423] + result_tmp[424] + result_tmp[425] + result_tmp[426] + result_tmp[427] + result_tmp[428] + result_tmp[429] + result_tmp[430] + result_tmp[431] + result_tmp[432] + result_tmp[433] + result_tmp[434] + result_tmp[435] + result_tmp[436] + result_tmp[437] + result_tmp[438] + result_tmp[439] + result_tmp[440] + result_tmp[441] + result_tmp[442] + result_tmp[443] + result_tmp[444] + result_tmp[445] + result_tmp[446] + result_tmp[447] + result_tmp[448] + result_tmp[449] + result_tmp[450] + result_tmp[451] + result_tmp[452] + result_tmp[453] + result_tmp[454] + result_tmp[455] + result_tmp[456] + result_tmp[457] + result_tmp[458] + result_tmp[459] + result_tmp[460] + result_tmp[461] + result_tmp[462] + result_tmp[463] + result_tmp[464] + result_tmp[465] + result_tmp[466] + result_tmp[467] + result_tmp[468] + result_tmp[469] + result_tmp[470] + result_tmp[471] + result_tmp[472] + result_tmp[473] + result_tmp[474] + result_tmp[475] + result_tmp[476] + result_tmp[477] + result_tmp[478] + result_tmp[479] + result_tmp[480] + result_tmp[481] + result_tmp[482] + result_tmp[483] + result_tmp[484] + result_tmp[485] + result_tmp[486] + result_tmp[487] + result_tmp[488] + result_tmp[489] + result_tmp[490] + result_tmp[491] + result_tmp[492] + result_tmp[493] + result_tmp[494] + result_tmp[495] + result_tmp[496] + result_tmp[497] + result_tmp[498] + result_tmp[499] + result_tmp[500] + result_tmp[501] + result_tmp[502] + result_tmp[503] + result_tmp[504] + result_tmp[505] + result_tmp[506] + result_tmp[507] + result_tmp[508] + result_tmp[509] + result_tmp[510] + result_tmp[511] + result_tmp[512] + result_tmp[513] + result_tmp[514] + result_tmp[515] + result_tmp[516] + result_tmp[517] + result_tmp[518] + result_tmp[519] + result_tmp[520] + result_tmp[521] + result_tmp[522] + result_tmp[523] + result_tmp[524] + result_tmp[525] + result_tmp[526] + result_tmp[527] + result_tmp[528] + result_tmp[529] + result_tmp[530] + result_tmp[531] + result_tmp[532] + result_tmp[533] + result_tmp[534] + result_tmp[535] + result_tmp[536] + result_tmp[537] + result_tmp[538] + result_tmp[539] + result_tmp[540] + result_tmp[541] + result_tmp[542] + result_tmp[543] + result_tmp[544] + result_tmp[545] + result_tmp[546] + result_tmp[547] + result_tmp[548] + result_tmp[549] + result_tmp[550] + result_tmp[551] + result_tmp[552] + result_tmp[553] + result_tmp[554] + result_tmp[555] + result_tmp[556] + result_tmp[557] + result_tmp[558] + result_tmp[559] + result_tmp[560] + result_tmp[561] + result_tmp[562] + result_tmp[563] + result_tmp[564] + result_tmp[565] + result_tmp[566] + result_tmp[567] + result_tmp[568] + result_tmp[569] + result_tmp[570] + result_tmp[571] + result_tmp[572] + result_tmp[573] + result_tmp[574] + result_tmp[575] + result_tmp[576] + result_tmp[577] + result_tmp[578] + result_tmp[579] + result_tmp[580] + result_tmp[581] + result_tmp[582] + result_tmp[583] + result_tmp[584] + result_tmp[585] + result_tmp[586] + result_tmp[587] + result_tmp[588] + result_tmp[589] + result_tmp[590] + result_tmp[591] + result_tmp[592] + result_tmp[593] + result_tmp[594] + result_tmp[595] + result_tmp[596] + result_tmp[597] + result_tmp[598] + result_tmp[599] + result_tmp[600] + result_tmp[601] + result_tmp[602] + result_tmp[603] + result_tmp[604] + result_tmp[605] + result_tmp[606] + result_tmp[607] + result_tmp[608] + result_tmp[609] + result_tmp[610] + result_tmp[611] + result_tmp[612] + result_tmp[613] + result_tmp[614] + result_tmp[615] + result_tmp[616] + result_tmp[617] + result_tmp[618] + result_tmp[619] + result_tmp[620] + result_tmp[621] + result_tmp[622] + result_tmp[623] + result_tmp[624] + result_tmp[625] + result_tmp[626] + result_tmp[627] + result_tmp[628] + result_tmp[629] + result_tmp[630] + result_tmp[631] + result_tmp[632] + result_tmp[633] + result_tmp[634] + result_tmp[635] + result_tmp[636] + result_tmp[637] + result_tmp[638] + result_tmp[639] + result_tmp[640] + result_tmp[641] + result_tmp[642] + result_tmp[643] + result_tmp[644] + result_tmp[645] + result_tmp[646] + result_tmp[647] + result_tmp[648] + result_tmp[649] + result_tmp[650] + result_tmp[651] + result_tmp[652] + result_tmp[653] + result_tmp[654] + result_tmp[655] + result_tmp[656] + result_tmp[657] + result_tmp[658] + result_tmp[659] + result_tmp[660] + result_tmp[661] + result_tmp[662] + result_tmp[663] + result_tmp[664] + result_tmp[665] + result_tmp[666] + result_tmp[667] + result_tmp[668] + result_tmp[669] + result_tmp[670] + result_tmp[671] + result_tmp[672] + result_tmp[673] + result_tmp[674] + result_tmp[675] + result_tmp[676] + result_tmp[677] + result_tmp[678] + result_tmp[679] + result_tmp[680] + result_tmp[681] + result_tmp[682] + result_tmp[683] + result_tmp[684] + result_tmp[685] + result_tmp[686] + result_tmp[687] + result_tmp[688] + result_tmp[689] + result_tmp[690] + result_tmp[691] + result_tmp[692] + result_tmp[693] + result_tmp[694] + result_tmp[695] + result_tmp[696] + result_tmp[697] + result_tmp[698] + result_tmp[699] + result_tmp[700] + result_tmp[701] + result_tmp[702] + result_tmp[703] + result_tmp[704] + result_tmp[705] + result_tmp[706] + result_tmp[707] + result_tmp[708] + result_tmp[709] + result_tmp[710] + result_tmp[711] + result_tmp[712] + result_tmp[713] + result_tmp[714] + result_tmp[715] + result_tmp[716] + result_tmp[717] + result_tmp[718] + result_tmp[719] + result_tmp[720] + result_tmp[721] + result_tmp[722] + result_tmp[723] + result_tmp[724] + result_tmp[725] + result_tmp[726] + result_tmp[727] + result_tmp[728] + result_tmp[729] + result_tmp[730] + result_tmp[731] + result_tmp[732] + result_tmp[733] + result_tmp[734] + result_tmp[735] + result_tmp[736] + result_tmp[737] + result_tmp[738] + result_tmp[739] + result_tmp[740] + result_tmp[741] + result_tmp[742] + result_tmp[743] + result_tmp[744] + result_tmp[745] + result_tmp[746] + result_tmp[747] + result_tmp[748] + result_tmp[749] + result_tmp[750] + result_tmp[751] + result_tmp[752] + result_tmp[753] + result_tmp[754] + result_tmp[755] + result_tmp[756] + result_tmp[757] + result_tmp[758] + result_tmp[759] + result_tmp[760] + result_tmp[761] + result_tmp[762] + result_tmp[763] + result_tmp[764] + result_tmp[765] + result_tmp[766] + result_tmp[767] + result_tmp[768] + result_tmp[769] + result_tmp[770] + result_tmp[771] + result_tmp[772] + result_tmp[773] + result_tmp[774] + result_tmp[775] + result_tmp[776] + result_tmp[777] + result_tmp[778] + result_tmp[779] + result_tmp[780] + result_tmp[781] + result_tmp[782] + result_tmp[783] + result_tmp[784] + result_tmp[785] + result_tmp[786] + result_tmp[787] + result_tmp[788] + result_tmp[789] + result_tmp[790] + result_tmp[791] + result_tmp[792] + result_tmp[793] + result_tmp[794] + result_tmp[795] + result_tmp[796] + result_tmp[797] + result_tmp[798] + result_tmp[799] + result_tmp[800] + result_tmp[801] + result_tmp[802] + result_tmp[803] + result_tmp[804] + result_tmp[805] + result_tmp[806] + result_tmp[807] + result_tmp[808] + result_tmp[809] + result_tmp[810] + result_tmp[811] + result_tmp[812] + result_tmp[813] + result_tmp[814] + result_tmp[815] + result_tmp[816] + result_tmp[817] + result_tmp[818] + result_tmp[819] + result_tmp[820] + result_tmp[821] + result_tmp[822] + result_tmp[823] + result_tmp[824] + result_tmp[825] + result_tmp[826] + result_tmp[827] + result_tmp[828] + result_tmp[829] + result_tmp[830] + result_tmp[831] + result_tmp[832] + result_tmp[833] + result_tmp[834] + result_tmp[835] + result_tmp[836] + result_tmp[837] + result_tmp[838] + result_tmp[839] + result_tmp[840] + result_tmp[841] + result_tmp[842] + result_tmp[843] + result_tmp[844] + result_tmp[845] + result_tmp[846] + result_tmp[847] + result_tmp[848] + result_tmp[849] + result_tmp[850] + result_tmp[851] + result_tmp[852] + result_tmp[853] + result_tmp[854] + result_tmp[855] + result_tmp[856] + result_tmp[857] + result_tmp[858] + result_tmp[859] + result_tmp[860] + result_tmp[861] + result_tmp[862] + result_tmp[863] + result_tmp[864] + result_tmp[865] + result_tmp[866] + result_tmp[867] + result_tmp[868] + result_tmp[869] + result_tmp[870] + result_tmp[871] + result_tmp[872] + result_tmp[873] + result_tmp[874] + result_tmp[875] + result_tmp[876] + result_tmp[877] + result_tmp[878] + result_tmp[879] + result_tmp[880] + result_tmp[881] + result_tmp[882] + result_tmp[883] + result_tmp[884] + result_tmp[885] + result_tmp[886] + result_tmp[887] + result_tmp[888] + result_tmp[889] + result_tmp[890] + result_tmp[891] + result_tmp[892] + result_tmp[893] + result_tmp[894] + result_tmp[895] + result_tmp[896] + result_tmp[897] + result_tmp[898] + result_tmp[899] + result_tmp[900] + result_tmp[901] + result_tmp[902] + result_tmp[903] + result_tmp[904] + result_tmp[905] + result_tmp[906] + result_tmp[907] + result_tmp[908] + result_tmp[909] + result_tmp[910] + result_tmp[911] + result_tmp[912] + result_tmp[913] + result_tmp[914] + result_tmp[915] + result_tmp[916] + result_tmp[917] + result_tmp[918] + result_tmp[919] + result_tmp[920] + result_tmp[921] + result_tmp[922] + result_tmp[923] + result_tmp[924] + result_tmp[925] + result_tmp[926] + result_tmp[927] + result_tmp[928] + result_tmp[929] + result_tmp[930] + result_tmp[931] + result_tmp[932] + result_tmp[933] + result_tmp[934] + result_tmp[935] + result_tmp[936] + result_tmp[937] + result_tmp[938] + result_tmp[939] + result_tmp[940] + result_tmp[941] + result_tmp[942] + result_tmp[943] + result_tmp[944] + result_tmp[945] + result_tmp[946] + result_tmp[947] + result_tmp[948] + result_tmp[949] + result_tmp[950] + result_tmp[951] + result_tmp[952] + result_tmp[953] + result_tmp[954] + result_tmp[955] + result_tmp[956] + result_tmp[957] + result_tmp[958] + result_tmp[959] + result_tmp[960] + result_tmp[961] + result_tmp[962] + result_tmp[963] + result_tmp[964] + result_tmp[965] + result_tmp[966] + result_tmp[967] + result_tmp[968] + result_tmp[969] + result_tmp[970] + result_tmp[971] + result_tmp[972] + result_tmp[973] + result_tmp[974] + result_tmp[975] + result_tmp[976] + result_tmp[977] + result_tmp[978] + result_tmp[979] + result_tmp[980] + result_tmp[981] + result_tmp[982] + result_tmp[983] + result_tmp[984] + result_tmp[985] + result_tmp[986] + result_tmp[987] + result_tmp[988] + result_tmp[989] + result_tmp[990] + result_tmp[991] + result_tmp[992] + result_tmp[993] + result_tmp[994] + result_tmp[995] + result_tmp[996] + result_tmp[997] + result_tmp[998] + result_tmp[999] + result_tmp[1000] + result_tmp[1001] + result_tmp[1002] + result_tmp[1003] + result_tmp[1004] + result_tmp[1005] + result_tmp[1006] + result_tmp[1007] + result_tmp[1008] + result_tmp[1009] + result_tmp[1010] + result_tmp[1011] + result_tmp[1012] + result_tmp[1013] + result_tmp[1014] + result_tmp[1015] + result_tmp[1016] + result_tmp[1017] + result_tmp[1018] + result_tmp[1019] + result_tmp[1020] + result_tmp[1021] + result_tmp[1022] + result_tmp[1023];

    t = omp_get_wtime()-t;
    std::cout<<"1024. result: "<<std::setprecision(13)<<result<<std::setprecision(3)<<", time: "<<t<<" s, GFLOPS: "<<N/(t*1E9)<<"\n";
  }
//END


  
  return 0;
}
