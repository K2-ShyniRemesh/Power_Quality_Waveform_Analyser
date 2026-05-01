#include"waveform.h"
#include <math.h>

#define Square(x) ((x)*(x))//preprocessor directive to square values

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

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

range rangeFinder( int rows,WaveformSample *Log, size_t column) {

 //for cases where the lowest value is higher than zero to work,lowest and highest is set to the first value
 double lowest = *(double*)  ((char*)&Log[0]+column);
 double highest = *(double*)  ( (char*)&Log[0]+column);

 //normal for loop to find highest and lowest value
 for (int i = 1; i < rows; i++) {
  double current_val = *(double*) ((char*)&Log[i]+column);
  if (current_val < lowest) lowest = current_val;
  if (current_val > highest) highest = current_val;
 }

 range tempRange={lowest,highest};
 return tempRange;
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