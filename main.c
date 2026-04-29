#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"
#include <dirent.h>
#include <string.h>

void processCSV(char *filePath) {
    FILE *file = fopen(filePath,"r");

    //Checks for error when opening the file
    if (file == NULL){
        if (errno==ENOENT) {
            printf("%s NOT FOUND!:   %s\n", filePath,strerror(errno));
            return;
        }
    }

    //NO ERROR
    printf("Opening %s!\n\n",filePath);

    int rows=countRows(file);
    rewind(file);

    WaveformSample *Log = malloc(rows * sizeof(WaveformSample));

    //Logs==NULL can be caused if there is insufficient memory to allocate
    if (Log == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    if (readingCheck(file,Log,rows)==0) {
        double rms[3], peak2peak[3], offset[3], stddev[3];
        int clippedCount[3];

        for (int i=0;i<3;i++) {
            rms[i]=compute_rms(Log,rows,i);
            peak2peak[i]=compute_peak_to_peak(Log,rows,i);
            offset[i]=compute_dc_offset(Log,rows,i);
            stddev[i]=compute_std_dev(Log,rows,i);
            clippedCount[i]=count_clipped(Log,rows,i);
        }

        //calls function to remove the extension from path
        removeExtension(filePath);

        //concatenates the file name and path (minus the .csv) with Report.txt after it
        strcat(filePath,"_Report.txt");

        //Creates the report file with permission to write
        FILE *textFile=fopen(filePath,"w");
        outputReport(textFile,rms,peak2peak,offset,stddev,clippedCount);

        printf("%s Created\n",filePath);
        printf("Outputting Report\n");
    }
        fclose(file);
        free(Log);

}

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