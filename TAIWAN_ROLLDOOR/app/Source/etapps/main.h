#ifndef _MAIN_H
#define _MAIN_H

#include "adcsmp.h"
#include "clkrst.h"
#include "config_get.h"
#include "dflash.h"
#include "global.h"
#include "keysmp.h"
#include "ledbuzlamp.h"
#include "motors.h"
#include "pwm_capture.h"
#include "remote.h"
#include "timwdt.h"
#include "uartct.h"

/**
  ******************************************************************************
  * @file    main.c
  * @author  ETdoor team
  * @version V0.0.6
  * @date    2018/8/15
  * @brief   This file provides all application of roll door opener
  *          functionalities of the application:
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
 * @brief  define system control parameter
 */
#define PRG_MAIN_VER (1)      /* program version number */
#define PWM_VAL_MIN_ALL (35)  /* minimum pwm value in all operation state */
#define PWM_VAL_DFT_REV (120) /* default pwm value in system rebound state */
#define PWM_VAL_MAX_RUN (250) /* maximum pwm value in system run state */
#define PWM_VAL_SLOW_STOP (175)
#define PWM_VAL_MAX_SET (210) /* maximum pwm value in route set state */
#define PWM_VAL_MAX_STP (160) /* maximum pwm value in slow stop state */

#define HAL_CNT_SLOW_STOP (400) /* slow stop distance in hall unit */

#define FORCE_LVL_MAX (8) /* maximum of speed level */

#define AD_MAX (0xf0) /* max over force calculation */

typedef enum {
  DIR_NONE = (0),
  DIR_STADY,
  DIR_OPEN,
  DIR_CLOSE,
} ENUM_DIRECTION;
/**
 * @brief  define system control structure
 */
typedef enum {
  SYS_ST_POWER_ON = 0,
  SYS_ST_READ_EEP,
  SYS_ST_LIMITTOP_SET,
  SYS_ST_LIMITBOT_SET,
  SYS_ST_TRAIN_OPEN,
  SYS_ST_TRAIN_CLOSE,
  SYS_ST_STANDBY,
  SYS_ST_EXCUTE_OPEN,
  SYS_ST_EXCUTE_CLOSE,
  SYS_ST_EXCUTE_BACK,
  SYS_ST_REMOTE_ST,
  SYS_ST_POWER_OFF,
  SYS_ST_ERR_PROCESS,
  SYS_ST_INVALID
} SYS_STATUS,
    SUB_STATUS;

/**
 * @brief  Define system control structure
 */
typedef struct {
  SYS_STATUS sys_sts; /* current system status */
  U08ET sub_sts;      /* current system sub status */
  SYS_STATUS sys_lts; /* last system status */
  U08ET sub_lts;      /* last system sub status */

  U08ET cur_pwm;            /* current pwm value */
  U08ET rem_key[RBUF_SIZE]; /* remote key buff */

  BITET bit_flg; /* bit flag */
  U08ET err_num; /* error number */

  U16ET delay0; /* time delay */
  U16ET delay1; /* time delay */
  U16ET delay2; /* auto stop */

  U16ET learn_up_distance;
  U16ET learn_down_distance;
  U16ET times1_distance;
  U16ET start_position;
  U16ET distance_integral;

  U16ET move_blink;

  U16ET hall_dest; /* destination */

  U08ET force_level;
  U08ET lst_dir;
} SYS_CTRL;

/**
 * @brief  Function declare
 */
VODET sys_pre_init(VODET);
VODET sys_power_on(VODET);
VODET sys_read_eep(VODET);
VODET sys_limittop_set(VODET);
VODET sys_limitbot_set(VODET);
VODET sys_train_open(VODET);
VODET sys_train_close(VODET);
VODET sys_standby(VODET);
VODET sys_excute_open(VODET);
VODET sys_excute_close(VODET);
VODET sys_excute_back(VODET);
VODET sys_remote_set(VODET);
VODET sys_power_off(VODET);
VODET sys_err_process(VODET);

VODET abnor_detectTask(VODET);
#endif /* _MAIN_H */
