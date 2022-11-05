#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    int id1 = fork();
    int id2=-2;
    if(id1==0){
        id2 = fork();
    }
    if(id1==0 && id2==0){
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        int x;
        printf("Enter 1st number: ");
        scanf("%d",&x);
        write(fd1[1],&x,sizeof(int));
        close(fd1[1]);
    }
    else if(id1==0 && id2>0){
        wait(NULL);
        close(fd1[1]);
        close(fd2[0]);
        int x,y;
        printf("Enter 2nd number: ");
        scanf("%d",&y);
        read(fd1[0],&x,sizeof(int));
        close(fd1[0]);
        int sum = x+y;
        write(fd2[1],&sum,sizeof(int));
        close(fd2[1]);
    }
    else if(id1!=0){
        wait(NULL);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        int result;
        read(fd2[0],&result,sizeof(int));
        close(fd2[0]);
        printf("Result: %d\n",result);
    }

    return 0;
}