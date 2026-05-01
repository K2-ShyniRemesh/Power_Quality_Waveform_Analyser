#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include <dirent.h>
#include <string.h>
#include <errno.h>

//for using uint8_t
#include <stdint.h>

void processCSV(char *filePath) {

    //opens the file in the given path to read only
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

    int rows=countRows(file);//calls the function in misc.c
    rewind(file);

    WaveformSample *Log = malloc(rows * sizeof(WaveformSample));

    //Logs==NULL can be caused if there is insufficient memory to allocate
    if (Log == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    if (readingCheck(file,Log,rows)==0) {

        //calls function mergesort
        Sort(Log,rows);//function in misc.c
        //saves the sorted csv file
        saveSorted(filePath,Log,rows);//function in io.c

        double rms[3], peak2peak[3], offset[3], stddev[3];
        int clippedCount[3];

        uint8_t phaseHealth[3]={0,0,0};

        //Calling All calculation functions for each phase
        for (int i=0;i<3;i++) {
            rms[i]=compute_rms(Log,rows,i);
            peak2peak[i]=compute_peak_to_peak(Log,rows,i);
            offset[i]=compute_dc_offset(Log,rows,i);
            stddev[i]=compute_std_dev(Log,rows,i);
            clippedCount[i]=count_clipped(Log,rows,i);

            // Check clipping
            if (clippedCount[i] > 0) {
                phaseHealth[i] |= 0x01;
            }
            // Check tolerance
            if (rms[i] > 253.0 || rms[i] < 207.0) {
                phaseHealth[i] |= 0x02;
            }
        }


        //calls function to remove the extension from path
        removeExtension(filePath);

        //concatenates the file name and path (minus the .csv) with Report.txt after it
        strcat(filePath,"_Report.txt");

        //Creates the report file with permission to write
        FILE *textFile=fopen(filePath,"w");
        if (textFile){
            outputReport(textFile,rms,peak2peak,offset,stddev,clippedCount,phaseHealth);
            fclose(textFile);
        }
        else {

        }
        printf("%s Created\n",filePath);
        printf("Outputting Report\n");
    }
    fclose(file);
    free(Log);

}

int main(int argc, char *argv[]) {
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    //takes user input for the file path and uses that as the argument ProcessCSV
    char inputPath[2048];
    printf("Enter the file/folder path\n");

    //case where the input path does not exist
    if (scanf("%[^\n]",inputPath)!=1) {
        printf("the ");
    }

    //checks if the user input is path to folder or file
    if (strcmp(inputPath + strlen(inputPath) - 4, ".csv") == 0) {

        processCSV(inputPath);//function is in main.c
        printf("------------------------------------------------------------------------------------------------------------------------\n");
    }
    else {
        DIR *directory=opendir(inputPath);
        struct dirent *entry;

        if (directory!=NULL) {
            while ((entry = readdir(directory)) != NULL) {
                char filePath[2048];
                    if (strcmp(entry->d_name + strlen(entry->d_name) - 4, ".csv") == 0) {
                        strcpy(filePath,inputPath);
                        strcat(filePath,"/");//no check for slash included(not necessary for most OS)
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