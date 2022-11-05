#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    int n;
    scanf("%d",&n);
    printf("Parent is: %d\n",getpid());
    printf("Number of children: %d\n",n);
    for(int i=0;i<n;i++){
        int pid = fork();
        if(pid==0){
            printf("Child %d is created\n",getpid());
            sleep(1);
            exit(0);
        }
    }
    for(int i=0;i<n;i++){
        int temp = wait(NULL);
        printf("Child %d of parent %d exited\n",temp,getppid());
    }
    printf("Parent exited\n");
}