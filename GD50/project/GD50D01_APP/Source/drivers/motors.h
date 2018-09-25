#ifndef _MOTORS_H
#define _MOTORS_H

#include "global.h"
/**
  ******************************************************************************
  * @file    motor.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/4/13
  * @brief   This file provides all application of motor and hall, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + hall val calculation.
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
  * @brief  Define hall detection parameter
  *
  */
#define HAL_DIR_UPUP (1)
#define HAL_DIR_DOWN (2)

#define HAL_CNT_MIN (0)
#define HAL_TOP_DFT (2000)
#define HAL_CNT_MAX (20000)

#define HAL_DTY_MAX (2000)

#define HAL_MOD_NOR (0)
#define HAL_MOD_LEARN (0x1)
#define HAL_MOD_NONE (0x2)
/**
  * @brief  Hall learn depth 
  *
  */
#define HAL_LEARN_LEN (8)

/**
  * @brief  Define hall error status 
  *
  */
//#define ERR_HAL_NONE    (0)
//#define ERR_HAL_MAIN    (0xe1)
//#define ERR_HAL_RFER    (0xe2)
//#define ERR_HAL_SUBS    (0xe3)
//#define ERR_HAL_MUTX    (0xe4)

/**
  * @brief  Define hall detection structure 
  *
  */
typedef struct
{
  U08ET main_lst : 2; /* main hall signal last status */
  U08ET main_cur : 2; /* main hall signal current status */

  U08ET rfer_lst : 2; /* main hall signal last status */
  U08ET rfer_cur : 2; /* main hall signal current status */

  U32ET hall_cnt; /* real time motor hall count */
  U32ET duty_val; /* real time hall duty value */
  U32ET duty_cnt; /* duty count to calcute duty value */
  U32ET hall_dir; /* current hall move direction */
#if 0
    U16ET hall_cnt;     /* real time motor hall count */
    U16ET duty_val;     /* real time hall duty value */
    U16ET duty_cnt;     /* duty count to calcute duty value */
    U08ET hall_dir;     /* current hall move direction */
#endif

  U32ET hall_mode; /* can be NORMAL LEARN and NONE*/
  U32ET hall_ratio;

  /* just use when learn mode */
  U16ET hall_learn_cnt[HAL_LEARN_LEN];
  U08ET hall_learn_index;

  U08ET errcnt_ena;  /* main hall error enable or not */
  U16ET mainerr_max; /* main hall error maximum time count */
  U16ET mainerr_cnt; /* main hall error time counter */
  U08ET rfererr_max;
  U08ET rfererr_cnt;

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
VODET motors_ctrl(U08ET ctrl);
VODET motors_pwms(U08ET pwms);
VODET halsig_samp(VODET);
VODET halsig_sdir(U08ET dir);
VODET halsig_scnt(U16ET cnt);
U16ET halsig_gcnt(VODET);
U16ET halsig_duty(VODET);
VODET halsig_mode(U08ET mode);
VODET halsig_ratio(U08ET ratio);
U08ET halsig_gerr(VODET);
ETST_HALL_LEARN hall_learn(VODET);

#endif /* _MOTORS_H */
