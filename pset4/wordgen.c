#include "programs.h"

void wordgen(int wordcount);

int main(int argc, char* argv[]) {
    if(argc == 1 || (argc == 2 && strcmp(argv[1], "0") == 0)) {
        wordgen(-1);
    }

    else {
        wordgen(atoi(argv[1]));
    }
}

void wordgen(int wordcount) {

    srand(time(NULL)); //seeding random number generator with current time

    if(wordcount == 0) {
        wordcount = -1;
    }

    int count = 0;

    while(count != wordcount) {
        int word_length = rand()%8+3; //random word length between 3 and 10
        
        char word[word_length+1];

        for(int i = 0; i < word_length; i++) {
            char letter = (rand()%26)+65; //random uppercase letter
            word[i] = letter; //add it to the "word"
        }

        word[word_length] = '\0';

        fprintf(stdout, "%s\n", word); //print the "word" in the file and then newline
        count++;
    }
    return;
}