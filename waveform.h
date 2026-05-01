#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#include <stddef.h>


typedef struct {
    char Headings[8][51];//2d array to store 8 strings of max 30 chars(or 8 instances)
    double timestamp;
    double phase_voltage[3];
    double line_current,frequency,power_factor,thd_percent;
} WaveformSample;


typedef struct {
    double lowest,highest;
}range;

double compute_rms(const WaveformSample *Log, int n,int phase);
double compute_peak_to_peak(const WaveformSample *Log,int n,int phase);
double compute_dc_offset(const WaveformSample *Log,int n,int phase);
int count_clipped(const WaveformSample *Log,int rows,int phase);

range rangeFinder(int rows, WaveformSample *Log, size_t column);

double compute_std_dev(const WaveformSample *Log,int rows,int phase);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H