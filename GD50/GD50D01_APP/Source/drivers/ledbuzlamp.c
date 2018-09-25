#include "regdef.h"
#include "ledbuzlamp.h"
/**
  ******************************************************************************
  * @file    ledbuzlamp.c
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
  * @brief  Local variable define
  */
static ETST_DIGIT_OUT et_DigitOuts[LED_ID_MAX];
static ETST_LAMP et_lamp;

/**
  * @brief  Local func
  */
static VODET LedBuzLmapCtrl(U08ET id, U08ET stats);
static VODET LedBuzTask(VODET);
static VODET LampTask(VODET);

/**
  * @brief  LedBuz Initalize;
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET LedBuzLampInital(VODET) {
  U08ET index;
  LedBuzLmapCtrl(LED_ID_SET, OUT_ST_OFF);
  LedBuzLmapCtrl(LED_ID_CODE, OUT_ST_OFF);
  LedBuzLmapCtrl(LED_ID_UP, OUT_ST_OFF);
  LedBuzLmapCtrl(LED_ID_DN, OUT_ST_OFF);
  LedBuzLmapCtrl(LAMP_ID, OUT_ST_OFF);
  LedBuzLmapCtrl(BUZ_ID_1, OUT_ST_OFF);
  /* LED_CODE <--> P4.1 */
  GPIO_P4_PMD |= ((U32ET)1 << 2);
  GPIO_P4_PMD &= ~((U32ET)1 << 3);
  /* LED_SET  <--> P0.2 */
  GPIO_P0_PMD |= ((U32ET)1 << 4);
  GPIO_P0_PMD &= ~((U32ET)1 << 5);
  /* LED_DOWN <--> P0.1 */
  GPIO_P0_PMD |= ((U32ET)1 << 2);
  GPIO_P0_PMD &= ~((U32ET)1 << 3);
  /* LED_UP   <--> P0.0 */
  GPIO_P0_PMD |= ((U32ET)1 << 0);
  GPIO_P0_PMD &= ~((U32ET)1 << 1);
  /* LAMP     <--> P2.4 */
  GPIO_P2_PMD |= ((U32ET)1 << 8);
  GPIO_P2_PMD &= ~((U32ET)1 << 9);
  /* PWM2  <--> P4.2 */
  GCR_P4MFP |= ((U32ET)1 << 2);
  GCR_P4MFP &= ~((U32ET)1 << 10);
  /* set PWM2 clock divider to 1  [10:8]=100 */
  PWMA_CSR &= ~((U32ET)0x7 << 8);
  PWMA_CSR |= ((U32ET)0x4 << 8);
  /* set PWM2 to auto reload mode and disable inverter */
  PWMA_PCR &= ~((U32ET)0x7 << 17);
  PWMA_PCR |= ((U32ET)0x4 << 17);
  /* set PWM2 cycle count to 769  fre = 2.6khz */
  PWMA_CNR2 = 769;
  /* set PWM2 default duty to 0 */
  PWMA_CMR2 = 384;
  /* PWM2 irq disable */
  PWMA_PIER &= ~((U32ET)0x1 << 2);
  PWMA_PIER &= ~((U32ET)0x1 << 10);
  PWMA_PIIR &= ~((U32ET)0x1 << 2);
  PWMA_PIIR &= ~((U32ET)0x1 << 10);
  /* initialize led control structure */
  for (index = 0; index < LED_ID_MAX; index++) {
    et_DigitOuts[index].stats = OUT_ST_OFF;
    et_DigitOuts[index].times = 0;
    et_DigitOuts[index].cur_times = 0;
    et_DigitOuts[index].period = 0;
    et_DigitOuts[index].count = 0;
  }
  /* start PWM2 */
  PWMA_PCR |= (1 << 16);
  /* disable PWM2 output */
  PWMA_POE &= ~((U32ET)1 << 2);

  et_lamp.times = 0;
  et_lamp.after_blink = 0;
}

/**
  * @brief  Led$Buzzer$Lamp system task;
  * @param  NONE
  * @retval NONE
  *
  */
