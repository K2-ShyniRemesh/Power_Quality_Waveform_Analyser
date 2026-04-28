#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H

typedef struct {
    double timestamp;
    double phase_voltage[3];
    double line_current,frequency,power_factor,thd_percent;
} WaveformSample;


double compute_rms(const WaveformSample *Log, int n,int phase);

double compute_peak_to_peak(const WaveformSample *Log,int n,int phase);

double compute_dc_offset(const WaveformSample *Log,int n,int phase);

int count_clipped(const WaveformSample *Log,int rows,int phase,double limit);

double check_compliance(double rms,double nominal);

double compute_std_dev(const WaveformSample *Log,int rows,int phase);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H