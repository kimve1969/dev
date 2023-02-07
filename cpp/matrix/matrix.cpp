#include<iostream>
#include<chrono>


int main(int argc, char** argv){
  std::cout<<"Start.\n";
  
  const int CNST_SIZE=10000;
  auto A = new long[CNST_SIZE][CNST_SIZE];
  //--------------------------------------------------
  auto t1 = std::chrono::high_resolution_clock::now();
  for(int i=0; i<CNST_SIZE; i++){
    for(int j=0; j<CNST_SIZE; j++){
      A[i][j]=i*CNST_SIZE+j;
    }
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> t_heap_by_string = t2-t1;
  std::cout<<"t_heap_by_string = "<<t_heap_by_string.count()<<std::endl;
  //--------------------------------------------------
  delete[] A;
  A = new long[CNST_SIZE][CNST_SIZE];
  auto t5 = std::chrono::high_resolution_clock::now();
  for(int j=0; j<CNST_SIZE; j++){
    for(int i=0; i<CNST_SIZE; i++){
      A[i][j]=i*CNST_SIZE+j;
    }
  }
  auto t6 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> t_heap_by_column = t6-t5;
  std::cout<<"t_heap_by_column = "<<t_heap_by_column.count()<<std::endl;
  //--------------------------------------------------
  auto print = [&A](){
    for(int i=0; i<10; i++){
      for(int j=0; j<10; j++){
	std::cout<<A[i][j]<<", ";
      }
      std::cout<<std::endl;
    }
  };
  //print();
  //--------------------------------------------------
  /*auto t3 = std::chrono::high_resolution_clock::now();
  long B[CNST_SIZE][CNST_SIZE];
  for(int i=0; i<CNST_SIZE; i++){
    for(int j=0; j<CNST_SIZE; j++){
      B[i][j]=i*CNST_SIZE+j;
    }
  }
  auto t4 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> t_stack = t4-t3;
  std::cout<<"t_stack = "<<t_stack.count()<<std::endl;*/
  //--------------------------------------------------
  //std::cout<<"RESULT:\nt_heap_by_column/t_heap_by_string = "<<(t_heap_by_column/t_heap_by_string)<<"\nt_heap_by_string/t_stack = "<<t_heap_by_string/t_stack<<std::endl;

  std::cout<<"End prorgam.\n";
  return 0;

}
