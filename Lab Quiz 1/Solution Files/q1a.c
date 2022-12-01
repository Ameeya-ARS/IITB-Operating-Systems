#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int pid1 = fork();
	int pid2 = -2;
	if(pid1>0){
		pid2 = fork();
	}
	if(pid1==0){
		char file1[50] = "secret1.txt";
		int fd1 = open(file1,O_RDONLY);
		char code1[6];
		read(fd1,code1,5);
		code1[6]='\0';
		printf("%d: Secret code for Alice is %s\n",getpid(),code1);
	}
	else if(pid2==0){
		char file2[50] = "secret2.txt";
		int fd2 = open(file2,O_RDONLY);
		char code2[6];
		read(fd2,code2,5);
		code2[6]='\0';
		printf("%d: Secret code for Bob is %s\n",getpid(),code2);
	}
	else{
		wait(NULL);
		wait(NULL);
	}


	return 0;
}
