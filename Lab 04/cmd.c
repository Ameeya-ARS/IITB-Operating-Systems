#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc,char *argv[]){
  char* arr[argc];
  for(int i=1;i<argc;i++){
    arr[i-1] = argv[i];
  }
  arr[argc-1]='\0';
  int pid = fork();
  if(pid==0){
    exec(arr[0],arr);
  }
  else if(pid>0){
    wait();
  }
  exit();
}
