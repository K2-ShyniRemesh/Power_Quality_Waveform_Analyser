#include"waveform.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double compute_rms(const WaveformSample *Log, int rows,int phase) {
 double sumSq=0;

 for (int i = 0; i < rows; i++){

  double currentValue=0;
  if (phase==0){ currentValue = Log[i].phase_A_voltage;}
  else if (phase==1){ currentValue = Log[i].phase_B_voltage;}
  else if (phase==2){ currentValue = Log[i].phase_C_voltage;}
  sumSq += currentValue * currentValue;
 }
 return sqrt(sumSq/rows);

}

/*compute_peaks(samples,n){}

compute_dc_offset(samples,n){}

count_clipped(samples,n,limit){}

check_compliance(rms,nominal){}

compute_std_dev(samples,n){}*/