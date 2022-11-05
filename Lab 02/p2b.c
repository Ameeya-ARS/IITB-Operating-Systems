#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    int n;
    scanf("%d",&n);
    int pid = fork();
    if(pid==0){
        for(int i=1;i<=n;i++){
            printf("C %d %d\n",getpid(),i);
        }
    }
    else{
        wait(NULL);
        for(int i=n+1;i<=2*n;i++){
            printf("P %d %d\n",getpid(),i);
        }
    }


    return 0;
}