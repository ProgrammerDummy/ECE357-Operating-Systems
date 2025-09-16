#include "pset1lib.h" 
#include <stdio.h>
int main(int argc, char **argv) {
    
    struct STREAM *file1 = myfopen(argv[1], "r");
    struct STREAM *file2 = myfopen(argv[2], "w");
    
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
    
    if(myfclose(file2) == 0 && myfclose(file1) == 0) {
        return 0;
    }

    else {
        return 255;
    }

}