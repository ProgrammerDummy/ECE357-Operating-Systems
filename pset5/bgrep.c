#include "bgrep.h"

int main(int argc, char* argv[]) {
    //this is the case where there is no flags, only the default

    char *pattern = malloc(1024*sizeof(char));

    strcpy(pattern, argv[1]);

    int fd = open(argv[2], O_CREAT | O_RDONLY);

    int length = lseek(fd, 0, SEEK_END);

    char *mapped_file = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);

    fprintf(stdout, "%s", mapped_file);

    fprintf(stdout, "\n");

    free(pattern);


}