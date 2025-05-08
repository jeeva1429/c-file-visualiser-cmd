#include "file_info.h"
#include "file_visualiser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/stat.h>
#include <linux/limits.h>


#define COLOR_BOLD_BLUE "\033[1;34m"
#define COLOR_OFF       "\033[0m"


//  read directory contents and returns info about the read directory
void read_directory(char* file_name, directory_info *dir, int directory_level) {
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
        {
            if (strcmp(dirent_struct->d_name, ".git") == 0 || strcmp(dirent_struct->d_name,".vscode") == 0) {
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
                    int extra_len = strlen("│   ");
                    char *temp = realloc(formatting_space, curr_len + extra_len+ 1); 
                    if(temp != NULL){
                        buffer_size = curr_len + extra_len + 1;
                        formatting_space  = temp;
                        snprintf(formatting_space + curr_len, buffer_size - strlen(formatting_space), "│   ");
                    }
                    else {
                        printf("Memory allocation failed!\n");
                        exit(1);     
                    }
                 
                    start++;
                }
            if(S_ISDIR(file_stat.st_mode)) {
                printf("%s├──%s%s%s\n", formatting_space, COLOR_BOLD_BLUE, dirent_struct->d_name, COLOR_OFF);
                dir->dirCount++;
                directory_level = directory_level + 1;
                 (filebuf, dir, directory_level);
            }
            // \└──
            else {
                printf("%s├──%s\n", formatting_space, dirent_struct->d_name);
                dir->fileCount++;
            }
            if(S_ISDIR(file_stat.st_mode)) {
            directory_level--;
            }
        }   
     }
   
    free(filebuf);  
    closedir(dirp); 
    free(formatting_space);
} 
