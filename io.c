#include"io.h"
#include"waveform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void readingHeader(FILE *file,WaveformSample *Log) {

    char storeHeading[500];//Buffer for the headings
    fgets(storeHeading,sizeof(storeHeading), file);

    //Split the line into separate headings using the token ","
    char *splitter = strtok(storeHeading, ",\n\r");
    int headingCount = 0;

    while (splitter != NULL && headingCount < 8) {
        //copies the characters to the Headings array of the Log struct
        strncpy(Log[0].Headings[headingCount], splitter, 50);
        Log[0].Headings[headingCount][50] = '\0';
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

int readingCSV(FILE *file,WaveformSample *Log, int rows){

    readingHeader(file,Log);

    int lines=0;//number of fields that have been read
    char buffer[sizeof(WaveformSample)*2];//stores values from one row

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

void outputReport(FILE *textFile, resultSample theResults, const uint8_t phaseHealth[3],const range *frequencyRange,const range *thdRange,const range *powerFactorRange) {

    for (int i = 0; i < 3; i++) {
        char phase = "ABC)"[i];

        fprintf(textFile, "Phase %c:\n", phase);

        fprintf(textFile, "     RMS: %.3lf V\n", theResults.rms[i]);
        fprintf(textFile, "     Peak-to-Peak: %.3lf V\n", theResults.peak2peak[i]);
        fprintf(textFile, "     DC Offset: %.3lf V\n", theResults.offset[i]);
        fprintf(textFile, "     Clipped Count: %d\n", theResults.clippedCount[i]);
        fprintf(textFile, "     Std Dev: %.3lf\n", theResults.stddev[i]);
        fprintf(textFile, "     Variance: %.3lf\n", theResults.stddev[i]*theResults.stddev[i]);

            // Checks if the value is zero/phase is healthy
        if (phaseHealth[i] == 0)  fprintf(textFile,   "[NO CLIPPING]\n[WITHIN TOLERANCE RANGE]\n\n");

        else {
            // Clipping
            if (phaseHealth[i] & 0x01) fprintf(textFile, "     [CLIPPING DETECTED]\n\n");
            //Tolerance
            if (phaseHealth[i] & 0x02) fprintf(textFile, "     [OUT OF TOLERANCE LIMIT]\n\n");
            }
    }

    fprintf(textFile,"Frequency Range: %.3lf to %.3lf Hz\n",frequencyRange->lowest,frequencyRange->highest);
    fprintf(textFile,"Power Factor Range: %.3lf to %.3lf\n",powerFactorRange->lowest,powerFactorRange->highest);
    fprintf(textFile,"THD percent Range: %.2lf%% to %.2lf%%\n",thdRange->lowest,thdRange->highest);
}

void saveSorted(const char *filePath, WaveformSample *Log,int rows){

    char sortedPath[2048];
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
        fprintf(sortedCSV, "%.19g,%.19g,%.19g,%.19g,%.19g,%.19g,%.19g,%.19g,%s",
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