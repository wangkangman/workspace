#ifndef _ADCSMP_H
#define _ADCSMP_H

#include "global.h"
/**
  ******************************************************************************
  * @file    adcsmp.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/1/10
  * @brief   This file provides all application of clock, 
  *          functionalities of the inital and listen for uart:
  *
 ===============================================================================
  
  ******************************************************************************
  * @attention
  *
  * &copy; COPYRIGHT 2018 ETdoor
  *
  *
  ******************************************************************************
 ===============================================================================
  
  ******************************************************************************
  * @verlog
  *
  * V0.0.0: original version;
  *
  *
  ******************************************************************************
  */
/**
  * @brief  Define ADC control parameter  
  *
  */
#define ADC_FILT_MAX (5)

#define ADC_CH_MOT (0)
#define ADC_CH_VOL (1)
#define ADC_CH_MAX (2)

/*-----------------------------------------------------------------*/
/* Define ADC control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
  U16ET result;
  U16ET value[ADC_FILT_MAX];
  U16ET ready;
  U16ET index;
} ETST_ADCS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET adcsmp_init(VODET);
VODET adcsmp_task(VODET);
U08ET getadc_data(U08ET chan);

#endif /* _ADCSMP_H */
