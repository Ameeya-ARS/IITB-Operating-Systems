#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char* argv[]){
	int pid = atoi(argv[1]);
	kill(pid,SIGINT);
	printf("SIGINT signal sent to PID: %d\n",pid);
	kill(pid,SIGTERM);
	printf("SIGTERM signal sent to PID: %d\n",pid);
	kill(pid,SIGKILL);
	printf("SIGKILL signal sent to PID: %d\n",pid);
	return 0;
}
