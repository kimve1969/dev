#include<iostream>
#include<mpi.h>

int main(int argc, char* argv[]){
  std::cout<<"Start\n";

  MPI_Status status;
  
  int mpi_res = MPI_Init(&argc, &argv);
  
  if(mpi_res != MPI_SUCCESS){
    std::cout<<"MPI init failed (code "<<mpi_res<<")\n";
    return -1;
  }

  int size,rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::cout<<"MPI rank = "<<rank<<", size = "<<size<<"\n";
  
  MPI_Finalize();
  
  return 0;
}
