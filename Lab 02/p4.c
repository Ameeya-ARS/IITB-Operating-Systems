#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 

int main(){
    char ch1,ch2;
    char file1[50], file2[50];
    int i=0;
    while(ch1!='\n'){
        ch1 = getchar();
        file1[i++] = ch1;
    }
    file1[--i] = '\0';
    i=0;
    while(ch2!='\n'){
        ch2 = getchar();
        file2[i++] = ch2;
    }
    file2[--i]='\0';
    char buf;
    int filed1 = open(file1, O_RDONLY);
    int filed2 = open(file2, O_WRONLY|O_CREAT);
    while(read(filed1,&buf,1)){
        write(filed2,&buf,1);
    }
    close(filed1);
    close(filed2);

    return 0;
}