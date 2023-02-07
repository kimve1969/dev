#include<iostream>
#include<cstdlib>
#include<array>
#include<algorithm>

int main(int argc, char* argv[]){
  std::cout<<"Start\n";

  std::srand(1);

  int arr[100];

  for(auto &e: arr) e = std::rand()%999;
  
  std::sort(std::begin(arr), std::end(arr));
 
  for(auto &e: arr) std::cout<<e<<" ";

  std::cout<<"\n";

  
  std::cout<<"End\n";
  return 0;
}
