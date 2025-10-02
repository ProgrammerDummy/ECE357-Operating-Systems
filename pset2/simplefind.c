#include "simplefind.h"

struct inodeinfo stored_info[1024];


int recursive_dfs_search1(char* starting_path) {

    DIR *currentdir = opendir(starting_path);

    if(currentdir == NULL) {
        return -1;
    }

    struct dirent *nextdir;
    char buffer[1024];

    //printf("%s\n", nextdir->d_name);

    
    while(true) {
        nextdir = readdir(currentdir);

        if(nextdir == NULL) {
            //printf("NULL\n");
            break;
        }

        if(strcmp(nextdir->d_name, ".") != 0 && strcmp(nextdir->d_name, "..") != 0) {
            //printf("nextdir->d_name %s\n", nextdir->d_name);
        }

        if(strcmp(nextdir->d_name, ".") != 0 && strcmp(nextdir->d_name, "..") != 0) { //does d_name include the whole pathname? if so slicing is needed
            snprintf(buffer, 1024, "%s/%s", starting_path, nextdir->d_name);
            printf("%s\n\n", buffer);
            if(nextdir->d_type == DT_DIR) {
                recursive_dfs_search(buffer);
            }
        }
    }
    

    if(closedir(currentdir) == -1) {
        return -1; 
    }

    return 0;
}




int recursive_dfs_search2(char* starting_path) { //with -l

    DIR *currentdir = opendir(starting_path);

    if(currentdir == NULL) {
        return -1;
    }

    struct dirent *nextdir;
    char buffer[1024];

    //printf("%s\n", nextdir->d_name);

    
    while(true) {
        nextdir = readdir(currentdir);

        if(nextdir == NULL) {
            //printf("NULL\n");
            break;
        }

        if(strcmp(nextdir->d_name, ".") != 0 && strcmp(nextdir->d_name, "..") != 0) {
            //printf("nextdir->d_name %s\n", nextdir->d_name);
        }

        if(strcmp(nextdir->d_name, ".") != 0 && strcmp(nextdir->d_name, "..") != 0) { 
            snprintf(buffer, 1024, "%s/%s", starting_path, nextdir->d_name);
            printf("%s\n\n", buffer);
            if(nextdir->d_type == DT_DIR) {
                recursive_dfs_search(buffer);
            }
        }
    }
    

    if(closedir(currentdir) == -1) {
        return -1; 
    }

    return 0;
}
