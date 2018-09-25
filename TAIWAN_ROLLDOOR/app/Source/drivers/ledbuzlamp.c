#include "regdef.h"
#include "ledbuzlamp.h"
/**
  ******************************************************************************
  * @file    ledbuzlamp.c
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
  * @brief  Local variable define
  */
static ETST_DIGIT_OUT et_DigitOuts[LED_ID_MAX];

/**
  * @brief  Local func
  */
static VODET LedCtrl(U08ET id, U08ET stats);

/**
  * @brief  LedBuz Initalize;
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET LedInital(VODET) {
  U08ET index;
  LedCtrl(LED_ID_REMOTE, OUT_ST_OFF);
  /* LED_ID_REMOTE <--> P0.3 */
  GPIO_P0_PMD |= ((U32ET)1 << 6);
  GPIO_P0_PMD &= ~((U32ET)1 << 7);
  /* initialize led control structure */
  for (index = 0; index < LED_ID_MAX; index++) {
    et_DigitOuts[index].stats = OUT_ST_OFF;
    et_DigitOuts[index].times = 0;
    et_DigitOuts[index].cur_times = 0;
    et_DigitOuts[index].period = 0;
    et_DigitOuts[index].count = 0;
  }
}

/**
  * @brief  LED low level control, can set led on/off directly;
  * @param
  *   id    : compnent ID
  *   stats : LED status
  * @retval NONE
  *
  */
/*static*/
VODET LedCtrl(U08ET id, U08ET stats) {
  switch (id) {
  case LED_ID_REMOTE: /* Active low */
    PORT_SET(ET_PORT0, 3, !stats);
    break;
  default:
    break;
  }
}

/**
  * @brief  LED low level control, can set led on/off directly;
  * @param
  *   id      : ID of slave compnent can be lamp, led or buz
  *   ledmode : led light mode
  *   stats   : LED first status
  *   times   : LED light times    most : 64
  *   period  : LED period  *
  * @retval NONE
  *
  *
  * if times==0 or period==0, led or buz light or ring infinitely times;
  * else it light or ring <times> times;
  * <stats> iS the first status;
  *
  */
VODET LedTaskCtrl(U08ET id, U08ET stats, U08ET times, U16ET period) {
  if (id < LED_ID_MAX){
    LedCtrl(id, stats);
    et_DigitOuts[id].times = times;
    et_DigitOuts[id].cur_times = times;
    et_DigitOuts[id].period = period / 2;
    et_DigitOuts[id].count = 0;
    et_DigitOuts[id].stats = !stats;
  }
}

/**
  * @brief  get current status;
  * @param
  *   id    : compnent ID
  * @retval
  *   status;
  *
  */
/*static*/
U08ET get_out_state(U08ET id) {
  switch (id) {
  case LED_ID_REMOTE: /* Active low */
    return !PORT_READ(ET_PORT0, 3);
  default:
    return 0;
  }
}

/**
  * @brief  LedBuz system task;
  * @param  NONE
  * @retval NONE
  *
  */
/*static*/
VODET LedTask(VODET) {
  U08ET index;
  /* led control */
  for (index = 0; index < LED_ID_MAX; index++) {
    /* segment control */
    if (0 != et_DigitOuts[index].period) {
      et_DigitOuts[index].count++;
      if (et_DigitOuts[index].count == et_DigitOuts[index].period) {
        if (OUT_ST_ON == et_DigitOuts[index].stats) {
          LedCtrl(index, OUT_ST_ON);
          et_DigitOuts[index].stats = OUT_ST_OFF;
        } else {
          if (0 != et_DigitOuts[index].times) {
            et_DigitOuts[index].cur_times--;
            if (0 == et_DigitOuts[index].cur_times) {
              et_DigitOuts[index].times = 0;
              et_DigitOuts[index].cur_times = 0;
              et_DigitOuts[index].period = 0;
              et_DigitOuts[index].count = 0;
              et_DigitOuts[index].stats = OUT_ST_OFF;
            }
          }
          LedCtrl(index, OUT_ST_OFF);
          et_DigitOuts[index].stats = OUT_ST_ON;
        }
        et_DigitOuts[index].count = 0;
      }
    }
  }
}
