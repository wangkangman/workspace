#ifndef _LedBuz_H
#define _LedBuz_H

#include "global.h"
/**
  ******************************************************************************
  * @file    ledbuzlamp.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/8/10
  * @brief   This file provides all application of lamp&buzzer&led , 
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
  * @brief  Define LED status
  *
  */
#define OUT_ST_OFF (0)
#define OUT_ST_ON (1)

/**
  * @brief  Define LED ID
  *
  */
#define LED_ID_REMOTE (0)
#define LED_ID_MAX (1)

/**
  * @brief  Define LED control structure
  *
  */
typedef struct
{
  U08ET stats;     /* led status */
  U08ET cur_times; /* current times */
  U08ET times;     /* light times */
  U32ET period;    /* control period */
  U32ET count;     /* current counter */
} ETST_DIGIT_OUT;

/**
  * @brief  Function declare
  *
  */
VODET LedInital(VODET);
VODET LedTask(VODET);
U08ET get_out_state(U08ET id);
VODET LedTaskCtrl(U08ET id, U08ET stats, U08ET times, U16ET period);

#endif /* _LEDSEG_H */
