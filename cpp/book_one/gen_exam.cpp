#include<iostream>
#include<string>

int main(int argc, char* argv[]){
  if(argc < 1){
    std::cout<<"Error: you should pass 1 arguments\n";
    return -1;
  }
  
  std::string sn = argv[1];
  
  std::cout<<"\n\n//THIS CODE AUTOGENETARE BEGIN\n\
  result = 0.0;\n\
  {\n\
    t = omp_get_wtime();\n\
    \n\
    ALIGN(64) double result_tmp["<<sn<<"]={";
    for(int i=0; i<(std::stoi(sn)-1); ++i){ std::cout<<"0.0,"; }
    std::cout<<"0.0};\n\n\
    for(int i=0; i<N/"<<std::stoi(sn)<<"; ++i ){\n";
    for(int i=0; i<std::stoi(sn); ++i){ std::cout<<"\t\tresult_tmp["<<i<<"] += a;\n"; }
    std::cout<<"\t}\n\n\
    for(int i=0; i<N%"<<std::stoi(sn)<<"; ++i) result += a;\n\n\
    result +=";
    for(int i=0; i<(std::stoi(sn)-1); ++i){ std::cout<<"result_tmp["<<i<<"] + "; }
    std::cout<<"result_tmp["<<std::stoi(sn)-1<<"];\n\n\
    t = omp_get_wtime()-t;\n\
    std::cout<<\""<<sn<<". result: \"<<std::setprecision(13)<<result<<std::setprecision(3)<<\", time: \"<<t<<\" s, GFLOPS: \"<<N/(t*1E9)<<\"\\n\";\n\
  }\n//END\n\n";
  
  return 0;
}
