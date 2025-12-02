#include "spinlock.h"

int main() {

    //use mmap here to make shared memory region
    //use MAP_SHARED
    //use 

    long long int *counter = mmap(NULL, 4096, PROT_READ| PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    volatile char *lock = mmap(NULL, 4096, PROT_READ| PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

    *counter = 0;

    int child_pid = 1;

    for(int i = 0; i < 9; i++) { //my macOS has 10 cores total
        if(child_pid == 0) {
            continue;
        }
        child_pid = fork();
    } 


    spin_lock(lock);
    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j < 1000; j++) {
            *counter += 1;
        } 
    }
    spin_unlock(lock);

    if(child_pid == 0) {
        exit(EXIT_SUCCESS);
    }

    while(wait(NULL) > 0); //wait for all child processes to die

    if(child_pid != 0) {
        fprintf(stdout, "%lld\n", *counter); //print out result
    }

}