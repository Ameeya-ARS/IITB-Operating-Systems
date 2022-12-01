#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAXDESCENDANTS 64

int main(){
    printf(1, "Parent process with pid %d started: Will have 4 descendants in total\n", getpid());

    // Fork a child process
    int pid1 = fork();
    if(pid1 == 0){
        sleep(200);
        printf(1, "Child process 1 with pid %d started: Will have 1 descendants in total\n", getpid());
        sleep(2000);
        exit();
    } else {
        // Fork a second child process
        int pid2 = fork();
        if(pid2 == 0){
        sleep(400);
        printf(1, "Child process 2 with pid %d started: Will have 1 descendants in total\n", getpid());
        sleep(2000);
        exit();
        } else {
            // Fork a third child
            int pid3 = fork();
            if(pid3 == 0){
                sleep(600);
                printf(1, "Child process 3 with pid %d started: Will have 1 descendants in total\n", getpid());
                sleep(2000);
                exit();
            } else{
                printf(1, "Parent process going to sleep for 1 second..\n");
                sleep(1000);

                // Count and List Descendants
                int descendants[MAXDESCENDANTS];
                int numDescendants = getDescendants(getpid(), MAXDESCENDANTS, descendants);

                if(numDescendants == 4){
                    printf(1, "getDescendants() for process with pid %d returned 4\n", getpid());
                } else{
                    printf(1, "getDescendants() for process with pid %d unexpectedly returned %d instead of 4\n", getpid(), numDescendants);
                }
        
                if(numDescendants > 0){
                    printf(1, "PIDs of the descendants of process with pid %d are: ", getpid());
                    for(int i = 0; i < numDescendants; i++){
                        printf(1, "%d ", descendants[i]);
                    }
                    printf(1, "\n");
                }

                // Wait for all children to exit
                wait();
                wait();
                wait();
            }
        }
    }
    exit();
} 