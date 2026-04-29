#include"io.h"
#include"waveform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void removeExtension(char *fileName) {
    char *end = fileName + strlen(fileName);

    while (end > fileName && *end != '.') {
        --end;
    }

    if (end > fileName) {
        *end = '\0';
    }
}



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



int readingCheck(FILE *file,WaveformSample *Log, int rows){

    //Stores the Headers here so that it will not be read by the fscanf function which cannot convert that string to float
    char storeHeading[200];//according to the number of characters in the combined headings
    if (fgets(storeHeading,sizeof(storeHeading),file)==NULL) {
        printf("Error reading file!\n");
        return -1;
    }

    int read=0;//stores number of columns read successfully
    int lines=0;//number of fields that have been read

    //stores data to the instance of the struct WaveformSample created
    while (lines<rows){
        read=fscanf(file,
                    "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                    &Log[lines].timestamp,
                    &Log[lines].phase_voltage[0],
                    &Log[lines].phase_voltage[1],
                    &Log[lines].phase_voltage[2],
                    &Log[lines].line_current,
                    &Log[lines].frequency,
                    &Log[lines].power_factor,
                    &Log[lines].thd_percent);

        //if one row is read without error read increment by 1
        if (read ==8){
            //outputs the first few
            if (lines<6){printf("Line%d: Time=%.4lf, V_A=%.6lf, V_B=%.6lf, V_C=%.6lf, L_C=%.5lf, FQ=%.4lf, P_F=%.4lf, THD=%.4lf \n",
                lines+1,
                Log[lines].timestamp,
                Log[lines].phase_voltage[0],
                Log[lines].phase_voltage[1],
                Log[lines].phase_voltage[2],
                Log[lines].line_current,
                Log[lines].frequency,
                Log[lines].power_factor,
                Log[lines].thd_percent);}
                lines++;
        }

        //error e.g. the number of fields in a row is not 8
        if (read !=8 ){
            printf("The file is not formatted correctly!\n");
            exit(-1);
        }

        //some other error with file
        if (ferror(file)){
            printf("Error reading file!\n");
            exit(-1);
        }
    }
    printf("\nFile format verified!\n");
    return 0;
}


void notQsort(WaveformSample *Log) {

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