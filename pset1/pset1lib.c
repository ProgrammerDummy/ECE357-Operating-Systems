#include "pset1lib.h"

struct MYSTREAM {
    int fd;
    char buf[BUFSIZ];
    int bufbytes;
    int index;
};

struct MYSTREAM *myfopen(const char *pathname, const char *mode) { //returns a pointer to a struct MYSTREAM object
    if(mode != "r" || mode != "w") {
        errno = EINVAL;
        return NULL;
    }

    struct MYSTREAM *v = (struct MYSTREAM*)malloc(sizeof(struct MYSTREAM)); //allocate memory for struct MYSTREAM vector v

    if(v == NULL) { //check that malloc has not failed
        return NULL; 
    }

    if(mode == "r") {
        v->fd = open(pathname, O_RDONLY | O_CREAT, 0666);
        v->index = 0;
        v->bufbytes = 0;
        if(v->fd == -1) {
            return NULL;
        }

        
    }

    else {
        v->fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        v->index = 0;
        v->bufbytes = 0;
        if(v->fd == -1) {
            return NULL;
        }
    }

    return v;
    
}

struct MYSTREAM *myfdopen(int filedesc, const char *mode) {
    if(mode != "r" || mode != "w") {
        errno = EINVAL;
        return NULL;
    }

    struct MYSTREAM *v = (struct MYSTREAM*)malloc(sizeof(struct MYSTREAM)); //allocate memory for struct MYSTREAM vector v

    if(v == NULL) { //check that malloc has not failed
        return NULL; 
    }

    v->fd = filedesc;
    v->index = 0;
    v->bufbytes = 0;

    if(v->fd == -1) {
        return NULL;
    }

    return v;
}

int myfgetc(struct MYSTREAM *stream) { 

    if(stream->index >= stream->bufbytes) { 
        stream->bufbytes = read(stream->fd, stream->buf, BUFSIZ);
        stream->index = 0;
        if(stream->bufbytes == -1) {
            return -1;
        }
        if(stream->bufbytes == 0) {
            errno = 0;
            return -1;
        }
    }

    return (int)stream->buf[stream->index++];

}

int myfputc(int c, struct MYSTREAM *stream) {

    if(stream->index == BUFSIZ) { //is it supposed to be (stream->index >= stream->bufsize) ??? check
        int bytesWritten = write(stream->fd, stream->buf, stream->bufbytes);

        if(bytesWritten != stream->bufbytes || bytesWritten == 0 || bytesWritten == -1) { 
            return -1;
        } 
    }

    stream->buf[stream->index++] = (char)c; //IS THIS RIGHT?
    return c;
}

int myfclose(struct MYSTREAM *stream) {

    //CHECK HERE IF STREAM IS OPEN FOR WRITING OR NOT BEFORE DOING close()

    int closestatus = close(stream->fd);

    free(stream);

    if(closestatus == 0) {
        return 0;
    }

    return -1;
}