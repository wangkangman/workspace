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
#include "uartct.h"
#include "si_4355.h"

/*--------------------------------------------------------------*/
/* define system control parameter                              */
/*--------------------------------------------------------------*/
#define PWM_VAL_MIN_ALL       (35)    /* minimum pwm value in all operation state */
#define PWM_VAL_DFT_REV       (140)   /* default pwm value in system rebound state */
#define PWM_VAL_MAX_RUN       (250)   /* maximum pwm value in system run state */
#define PWM_VAL_MAX_SET       (250)   /* maximum pwm value in route set state */
#define PWM_VAL_MAX_STP       (160)   /* maximum pwm value in slow stop state */

#define HAL_DUTY_FAST         (265)   /* permitted hall duty during normal state when close */
#define HAL_DUTY_SLOW         (380)   /* permitted hall duty during slow stop when close */

#define HAL_CNT_SLOW_STOP     (400)   /* slow stop distance in hall unit */
#define HAL_CNT_REAL_STOP     (20)    /* real stop distance in hall unit */

/*--------------------------------------------------------------*/
/* define system control structure                              */
/*--------------------------------------------------------------*/
typedef enum
{
    SYS_ST_POWER_ON       = 0,        /* system power on */
    SYS_ST_READ_EEP,                  /* read eeprom data */
    SYS_ST_ROUTE_UP,                  /* system route train up */
    SYS_ST_ROUTE_DN,                  /* system route train down */
    SYS_ST_TRAIN_UP,                  /* learn close door force */
    SYS_ST_TRAIN_DN,                  /* learn open door force */
    SYS_ST_STAY_STOP,                 /* system stay stop mode */
    SYS_ST_EXCUTE_UP,                 /* system excute close door */
    SYS_ST_EXCUTE_DN,                 /* system excute open door */
    SYS_ST_EXCUTE_BK,                 /* system force back */
    SYS_ST_SYSTEM_ST,                 /* system set mode */
    SYS_ST_REMOTE_ST,                 /* remote program */
    SYS_ST_POWER_OFF,                 /* system power off */
    SYS_ST_BAD_ERROR,                 /* system get serious error */
    SYS_ST_SIMU_FUNC,                 /* system simulation function */
    SYS_ST_INVALID
}SYS_STATUS, SUB_STATUS;

/*-----------------------------------------------------------------*/
/* Define system control structure                                 */
/*-----------------------------------------------------------------*/
typedef struct
{
    SYS_STATUS sys_sts;               /* current system status */
    SUB_STATUS sub_sts;               /* current system sub status */
    SYS_STATUS sys_lts;               /* last system status */
    SUB_STATUS sub_lts;               /* last system sub status */

    U08ET lst_dir;                    /* last motor direction */
    U08ET cur_pwm;                    /* current pwm value */
    U16ET opn_pos;                    /* reopen position after obstacle */
    U16ET lst_pos;                    /* last position record */
    U08ET rem_key[4];                 /* remote key buff */
    U32ET clo_cnt;                    /* auto close 2ms count */
    U32ET lmp_cnt;                    /* auto lamp off 2ms count */
    U08ET sys_set;                    /* system set item */
    U32ET slp_cnt;                    /* system sleep time count */
    U08ET vir_lvl;                    /* system force virtual level */
    BITET bit_flg;                    /* bit flag */
    U08ET err_num;                    /* error number */
    U32ET val_sum;                    /* check sum value */

    U16ET delay0;                     /* time delay */
    U16ET delay1;                     /* time delay */

    U08ET dyn_ups;                    /* dynamic open force record flag */
    U08ET dyn_dns;                    /* dynamic close force record flag */
    U32ET dym_cnt;                    /* adynamic record 2ms count */
    U08ET uformax;                    /* max force value in open duration */
    U08ET dformax;                    /* max force value in close duration */
    U08ET upForce[UP_FORCE_SAMP];     /* record open force value */
    U08ET dnForce[DN_FORCE_SAMP];     /* record close force value */
}SYS_CTRL;

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/
VODET sys_pre_init(VODET);
VODET sys_power_on(VODET);
VODET sys_read_eep(VODET);
VODET sys_route_up(VODET);
VODET sys_route_dn(VODET);
VODET sys_train_up(VODET);
VODET sys_train_dn(VODET);
VODET sys_stay_stop(VODET);
VODET sys_excute_up(VODET);
VODET sys_excute_dn(VODET);
VODET sys_excute_bk(VODET);
VODET sys_system_st(VODET);
VODET sys_remote_st(VODET);
VODET sys_power_off(VODET);
VODET sys_bad_error(VODET);
VODET sys_simu_func(VODET);

#endif /* _MAIN_H */
