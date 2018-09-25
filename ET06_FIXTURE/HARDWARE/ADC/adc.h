#ifndef __ADC_H
#define __ADC_H

#include "sys.h"

#define ADC_FILTER_NUM (5)

#define ADC1_NUM (4) 
#define ADC3_NUM (6) 

typedef enum ADCn{
  ADC1_CURRENT = 0,  // 16 6 
  ADC2_reserve,  // 16 6 
  ADC3_reserve,  // 16 6 
  ADC4_reserve,  // 16 6 
  VOT1_MAIN,   // 11
  VOT2_BAT,    // 11
  VOT3_24V,    // 11 
  VOT4_LAMP,   // 11
  VOT5_12V,
  VOT6_5V,     // 4.03
  ADC_CHANNEL_NUM
}ADCn_Type;

typedef struct{
  float current;
  float vot_main;
  float vot_bat;
  float vot_12v;
  float vot_lamp;
  float vot_5v;
}ADCn_FLOAT;



typedef struct {
  void(*init)(void);
  void(*get)(ADCn_FLOAT* );
}ADC_API;


extern ADC_API adc;

#endif

