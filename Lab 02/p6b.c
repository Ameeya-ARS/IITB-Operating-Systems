#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char* argv[]){
    int pid = fork();
    if(pid==0){
        printf("Child: My process ID is: %d\n",getpid());
        printf("Child: The parent process ID is: %d\n",getppid());
        sleep(5);
        printf("Child: Check child process state\n");
        int z;
        printf("Press enter to continue\n");
        scanf("%d",&z);
        printf("Child: Check child process state\n");
    }
    else{
        printf("Parent: My process ID is: %d\n",getpid());
        printf("Parent: The child process ID is: %d\n",pid);
        sleep(60);
    }

    return 0;
}