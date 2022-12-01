#include "ulock.h"
#include "x86.h"
#include "defs.h"

void init_lock(struct lock_t *s) {
    s->locked = 0;
}

void 
acquire_lock(struct lock_t *s) 
{
    while(xchg(&(s->locked), 1) != 0);
    __sync_synchronize();
    return;
}

void 
release_lock(struct lock_t *s) 
{
     __sync_synchronize();
    asm volatile("movl $0, %0" : "+m" (s->locked) : );
    return;
}


void 
init_sem(struct sem_t *s, int initval)
{
    s->val = initval;
    init_lock(&(s->lk));
}

void
up_sem(struct sem_t *s) 
{
    acquire_lock(&(s->lk));
    s->val++;
    signal_sem(&(s->lk));
    release_lock(&(s->lk));
}

void 
down_sem(struct sem_t *s) 
{
    acquire_lock(&(s->lk));
    while(s->val == 0){
        block_sem(&(s->lk));
    }
    s->val--;
    release_lock(&(s->lk));
}
