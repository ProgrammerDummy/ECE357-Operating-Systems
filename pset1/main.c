#include "pset1lib.h" 

int main() {
    
    struct STREAM *file1 = myfopen("testinput.txt", "r");
    struct STREAM *file2 = myfopen("testoutput.txt", "w");
    
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