#ifndef SEM_H

#include "spinlock.h"
#define N_PROC 64

struct sem {
    volatile unsigned int semaphore; //protects the data_example
    volatile char semaphore_lock; //protects the semaphore integer 
    pid_t wait_list[N_PROC]; //keeps track of processes waiting 
    int sleep_list[N_PROC]; //flag array (0 = awake, 1 = asleep)
};

extern int my_procnum;

void sem_init(struct sem *s, int count);
int sem_try(struct sem *s);
void sem_wait(struct sem *s);
void sem_inc(struct sem *s);

#define SEM_H
#endif
