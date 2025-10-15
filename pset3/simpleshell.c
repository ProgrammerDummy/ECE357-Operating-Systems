#include "simpleshell.h"

int io_redirection(char **io_redirect_info, int io_redirect_info_index);
int simpleshell();

int simpleshell() {

    int linebufsize = 0;

    printf("simpleshell: ");
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
    int arguments_index = 1;

    token = strtok(linebuf, " \t\r\n\a"); //breaks up into tokens (3)
    
    if(strcpy(command, token) == NULL || strcpy(arguments[0], token) == NULL) { //first token should always be command
        return -1;
    }
    
    token = strtok(NULL, " \t\r\n\a");

    //format: command {argument {argument...}} {redirection operation {redirection operation}} 

    //lets assume the simplest case here --> no bullshit like cd <dir> && cat <file>

    //QUESTION: do i need to malloc/free?

    while(token != NULL) {

        if((token[0]) == '>' || token[0] == '<' || token[0] == '2') { //look for IO redirection
            //need to store IO redirection shit here to separate --> TODO
            io_redirect_info[io_redirect_info_index++] = token;
        }

        else { //everything else lets put it into arguments 
            arguments[arguments_index++] = token; 
        }

        token = strtok(NULL, " \t\r\n\a");
    }

    
    int pid;
    int real_time_elapsed_begin = 0;
    int user_CPU_time = 0;
    int system_CPU_time = 0;
    int status;


    switch(pid = fork()) { //forking (4)
        case -1:
            perror("fork error");
            return -1;

        case 0: //child process runs (5C, 6C) --> need to retokenize to separate the filename to the io redirection command 

            if(io_redirection(io_redirect_info, io_redirect_info_index) != 0) {
                _exit(EXIT_FAILURE);
            }

            if(execvp(command, arguments) == -1) { //executes command and the arguments
                _exit(EXIT_FAILURE); //if execvp fails, then exits with EXIT_FAILURE status code
            }
            
            _exit(EXIT_SUCCESS); //execvp works, so exits with EXIT_SUCCESS

        default: //parent (5P, 6P)
            waitpid(pid, &status, 0); //wait for child process to exit

            

            return 0;
    }
}

int io_redirection(char **io_redirect_info, int io_redirect_info_index) {

    for(int i = 0; i < io_redirect_info_index; i++) { 

        int fd;

        if(io_redirect_info[i][0] == '<') { //simplest case <filename
            char *filename = strtok(io_redirect_info[i], "<"); //filename is extracted

            fd = open(filename, O_RDONLY); 
            if(fd == -1) {
                perror("error opening file for input");
                return -1;
            }

            dup2(fd, 0);
            close(fd);
        }

        else if(io_redirect_info[i][0] == '>') { 
            char *filename;

            if(io_redirect_info[i][1] == '>') { //case: >>filename
                filename = strtok(io_redirect_info[i], ">>");
                fd = open(filename, O_CREAT | O_APPEND); 
            }

            else { //case: >filename
                filename = strtok(io_redirect_info[i], ">");
                fd = open(filename, O_CREAT | O_TRUNC);
            }

            if(fd == -1) {
                perror("error opening file for output");
                return -1;
            }


            dup2(fd, 1);
            close(fd);
        }

        else { //this is for the case of redirecting stderr 2> or 2>> 
            char *filename;
     
            if(io_redirect_info[i][1] == '>' && io_redirect_info[i][2] == '>') { //case: 2>>filename
                filename = strtok(io_redirect_info[i], ">>");
                fd = open(filename, O_CREAT | O_APPEND);
            }

            else { //case: 2>filename
                filename = strtok(io_redirect_info[i], ">");
                fd = open(filename, O_CREAT | O_TRUNC);
            }

            if(fd == -1) {
                perror("error opening file for output");
                return -1;
            }

            dup2(fd, 2);
            close(fd);
            
        }
    }

    return 0;
}
