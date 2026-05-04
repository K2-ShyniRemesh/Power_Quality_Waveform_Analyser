#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_IO_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_IO_H

#include <stdio.h>
#include "waveform.h"
#include <stdint.h>



typedef struct {
    double rms[3],peak2peak[3],offset[3],stddev[3];
    int clippedCount[3];
}resultSample;



int readingCSV(FILE *file, WaveformSample *Log, int rows);

void saveSorted(const char *filePath, WaveformSample *Log,int rows);

void outputReport(FILE *fp,resultSample theResults,const uint8_t phaseHealth[3],const range *frequencyRange,const range *thdRange,const range *powerFactorRange);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_IO_H