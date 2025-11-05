#include "programs.h"

int main() {
 
    int count = 0;

    FILE* terminal_input = fopen("/dev/tty", "r+");

    char *terminal_command = NULL;
    size_t terminal_command_len = 0;

    char *word = NULL;
    size_t word_len = 0;

    ssize_t status = 0;

    while(true) {

        for(int count = 0; count < 23; count++) {
            status = getline(&word, &word_len, stdin); 
            if(status == -1) {
                //reached end of file
                fprintf(stdout, "*** Pager terminated by EOF ***\n");
                free(word);
                free(terminal_command);
                return 0;
            }

            fprintf(stdout, "%s", word);
        }

        fprintf(stdout, "---Press RETURN for more---\n");

        while(true) {
            getline(&terminal_command, &terminal_command_len, terminal_input);
            if(strcmp(terminal_command, "\n") == 0) {
                break;
            }

            if(strcmp(terminal_command, "q\n") == 0) {
                fprintf(stdout, "*** Pager terminated by Q command ***");
                free(terminal_command);
                free(word);
                fclose(terminal_input);
                return 0;
            }
        }

    }

}