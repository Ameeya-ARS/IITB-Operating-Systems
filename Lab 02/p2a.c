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
    int i;
    if(pid==0) i=1;
    else i=n+1;
    for(int j=i;j<i+n;j++){
        if(j<=n) printf("C %d %d\n",getpid(),j);
        else printf("P %d %d\n",getpid(),j);
    }


    return 0;
}