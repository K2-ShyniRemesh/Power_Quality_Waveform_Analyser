#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_IO_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_IO_H

#include <stdio.h>
#include "waveform.h"

int readingCheck(FILE *file, WaveformSample *Log, int rows);

void saveSorted(const char *filePath, WaveformSample *Log,int rows);

void outputReport(FILE *fp, double rms[3], double p2p[3], double offset[3], double stddev[3],int clipped[3]);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_IO_H