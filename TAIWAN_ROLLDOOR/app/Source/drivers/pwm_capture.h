#ifndef _PWM_CAPTURE_H
#define _PWM_CAPTURE_H

#include "global.h"
/**
  ******************************************************************************
  * @file    timwdt.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/8/11
  * @brief   This file provides all application of pwm capture,
  *          functionalities of the pwm capture:
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
#define PWM_FILTER_NUM (10)


typedef struct {
   U16ET pulse[PWM_FILTER_NUM];	
	 U16ET cycle[PWM_FILTER_NUM];
   U08ET step;
   U16ET base_val;
	 U16ET overflow_cnt;
}ETST_POSITION;

void pwm_capture_init(void);
VODET position_buf_get(U16ET *pbuf);
VODET cycle_buf_get(U16ET *pbuf);
U16ET overflow_cnt_get(void);
#endif /* _TIMWDT_H */
