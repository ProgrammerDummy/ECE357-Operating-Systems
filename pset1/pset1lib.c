#include "pset1lib.h"

struct MYSTREAM *myfopen(const char *pathname, const char *mode) { //returns a pointer to a struct MYSTREAM object
    if(strcmp(mode, "r") != 0 && strcmp(mode, "w") != 0) {
        errno = EINVAL;
        return NULL;
    }

    struct MYSTREAM *v = (struct MYSTREAM*)malloc(sizeof(struct MYSTREAM)); //allocate memory for struct MYSTREAM vector v

    if(v == NULL) { //check that malloc has not failed
        return NULL; 
    }

    if(strcmp(mode, "r") == 0) {
        v->fd = open(pathname, O_RDONLY | O_CREAT, 0666);
        v->index = 0;
        v->bufbytes = 0;
        v->mode = 'r';
        if(v->fd == -1) {
            return NULL;
        }
        
    }

    else if(strcmp(mode, "w") == 0) {
        v->fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        v->index = 0;
        v->bufbytes = 0;
        v->mode = 'w';
        if(v->fd == -1) {
            return NULL;
        }
    }

    return v;
    
}

struct MYSTREAM *myfdopen(int filedesc, const char *mode) {
    if(strcmp(mode, "r") != 0 && strcmp(mode, "w") != 0) {
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
    v->mode = *mode; 

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
    if(stream->index == BUFSIZ) {
        int bytesWritten = write(stream->fd, stream->buf, stream->bufbytes);

        if(bytesWritten != stream->bufbytes || bytesWritten == 0 || bytesWritten == -1) { 
            return -1;
        } 
    }
    
    stream->buf[stream->index++] = (char)c; 
    stream->bufbytes++;
    return c;
}

int myfclose(struct MYSTREAM *stream) {

    if(stream->mode == 'w') {
        int a = write(stream->fd, stream->buf, stream->bufbytes);
        if(a == -1) {
            return -1;
        }
    }

    int closestatus = close(stream->fd);
    free(stream);

    if(closestatus == 0) {
        return 0;
    }

    return -1;
}