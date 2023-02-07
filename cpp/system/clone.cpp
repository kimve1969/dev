#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sched.h>
#include<sys/wait.h>

int fn(void* arg){
  int xcount{0};
  std::cout<<"child process\n";
  
  while(xcount < 100){
    sleep(2);
    std::cout<<xcount++<<std::endl;
  }
  
  std::cout<<"stop int fn()"<<std::endl;
  return xcount;
}

int  main(int argc, char *argv[])
{
  char stack[1024];
  pid_t pid = clone(fn, stack+1024, SIGCHLD, argv[1]);

  if(waitpid(pid, NULL, 0) == -1){
    std::cerr<<"error waitpid()\n";
    exit(EXIT_FAILURE);
  }
  
  exit(EXIT_SUCCESS);
}
