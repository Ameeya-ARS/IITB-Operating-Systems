#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(int argc, char *argv[])
{
  if(fork() == 0){
    printf(1,"Heap Size till now: %d bytes\n", heapSize());
    sleep(10);

    char* a = sbrk(1024);
    printf(1,"sbrk return address: 0x%x\nHeap Size till now: %d bytes\n", a, heapSize());
    
    exit();
  }
  else{
    wait();
  }
  exit();
}
