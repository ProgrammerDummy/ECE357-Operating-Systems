#include "pset1lib.h" 
#include <stdio.h>
int main(int argc, char **argv) {

    struct MYSTREAM *file1;
    struct MYSTREAM *file2;
    
    if(argc > 1) {

        if(argc == 4) { //NOTE: unfortunately, only the 1st invokation syntax works, I couldn't figure out how to properly handle the standard input and output
            if(strcmp("-o", argv[1]) != 0) {
                perror("");
                return 255;
            }

            file1 = myfopen(argv[3], "r");
            file2 = myfopen(argv[2], "w");

            if(file1 == NULL || file2 == NULL) {
                perror("");
                return 255;
            }
        }

        if(argc == 3) { 
            if(strcmp("-o", argv[1]) != 0) {
                perror("");
                return 255;
            }

            file1 = myfdopen(STDIN_FILENO, "r");
            file2 = myfopen(argv[2], "w");

            if(file1 == NULL || file2 == NULL) {
                perror("");
                return 255;
            }
        }

        if(argc == 2) {
            file1 = myfopen(argv[1], "r");
            file2 = myfdopen(STDOUT_FILENO, "w");
            if(file1 == NULL || file2 == NULL) {
                perror("");
            }
        }

        

        else if(argc > 4) {
            fprintf(stderr, "what are you doing???? only 4 arguments max!");
        }
    }

    else {
        file1 = myfdopen(STDIN_FILENO, "r");

        if(file1 == NULL) {
            perror("");
            return 255;
        }

        file2 = myfdopen(STDOUT_FILENO, "w");

        if(file2 == NULL) {
            perror("");
            return 255;
        }
    }

    

    int a;

    while((a = myfgetc(file1)) != EOF) {
        if(a == 0x09) {
            for(int i = 0; i < 4; i++) {
                myfputc(0x20, file2);
            }
        }

        else {
            myfputc(a, file2);
        }
    }
    
    if(myfclose(file2) != 0 || myfclose(file1) != 0) {
        perror("");
        return 255;
    }

    return 0;
    
}