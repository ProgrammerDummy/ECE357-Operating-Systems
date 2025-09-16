#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define EOF -1
#define BUFSIZ 4096

struct MYSTREAM {
    int fd;
    char buf[BUFSIZ];
    int bufbytes;
    int index;
    char mode;
};

struct MYSTREAM *myfopen(const char *pathname, const char *mode);

struct MYSTREAM *myfdopen(int filedesc, const char *mode);

int myfgetc(struct MYSTREAM *stream);

int myfputc(int c,struct MYSTREAM *stream);

int myfclose(struct MYSTREAM *stream);