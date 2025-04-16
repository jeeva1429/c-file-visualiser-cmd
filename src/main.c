#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/stat.h>
#include <linux/limits.h>

typedef struct  directory_info {
    int dirCount;
    int fileCount;
} dirInfo;

//  read directory contents and returns info about the read directory
void read_directory(char* file_name, dirInfo *dir) {
    DIR *dirp = opendir(file_name);
    if(dirp == NULL) {
        return;
    }
    struct dirent *dirent_struct;
    char *filebuf = malloc(PATH_MAX * sizeof(char));  
    if (filebuf == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);  
    }
    while((dirent_struct = readdir(dirp)) != NULL){
        if (strcmp(dirent_struct->d_name, ".") == 0 || strcmp(dirent_struct->d_name, "..") == 0) {
            continue;  
        }        
        snprintf(filebuf, PATH_MAX, "%s/%s", file_name, dirent_struct->d_name);

        struct stat file_stat;
        stat(filebuf, &file_stat);  
        if(S_ISDIR(file_stat.st_mode)) {
            printf("DIR:%s \n",dirent_struct->d_name);
            dir->dirCount++;
            read_directory(filebuf, dir);
        }
        else {
            dir->fileCount++;
            printf("    FILE:%s \n",dirent_struct->d_name);
        }
    }    
    free(filebuf);  
    closedir(dirp); 
} 

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
    dirInfo direct_info = {0,0};
    read_directory(folder_name, &direct_info);
    printf("%d directories, %d files \n",direct_info.dirCount, direct_info.fileCount);
    free(folder_name);    

    return 0;
    
}


