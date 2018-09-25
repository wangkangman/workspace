#include "regdef.h"
#include "motors.h"
#include "stdio.h"
/**
  ******************************************************************************
  * @file    motor.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/8/13
  * @brief   This file provides all application of motor and hall, 
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
/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_HALL et_hall;

/* local funtion */
static U16ET get_filter_val(U16ET *pval, U16ET length);
static VODET hall_move_err_detect_task(VODET);

/*-----------------------------------------------------------------*/
/* FUNC : Motor control initialize                                 */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET motors_init(VODET) {
  /* set control port to stop level as default */
  PORT_SET(ET_PORT3, 6, LOW);
  PORT_SET(ET_PORT2, 1, LOW);
  /* PWM3  <--> P4.3 */
  GCR_P4MFP |= ((U32ET)1 << 3);
  GCR_P4MFP &= ~((U32ET)1 << 11);
  /* select HCLK as PWM23 clock source */
  CLK_SEL1 |= ((U32ET)2 << 30);
  /* enable PWM23 clock */
  CLK_APB |= ((U32ET)1 << 21);
  /* set PWM23 clock pre divider to 11 */
  PWMA_PPR &= 0x00ff;
  PWMA_PPR |= ((U32ET)10 << 8);
  /* set PWM3 clock divider to 1  [14:12]=100 */
  PWMA_CSR &= ~((U32ET)0x7 << 12);
  PWMA_CSR |= ((U32ET)0x4 << 12);
  /* set PWM3 to auto reload mode and disable inverter */
  PWMA_PCR &= ~((U32ET)0x7 << 25);
  PWMA_PCR |= ((U32ET)0x4 << 25);  
  /* set PWM3 cycle count to 250 */
  PWMA_CNR3 = 250;
  /* set PWM3 default duty to 0 */
  PWMA_CMR3 = 0;
  /* PWM3 irq disable */
  PWMA_PIER &= ~((U32ET)0x1 << 3);
  PWMA_PIER &= ~((U32ET)0x1 << 11);
  PWMA_PIIR &= ~((U32ET)0x1 << 3);
  PWMA_PIIR &= ~((U32ET)0x1 << 11);
  /* motor control M00+ <--> P3.6 */
  GPIO_P3_PMD |= ((U32ET)1 << 12);
  GPIO_P3_PMD &= ~((U32ET)1 << 13);
  /* motor control M00- <--> P2.1 */
  GPIO_P2_PMD |= ((U32ET)1 << 2);
  GPIO_P2_PMD &= ~((U32ET)1 << 3);

  /* start PWM3 */
  PWMA_PCR |= (1 << 24);
  /* enable PWM3 output */
  PWMA_POE |= ((U32ET)1 << 3);
	
  et_hall.HallErr_max = 200;

	pwm_capture_init();

}

/**
  * @brief  Control motor up or down and its speed 
  * @param  
  *    ctrl : motor control code
  * @retval NONE
  *
  */
VODET motors_ctrl(U08ET ctrl, ENUM_MOTOR_INSTALL_nMODE(*lr_get)(VODET)){
  /* lift or right convert */
  et_hall.cur_direction = ctrl;

  /* lift or right convert */
  if (INSTALL_RIGHT == lr_get()) {
    if (MOT_CTS_DOWN == ctrl) ctrl = MOT_CTS_UPUP;
    else if (MOT_CTS_UPUP == ctrl) ctrl = MOT_CTS_DOWN;
  }

  switch (ctrl) {
  case MOT_CTS_DOWN: //M+ = 0; M- = 1
    PORT_SET(ET_PORT3, 6, LOW);
    PORT_SET(ET_PORT2, 1, HIGH);
    break;
  case MOT_CTS_UPUP:
    PORT_SET(ET_PORT3, 6, HIGH);
    PORT_SET(ET_PORT2, 1, LOW);
    break;
  case MOT_CTS_STOP:
    PORT_SET(ET_PORT3, 6, LOW);
    PORT_SET(ET_PORT2, 1, LOW);
    break;
  default:
    break;
  }
}

/**
  * @brief  Control motor pwm signal 
  * @param  
  *    pwms : motor pwm duty 
  * @retval NONE
  *
  */
VODET motors_pwms(U08ET pwms) {  
  pwms = (pwms > 250) ? 250 : pwms;  
  if (0 == pwms) {
    et_hall.errcnt_ena = ET_FALSE;
    et_hall.HallErr_cnt = 0;
    et_hall.start_cnt = 0;
  } else et_hall.errcnt_ena = ET_TRUE;
  PWMA_CMR3 = pwms;
}

