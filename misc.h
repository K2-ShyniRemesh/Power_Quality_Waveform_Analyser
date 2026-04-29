#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_MISC_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_MISC_H

#include "waveform.h"
#include "io.h"

int countRows(FILE *file);
void removeExtension(char *fileName);
void notQsort(WaveformSample *Log);
void processCSV(char *filePath);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_MISC_H