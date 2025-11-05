#include "programs.h"

void word_linear_compare(char *dict);

int main(int argc, char* argv[]) {

    if(argc != 2) {
        perror("No file to read provided");
        return 0;
    }

    char *word = NULL;
    size_t word_len = 0;

    char *dict_word = NULL;
    size_t dict_word_len = 0;

    ssize_t dict_read_length = 0;
    
    
    while(true) {
        FILE* dictionary = fopen(argv[1], "r");
        //open file for reading

        if(dictionary == NULL) {
            perror("Error opening file");
            return 0;
        }

        int dict_fd = fileno(dictionary);

        ssize_t read_length = getline(&word, &word_len, stdin); 
        //read one word from stdin

        if(read_length == -1) {
            perror("Failed to read line");
            free(word);
            free(dict_word);
            break;
        }

        while(getline(&dict_word, &dict_word_len, dictionary) != -1) {
            if(strcmp(dict_word, word) == 0) {
                fprintf(stdout, "%s\n", word);
            }
        }

        fclose(dictionary);

        /*if(lseek(dict_fd, 0, SEEK_SET) == -1) {
            perror("");
            return 0;
        } */
        //resets reading offset to start of file

    }
    
    
}
