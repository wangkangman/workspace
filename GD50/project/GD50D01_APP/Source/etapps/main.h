#ifndef _MAIN_H
#define _MAIN_H

#include "global.h"
#include "clkrst.h"
#include "motors.h"
#include "remote.h"
#include "dflash.h"
#include "ledbuzlamp.h"
#include "adcsmp.h"
#include "keysmp.h"
#include "timwdt.h"
#include "uartct.h"
#include "wifi.h"
/**
  ******************************************************************************
  * @file    main.c
  * @author  ETdoor team
  * @version V0.0.6
  * @date    2018/1/17
  * @brief   This file provides all application of GD50 garage door opener 
  *          functionalities of the inital and listen for wifi adapter:
  *           + Interface seting, the current Interface is usart_1
  *           + error process 
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
#define PRG_MAIN_VER (2) /* program version number */

#define PWM_VAL_MIN_ALL (35)  /* minimum pwm value in all operation state */
#define PWM_VAL_DFT_REV (120) /* default pwm value in system rebound state */
#define PWM_VAL_MAX_RUN (250) /* maximum pwm value in system run state */
#define PWM_VAL_MAX_SET (210) /* maximum pwm value in route set state */
#define PWM_VAL_MAX_STP (160) /* maximum pwm value in slow stop state */

#define HAL_DUTY_FAST (360) /* permitted hall duty during normal state when close */
#define HAL_DUTY_SLOW (550) /* permitted hall duty during slow stop when close */

#define HAL_CNT_SLOW_STOP (400) /* slow stop distance in hall unit */

#define FORCE_LVL_MAX (8) /* maximum of speed level */

#define AD_MAX (0xf0) /* max over force calculation */

typedef enum {
   DIR_NONE = (0),
   DIR_STADY,
   DIR_OPEN,
   DIR_CLOSE,
}ENUM_DIRECTION;
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
  * @brief  define wireless command
  */
typedef enum {
  COMMAND_NONE = (0x0),
  COMMAND_OUTDOOR_DOOR,
  COMMAND_KEYPAD_DOOR,
  COMMAND_KEYPAD_LOCK,
  COMMAND_KEYPAD_LAMP,
  COMMAND_KEYPAD_HOLD_EN,
  COMMAND_KEYPAD_HOLD_DIS,
  COMMAND_WIFI_LAMP_ON,
  COMMAND_WIFI_LAMP_OFF,
  COMMAND_WIFI_DOOR_CLOSE,
  COMMAND_WIFI_DOOR_OPEN,
  COMMAND_WIFI_DOOR_PRESET,
  COMMAND_WIFI_DOOR_TOGGLE,
  COMMAND_WIFI_PROGRAM_ON,
  COMMAND_WIFI_PROGRAM_OFF,
}WIRELESS_COMMAND;


/**
  * @brief  Define system control structure
  */
typedef struct
{
  SYS_STATUS sys_sts; /* current system status */
  U08ET sub_sts;      /* current system sub status */
  SYS_STATUS sys_lts; /* last system status */
  U08ET sub_lts;      /* last system sub status */

  U08ET lst_dir; /* last motor direction */
  U08ET cur_dir;
  U08ET cur_pwm;    /* current pwm value */
  U16ET opn_pos;    /* reopen position after obstacle */
  U16ET lst_pos;    /* last position record */
  U08ET rem_key[4]; /* remote key buff */
  
  U16ET hall_dest;  /* distination of motor control */

  U16ET clo_cnt;
  U08ET sys_set; /* system set item */
  BITET bit_flg; /* bit flag */
  U08ET err_num; /* error number */
  U08ET door_command_type;

  U16ET delay0; /* time delay */
  U16ET delay1; /* time delay */
  U16ET delay2; /* auto stop */
  U16ET delay3_vacation; /* fir vacation blink */

  
  U16ET WiFi_connected_cnt;
  U08ET WiFi_connected;  
  U08ET WiFi_PreLock;  
  U08ET WiFi_DoorCommand;
  U08ET WiFi_LampCommand;
  U08ET WiFi_ProgramCommand;  
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

VODET WiFi_process(VODET);
WIRELESS_COMMAND get_wireless_command(VODET);

#endif /* _MAIN_H */
