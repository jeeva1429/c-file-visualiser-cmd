#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_info.h"
#include "file_visualiser.h"


int main(int argc, char *argv[]) {  
    char *folder_name;
    if (argv[1]) {
        folder_name = malloc(strlen(argv[1])+1 * sizeof(char));
        if (folder_name == NULL) {
            printf("Memory allocation failed!\n");
            return 1;
        }        
        strcpy(folder_name,argv[1]);
    }
    else {
        char *curr_path = ".";
        folder_name = malloc(strlen(curr_path)+1 * sizeof(char));
        if (folder_name == NULL) {
            printf("Memory allocation failed!\n");
            return 1;  
        }
        strcpy(folder_name, curr_path);
    }
    // initial directory and files count
    directory_info direct_info = {0,0};
    int directory_level = 0;
    // traverse directory and print the contents
    read_directory(folder_name, &direct_info, directory_level);
    // print the directories and files count
    printf("%d directories, %d files \n",direct_info.dirCount, direct_info.fileCount);

    // free allocated memory for the folder path
    free(folder_name);    

    return 0;
    
}


