#ifndef _LedBuz_H
#define _LedBuz_H

#include "global.h"
/**
  ******************************************************************************
  * @file    ledbuzlamp.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/1/10
  * @brief   This file provides all application of lamp&buzzer&led , 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + system task to achive period function.
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
#define LED_ID_SET (0)
#define LED_ID_CODE (1)
#define LED_ID_UP (2)
#define LED_ID_DN (3)
#define BUZ_ID_1 (4)
#define LED_ID_MAX (5)

#define LAMP_ID (0x0f)



/**
  * @brief  Define Parameter for lamp
  *
  */
#define LAMP_1MIN (500 * 60)
#define LAMP_BLINK_TIMES  (5)
#define LAMP_BLINK_PERIOD (500)
#define LAMP_BLINK_RATIO  (90)


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
  * @brief  Define LED control structure
  *
  */
typedef struct
{
  U08ET stats;     /* led status */
  
  U08ET cur_times; /* current times */
  U08ET times;     /* light times */
  
  U32ET period;    /* blink period */
  U32ET ratio_cal_val;     /* blink ratio */
  
  U32ET count;     /* current counter */
    
  U32ET after_blink;
} ETST_LAMP;

/**
  * @brief  Function declare
  *
  */
VODET LedBuzLampInital(VODET);
VODET LedBuzLampTask(VODET);
U08ET get_out_state(U08ET id);

VODET LedBuzTaskCtrl(U08ET id
                     , U08ET stats
                     , U08ET times
                     , U16ET period);
VODET Lamp_ctrl(U08ET blink_times
               , U32ET blink_period
               , U08ET blink_ratio
               , U32ET after_blink);




#endif /* _LEDSEG_H */
