#ifndef SIMPLEFIND_H
#define SIMPLEFIND_H

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
#include <sys/sysmacros.h>
#include <time.h>

int print_verbose(char *buffer);
int print_info(bool ls_flag, bool xdev_flag, bool name_flag, char *name_pattern, char *starting_path, char *buffer, struct dirent *nextdir);
int recursive_dfs_search(bool ls_flag, bool xdev_flag, bool name_flag, char *name_pattern, char *starting_path);

#endif

