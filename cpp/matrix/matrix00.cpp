#include<iostream>
#include<chrono>

auto t_now(){ return std::chrono::high_resolution_clock::now(); }
auto t_diff(const std::chrono::time_point<std::chrono::high_resolution_clock> &start,
	    const std::chrono::time_point<std::chrono::high_resolution_clock> &end){
  std::chrono::duration<double> diff = end - start;
  return diff.count();
}

int main(int argc, char** argv){
  std::cout<<"Start.\n";
  
  const int CNST_SIZE=10'000;
  auto A = new int[CNST_SIZE][CNST_SIZE];
  std::cout<<"A["<<CNST_SIZE<<"]["<<CNST_SIZE<<"]"<<std::endl;
  //--------------------------------------------------
  auto t = t_now();
  for(int i=0; i<CNST_SIZE; i++){
    for(int j=0; j<CNST_SIZE; j++){
      A[i][j]=i*CNST_SIZE+j;
    }
  }
  auto d1 = t_diff(t, t_now());
  std::cout<<"access by row A[i][0:N-1] t1 = "<<d1<<" sec."<<std::endl;
  //--------------------------------------------------
  t = t_now();
  for(int j=0; j<CNST_SIZE; j++){
    for(int i=0; i<CNST_SIZE; i++){
      A[i][j]=i*CNST_SIZE+j;
    }
  }
  auto d2 = t_diff(t, t_now());
  std::cout<<"access by col A[0:N-1][j] t2 = "<<d2<<" sec."<<std::endl;
  std::cout<<"length of cash line is t2/t1 = "<<d2/d1<<std::endl;

  std::cout<<"End prorgam.\n";
  return 0;

}
