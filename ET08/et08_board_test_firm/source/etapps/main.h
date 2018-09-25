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
#include "typedef.h"

/*--------------------------------------------------------------*/
/* define system control parameter                              */
/*--------------------------------------------------------------*/
#define PRG_MAIN_VER (2) /* program version number */

#define PWM_VAL_MIN_ALL (35)  /* minimum pwm value in all operation state */
#define PWM_VAL_DFT_REV (140) /* default pwm value in system rebound state */
#define PWM_VAL_MAX_RUN (250) /* maximum pwm value in system run state */
#define PWM_VAL_MAX_SET (230) /* maximum pwm value in route set state */
#define PWM_VAL_MAX_STP (160) /* maximum pwm value in slow stop state */

#define HAL_DUTY_FAST (265) /* permitted hall duty during normal state when close */
#define HAL_DUTY_SLOW (380) /* permitted hall duty during slow stop when close */

#define HAL_CNT_SLOW_STOP (400) /* slow stop distance in hall unit */
#define HAL_CNT_REAL_STOP (20)  /* real stop distance in hall unit */

#define FORCE_LVL_MAX (9) /* maximum of speed level */

/*--------------------------------------------------------------*/
/* define system control structure                              */
/*--------------------------------------------------------------*/
typedef enum {
    SYS_ST_POWER_ON = 0, /* system power on */
    SYS_ST_READ_EEP,     /* read eeprom data */
    SYS_ST_ROUTE_UP,     /* system route train up */
    SYS_ST_ROUTE_DN,     /* system route train down */
    SYS_ST_TRAIN_UP,     /* learn close door force */
    SYS_ST_TRAIN_DN,     /* learn open door force */
    SYS_ST_STAY_STOP,    /* system stay stop mode */
    SYS_ST_EXCUTE_UP,    /* system excute close door */
    SYS_ST_EXCUTE_DN,    /* system excute open door */
    SYS_ST_EXCUTE_BK,    /* system force back */
    SYS_ST_SYSTEM_ST,    /* system set mode */
    SYS_ST_REMOTE_ST,    /* remote program */
    SYS_ST_POWER_OFF,    /* system power off */
    SYS_ST_BAD_ERROR,    /* system get serious error */
    SYS_ST_SIMU_FUNC,    /* system simulation function */
    SYS_ST_INVALID
} SYS_STATUS,
    SUB_STATUS;

/*-----------------------------------------------------------------*/
/* Define system control structure                                 */
/*-----------------------------------------------------------------*/
//typedef struct
//{
//    SYS_STATUS sys_sts; /* current system status */
//    SUB_STATUS sub_sts; /* current system sub status */
//    SYS_STATUS sys_lts; /* last system status */
//    SUB_STATUS sub_lts; /* last system sub status */
//
//    u8 lst_dir;    /* last motor direction */
//    u8 cur_pwm;    /* current pwm value */
//    u16 opn_pos;    /* reopen position after obstacle */
//    u16 lst_pos;    /* last position record */
//    u8 rem_key[4]; /* remote key buff */
//    u16 clo_sec;    /* auto close seconds count */
//    u16 clo_cnt;    /* auto close 2ms count */
//    u16 lmp_sec;    /* auto lamp off seconds */
//    u16 lmp_cnt;    /* auto lamp off 2ms count */
//    u8 sys_set;    /* system set item */
//    BITET bit_flg;    /* bit flag */
//    u8 err_num;    /* error number */
//    u32 val_sum;    /* check sum value */
//
//    u16 delay0; /* time delay */
//    u16 delay1; /* time delay */
//} SYS_CTRL;

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/

u32 usmart_getadc_data(u8* chan);

u32 usmart_getcurrent(void);
u32 usmart_getkeyad(void);
void usmart_segctrl(u32 digit, u32 piont);
void usmart_motorctrl(u32 dir);
void usmart_pwmsctrl(u32 pwmval);
void usmart_remotekey(void);
u32 usmart_infra_sta(void);
u32 usmart_wall_sta(void);
u32 usmart_door_sta(void);
u32 usmart_power_sta(void);
u32 usmart_main_hall_sta(void);
u32 usmart_ref_hall_sta(void);
#endif /* _MAIN_H */
