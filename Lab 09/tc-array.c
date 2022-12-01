#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "ulock.h"

int arr1[10] = {7,14,2,3,5,6,9,10,12,1};
int arr2[10] = {1,2,3,4,5,6,7,8,9,10};
int arr_size = 10;
int global_sum = 124;
int thread_sums[2] = {0, 0};

void array_sum(int *thread_rank) {
    int sum = 0;
    if(*thread_rank == 0) {
        for(int i=0;i<arr_size/2;i++) {
            sum += arr1[i];
        }
        for (int i=arr_size/2;i<arr_size;i++) {
            sum += arr2[i];
        }
    }
    else if(*thread_rank == 1) {
        for(int i=0;i<arr_size/2;i++) {
            sum += arr2[i];
        }
        for (int i=arr_size/2;i<arr_size;i++) {
            sum += arr1[i];
        }
    }
    
    thread_sums[*thread_rank] = sum;
    printf(1, "Thread Rank: %d, Sum Value: %d\n", *thread_rank, sum);
    exit();
}

int main(int argc, char *argv[]) {

    printf(1, "Calling Process Print Check\n");

    for(int i=0;i<2;i++) {
        create_thread(array_sum, &i);
        sleep(100);
    }

    for(int i=0;i<2;i++) {
        join();
    }

    printf(1, "All threads joined\n");
    if (thread_sums[0] + thread_sums[1] == global_sum) {
    	printf(1, "Sum of thread calls is equal to that of both array sums, value: %d\n", global_sum);
    }
    else {
    	printf(1, "Sum of thread calls is not equal to that of both array sums, thread sums: %d and array sum: %d\n", thread_sums[0] + thread_sums[1], global_sum);
    }
    
    
    exit();
}
