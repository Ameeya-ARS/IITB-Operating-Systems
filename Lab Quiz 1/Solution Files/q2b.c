#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<signal.h>
int pid;
void sigint_handler(int sig){
	printf("\n");
	kill(pid,SIGINT);
}

void sigquit_handler(int sig){
	printf("\nshell is exiting\n");
	exit(0);
}

int main(){
    signal(SIGINT,&sigint_handler);
    signal(SIGQUIT,&sigquit_handler);

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
        }
        else{
            execl(command,newargs,(char*)NULL);
            return 1;
        }
    }
    
    return 0;
}
