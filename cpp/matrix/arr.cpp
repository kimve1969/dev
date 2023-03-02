#include<iostream>


int main(){
  int arr27[27]={0};
  int arr3x3x3[3][3][3]={0};

  std::cout<<"sizeof(arr27)=="<<sizeof(arr27)<<"\n";
  std::cout<<"sizeof(arr3x3x3)=="<<sizeof(arr3x3x3)<<"\n\n";
  
  const int N=3;
  auto a=arr3x3x3;

  std::cout<<"a[3][3][3]\n\na[0-2]:\n";
  for(int i=0; i<N; ++i){
    std::cout<<a[i]<<"\t";
  }
  std::cout<<"\n\n";

  std::cout<<"a[0-2][-0-2]:\n";
  for(int i=0; i<N; ++i){
    std::cout<<"\t";
    for(int j=0; j<N; ++j){
      std::cout<<a[i][j]<<"\t";
    }
    std::cout<<"\n";
  }
  std::cout<<"\n\n";

  std::cout<<"&a[0-2][0-2][0-2]:\n";
  for(int i=0; i<N; ++i){
    std::cout<<"\t";
    for(int j=0; j<N; ++j){
      std::cout<<"\t";
      for(int k=0; k<N; ++k){
	std::cout<<&a[i][j][k]<<"\t";
      }
      std::cout<<"\n\t";
    }
    std::cout<<"\n";
  }
  std::cout<<"\n";

  struct s_arr27_type{
    int a[27]{0};
  } s_arr27;

  struct s_arr3x3x3_type{
    int a[3][3][3]{0};
  } s_arr3x3x3;

  std::cout<<"sizeof(s_arr27)=="<<sizeof(s_arr27)<<"\n";
  std::cout<<"sizeof(s_arr3x3x3)=="<<sizeof(s_arr3x3x3)<<"\n\n";

  int  *p = new int[10][10];
  
  return 0;
}