/**
  * @brief  get pwm_capture over flow counter
  * @param 
  *   NONE
  * @retval 
  *   over flow counter
  *
  */
U16ET hall_overflow_cnt(void) { return overflow_cnt_get();}

/**
  * @brief  hal position val get
  * @param 
  *   <lr_get> get intall mode of motor
  *   <ifleran_mode_get> get if the motor is in learn mode
  * @retval current position
  *
  */
U16ET position_get(U08ET(*lr_get)(VODET), U08ET(*ifleran_mode_get)(VODET)){
  U08ET folw_critical_cnt, upper_critical_cnt, index;
  U16ET buf[PWM_FILTER_NUM], res, threshold;
  position_buf_get(buf);
  hall_move_err_detect_task();

  for (folw_critical_cnt = 0, upper_critical_cnt = 0,index = 0; index < PWM_FILTER_NUM; index++) {
    if (buf[index] < PWM_MIN + LERAN_CRITICAL_RANGE) folw_critical_cnt++;
    if (buf[index] > PWM_MAX - LERAN_CRITICAL_RANGE) upper_critical_cnt++;
  }

  if (folw_critical_cnt != 0 && upper_critical_cnt != 0) res = PWM_MIN;
  else {
    res = get_filter_val(buf, PWM_FILTER_NUM);
    if (res < PWM_MIN) res = PWM_MIN;
    if (res > PWM_MAX) res = PWM_MAX;
  }

  if (res > PWM_MIN) res -= PWM_MIN;
  else res = 0;

  if (INSTALL_RIGHT == lr_get()) res = PWM_FULL - res;

  if (MOTOR_NORMAL == ifleran_mode_get()) { 
    if (et_hall.hall_bot > PWM_FULL) {
      threshold = et_hall.hall_bot - PWM_FULL + BASELINE_RANGE;
      if (res < threshold) res += PWM_FULL;
    } else {
      threshold = PWM_FULL - et_hall.hall_bot;
      if (threshold < BASELINE_RANGE) {
        threshold = BASELINE_RANGE - threshold;
        if (res < threshold) res += PWM_FULL;
      }
    } 
  }

  et_hall.position = res;
  return res;
}

/**
  * @brief  set position value<bottom side>
  * @param 
  *   pos : the position which should be set
  * @retval 
  *   NONE
  *
  */
VODET halset_bot(U16ET pos){et_hall.hall_bot = pos;}

/**
  * @brief  set position value<top side>
  * @param 
  *   pos : the position which should be set
  * @retval 
  *   NONE
  *
  */
VODET halset_top(U16ET pos){et_hall.hall_top = pos;}
 
/**
  * @brief  get filter val of buf 
  * @param  
  *    pval : piont to buf
  *    length : length of the current buf
  * @retval 
  *    filter val
  *
  * @details delete max val & min val, then get average
  * 
  */
U16ET get_filter_val(U16ET *pval, U16ET length){
  U32ET sum; U16ET min, max; U08ET index;  
  for (sum = 0, min = 0xffff, max = 0x0, index = 0; index < length; index++){
    min = pval[index] < min ? pval[index] : min;
    max = pval[index] > max ? pval[index] : max;
    sum += pval[index];
  }
  return (U16ET)((sum - max - min)/(length - 2));
}

/**
  * @brief  hall move error detect
  * @param  
  *    NONE
  * @retval 
  *    NONE
  *
  * @details called by get position
  * 
  */
static VODET hall_move_err_detect_task(VODET){
  static U16ET lst_position;
  U16ET cur_position;

  if (et_hall.errcnt_ena == ET_TRUE) {
    if (0 == et_hall.start_cnt) {
      cur_position = et_hall.position;
      lst_position = et_hall.position;
    }

    if (et_hall.start_cnt < 1500) et_hall.start_cnt++;
    else et_hall.HallErr_cnt++;

    cur_position = et_hall.position;

    if (((MOT_CTS_UPUP == et_hall.cur_direction) && (lst_position > cur_position) && (lst_position - cur_position > 10))
       || ((MOT_CTS_DOWN == et_hall.cur_direction) && (lst_position < cur_position) && (cur_position - lst_position > 10))) {
      et_hall.HallErr_cnt = 0;
      lst_position = cur_position;
    }
  }
}

/**
  * @brief  hall move error get
  * @param  
  *    NONE
  * @retval 
  *    NONE
  *
  * @details called by application<abnormal detect>
  * 
  */
U08ET hall_move_err(VODET){
  U08ET res;
  if (et_hall.HallErr_cnt > et_hall.HallErr_max) res = ET_TRUE;
  else res = ET_FALSE;
  return res;
}

