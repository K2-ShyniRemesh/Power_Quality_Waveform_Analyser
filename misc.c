#include"waveform.h"
#include "io.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countRows(FILE *file) {
    int ch;
    int count = 0;
    int last_ch = '\n';

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
        last_ch = ch;
    }

    // Handle files that don't end with a newline
    if (last_ch != '\n') {
        count++;
    }



    // Subtract 1 to remove the "timestamp,phase_A_voltage..." header
    return (count > 0) ? (count - 1) : 0;
}

void removeExtension(char *fileName) {
    char *end = fileName + strlen(fileName);

    while (end > fileName && *end != '.') {
        --end;
    }

    if (end > fileName) {
        *end = '\0';
    }
}

void notQsort(WaveformSample *Log) {

}

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