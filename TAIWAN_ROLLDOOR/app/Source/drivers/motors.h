#ifndef _MOTORS_H
#define _MOTORS_H

#include "global.h"
#include "pwm_capture.h"
/**
  ******************************************************************************
  * @file    motor.h
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
/**
  * @brief  Define motor control code 
  *
  */
#define MOT_CTS_UPUP (1)
#define MOT_CTS_DOWN (2)
#define MOT_CTS_STOP (3)

/**
  * @brief  Define pwm data for hall sensor 
  *
  */
#define LERAN_CRITICAL_RANGE (0x40)
#define BASELINE_RANGE (0xba)
#define PWM_MIN (0xe0)
#define PWM_MAX (0x1091)
#define PWM_FULL (PWM_MAX - PWM_MIN)

#define HAL_MAX (PWM_MAX * 2)

/**
  * @brief  Define hall detection structure 
  *
  */
typedef struct
{
  U16ET hall_bot;
  U16ET hall_top;
  U08ET cur_direction;

  U08ET errcnt_ena;  /* main hall error enable or not */
  U16ET HallErr_max; /* main hall error maximum time count */
  U16ET HallErr_cnt; /* main hall error time counter */
  U16ET start_cnt;
  U16ET position;
} ETST_HALL;

/**
  * @brief  just use when learn mode
  * @param  NONE
  * @retval NONE
  *
  */
typedef struct
{
  U08ET ratio;
  U08ET skew;
} ETST_HALL_LEARN;

/**
  * @brief  Function declare 
  *
  */
VODET motors_init(VODET);
VODET motors_ctrl(U08ET ctrl, ENUM_MOTOR_INSTALL_nMODE(*lr_get)(VODET));
VODET motors_pwms(U08ET pwms);
U16ET position_get(ENUM_MOTOR_INSTALL_nMODE(*lr_get)(VODET), ENUM_MOTOR_IFLEARN_nMODE(*ifleran_mode_get)(VODET));
VODET halset_bot(U16ET pos);
VODET halset_top(U16ET pos);
U16ET hall_overflow_cnt(void);
U08ET hall_move_err(VODET);
#endif /* _MOTORS_H */
