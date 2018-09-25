#ifndef _MAIN_H
#define _MAIN_H

#include "global.h"
#include "ClkSet.h"
#include "AdcFunc.h"
#include "MotorCtrl.h"
#include "UartPrin.h"
#include "Key_AandD.h"
#include "Timer_Wdg.h"
#include "BuzLamp.h"
#include "SegCtrl.h"
#include "Remote.h"
#include "dataflash.h"

#define PWM_VAL_MIN_ALL       (35)    /* minimum pwm value in all operation state */
#define PWM_VAL_DFT_REV       (140)   /* default pwm value in system rebound state */
#define PWM_VAL_MAX_RUN       (250)   /* maximum pwm value in system run state */
#define PWM_VAL_MAX_SET       (230)   /* maximum pwm value in route set state */
#define PWM_VAL_MAX_STP       (140)   /* maximum pwm value in slow stop state */


#define FORCE_LVL_MAX         (9)     /* maximum of speed level */
#define HAL_CNT_SLOW_STOP     (65)   /* slow stop distance in encoder val unit */


typedef enum
{
    SYS_ST_POWER_ON       = 0,        /* system power on */
    SYS_ST_READ_EEP,                  /* read eeprom data */
    SYS_ST_LIMIT_ST,		      /* Set limit Postion */	
    SYS_ST_TRAIN_DN,                  /* learn down force */
    SYS_ST_TRAIN_UP,		      /* learn up force */
    SYS_ST_STAY_STOP,                 /* Standby mode */
    SYS_ST_EXCUTE_UP,		      /* Open the door */
    SYS_ST_EXCUTE_DN,		      /* Close the door */	
    SYS_ST_EXCUTE_BK,		      /* Force back when Close Sequence*/
    SYS_ST_REMOTE_ST,		      /* Learn or Clear the key */
    SYS_ST_SYSTEM_ST,		      /* Judge which set Function will jump to */	
    SYS_ST_FORCE_ST,                  /* Force Lv. set */
    SYS_ST_LR_ST,		      /* Set L/R mode */
    SYS_ST_ATCLO_ST,                  /* Auto close time set */
    SYS_ST_INFRA_ST,		      /* Set infra function valid or not */
    SYS_ST_ERROR,		      /* Stop motor then Display error code */ 
    SYS_ST_INVALID,
}SYS_STATUS, SUB_STATUS;

typedef struct
{
    SYS_STATUS sys_sts;               /* current system status */
    SYS_STATUS sys_lts;               /* last system status */ 
    U08ET sub_sts;                    /* current system sub status */              
    U08ET sub_lts;                    /* last system sub status */

    U08ET lst_dir;                    /* last motor direction */
    U08ET cur_dir;		      /* current motor direction */
    U16ET start_count;		      /* when motor start -> 0 */
    
    U08ET cur_pwm;                    /* current pwm value */
    U16ET lst_pos;                    /* last position record */
    U08ET rem_key[4];                 /* remote key buff */
//    U16ET clo_sec;                    /* auto close seconds count */
    U32ET clo_cnt;                    /* auto close 2ms count */
    U16ET lmp_sec;                    /* auto lamp off seconds */
    U16ET lmp_cnt;                    /* auto lamp off 2ms count */
    U08ET sys_set;                    /* system set item */
    BITET bit_flg;                    /* bit flag */
    U08ET err_num;                    /* error number */
 //   U08ET OvCurCount;
    
    U16ET EncoderErrCnt;	      /* error counter when the Motor move */
    U16ET ErrPrePosition;             /* prePosition of Motor */

    U16ET delay0;                     /* time delay */
    U16ET delay1;                     /* time delay */
 //   U16ET delay2;
}SYS_CTRL;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void sys_pre_init(void);

void sys_power_on(void);
void sys_read_eep(void);
void sys_limit_st(void);
void sys_train_dn(void);
void sys_train_up(void);
void sys_stay_stop(void);
void sys_excute_up(void);
void sys_excute_dn(void);
void sys_excute_bk(void);
void sys_remote_st(void);
void sys_system_st(void);
void sys_force_st(void);
void sys_lr_st(void);
void sys_atclo_st(void);
void sys_infra_st(void);
void sys_error(void);




#endif /* _MAIN_H */
