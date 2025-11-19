#include "bgrep.h"

int bgrep(bool pattern_flag, bool context_flag, char *pattern, char **file_arr, int file_count);

int main(int argc, char* argv[]) {
    //this is the case where there is no flags, only the default

    bool pattern_flag = false;
    bool context_flag = false;
    char *pattern = NULL;

    char **file_arr = malloc(BUFSIZ * sizeof(char*)); // can store up to BUFSIZ filenames

    int opt;
    while ((opt = getopt(argc, argv, "cp:")) != -1) {
        switch (opt) {
            case 'c':
                context_flag = true;
                break;
            case 'p':
                pattern_flag = true;
                pattern = optarg;
                break;
            case '?':
                return 1;
        }
    }

    if (!pattern_flag) {
        pattern = argv[optind++];
    }
    
    int file_count = 0;
    while ((argc - optind) > 0) {
        file_arr[file_count++] = argv[optind++]; 
    }

    int bgrep_return_value = bgrep(pattern_flag, context_flag, pattern, file_arr, file_count);

    free(file_arr);

    if(bgrep_return_value != 0) {
        // error occured in bgrep function call
        return bgrep_return_value;
    }
    return 0;

}


int bgrep(bool pattern_flag, bool context_flag, char *pattern, char **file_arr, int file_count) {

    int pattern_length = 0;
    
    if(pattern_flag == true) {
        int pattern_fd = open(pattern, O_RDONLY);
        
        if(pattern_fd == -1) {
            perror("could not open pattern file");
            return -1;
        }

        pattern_length = lseek(pattern_fd, 0, SEEK_END);
        
        pattern = mmap(NULL, pattern_length, PROT_READ, MAP_PRIVATE, pattern_fd, 0);

        if(close(pattern_fd) == -1) {
            perror("could not close pattern file");
            return -1;
        }

        if(pattern == MAP_FAILED) {
            perror("failed to map pattern files");
            return -1;
        }
    }

    else {
        pattern_length = strlen(pattern);
    }



    for(int i = 0; i < file_count; i++) {

        int file_fd = open(file_arr[i], O_CREAT | O_RDONLY);

        if(file_fd == -1) {
            perror("could not open file");
            return -1;
        }

        int mapped_file_length = lseek(file_fd,0, SEEK_END);

        char *mapped_file = mmap(NULL, mapped_file_length, PROT_READ, MAP_PRIVATE, file_fd, 0);

        if(close(file_fd) == -1) {
            perror("could not close file");
            return -1;
        }

        if(mapped_file == MAP_FAILED) {
            perror("failed to map file");
            return -1;
        }

        //from this point on now i have everything ready


        for(int i = 0; i < mapped_file_length; i++) {

            int count = 0;
            bool match_failed_flag = 0;

            if(pattern[0] == mapped_file[i]) {
                for(int j = 0; j < pattern_length; j++) {
                    if(pattern[j] != mapped_file[i+j]) {
                        match_failed_flag = true;
                    }
                }
            }

            if(match_failed_flag == false) {
                fprintf(stdout, "match complete\n");
                return 0;
            }
        }


        if(munmap(mapped_file, mapped_file_length) == -1) {
            perror("munmap failed");
            return -1;
        }

        if(pattern_flag == true) {
            if(munmap(pattern, pattern_length) == -1) {
                perror("munmap failed for pattern file");
                return -1;
            }
        }

    }

    fprintf(stdout, "no errors or matches found\n");

    return 1;

}