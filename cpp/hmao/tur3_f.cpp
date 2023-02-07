#include<iostream>
#include<stack>

int main(int argc, char* argv[]){
  int N=0;
  int a[50'000], b[50'000], c[2]={0,0};
  int suma{0}, sumb{0};
  bool a_complexity_b{true};

  
  std::cin>>N;
  for(int i=0; i<N; ++i){
    std::cin>>a[i]>>b[i];
    suma+=a[i];
    sumb+=b[i];
  }

  a_complexity_b = (suma < sumb) ? true : false;
  a_complexity_b ? std::cout<<"a complexity then b"<<std::endl : std::cout<<"b complexity then a"<<std::endl;
    
  auto set_new_max = [&c](int i){
    c[1] = c[0];
    c[0] = i;
  };

  auto set_equal = [&c](int i){
    c[1]=i;
  };
  
  for(int i=0; i<N; ++i){
    if( (a[i]+b[i]) > (a[c[0]] + b[c[0]]) ){
      set_new_max(i);
    }
    else if( (a[i]+b[i]) == (a[c[0]] + b[c[0]]) ){
      set_equal(i);
      if( a_complexity_b ){
	if( (a[i] > a[c[0]]) ||
	    ((a[i] == a[c[0]]) && (i > c[0])) ){
	  set_new_max(i);
	}
      }
      else{
	if ( (b[i] > b[c[0]]) ||
	     ((b[i] == b[c[0]]) && (i > c[0])) ){
	  set_new_max(i);
	}
      }
    }
  }

  auto prn = [&N, &a, &b](){
    for(int i=0; i<N; ++i){
      std::cout<<(i+1)<<", "<<a[i]<<", "<<b[i]<<std::endl;
    }
    std::cout<<std::endl;
  };
 
  prn();

  std::cout<<(c[0]+1)<<" "<<(c[1]+1)<<std::endl;
    
  return 0;
}
