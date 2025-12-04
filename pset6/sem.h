#include "spinlock.h"
#define n_proc 64

extern int my_procnum;

void sem_init(struct sem *s, int count);
int sem_try(struct sem *s);
void sem_wait(struct sem *s);
void sem_inc(struct sem *s);
