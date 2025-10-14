#include "simpleshell.h"

int simpleshell() {

    int linebufsize = 0;

    if(fgets(linebuf, 1024, stdin) == NULL) { 
        perror("");
    } //maybe for here i can use something else...?


    if(linebuf[0] == '#') { //if the line of input is a comment, just return (2)
        return 0;
    }

    char *token;
    char *command;
    char **io_redirect_info;
    int io_redirect_info_index = 0;

    char **arguments;
    int arguements_index = 0;

    token = strtok(linebuf, " \t\r\n\a"); //breaks up into tokens (3)

    if(strcpy(command, token) == NULL) { //first token should always be command
        return -1;
    }

    token = strtok(NULL, " \t\r\n\a");

    //format: command {argument {argument...}} {redirection operation {redirection operation}} 

    //lets assume the simplest case here --> no bullshit like cd <dir> && cat <file>

    //QUESTION: do i need to malloc/free?

    while(token != NULL) {

        //printf("%s\n", token);
        if((token[0]) == '>' || token[0] == '<' || token[0] == '2') { //look for IO redirection
      d      //need to store IO redirection shit here to separate --> TODO
            io_redirect_info[io_redirect_info_index++] = token;
        }

        else { //everything else lets put it into arguments 
            arguments[arguements_index++] = token; 
        }

        token = strtok(NULL, " \t\r\n\a");
    }

    for(int i = 0; i < io_redirect_info_index; i++) {
        printf("%s ", io_redirect_info[i]);
    }

    for(int i = 0; i < arguements_index; i++) {
        printf("%s ", arguments[i]);
    }


    /*
    int pid;
    switch(pid = fork()) { //forking (4)
        case -1:
            return -1;

        case 0: //child process runs (5C, 6C) --> need to retokenize to separate the filename to the io redirection command 

        for(int i = 0; i < io_redirect_info; i++) { 
            if(io_redirect_info[i][0] == '>') {
                char *filename = r
            }

            else if(io_redirect_info[i][0] == '<') { 
                char *filename;
            }

            else {
                char *filename;
            }
        }
        //exec the command here w the arguments
            


        default: //parent (5P, 6P)
        
    }
    */
    

    

    return 0;
}


