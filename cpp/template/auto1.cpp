#include<iostream>

/********************************
template<typename T>
void function(ParamType param)

f(expr);

const auto &rx = expr;
T - auto
ParamType - const auto & 
 *******************************/

int main(){
  
  int x = 0;
  const auto cx = x;
  const auto &rx = x; 
  
  return 0;
}
