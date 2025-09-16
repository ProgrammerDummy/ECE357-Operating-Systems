#include "pset1lib.h" 

int main(int argc, char **argv) {
    
    struct STREAM *file1 = myfopen(argv[1], "r");
    struct STREAM *file2 = myfopen(argv[2], "w");
    
    int a;
    while((a = myfgetc(file1)) != EOF) {
        if(a == 9) {
            myfputc(32, file2);
        }

        else {
            myfputc(a, file2);
        }
    }
    
    myfclose(file2);
    myfclose(file1);

    
}