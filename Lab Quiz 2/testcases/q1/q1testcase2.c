#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(){
    printf(1, "Parent process started\n");

        // Fork a child process
        int pid = fork();
        if(pid == 0){
            sleep(100);
            printf(1, "Child process started\n");
            printf(1, "Child process going to sleep for 3 seconds..\n");
            sleep(3000);
            printf(1, "Child process woke up. Exiting..\n");
            exit();
        }
        else{
            // Non-blocking call, expected to return immediately
            int status = waitpid(pid, 0);
            if(status == 0){
                printf(1, "Non-Blocking Call 1: Returned 0 as expected\n");
            } else{
                printf(1, "Non-Blocking Call 1: Unexpectedly returned %d.\n", &status);
            }

            // Sleep to wait for child process 2 to exit
            printf(1, "Parent process going to sleep for 4 seconds..\n");
            sleep(4000);

            // Non-blocking call to reap the second child
            status = waitpid(pid, 0);
            if(status == pid){
                printf(1, "Non-Blocking Call 2: Valid Child process reaped by parent\n");
            } else{
                printf(1, "Non-Blocking Call 2: Valid Child process not reaped properly by parent.\n");
            }

            // Non-blocking call for a non-existent child
            status = waitpid(pid, 0);
            if(status == -1){
                printf(1, "Non-Blocking Call 3: Returned -1 as expected\n");
            } else{
                printf(1, "Non-Blocking Call 3: Unexpectedly returned %d\n", &status);
            }
        }
        exit();
}