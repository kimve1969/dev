/************************************************************************
Copyright:      "Surgutneftegas" PJSC
Autors:         Created by Kim V.E. 2023/08/24
Target:         For education
Annotation:     Index of threads & blocks

*************************************************************************/
#include<iostream>
#include<string>
#include<omp.h>


__global__ void VecAdd(int* pA){
        // index of thread by block
        int ind_tb = threadIdx.x + blockDim.x*threadIdx.y + blockDim.x*blockDim.y*threadIdx.z;
        // index of block by grid
        int ind_bg = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
        // index of tread by grid
        int ind_tg = ind_tb + blockDim.x*blockDim.y*blockDim.z*ind_bg;

        printf("bx.by.bz:tx.ty.tz\t%d.%d.%d:%d.%d.%d\tblock of grid: %d, thread of block: %d, thread of grid: %d\n",blockIdx.x, blockIdx.y, blockIdx.z, threadIdx.x, threadIdx.y, threadIdx.z, ind_bg, ind_tb, ind_tg);
}

/*
   Ким В.Е.
   Если компилятор видит, что память на девайсе не используется (т.е. или не выделяется, или выделяется и удаляется до вызовв кернела) то сам кернел не вызывается!!! В этом есть логика, т.к. зачем что-то исполнять если реузльтат не забирается....

*/
__host__ int main(int argc, char* argv[]){
        if(argc < 7){
                std::cout<<"You should run: "<<argv[0]<<" xg yg zg xb yb zb\n\txg yg zg - x y z of grid\n\txb yb zb - x y z of block\n\texample: "<<argv[0]<<" 16 1 1 1024 1 1\n";
                return -1;
        }

        //std::cout<<"Start"<<std::endl;
        dim3 BpG(std::stoi(argv[1]),std::stoi(argv[2]),std::stoi(argv[3]));
        dim3 TpB(std::stoi(argv[4]),std::stoi(argv[5]),std::stoi(argv[6]));

        int* d_A;
        cudaMalloc((void**)&d_A, sizeof(int));

        // Call kernel
        //std::cout<<"\ncomputation on GPU...\n";
        VecAdd<<<BpG, TpB>>>(d_A);

        cudaEvent_t syncEvent;
        cudaEventCreate(&syncEvent);
        cudaEventRecord(syncEvent);
        cudaEventSynchronize(syncEvent);

        //std::cout<<"\ndone\n";

        // Free cuda resources
        cudaEventDestroy(syncEvent);
        cudaFree(d_A);

        //std::cout<<"End"<<std::endl;
        return 0;
}