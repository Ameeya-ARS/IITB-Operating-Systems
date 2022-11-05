#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

// Global variable
long int account_balance = 0;

pthread_mutex_t lock;

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void *increment(void *args)
{
    int noMillion = *(int *)args;
    for (int i = 0; i < noMillion; i++)
    {
        for (int i = 0; i < 1000000; i++)
        {
            pthread_mutex_lock(&lock);
            account_balance++;
            pthread_mutex_unlock(&lock);
        }
    }
}

int main(int argc, char *argv[])
{
    struct timeval start;
    struct timeval end;
    float elapsed;

    gettimeofday(&start, 0);

    if (argc != 2)
    {
        printf("Usage: %s <number of threads>\n", argv[0]);
        exit(1);
    }

    int threadNum = atoi(argv[1]);
    pthread_t th[threadNum];

    int noMillion = 2048 / threadNum;

    // Initialize mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex init failed\n");
    }

    // Create threads
    for (int i = 0; i < threadNum; i++)
    {
        if (pthread_create(th + i, NULL, &increment, (void *)&noMillion) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }
        printf("Transaction %d has started\n", i);
    }

    // Join threads
    for (int i = 0; i < threadNum; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            return 2;
        }
        printf("Transaction %d has finished\n", i);
    }
    printf("Account Balance is : %ld\n", account_balance);

    // Destroy mutex
    if (pthread_mutex_destroy(&lock) != 0)
    {
        printf("Mutex destroy failed\n");
    }

    gettimeofday(&end, 0);
    // Time spent
    elapsed = timedifference_msec(start, end);
    printf("Time spent: %.0f ms\n", elapsed);

    return 0;
}
