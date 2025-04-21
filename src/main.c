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
void read_directory(char* file_name, dirInfo *dir, int directory_level) {
    DIR *dirp = opendir(file_name);
    if(dirp == NULL) {
        return;
    }
    struct dirent *dirent_struct;
    char *filebuf = malloc(PATH_MAX);  
    if (filebuf == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);  
    }

    char *formatting_space = malloc(strlen("")+1 * sizeof(char));
    strcpy(formatting_space, "");

    while((dirent_struct = readdir(dirp)) != NULL){
        if (strcmp(dirent_struct->d_name, ".git") == 0) {
            continue;
        }        
        if (strcmp(dirent_struct->d_name, ".") == 0 || strcmp(dirent_struct->d_name, "..") == 0) {
            continue;  
        }        
        snprintf(filebuf, PATH_MAX, "%s/%s", file_name, dirent_struct->d_name);
        struct stat file_stat;
        stat(filebuf, &file_stat);  
        char *formatting_space = malloc(sizeof(""));
        strcpy(formatting_space,  "");
        int start = 0;
        int buffer_size = 1;
        while (start < directory_level) {
                int curr_len = strlen(formatting_space);
                int extra_len = strlen("   ");
                char *temp = realloc(formatting_space, curr_len + extra_len+ 1); 
                if(temp != NULL){
                    buffer_size = curr_len + extra_len + 1;
                    formatting_space  = temp;
                    snprintf(formatting_space + curr_len, buffer_size - strlen(formatting_space), "   ");
                }
                else {
                    printf("Memory allocation failed!\n");
                    exit(1);     
                }
                start++;
            }
        printf("%s|__ %s\n",formatting_space,dirent_struct->d_name);
        if(S_ISDIR(file_stat.st_mode)) {
            dir->dirCount++;
            directory_level = directory_level + 1;
            read_directory(filebuf, dir, directory_level);
        }
        else {
            dir->fileCount++;
        }
        if(S_ISDIR(file_stat.st_mode)) {
        directory_level--;
        }
    }   
    free(filebuf);  
    closedir(dirp); 
    free(formatting_space);
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
    int directory_level = 0;
    read_directory(folder_name, &direct_info, directory_level);
    printf("%d directories, %d files \n",direct_info.dirCount, direct_info.fileCount);
    free(folder_name);    

    return 0;
    
}


// [0,1,2,3,4]
// 
// 7
// 1 2 1 2 1 2 1
// -1 0 0 0 0 0 0