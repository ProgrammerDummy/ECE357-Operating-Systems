#include "spinlock.h"

void spin_lock(volatile char *lock) {

    while(tas(lock) == 1) {
        if(sched_yield() == -1) { //yields while waiting for lock to be unlocked 
            perror("yielding failed");
            return;
        }
    }
}

void spin_unlock(volatile char *lock) {
    *lock = 0; 
}

//test
