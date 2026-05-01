#include"waveform.h"
#include <math.h>

#define Square(x) ((x)*(x))//preprocessor directive to square values


double compute_rms(const WaveformSample *Log, int rows,int phase){
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

 for (int i = 0; i < rows; i++) {
  if (fabs(Log[i].phase_voltage[phase])>=324.9) {
   count++;
  }
 }
 return count;
}

void rangeFinder(WaveformSample *Log,int rows,char column){
 double lowest=Log[0].frequency,highest=Log[0].frequency;
 range frequencyRange={frequencyRange};

 for (int i = 0; i < rows; i++) {
  if (Log[i].frequency<lowest) lowest = Log[i].frequency;
  if (Log[i].frequency>highest) highest = Log[i].frequency;
 }
}

void powerFactor(WaveformSample *Log,int rows) {
 double lowest=Log[0].power_factor,highest=Log[0].power_factor;

 for (int i = 0; i < rows; i++) {
  if (Log[i].power_factor<lowest) lowest = Log[i].power_factor;

  if (Log[i].power_factor>highest) highest = Log[i].power_factor;
 }
}

void THD(WaveformSample *Log,int rows) {
 double lowest=Log[0].thd_percent,highest=Log[0].thd_percent;

 for (int i = 0; i < rows; i++) {
  if (Log[i].thd_percent<lowest) lowest = Log[i].thd_percent;

  if (Log[i].thd_percent>highest) highest = Log[i].thd_percent;
}
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