#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "waveform.h"

int checkFormat(FILE *file){

    /*Stores the Headers here so that it will not be read by the
      fscanf function which cannot convert that string to float*/
    char storeHeader[109];//according to the number of characters in the combined headings
    if (fgets(storeHeader,sizeof(storeHeader),file)==NULL) {
        printf("Error reading file!\n");
        return -1;
    }

    WaveformSample Log[7];
    int read=0;//stores number of columns read successfully
    int lines=0;//number of fields that have been read

    //Checks the first few lines
    while (lines<5){
        read=fscanf(file,
                    "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                    &Log[lines].timestamp,
                    &Log[lines].phase_A_voltage,
                    &Log[lines].phase_B_voltage,
                    &Log[lines].phase_C_voltage,
                    &Log[lines].line_current,
                    &Log[lines].frequency,
                    &Log[lines].power_factor,
                    &Log[lines].thd_percent);

        //if one row is read without error read increment by 1
        if (read ==8){
            printf("Line%d: Time=%.4lf, V_A=%.6lf, V_B=%.6lf, V_C=%.6lf, L_C=%.5lf, FQ=%.4lf, P_F=%.4lf, THD=%.4lf \n",
                lines+1,
                Log[lines].timestamp,
                Log[lines].phase_A_voltage,
                Log[lines].phase_B_voltage,
                Log[lines].phase_C_voltage,
                Log[lines].line_current,
                Log[lines].frequency,
                Log[lines].power_factor,
                Log[lines].thd_percent);
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


 //compute_rms(samples,n){}



/*compute_peaks(samples,n){}

compute_dc_offset(samples,n){}

count_clipped(samples,n,limit){}

check_compliance(rms,nominal){}

compute_std_dev(samples,n){}*/


int main(void){

    printf("------------------------------------------------------------------------------------------------------------------------\n");

    //takes user input for the file path and uses that as the argument for fopen
    char filePath[300];
    printf("Enter the file path (use ../\"file.csv\" for file in main project folder)\n");
    scanf("%s",filePath);

    FILE *file = fopen("../theLog.csv","r");
    //Checks for error when opening the file
    if (file == NULL){
        printf("Error opening file!\nCheck the file path entered!\n");
        exit(-1);
    }
    //NO ERROR :)
    printf("Opening the file!\n\n");

    checkFormat(file);

    printf("------------------------------------------------------------------------------------------------------------------------\n");






    return 0;
}