VODET LedBuzLampTask(VODET) {
  LedBuzTask();
  LampTask();
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
VODET LedBuzLmapCtrl(U08ET id, U08ET stats) {
  switch (id) {
  case LED_ID_SET: /* Active low */
    PORT_SET(ET_PORT0, 2, !stats);
    break;
  case LED_ID_CODE: /* Active low */
    PORT_SET(ET_PORT4, 1, !stats);
    break;
  case LED_ID_UP: /* Active low */
    PORT_SET(ET_PORT0, 0, !stats);
    break;
  case LED_ID_DN: /* Active low */
    PORT_SET(ET_PORT0, 1, !stats);
    break;
  case LAMP_ID: /* Active high */
    PORT_SET(ET_PORT2, 4, stats);
    //         PORT_SET(ET_PORT2, 4, 1);
    break;
  case BUZ_ID_1:
    if (stats)
      PWMA_POE |= ((U32ET)1 << 2);
    else
      PWMA_POE &= ~((U32ET)1 << 2);
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
VODET LedBuzTaskCtrl(U08ET id, U08ET stats, U08ET times, U16ET period) {
  LedBuzLmapCtrl(id, stats);
  et_DigitOuts[id].times = times;
  et_DigitOuts[id].cur_times = times;
  et_DigitOuts[id].period = period;
  et_DigitOuts[id].count = 0;
  et_DigitOuts[id].stats = !stats;
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
  case LED_ID_SET: /* Active low */
    return !PORT_READ(ET_PORT0, 2);
  case LED_ID_CODE: /* Active low */
    return !PORT_READ(ET_PORT4, 1);
  case LED_ID_UP: /* Active low */
    return !PORT_READ(ET_PORT0, 0);
  case LED_ID_DN: /* Active low */
    return !PORT_READ(ET_PORT0, 1);
  case LAMP_ID: /* Active high */
    return et_lamp.stats;
  default:
    return 0;
  }
}
/**
  * @brief  Lamp ctrl;
  * @param
  *   blink_times : Lamp blink times
  *   blink_period: Lamp blink period
  *   blink_ratio : Lamp blink light ratio
  *   after_blink : light after blink
  * @retval
  *   status;
  *
  */
VODET Lamp_ctrl(U08ET blink_times
                , U32ET blink_period
                , U08ET blink_ratio
                , U32ET after_blink) {
  if (0 == blink_times) {
    if (0 != after_blink) {
      et_lamp.times = 1;
      et_lamp.period = after_blink;
      et_lamp.ratio_cal_val = et_lamp.period;
      et_lamp.after_blink = 0;
      et_lamp.count = 0;
    } else {
      et_lamp.times = 0;
      et_lamp.after_blink = 0;
    }
    return;
  }

  if (0 == blink_period || 0 == blink_ratio || blink_ratio > 100)
    return;

  et_lamp.period = blink_period;
  et_lamp.times = blink_times;
  et_lamp.ratio_cal_val = (et_lamp.period * blink_ratio) / 100;
  et_lamp.count = 0;
  et_lamp.after_blink = after_blink;
  LedBuzLmapCtrl(LAMP_ID, OUT_ST_ON);
}

/**
  * @brief  Lamp system task;
  * @param  NONE
  * @retval NONE
  *
  */
/*static*/
VODET LampTask(VODET) {
  if (0 == et_lamp.times) {
    LedBuzLmapCtrl(LAMP_ID, OUT_ST_OFF);
    et_lamp.stats = OUT_ST_OFF;
    return;
  }

  else {
    et_lamp.stats = OUT_ST_ON;
    et_lamp.count++;
    if (et_lamp.count < et_lamp.ratio_cal_val) {
      LedBuzLmapCtrl(LAMP_ID, OUT_ST_ON);
    } else
      LedBuzLmapCtrl(LAMP_ID, OUT_ST_OFF);

    if (et_lamp.count >= et_lamp.period) {
      et_lamp.count = 0;
      et_lamp.times--;
    }

    if (0 == et_lamp.times) {
      if (0 != et_lamp.after_blink) {
        et_lamp.times = 1;
        et_lamp.period = et_lamp.after_blink;
        et_lamp.ratio_cal_val = et_lamp.period;
        et_lamp.after_blink = 0;
        et_lamp.count = 0;
      } else {
        et_lamp.times = 0;
        et_lamp.after_blink = 0;
      }
    }
  }
}

/**
  * @brief  LedBuz system task;
  * @param  NONE
  * @retval NONE
  *
  */
/*static*/
VODET LedBuzTask(VODET) {
  U08ET index;
  /* led control */
  for (index = 0; index < LED_ID_MAX; index++) {
    /* segment control */
    if (0 != et_DigitOuts[index].period)
    {
      et_DigitOuts[index].count++;
      if (et_DigitOuts[index].count == et_DigitOuts[index].period)
      {
        if (OUT_ST_ON == et_DigitOuts[index].stats)
        {
          LedBuzLmapCtrl(index, OUT_ST_ON);
          et_DigitOuts[index].stats = OUT_ST_OFF;
        }
        else
        {
          if (0 != et_DigitOuts[index].times)
          {
            et_DigitOuts[index].cur_times--;
            if (0 == et_DigitOuts[index].cur_times)
            {
              et_DigitOuts[index].times = 0;
              et_DigitOuts[index].cur_times = 0;
              et_DigitOuts[index].period = 0;
              et_DigitOuts[index].count = 0;
              et_DigitOuts[index].stats = OUT_ST_OFF;
            }
          }
          LedBuzLmapCtrl(index, OUT_ST_OFF);
          et_DigitOuts[index].stats = OUT_ST_ON;
        }
        et_DigitOuts[index].count = 0;
      }
    }
  }
}
