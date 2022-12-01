#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<signal.h>
int pid;
int main(){
    char command[256];
    char newargs[1] = "";
    while (1)
    {
        char prompt[] = ">>> ";
        printf("%s",prompt);
        scanf("%s",command);
        pid = fork();
        if(pid){
            wait(NULL);
        }else{
            execl(command,newargs,(char*)NULL);
            return 1;
        }
    }
    
    return 0;
}
