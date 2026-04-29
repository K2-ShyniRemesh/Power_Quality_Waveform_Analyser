#include"waveform.h"
#include <math.h>

#define Square(x) ((x)*(x))//preprocessor directive to square values

double compute_rms(const WaveformSample *Log, int rows,int phase) {
 double currentValue=0;

 for (int i = 0; i < rows; i++){
  currentValue+= Square(Log[i].phase_voltage[phase]);
 }
 return sqrt(currentValue/(double)rows);

}

double compute_peak_to_peak(const WaveformSample *Log,int rows,int phase) {
 //for cases where the lowest value is higher than zero to work,lowest and highest is set to the first value
 double lowest=Log[0].phase_voltage[phase],highest=Log[0].phase_voltage[phase];

 for (int i = 0; i < rows; i++) {
  if (Log[i].phase_voltage[phase]<lowest) {
   lowest = Log[i].phase_voltage[phase];
  }
  if (Log[i].phase_voltage[phase]>highest) {
   highest = Log[i].phase_voltage[phase];
  }
 }
 return highest-lowest;
}

 double compute_dc_offset(const WaveformSample *Log,int rows,int phase) {
  double sumOfVoltage=0;

  for (int i = 0; i < rows; i++) {
  sumOfVoltage+=Log[i].phase_voltage[phase];
  }
  double dc_offset=(1/(double)rows)*sumOfVoltage;

 return dc_offset;
}

int count_clipped(const WaveformSample *Log,int rows,int phase) {
 int count=0;
 double limit=324.9;

 for (int i = 0; i < rows; i++) {
  if (fabs(Log[i].phase_voltage[phase])>=limit) {
   count++;
  }
 }
 return count;
}


double compute_std_dev(const WaveformSample *Log, int rows,int phase) {
 double sumOfVoltage=0;

 for (int i = 0; i < rows; i++) {
  sumOfVoltage+=Log[i].phase_voltage[phase];
 }

 double mean=sumOfVoltage/(double)rows;
 double temp=0;
 double squaredSum=0;

 for (int i = 0; i < rows; i++) {
  temp=Square(Log[i].phase_voltage[phase]-mean);
  squaredSum+=temp;
 }
 double variance=squaredSum/((double)rows-1);
 return sqrt(variance);
}