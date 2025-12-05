#include "sem.h"

int my_procnum = 0;

void blank_handler(int sn) {
    //does nothing, just prevents process from terminating
}

void sem_init(struct sem *s, int count) {
    s->semaphore = count;
    s->semaphore_lock = 0; //start off as unlocked obviously

    // // early return if count is larger than our max process number
    // if(!(count <= N_PROC)) {
    //     fprintf(stdout, "count is larger than maximum resource pool");
    //     return;
    // }

    for(int i = 0; i < N_PROC; i++) {
        s->wait_list[i] = 0;
        s->sleep_list[i] = 0;
    }

    // s->wait_list[my_procnum] = getpid();


    struct sigaction SIGUSR1_response;
    SIGUSR1_response.sa_flags = 0;
    sigemptyset(&SIGUSR1_response.sa_mask);
    SIGUSR1_response.sa_handler = blank_handler;

    if(sigaction(SIGUSR1, &SIGUSR1_response, NULL) == -1) {
        fprintf(stderr, "Sigaction for assigning new handler for SIGUSR1 failed: %s", strerror(errno));
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
        fprintf(stderr, "Initial signal masking in sem_try failed: %s", strerror(errno));
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

    if(sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) { //reset original mask
        fprintf(stderr, "Signal mask restoration failed in sem_try after semaphore was decremented: %s", strerror(errno));
        // no return here because the sem_try still worked
    }

    return 1;
}

void sem_wait(struct sem *s) { // P operation

    sigset_t oldset; //will contain previous signal mask
    sigemptyset(&oldset); 

    sigset_t newset;
    sigemptyset(&newset);

    sigfillset(&newset); //blocks all signals

    if(sigprocmask(SIG_SETMASK, &newset, &oldset) == -1) {
        fprintf(stderr, "Initial signal masking in sem_wait failed: %s", strerror(errno));
    }

    spin_lock(&s->semaphore_lock);

    while(s->semaphore == 0) {
        s->wait_list[my_procnum] = getpid();
        s->sleep_list[my_procnum] = 1; //add the current process to list of processes in wait list
        spin_unlock(&s->semaphore_lock);
        // sigprocmask(SIG_SETMASK, &oldset, NULL); //reset signal mask to original
        sigemptyset(&newset);
        sigsuspend(&newset); //any signal works, goes to sleep (blocks)
        spin_lock(&s->semaphore_lock);
    }
    
    s->semaphore--;
    s->sleep_list[my_procnum] = 0; // clear sleep flag now

    spin_unlock(&s->semaphore_lock);

    
    if(sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) { //reset original mask
        fprintf(stderr, "Signal mask restoration failed in sem_wait after semaphore was decremented: %s", strerror(errno));
    }

}

void sem_inc(struct sem *s) { // V operation

    sigset_t oldset; //will contain previous signal mask
    sigemptyset(&oldset);

    sigset_t newset; 
    sigemptyset(&newset);

    sigfillset(&newset); //blocks all signals

    if(sigprocmask(SIG_SETMASK, &newset, &oldset) == -1) { //mask all signals
        fprintf(stderr, "Initial signal masking in sem_inc failed: %s", strerror(errno));
    }
    
    spin_lock(&s->semaphore_lock);

    s->semaphore++;

    for(int i = 0; i < N_PROC; i++) {
        // only wake sleeping processes and handle edge case incase parent pid does something weird
        if ((s->sleep_list[i] == 1) && (s->wait_list[i] != 0)) {
            kill(s->wait_list[i], SIGUSR1); // wakes up every sleeping process
            s->sleep_list[i] = 0; // empties sleep list
            s->wait_list[i] = 0; // empties wait list
        }
    }

    spin_unlock(&s->semaphore_lock);

    if(sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) { //reset original mask
        fprintf(stderr, "Signal mask restoration failed in sem_inc after semaphore was incremented: %s", strerror(errno));
    }
}


