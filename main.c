#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

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
    //NO ERROR
    printf("Opening the file!\n\n");

    WaveformSample *Log = malloc(1000 * sizeof(WaveformSample));

    //Logs==NULL can be caused if there is insufficient memory to allocate
    if (Log == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return -1;
    }

    if (readingCheck(file,Log)==0){
        int rows=1000;

        int phase=0;
        double rmsA=compute_rms(Log,rows,phase);

        phase=1;
        double rmsB=compute_rms(Log,rows,phase);

        phase=2;
        double rmsC=compute_rms(Log,rows,phase);

        printf("========================================================================================================================\n");

        if (rmsA<=253 && rmsA>=207) {
            printf("Phase A RMS: ~%.19lf V (Out of band)\n",rmsA);
        }
        else{printf("Phase A RMS: ~%.19lf V (COMPLIANT)\n",rmsA);}

        if (rmsB<=253 && rmsB>=207) {
            printf("Phase B RMS: ~%.19lf V (Out of band)\n",rmsB);
        }
        else{printf("Phase B RMS: ~%.19lf V (COMPLIANT)\n",rmsB);}

        if (rmsC<=253 && rmsC>=207) {
            printf("Phase C RMS: ~%.19lf V (Out of band)\n",rmsC);
        }
        else{ printf("Phase C RMS: ~%.19lf V (COMPLIANT)\n",rmsC);}





        printf("========================================================================================================================\n");
    }

    printf("------------------------------------------------------------------------------------------------------------------------\n");




    fclose(file);
    free(Log);
    return 0;
}



