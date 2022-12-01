#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char* argv[]){
	int pid = fork();
	if(pid==0){
		printf("pid %d: I am child\n",getpid());
		execl(argv[1],argv[1],NULL);
	}
	else{
		sleep(5);
		int wstatus;
		waitpid(pid,&wstatus,WNOHANG);
		if(wstatus!=0){
			printf("Terminating process %d\n",pid);
			kill(pid,SIGINT);
			wait(NULL);
		}
	}
}
