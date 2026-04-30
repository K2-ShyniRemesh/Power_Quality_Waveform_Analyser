#include"io.h"
#include"waveform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readingHeader(FILE *file,WaveformSample *Log) {

    char storeHeading[300];//Buffer for the headings
    fgets(storeHeading,sizeof(storeHeading), file);

    //Split the line into separate headings using the token ","
    char *splitter = strtok(storeHeading, ",\n\r");
    int headingCount = 0;

    while (splitter != NULL && headingCount < 8) {
        //copies the characters to the Headings array of the Log struct
        strncpy(Log[0].Headings[headingCount], splitter, 30);
        Log[0].Headings[headingCount][30] = '\0';
        splitter = strtok(NULL, ",\n\r");
        headingCount++;
    }

    printf("Line0: ");
    for (int i=0;i<headingCount;i++) {
        printf("%s ",Log[0].Headings[i]);
        if (i==headingCount-1) {
            printf("\n");
        }
    }
}

int readingCheck(FILE *file,WaveformSample *Log, int rows){

    readingHeader(file,Log);

    int lines=0;//number of fields that have been read
    char buffer[256];//stores values from one row

    //stores data to the instance of the struct WaveformSample created
    while (lines<rows && fgets(buffer,sizeof(buffer),file) != NULL) {

        char *ptr=buffer;
        char *endptr;

        Log[lines].timestamp = strtod(ptr, &endptr);
        ptr = endptr;
        if (*ptr == ',') ptr++;

        Log[lines].phase_voltage[0]=strtod(ptr,&endptr);
        ptr = endptr;
        if (*ptr == ',') ptr++;

        Log[lines].phase_voltage[1]=strtod(ptr,&endptr);
        ptr = endptr;
        if (*ptr == ',') ptr++;

        Log[lines].phase_voltage[2]=strtod(ptr,&endptr);
        ptr = endptr;
        if (*ptr == ',') ptr++;

        Log[lines].line_current=strtod(ptr,&endptr);
        ptr = endptr;
        if (*ptr == ',') ptr++;

        Log[lines].frequency=strtod(ptr,&endptr);
        ptr = endptr;
        if (*ptr == ',') ptr++;

        Log[lines].power_factor=strtod(ptr,&endptr);
        ptr = endptr;
        if (*ptr == ',') ptr++;

        Log[lines].thd_percent=strtod(ptr,&endptr);

        //outputs the first few
        if (lines<6){
            printf("Line%d: Time=%.4lf, V_A=%.6lf, V_B=%.6lf, V_C=%.6lf, L_C=%.5lf, FQ=%.4lf, P_F=%.4lf, THD=%.4lf \n",
            lines+1,
            Log[lines].timestamp,
            Log[lines].phase_voltage[0],
            Log[lines].phase_voltage[1],
            Log[lines].phase_voltage[2],
            Log[lines].line_current,
            Log[lines].frequency,
            Log[lines].power_factor,
            Log[lines].thd_percent);
        }
        //goes to next row
        lines++;


        //some other error with file
        if (ferror(file)){
            printf("Error reading file!\n");
            exit(-1);
        }
    }
    printf("\nFile format verified!\n");
    return 0;
}

void outputReport(FILE *fp, double rms[3], double peak2peak[3], double offset[3], double stddev[3], int clipped[3]) {


    // Peak-to-Peak
    fprintf(fp, "Phase A peak-to-peak: ~%lf V\n", peak2peak[0]);
    fprintf(fp, "Phase B peak-to-peak: ~%lf V\n", peak2peak[1]);
    fprintf(fp, "Phase C peak-to-peak: ~%lf V\n\n", peak2peak[2]);

    // DC Offset
    fprintf(fp, "Phase A DC offset: ~%lf V\n", offset[0]);
    fprintf(fp, "Phase B DC offset: ~%lf V\n", offset[1]);
    fprintf(fp, "Phase C DC offset: ~%lf V\n\n", offset[2]);

    // Clipped Counts
    fprintf(fp, "Phase A Clipped sample count: %i\n", clipped[0]);
    fprintf(fp, "Phase B Clipped sample count: %i\n", clipped[1]);
    fprintf(fp, "Phase C Clipped sample count: %i\n\n", clipped[2]);

    // Standard Deviation
    fprintf(fp, "Standard Deviation of phase A: %lf\n", stddev[0]);
    fprintf(fp, "Standard Deviation of phase B: %lf\n", stddev[1]);
    fprintf(fp, "Standard Deviation of phase C: %lf\n\n", stddev[2]);

}

void saveSorted(const char *filePath, WaveformSample *Log,int rows) {

    char sortedPath[512];
    strcpy(sortedPath,filePath);

    //uses a pointer to go to the end of the string and finds the . before the extension
    char *dot = sortedPath+strlen(sortedPath);
    while (dot>sortedPath && *dot!='.') dot--;

    //ends the filename at the final dot and adds a string at its end
    if (dot >sortedPath) {
        *dot = '\0';
        strcat(sortedPath,"(sorted).csv");
    }

    //creates the file to store sorted data
    FILE *sortedCSV = fopen(sortedPath, "w");
    if (sortedCSV == NULL) {
        printf("Error: Could not create sorted file %s\n", sortedPath);
        return;
    }

    //writes the headings onto the new csv file
    for (int i=0;i<8;i++) {
        fprintf(sortedCSV,"%s%s",Log[0].Headings[i],(i<7)?",":"\n");
    }

    for (int i = 0; i < rows; i++) {
        fprintf(sortedCSV, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%s",
                Log[i].timestamp,
                Log[i].phase_voltage[0],
                Log[i].phase_voltage[1],
                Log[i].phase_voltage[2],
                Log[i].line_current,
                Log[i].frequency,
                Log[i].power_factor,
                Log[i].thd_percent,
                (i==rows-1)?"":"\n");
    }
    fclose(sortedCSV);
    printf("Sorted data saved to: %s\n", sortedPath);
}