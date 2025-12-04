#include "sem.h"

struct sem {
    volatile unsigned int semaphore; //protects the data_example
    volatile char semaphore_lock; //protects the semaphore integer 
    pid_t wait_list[n_proc]; //keeps track of processes waiting 
    int sleep_list[n_proc]; //flag array (0 = awake, 1 = asleep)
};

void handler() {
    //does nothing, just prevents process from terminating
}

void sem_init(struct sem *s, int count) {
    s->semaphore = count;
    s->semaphore_lock = 0; //start off as unlocked obviously


    if(!(count <= my_procnum)) {
        fprintf(stdout, "count is larger than maximum resource pool");
        return -1;
    }

    for(int i = 0; i < n_proc; i++) {
        s->wait_list[i] = 0;
        s->sleep_list[i] = 0;
    }

    s->wait_list[my_procnum] = getpid();


    struct sigaction SIGUSR1_response;

    SIGUSR1_response.sa_flags = 0;
    SIGUSR1_response.sa_mask = 0;
    SIGUSR1_response.sa_sigaction = handler;

    if(sigaction(SIGUSR1, &SIGUSR1_response, NULL) == -1) {
        perror("sigaction for assigning new handler for SIGUSR1 failed");
        return;
    }

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

    while(s->semaphore == 0) {
        s->sleep_list[my_procnum] = 1; //add the current process to list of processes in wait list
        spin_unlock(&s->semaphore_lock);
        
        sigprocmask(SIG_SETMASK, &oldset, NULL); //reset signal mask to original
        sigemptyset(&newset);
        sigsuspend(&newset); //any signal works, goes to sleep (blocks)
        spin_lock(&s->semaphore_lock);
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

    for(int i = 0; i < n_proc; i++) {
        kill(s->wait_list[i], SIGUSR1); //wakes up every sleeping process
    } 

    for(int i = 0; i < n_proc; i++) {
        s->wait_list[i] = 0; //empties the waitlist
    }

    spin_unlock(&s->semaphore_lock);

    if(sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) { //reset original mask
        perror("signal masking failed");
    }



}
