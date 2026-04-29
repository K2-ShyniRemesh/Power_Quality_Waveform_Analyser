#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"
#include <dirent.h>

int main(void) {
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    DIR *directory;
    struct dirent *entry;

    //takes user input for the file path and uses that as the argument for fopen
    char Path[300];
    printf("Enter the file/folder path\n");
    scanf("%s",Path);

    directory=opendir(Path);


    //checks if the directory is empty
    if (directory!=NULL) {

        while ((entry = readdir(directory)) != NULL) {
            //checks files that have .csv extension
            if (strstr(entry->d_name,".csv")) {

                char filePath[512];
                strcpy(filePath,Path);
                strcat(filePath,"/");
                strcat(filePath,entry->d_name);

                FILE *file = fopen(filePath,"r");

                //Checks for error when opening the file
                if (file == NULL){
                    printf("Error opening file!\nCheck the file path entered!\n");
                    exit(-1);
                }
                //NO ERROR
                printf("Opening %s!\n\n",entry->d_name);

                int rows=countRows(file);

                rewind(file);
                WaveformSample *Log = malloc(rows * sizeof(WaveformSample));

                //Logs==NULL can be caused if there is insufficient memory to allocate
                if (Log == NULL) {
                    printf("Memory allocation failed!\n");
                    fclose(file);
                    return -1;
                }

                if (readingCheck(file,Log,rows)==0){
                    int phase=0;

                    printf("------------------------------------------------------------------------------------------------------------------------\n");

                    double rmsA=compute_rms(Log,rows,phase);
                    double peak_to_peak_A=compute_peak_to_peak(Log,rows,phase);
                    double offsetA=compute_dc_offset(Log,rows,phase);
                    int countClippedA=count_clipped(Log,rows,phase);
                    double stdevA=compute_std_dev(Log,rows,phase);

                    phase=1;
                    double rmsB=compute_rms(Log,rows,phase);
                    double peak_to_peak_B=compute_peak_to_peak(Log,rows,phase);
                    double offsetB=compute_dc_offset(Log,rows,phase);
                    int countClippedB=count_clipped(Log,rows,phase);
                    double stdevB=compute_std_dev(Log,rows,phase);

                    phase=2;
                    double rmsC=compute_rms(Log,rows,phase);
                    double peak_to_peak_C   =compute_peak_to_peak(Log,rows,phase);
                    double offsetC=compute_dc_offset(Log,rows,phase);
                    int countClippedC=count_clipped(Log,rows,phase);
                    double stdevC=compute_std_dev(Log,rows,phase);

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
                        printf("Phase C RMS: ~%.19lf V (Out of band)\n\n",rmsC);
                    }
                    else{ printf("Phase C RMS: ~%.19lf V (COMPLIANT)\n\n",rmsC);}


                    printf("Phase A peak-to-peak: ~%lf V\n",peak_to_peak_A);
                    printf("Phase B peak-to-peak: ~%lf V\n",peak_to_peak_B);
                    printf("Phase C peak-to-peak: ~%lf V\n\n",peak_to_peak_C);

                    printf("Phase A DC offset: ~%lf V\n",offsetA);
                    printf("Phase B DC offset: ~%lf V\n",offsetB);
                    printf("Phase C DC offset: ~%lf V\n\n",offsetC);

                    printf("Phase A Clipped sample count: %i\n", countClippedA);
                    printf("Phase B Clipped sample count: %i\n", countClippedB);
                    printf("Phase C Clipped sample count: %i\n\n", countClippedC);

                    printf("Standard Deviation of phase A: %lf\n",stdevA);
                    printf("Standard Deviation of phase B: %lf\n",stdevB);
                    printf("Standard Deviation of phase C: %lf\n\n",stdevC);

                    printf("========================================================================================================================\n");
                }






                fclose(file);
                free(Log);
            }
        }
    closedir(directory);
    }
        return 0;
}