#ifndef POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H
#define POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H


typedef struct {
    double timestamp;
    double phase_A_voltage,phase_B_voltage,phase_C_voltage;
    double line_current,frequency,power_factor,thd_percent;
} WaveformSample;


double compute_rms(const WaveformSample *Log, int n,int phase);

double compute_peaks(const WaveformSample *Log,int n);

double compute_dc_offset(const WaveformSample *Log,int n);

int count_clipped(const WaveformSample *Log,int n,int limit);

double check_compliance(rms,nominal);

double compute_std_dev(const WaveformSample *Log,int n);

#endif //POWER_QUALITY_WAVEFORM_ANALYSER_WAVEFORM_H