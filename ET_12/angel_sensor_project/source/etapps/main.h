#ifndef _MAIN_H
#define _MAIN_H

#include "global.h"
#include "clkrst.h"
#include "motors.h"
#include "remote.h"
#include "eeprom.h"
#include "ledseg.h"
#include "adcsmp.h"
#include "keysmp.h"
#include "timwdt.h"
#include "powers.h"
#include "uartct.h"

/*--------------------------------------------------------------*/
/* define system control parameter                              */
/*--------------------------------------------------------------*/
#define PRG_MAIN_VER          (1)     /* program version number */

#define PWM_VAL_MIN_ALL       (35)    /* minimum pwm value in all operation state */
#define PWM_VAL_DFT_REV       (140)   /* default pwm value in system rebound state */
#define PWM_VAL_MAX_RUN       (250)   /* maximum pwm value in system run state */
#define PWM_VAL_MAX_SET       (230)   /* maximum pwm value in route set state */
#define PWM_VAL_MAX_STP       (160)   /* maximum pwm value in slow stop state */

#define HAL_DUTY_FAST         (365)   /* permitted hall duty during normal state when close */
#define HAL_DUTY_SLOW         (410)   /* permitted hall duty during slow stop when close */

#define HAL_CNT_SLOW_STOP     (200)   /* slow stop distance in hall unit */

#define FORCE_LVL_MAX         (8)     /* maximum of speed level */


#define AD_MAX                (180)   /* max over force calculation */


/*--------------------------------------------------------------*/
/* define system control structure                              */
/*--------------------------------------------------------------*/
typedef enum
{
    SYS_ST_POWER_ON       = 0,        /* system power on */
    SYS_ST_READ_EEP,                  /* read eeprom data */
    SYS_ST_LIMIT_ST,                  /* system limit set */
    SYS_ST_LIMIT_DN,                  /* motor go to down limit */
    SYS_ST_LIMIT_GT,                  /* hall count get between limits */
    SYS_ST_TRAIN_DN,                  /* learn close door force */
    SYS_ST_TRAIN_UP,                  /* learn open door force */
    SYS_ST_STAY_STOP,                 /* system stay stop mode */
    SYS_ST_EXCUTE_UP,                 /* system excute close door */
    SYS_ST_EXCUTE_DN,                 /* system excute open door */
    SYS_ST_EXCUTE_BK,                 /* system force back */
    SYS_ST_SYSTEM_ST,                 /* system set mode */
    SYS_ST_REMOTE_ST,                 /* remote program */
    SYS_ST_POWER_OFF,                 /* system power off */
    SYS_ST_BAD_ERROR,                 /* system get serious error */
    SYS_ST_INVALID
}SYS_STATUS, SUB_STATUS;

/*-----------------------------------------------------------------*/
/* Define system control structure                                 */
/*-----------------------------------------------------------------*/
typedef struct {
    SYS_STATUS sys_sts;               /* current system status */
    SUB_STATUS sub_sts;               /* current system sub status */
    SYS_STATUS sys_lts;               /* last system status */
    SUB_STATUS sub_lts;               /* last system sub status */

    U08ET lst_dir;                    /* last motor direction */
    U08ET cur_pwm;                    /* current pwm value */
    U16ET opn_pos;                    /* reopen position after obstacle */
    U16ET lst_pos;                    /* last position record */
    U08ET rem_key[4];                 /* remote key buff */
    U16ET clo_sec;                    /* auto close seconds count */
//    U16ET clo_cnt;                    /* auto close 2ms count */
    U16ET clo_cnt;
    U16ET lmp_sec;                    /* auto lamp off seconds */
    U16ET lmp_cnt;                    /* auto lamp off 2ms count */
    U08ET sys_set;                    /* system set item */
    BITET bit_flg;                    /* bit flag */
    U08ET err_num;                    /* error number */
    
    U08ET train_er_flg;
    U08ET train_jump;

    U16ET delay0;                     /* time delay */
    U16ET delay1;                     /* time delay */
    U16ET delay2;											/* auto stop */
}SYS_CTRL;

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/
VODET sys_pre_init(VODET);
VODET sys_power_on(VODET);
VODET sys_read_eep(VODET);
VODET sys_limit_st(VODET);
VODET sys_limit_dn(VODET);
VODET sys_limit_gt(VODET);
VODET sys_train_dn(VODET);
VODET sys_train_up(VODET);
VODET sys_stay_stop(VODET);
VODET sys_excute_up(VODET);
VODET sys_excute_dn(VODET);
VODET sys_excute_bk(VODET);
VODET sys_system_st(VODET);
VODET sys_remote_st(VODET);
VODET sys_power_off(VODET);
VODET sys_bad_error(VODET);
VODET UP_FORCE_STOP(VODET); 
VODET DN_FORCE_STOP(VODET);
VODET DN_FORCE_BACK(VODET);
VODET abnormal_detect(VODET);

#endif /* _MAIN_H */
