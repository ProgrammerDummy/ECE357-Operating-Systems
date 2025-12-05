#ifndef FIFO_H
#include "sem.h"

#define MYFIFO_BUFSIZ 4096

struct myfifo {
    unsigned long buf[MYFIFO_BUFSIZ];
    int next_write, next_read; // next open read or write indices
    int item_count;
    struct sem empty_sem; // counts the number of empty spaces
    struct sem full_sem; // counts the number of full spaces
    struct sem lock_sem; // used as a simple mutex lock
};

void fifo_init(struct myfifo *f);
void fifo_wr(struct myfifo *f, unsigned long d);
unsigned long fifo_rd(struct myfifo *f);

#define FIFO_H
#endif