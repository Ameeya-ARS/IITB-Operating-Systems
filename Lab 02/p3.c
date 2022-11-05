#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    while(1){
        char val[50];
        printf(">>> ");
        scanf("%s",val);
        int id = fork();
        if(id==0){
            execl(val,val,NULL);
        }
        else{
            wait(NULL);
        }
    }

    return 0;
}