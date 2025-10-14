#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <fnmatch.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <pwd.h>     
#include <grp.h>
#include <time.h>

int simpleshell();

char linebuf[1024];


