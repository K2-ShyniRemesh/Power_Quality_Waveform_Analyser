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

int countRows(FILE *file);
int readingCheck(FILE *file, WaveformSample *Log, int rows);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_IO_H