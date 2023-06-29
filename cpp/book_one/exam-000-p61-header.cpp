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
  
