#include "fifo.h"

int main(int argc, char* argv[]) {
    struct myfifo *fifo = mmap(NULL, sizeof(struct myfifo) + sizeof(struct sem) * 3, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);    
    fifo_init(fifo);
    int num_writers = 0; // number of writers
    int num_items_each = 0; // number of items each writer wants to write

    int opt;
    while ((opt = getopt(argc, argv, "w:n:")) != -1) {
        switch (opt) {
            case 'w':
                num_writers = atoi(optarg);
                break;
            case 'n':
                num_items_each = atoi(optarg); 
                break;
            case '?':
                return -1;
        }
    }

    int pid = 1; // make sure parent doesnt get mistaken as a child by our weird test code
    my_procnum = 0;
    // fork a num_writers processes
    for(int i = 0; i < num_writers; i++) {
        if(pid == 0) {
            continue;
        }
        pid = fork();
        if(pid == 0) {
            my_procnum = i + 1; 
        }
    }
    // at this point, parent has my_procnum 0, and we have num_writers children
    // (i will treat the parent as the one reader)

    // if the process is a child and a writer

    fprintf(stdout, "Beginning test with %d writers, %d items each\n", num_writers, num_items_each);
    if ((pid == 0) && (my_procnum > 0)) {
        for (int i = 0; i < num_items_each; i++) {
            fifo_wr(fifo, i);
            fprintf(stdout, "Writer %d completed\n", my_procnum);
        }
    }

    // children exit here
    if(pid == 0) {
        exit(EXIT_SUCCESS);
    }
    
    // parent waits for all children
    while(wait(NULL) > 0);

    if (my_procnum == 0) {
        while (fifo->item_count > 0) {
            fprintf(stdout, "Reader stream %d completed\n", fifo_rd(fifo));
        }
    }

    // // while (f->item_count > 0) {
    // //     printf("Found %d in fifo\n", fifo_rd(f));
    // // }
    return 0;
}



      
    // int child_pid = 1;
    // fifo_init(f);

    // // fork a N_PROC processes
    // for(int i = 0; i < N_PROC; i++) {
    //     if(child_pid == 0) {
    //         continue;
    //     }
    //     child_pid = fork();
    //     if(child_pid == 0) {
    //         my_procnum = i + 1; 
    //     }
    // }
    
    // if(child_pid != 0) {
    //     my_procnum = 0; 
    // }
        
    // // every child process with my_procnum < 10 waits for resources
    // if ((child_pid == 0) && (my_procnum < 10)){
    //     for (int i = 0; i < 1000; i++) {
    //     fifo_wr(f, 1);
    //     printf("Process with my_procnum %d wrote 1 to fifo\n", my_procnum);
    //     // usleep(100000); // process hoards semaphore for 1000000 microseconds (0.1 ms)
    //     }
        
    // }

    // // every child process with my_procnum >= 10 trys to get resource
    // if ((child_pid == 0) && (my_procnum >= 10)) {
    //     fifo_wr(f, 2);
    //     printf("Process with my_procnum %d wrote 2 to fifo\n", my_procnum);
    // }


    // // children exit here
    // if(child_pid == 0) {
    //     exit(EXIT_SUCCESS);
    // }
    
    // // parent waits for all children
    // while(wait(NULL) > 0);
    
    // // // Parent prints result
    // // if(child_pid != 0) {
    // //     fprintf(stdout, "Counter: %lld (expected: 10000000)\n", *counter);
    // //     if(*counter == 10000000) {
    // //         fprintf(stdout, "SUCCESS!\n");
    // //     } else {
    // //         fprintf(stdout, "FAILURE - race condition detected\n");
    // //     }
    // // }
    