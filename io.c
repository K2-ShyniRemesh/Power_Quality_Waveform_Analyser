#include"io.h"
#include"waveform.h"
#include <stdio.h>
#include <stdlib.h>

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



/*
//Creates a new txt file and writes the below outputs onto it
FILE *Output = fopen("../output.txt","w");

fprintf(Output,"Phase A RMS:");
fprintf(Output,"\nPhase A Peak-to-Peak:");
fprintf(Output,"\nPhase A DC offset:");
fprintf(Output,"\nPhase B RMS");
fprintf(Output,"\nPhase B Peak-to-Peak:");
fprintf(Output,"\nPhase B DC offset:");
fprintf(Output,"\nPhase C RMS");
fprintf(Output,"\nPhase B Peak-to-Peak:");
fprintf(Output,"\nPhase B DC offset:");
fprintf(Output,"\nClipped samples:");
fprintf(Output,"\nFrequency range:");
fprintf(Output,"\nPower factor range:");
fprintf(Output,"\nTHD range:");
fprintf(Output,"\nStandard deviation:");

printf("Output saved as output.txt in the main program directory");
fclose(Output);*/