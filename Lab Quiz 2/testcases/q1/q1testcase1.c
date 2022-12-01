#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(){
    printf(1, "Parent process started\n");

    // Fork a child process
    int pid = fork();
    if(pid == 0){
        printf(1, "Child process started\n");
        printf(1, "Child process going to sleep for 3 seconds..\n");
        sleep(3000);
        printf(1, "Child process woke up. Exiting..\n");
        exit();
    }
    else{
        // Blocking call to reap the child process
        int status = waitpid(pid, 1);
        if(status == pid){
            printf(1, "Blocking Call 1: Valid Child process reaped by parent\n");
        } else{
            printf(1, "Blocking Call 1: Valid Child process not reaped properly by parent.\n");
        }

        // Blocking call for a non-existent child
        status = waitpid(pid, 1);
        if(status == -1){
            printf(1, "Blocking Call 2: Returned -1 as expected\n");
        } else{
            printf(1, "Blocking Call 2: Unexpectedly returned %d\n", &status);
        }
    }
    exit();
}