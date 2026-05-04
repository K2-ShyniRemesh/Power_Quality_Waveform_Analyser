#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include <dirent.h>
#include <string.h>
#include <errno.h>

//for using uint8_t

void processCSV(char *filePath) {

    //opens the file in the given path to read only
    FILE *file = fopen(filePath,"r");

    //Checks for error when opening the file
    if (file == NULL){
        if (errno==ENOENT) {
            printf("%s NOT FOUND!:   %s\n", filePath,strerror(errno));
        }
        else{
            perror("ERROR: Could not open file");
        }
        return;
    }

    //NO ERROR
    printf("Opening %s!\n\n",filePath);

    int rows=countRows(file);//calls the function in misc.c
    rewind(file);

    if (rows==0) {
        printf("%s: IS EMPTY\n\n",filePath);
        fclose(file);
        return;
    }

    //instantiating the structure
    WaveformSample *Log = malloc(rows * sizeof(WaveformSample));


    //if there is insufficient memory to allocate to the structure
    if (Log == NULL) {

        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    if (readingCSV(file,Log,rows)==0) {
        fclose(file);

        //calls function mergesort
        Sort(Log,rows);//function in misc.c

        //saves the sorted csv file
        saveSorted(filePath,Log,rows);//function in io.c

        //all the variables for storing calculating results
        resultSample theResults;
        uint8_t phaseHealth[3]={0,0,0};

        //Calling All calculation functions for each phase
        for (int i=0;i<3;i++) {
            theResults.rms[i]=compute_rms(Log,rows,i);
            theResults.peak2peak[i]=compute_peak_to_peak(Log,rows,i);
            theResults.offset[i]=compute_dc_offset(Log,rows,i);
            theResults.stddev[i]=compute_std_dev(Log,rows,i);
            theResults.clippedCount[i]=count_clipped(Log,rows,i);

            // Check clipping
            if (theResults.clippedCount[i] > 0) {
                phaseHealth[i] |= 0x01;
            }
            // Check tolerance
            if (theResults.rms[i] > 253.0 || theResults.rms[i] < 207.0) {
                phaseHealth[i] |= 0x02;
            }
        }

        range frequencyRange=rangeFinder(Log,rows,offsetof(WaveformSample,frequency));
        range thdRange=rangeFinder(Log,rows,offsetof(WaveformSample,thd_percent));
        range powerFactorRange=rangeFinder(Log,rows,offsetof(WaveformSample,power_factor));

        //calls function to remove the extension from path
        removeExtension(filePath);

        //concatenates the file name and path (minus the .csv) with Report.txt after it
        strcat(filePath,"_Report.txt");

        //Creates the report file with permission to write
        FILE *textFile=fopen(filePath,"w");

        //if the file cannot be made
        if (textFile==NULL) {
            if (errno==EACCES) {
                perror("ERROR: No permission to Create file \nRun program as ADMIN to fix");
            }
            else {
                perror("ERROR: Could not open file");
            }
            return;
        }

        //calling function to output the sorted file
        outputReport(textFile,theResults,phaseHealth,&frequencyRange,&thdRange,&powerFactorRange);
        fclose(textFile);

        printf("%s Created\n",filePath);
        printf("Outputting Report\n");
    }
    free(Log);
}

int main() {

    //takes user input for the file path and uses that as the argument ProcessCSV
    char inputPath[2048];
    printf("Enter the file/folder path\n");
    scanf("%[^\n]",inputPath);


    //checks if the user input is path to folder or file
    if (strlen(inputPath)>4 && strcmp(inputPath + strlen(inputPath) - 4, ".csv") == 0) {
        processCSV(inputPath);//function is in main.c
    }
    else {
        DIR *directory=opendir(inputPath);
        //error handling for invalid directory
        if (directory==NULL){
            perror("ERROR: Could not open directory");
            return 0;
        }

        struct dirent *entry;
        int zeroEntries=0;

        while ((entry = readdir(directory)) != NULL) {
            char filePath[2048];

            if ( strlen(entry->d_name)>4 && strcmp(entry->d_name + strlen(entry->d_name) - 4, ".csv") == 0){
                strcpy(filePath,inputPath);
                strcat(filePath,"/");//no check for slash included(not necessary for most OS)
                strcat(filePath,entry->d_name);

                processCSV(filePath);
                zeroEntries=1;
            }
        }
        closedir(directory);

        if (zeroEntries==0) printf("NO CSV FILE IN THE INPUT DIRECTORY");
    }
    if (errno!=0) printf("%s\n",strerror(errno));

    char exit;
    printf("\n\nPress Enter to exit.....");
    scanf("%c",&exit);
    getchar();
    return 0;
}
