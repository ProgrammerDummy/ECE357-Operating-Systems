#include "bgrep.h"

int main(int argc, char* argv[]) {
    //this is the case where there is no flags, only the default

    char *pattern = malloc(1024*sizeof(char));

    if(pattern == NULL) {
        fprintf(stdout, "malloc failed\n");
        return -1;
    }

    if(strcpy(pattern, argv[1]) == NULL) {
        fprintf(stdout, "strcpy failed\n");
        return -1;
    }

    int pattern_length = strlen(pattern);

    int fd = open(argv[2], O_CREAT | O_RDONLY);

    if(fd == -1) {
        fprintf(stdout, "open failed\n");
        return -1;
    }

    int file_length = lseek(fd, 0, SEEK_END);

    char *mapped_file = mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd, 0);

    if(mapped_file == MAP_FAILED) {
        fprintf(stdout, "map failed\n");
        return -1;
    }

    
    for(int i = 0; i < file_length; i++) {
        if(pattern[0] == mapped_file[i]) {
            static int count = 0;
            
            count++;

            fprintf(stdout, "%d ", count);

            int matched_bit_count = 0;

            for(int j = 0; j < file_length; j++) {
                if(pattern[j] == mapped_file[i+j]) {
                    matched_bit_count += 1; 
                }
            }

            if(matched_bit_count == pattern_length) { //bit signature completely matches
                fprintf(stdout, "match complete\n");
                return 0;
                
            }
        }
    }


    free(pattern);

    fprintf(stdout, "no errors or matches found\n");

    return 1;
}