#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include <dirent.h>
#include <string.h>


int main(void) {
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    //takes user input for the file path and uses that as the argument for fopen
    char Path[300];
    printf("Enter the file/folder path\n");
    scanf("%[^\n]",Path);

    //opens the given directory and creates the structure for storing info


    if (strcmp(Path + strlen(Path) - 4, ".csv") == 0) {
        processCSV(Path);
        printf("------------------------------------------------------------------------------------------------------------------------\n");
    }

    else {
        DIR *directory=opendir(Path);
        struct dirent *entry;
        if (directory!=NULL) {

            while ((entry = readdir(directory)) != NULL) {

                //checks files that have .csv extension
                int fileNameLength=strlen(entry->d_name);

                if (strcmp(entry->d_name + fileNameLength - 4, ".csv") == 0) {
                    char filePath[512];
                    strcpy(filePath,Path);
                    strcat(filePath,"/");
                    strcat(filePath,entry->d_name);
                    processCSV(filePath);
                }
            }
            closedir(directory);
            printf("------------------------------------------------------------------------------------------------------------------------\n");
        }
    }
    return 0;
}