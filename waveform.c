#include"waveform.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double compute_rms(const WaveformSample *Log, int rows,int phase) {
 double sumSq=0;

 for (int i = 0; i < rows; i++){

  double currentValue=0;
  currentValue = Log[i].phase_voltage[phase];
  sumSq += currentValue * currentValue;
 }
 return sqrt(sumSq/rows);

}

double compute_peak_to_peak(const WaveformSample *Log,int rows,int phase,int *peakindex) {
 double lowest=0,highest=0;
 *peakindex=0;

 for (int i = 0; i < rows; i++) {
  if (Log[i].phase_voltage[phase]<lowest) {
   lowest = Log[i].phase_voltage[phase];
  }
  if (Log[i].phase_voltage[phase]>highest) {
   highest = Log[i].phase_voltage[phase];
   *peakindex=i;
  }
 }
 return highest-lowest;
}

 double compute_dc_offset(const WaveformSample *Log,int rows,int phase) {
  double sumOfVoltage=0;
 for (int i = 0; i < rows; i++) {
  sumOfVoltage+=Log[i].phase_voltage[phase];
 }
  double dc_offset=(1/rows)*sumOfVoltage;

 return dc_offset;
}

/* count_clipped(samples,n,limit){}

 check_compliance(rms,nominal){}

 compute_std_dev(samples,n){}*/