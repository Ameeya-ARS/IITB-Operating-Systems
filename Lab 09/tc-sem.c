#include "types.h"
#include "stat.h"
#include "ulock.h"
#include "user.h"
#include "fcntl.h"

int VAR = 0;
struct sem_t sem;

void increment(int *thread_rank) {
    for(int i=0; i<1000; i++){
    down_sem(&sem);
    VAR++;
    up_sem(&sem);
    }
    printf(1, "Thread Rank: %d, VAR: %d\n", *thread_rank, VAR);
    exit();
}

int main(int argc, char *argv[]) {

    printf(1, "Calling Process Print VAR value: %d\n", VAR);
    int N = 5;
    int thread_num[N];
    init_sem(&sem, 1);

    for(int i=0;i<N;i++) {
        thread_num[i] = i;
        create_thread(increment, &thread_num[i]);
        //sleep(100);
    }

    for(int i=0;i<N;i++) {
        join();
    }

    printf(1, "All threads joined, VAR value: %d\n", VAR);
    exit();
}
