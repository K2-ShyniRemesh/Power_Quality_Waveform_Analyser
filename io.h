#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_IO_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_IO_H

#include <stdio.h>
#include"waveform.h"

int readingCheck(FILE *file, WaveformSample *Log);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_IO_H