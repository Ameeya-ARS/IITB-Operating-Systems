#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ulock.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_clone(void)
{
  void (*fn)(int *);
  int arg;
  void *stack;

  if(argptr(0, (char**)&fn, 0) < 0)
    return -1;
  if(argint(1, &arg) < 0)
    return -1;
  if(argint(2, (int*)&stack) < 0)
    return -1;

  return clone(fn, &arg, stack);
}

int sys_join(void) 
{
  return join();
}

int sys_init_lock(void)
{
  struct lock_t *lk;
  if(argptr(0,(char **)&lk, sizeof(struct lock_t *))==-1)
    return -1;
  init_lock(lk);
  return 0;
}

int sys_acquire_lock(void)
{
  struct lock_t *lk;
  if(argptr(0,(char **)&lk, sizeof(struct lock_t *))==-1)
    return -1;
  acquire_lock(lk);
  return 0;
}

int sys_release_lock(void)
{
  struct lock_t *lk;
  if(argptr(0,(char **)&lk, sizeof(struct lock_t *))==-1)
    return -1;
  release_lock(lk);
  return 0;
}

int sys_init_sem(void)
{
  struct sem_t *sm;
  int val;
  if(argptr(0,(char **)&sm, sizeof(struct sem_t *))==-1)
    return -1;
  if(argint(1,&val)==-1)
    return -1;
  init_sem(sm,val);
  return 0;
}

int sys_up_sem(void)
{
  struct sem_t *sm;
  if(argptr(0,(char **)&sm, sizeof(struct sem_t *))==-1)
    return -1;
  up_sem(sm);
  return 0;
}

int sys_down_sem(void)
{
  struct sem_t *sm;
  if(argptr(0,(char **)&sm, sizeof(struct sem_t *))==-1)
    return -1;
  down_sem(sm);
  return 0;
}