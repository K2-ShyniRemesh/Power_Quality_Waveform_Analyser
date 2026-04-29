#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"
#include <dirent.h>
#include <string.h>

int main(void) {
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    //takes user input for the file path and uses that as the argument for fopen
    char Path[300];
    printf("Enter the file/folder path\n");
    scanf("%[^\n]",Path);

    //opens the given directory and creates the structure for storing info
    DIR *directory=opendir(Path);
    struct dirent *entry;

    if (directory!=NULL) {

        while ((entry = readdir(directory)) != NULL) {

            //checks files that have .csv extension
            int fileNameLength=strlen(entry->d_name);
            if (strcmp(entry->d_name + fileNameLength - 4, ".csv") == 0) {

                char filePath[512];
                strcpy(filePath,Path);
                strcat(filePath,"/");
                strcat(filePath,entry->d_name);

                FILE *file = fopen(filePath,"r");

                //Checks for error when opening the file
                if (file == NULL){
                    if (errno==ENOENT) {
                        printf("%s NOT FOUND!:   %s\n", entry->d_name,strerror(errno));
                    }
                    continue;
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

                if (readingCheck(file,Log,rows)==0) {
                    int phase=0;



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

                    //calls function to remove the extension from path
                    removeExtension(filePath);

                    //concatenates the file name and path (minus the .csv) with Report.txt after it
                    strcat(filePath,"_Report.txt");

                    //Creates the report file with permission to write
                    FILE *textFile=fopen(filePath,"w");

                    printf("%s Created\n",filePath);
                    printf("Outputing Report\n");


                        if (rmsA>=253 || rmsA<=207) {
                            fprintf(textFile,"Phase A RMS: ~%.19lf V (Out of band)\n",rmsA);
                        }
                        else {
                            fprintf(textFile,"Phase A RMS: ~%.19lf V (COMPLIANT)\n",rmsA);
                        }

                        if (rmsB>=253 || rmsB<=207) {
                            fprintf(textFile,"Phase B RMS: ~%.19lf V (Out of band)\n",rmsB);
                        }
                        else{fprintf(textFile,"Phase B RMS: ~%.19lf V (COMPLIANT)\n",rmsB);}

                        if (rmsC>=253 || rmsC<=207) {
                            fprintf(textFile,"Phase C RMS: ~%.19lf V (Out of band)\n\n",rmsC);
                        }
                        else{ fprintf(textFile,"Phase C RMS: ~%.19lf V (COMPLIANT)\n\n",rmsC);}


                        fprintf(textFile,"Phase A peak-to-peak: ~%lf V\n",peak_to_peak_A);
                        fprintf(textFile,"Phase B peak-to-peak: ~%lf V\n",peak_to_peak_B);
                        fprintf(textFile,"Phase C peak-to-peak: ~%lf V\n\n",peak_to_peak_C);

                        fprintf(textFile,"Phase A DC offset: ~%lf V\n",offsetA);
                        fprintf(textFile,"Phase B DC offset: ~%lf V\n",offsetB);
                        fprintf(textFile,"Phase C DC offset: ~%lf V\n\n",offsetC);

                        fprintf(textFile,"Phase A Clipped sample count: %i\n", countClippedA);
                        fprintf(textFile,"Phase B Clipped sample count: %i\n", countClippedB);
                        fprintf(textFile,"Phase C Clipped sample count: %i\n\n", countClippedC);

                        fprintf(textFile,"Standard Deviation of phase A: %lf\n",stdevA);
                        fprintf(textFile,"Standard Deviation of phase B: %lf\n",stdevB);
                        fprintf(textFile,"Standard Deviation of phase C: %lf\n\n",stdevC);

                        fclose(textFile);

                }

                fclose(file);
                free(Log);
            }
        }
        closedir(directory);
        printf("------------------------------------------------------------------------------------------------------------------------\n");
    }
        return 0;
}