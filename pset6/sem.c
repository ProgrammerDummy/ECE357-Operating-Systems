#include "sem.h"

struct sem {
    volatile unsigned int semaphore; //protects the data_example
    volatile char semaphore_lock; //protects the semaphore integer 
    pid_t wait_list[my_procnum]; //keeps track of processes waiting 
    int wait_list_index;
};


void sem_init(struct sem *s, int count) {
    s->semaphore = count;
    s->semaphore_lock = 0; //start off as unlocked obviously


    if(!(count <= my_procnum)) {
        fprintf(stdout, "count is larger than maximum resource pool");
        return -1;
    }

    for(int i = 0; i < my_procnum; i++) {
        s->wait_list[i] = 0;
    }

    s->wait_list_index = 0; 
    //index is the next element (process) TO BE added, meaning currently no waiting process id is stored in wait_list[wait_list_index]
}

int sem_try(struct sem *s) {  //attempt P operation

    sigset_t oldset; //will contain previous signal mask
    sigemptyset(&oldset); 

    sigset_t newset;
    sigemptyset(&newset);

    sigfillset(&newset); //blocks all signals

    if(sigprocmask(SIG_SETMASK, &newset, &oldset) == -1) {
        perror("signal masking failed");
    }

    spin_lock(&s->semaphore_lock);

    if(s->semaphore == 0) {
        spin_unlock(&s->semaphore_lock);
        sigprocmask(SIG_SETMASK, &oldset, NULL); //reset signal mask to original
        return 0;
    }
    
    s->semaphore--;

    spin_unlock(&s->semaphore_lock);

    return 1;
    
    if(sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) { //reset original mask
        perror("signal masking failed");
        return -1;
    }
    
}

void sem_wait(struct sem *s) { // P operation

    sigset_t oldset; //will contain previous signal mask
    sigemptyset(&oldset); 

    sigset_t newset;
    sigemptyset(&newset);

    sigfillset(&newset); //blocks all signals

    if(sigprocmask(SIG_SETMASK, &newset, &oldset) == -1) {
        perror("signal masking failed");
    }

    spin_lock(&s->semaphore_lock);

    if(s->semaphore == 0) {
        s->wait_list[s->wait_list_index] = getpid(); //add the current process to list of processes in wait list
        s->wait_list_index++; //increment the wait list index
        spin_unlock(&s->semaphore_lock);
        
        sigprocmask(SIG_SETMASK, &oldset, NULL); //reset signal mask to original
        sigsuspend(&newset); //any signal works, goes to sleep (blocks)
    }
    
    s->semaphore--;

    spin_unlock(&s->semaphore_lock);

    
    if(sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) { //reset original mask
        perror("signal masking failed");
    }

}

void sem_inc(struct sem *s) { // V operation

    sigset_t oldset; //will contain previous signal mask
    sigemptyset(&oldset);

    sigset_t newset; 
    sigemptyset(&newset);

    sigfillset(&newset); //blocks all signals

    if(sigprocmask(SIG_SETMASK, &newset, &oldset) == -1) { //mask all signals
        perror("signal masking failed");
    }
    
    spin_lock(&s->semaphore_lock);

    s->semaphore++;

    for(int i = 0; i < s->wait_list_index; i++) {
        kill(s->wait_list[i], SIGUSR1); //wakes up every sleeping process
    } 

    for(int i = 0; i < s->wait_list_index; i++) {
        s->wait_list[i] = 0; //empties the waitlist
    }

    s->wait_list_index = 0;

    spin_unlock(&s->semaphore_lock);

    if(sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) { //reset original mask
        perror("signal masking failed");
    }



}
