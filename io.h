#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_IO_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_IO_H

#include <stdio.h>
#include "waveform.h"

typedef struct {
    double RMS;
    double offset;
    double peak_to_peak;
    int clippedCount;
}PhaseResults;

void removeExtension(char *fileName);
int countRows(FILE *file);
int readingCheck(FILE *file, WaveformSample *Log, int rows);
void notQsort(WaveformSample *Log);
void outputReport(FILE *fp, double rms[3], double p2p[3], double offset[3], double stddev[3],int clipped[3]);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_IO_H