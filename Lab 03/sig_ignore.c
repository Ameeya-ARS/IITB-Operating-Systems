#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signal_handle(int sig){

}

int main(){
	signal(SIGINT,&signal_handle);
	signal(SIGTERM,&signal_handle);	
	
	printf("Process Id is: %d\n",getpid());
	while(1){
		printf("Waiting...\n");
		sleep(3);
	}
	
	return 0;
}
