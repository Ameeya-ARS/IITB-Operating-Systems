#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* argv[]){
    
    
    printf(1, "Total Number of Processes: %d\n", getNumProc());
    printf(1, "Maximum PID: %d\n", getMaxPid());
    
    int id = fork();
    if(id==0){
        printf(1, "---After single fork---\n");             
        printf(1, "Total Number of Processes: %d\n", getNumProc());
        int id = fork();
        if(id==0){       
            printf(1, "Maximum PID: %d\n", getMaxPid());
            int id = fork();
            if(id==0){
                int id = fork();
                if(id==0){
                    printf(1, "---After multiple fork---\n");
                    printf(1, "Total Number of Processes: %d\n", getNumProc());
                    printf(1, "Maximum PID: %d\n", getMaxPid());
                }else{
                    wait();
                }
            }else{
                wait();
            }
        }else{
            wait();
        }
    }else{
    	wait();
    }

    exit();
}