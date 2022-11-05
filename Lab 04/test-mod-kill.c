#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* argv[]){
    int id = fork();
    if(id==0){
        printf(1, "Child Pid - %d\n", getpid());
        sleep(1000);
    }else{
        sleep(10);
        printf(1, "Parent Pid - %d\n", getpid());
        kill(id);
    }
    exit();
}