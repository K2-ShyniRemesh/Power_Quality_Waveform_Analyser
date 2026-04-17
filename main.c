#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Waveform.h"

int main(void){

    printf("----------------------------------------------------\n");
    FILE *file = fopen("power_quality_log.csv","r");

    //Checks for error when opening the file
    if (file == NULL){
        printf("Error opening file!\n");
        exit(-1);
    }
    printf("Opening the file!!!!!!!!\n");

    WaveformSample Log[1000];

    int read=0;//stores number of values read successfully
    int lines=0;//number of fields that have been read

    do {
        read=fscanf(file,
                    "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                    &Log[lines].frequency,
                    &Log[lines].phase_A_voltage,
                    &Log[lines].phase_B_voltage,
                    &Log[lines].phase_C_voltage,
                    &Log[lines].line_current,
                    &Log[lines].frequency,
                    &Log[lines].power_factor,
                    &Log[lines].thd_percent);

        //if one row is read without error read increment by 1
        if (read ==8)lines++;

        //error e.g. the number of fields in a row is not 8
        if (read !=8 && feof(file)){
            printf("The file is not formatted correctly!\n");
            exit(-1);
        }

        //some other error with file
        if (ferror(file)){
            printf("Error reading file!\n");exit(-1);
        }
    }
    while (!feof(file));//Loops until end of file reached
    printf("file successfully read!\n");
    printf("----------------------------------------------------\n");


    return 0;
}