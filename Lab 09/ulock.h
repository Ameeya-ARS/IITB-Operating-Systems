#include "types.h"

struct lock_t {
    uint locked; 
};

// semaphore implementation for xv6
struct sem_t {
    int val;
    struct lock_t lk;
};

