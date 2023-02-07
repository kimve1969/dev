#include<iostream>
#include<sys/types.h>
#include<unistd.h>


int  main(int argc, char *argv[])
{
  pid_t pid = fork();
  int xcount = 0;
  if(pid){
    // TODO for child process
    std::cout<<"child pid = "<<pid<<std::endl;
    while(true){
      sleep(2);
      std::cout<<xcount++<<std::endl;
    }
  }
  else{
    // TODO for current process
    std::cout<<"parent pid = "<<pid<<std::endl;
    while(true){
      sleep(2);
      std::cout<<xcount++<<std::endl;
    }
  }
  
  return 0;
}
