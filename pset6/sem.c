#include "sem.h"

struct sem {
    volatile char spinlock;
    
};

void sem_init(struct sem *s, int count) {

}

void sem_try(struct sem *s) {

}

void sem_wait(struct sem *s) {

}

void sem_inc(struct sem *s) {

}