#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"
#include<math.h>

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

    if (readingCheck(file,Log)==0) {
        int rows=1000;
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        int phase=0;
        int indexA=0,indexB=0,indexC=0;

        int *peakindex=&indexA;
        double rmsA=compute_rms(Log,rows,phase);
        double peak_to_peak_A=compute_peak_to_peak(Log,rows,phase,&indexA);
        double offsetA=compute_dc_offset(Log,rows,phase);

        phase=1;
        double rmsB=compute_rms(Log,rows,phase);
        double peak_to_peak_B=compute_peak_to_peak(Log,rows,phase,&indexB);
        double offsetB=compute_dc_offset(Log,rows,phase);

        phase=2;
        double rmsC=compute_rms(Log,rows,phase);
        double peak_to_peak_C   =compute_peak_to_peak(Log,rows,phase,&indexC);
        double offsetC=compute_dc_offset(Log,rows,phase);

            printf("========================================================================================================================\n");

        if (rmsA>=253 || rmsA<=207) {
            printf("Phase A RMS: ~%.19lf V (Out of band)\n",rmsA);
        }
        else{printf("Phase A RMS: ~%.19lf V (COMPLIANT)\n",rmsA);}

        if (rmsB>=253 || rmsB<=207) {
            printf("Phase B RMS: ~%.19lf V (Out of band)\n",rmsB);
        }
        else{printf("Phase B RMS: ~%.19lf V (COMPLIANT)\n",rmsB);}

        if (rmsC>=253 || rmsC<=207) {
            printf("Phase C RMS: ~%.19lf V (Out of band)\n",rmsC);
        }
        else{ printf("Phase C RMS: ~%.19lf V (COMPLIANT)\n",rmsC);}


        printf("Phase A peak-to-peak: ~%lf V\n",peak_to_peak_A);
        printf("Phase B peak-to-peak: ~%lf V\n",peak_to_peak_B);
        printf("Phase C peak-to-peak: ~%lf V\n",peak_to_peak_C);

        printf("Phase A DC offset: ~%lf V\n",offsetA);
        printf("Phase B DC offset: ~%lf V\n",offsetB);
        printf("Phase C DC offset: ~%lf V\n",offsetC);




        printf("========================================================================================================================\n");
        }






    fclose(file);
    free(Log);
    return 0;
}