#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
head(int count,int fd)
{
  int n;
  while((n=read(fd,buf,sizeof(buf)))>0 && count){
    if(n<0){
      exit();
    }
    int i = 0;
    while(count && i<n){
      if(buf[i]=='\n'){
        count--;
      }
      printf(1,"%c",buf[i]);
      i++;
    }
  }
 }

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    exit();
  }
  int count = atoi(argv[1]);
  for(i = 2; i < argc; i++){
    printf(1,"--------%s--------\n",argv[i]);
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "head: cannot open %s\n", argv[i]);
      exit();
    }
    head(count,fd);
    close(fd);
  }
  exit();
}
