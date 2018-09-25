#ifndef _MOTORS_H
#define _MOTORS_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define motor control code                                       */
/*-----------------------------------------------------------------*/
#define MOT_CTS_UPUP (1)
#define MOT_CTS_DOWN (2)
#define MOT_CTS_STOP (3)

/*-----------------------------------------------------------------*/
/* Define hall detection parameter                                 */
/*-----------------------------------------------------------------*/
#define HAL_DIR_UPUP (1)
#define HAL_DIR_DOWN (2)

#define HAL_CNT_MIN (0)
#define HAL_BOT_DFT (10000)
#define HAL_CNT_MAX (20000)

#define HAL_DTY_MAX (2000)

/*-----------------------------------------------------------------*/
/* Define hall error status                                        */
/*-----------------------------------------------------------------*/
#define ERR_HAL_NONE (0)
#define ERR_HAL_MAIN (1)
#define ERR_HAL_SUBS (2)
#define ERR_HAL_MUTX (3)

/*-----------------------------------------------------------------*/
/* Define hall detection structure                                 */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 main_lst : 2; /* main hall signal last status */
    u8 main_cts : 2; /* main hall signal current status */
    u8 subs_lst : 2; /* subs hall signal last status */
    u8 subs_cts : 2; /* subs hall signal current status */

    u16 hall_cnt; /* real time motor hall count */
    u16 duty_val; /* real time hall duty value */
    u16 duty_cnt; /* duty count to calcute duty value */
    u8 hall_dir; /* current hall move direction */

    u16 herr_max; /* main hall error maximum time count */
    u16 herr_cnt; /* main hall error time counter */
    u8 herr_ena; /* main hall error enable or not */

    u16 hcnt_sub; /* hall count that sub hall last detected */
    u8 hevt_sub; /* sub hall detected event */
    u8 herr_sub; /* sub hall error counter */
} ETST_HALL;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void motors_init(void);
void motors_ctrl(u8 ctrl);
void motors_pwms(u8 pwms);
void halsig_samp(void);
void halsig_sdir(u8 dir);
void halsig_scnt(u16 cnt);
u16 halsig_gcnt(void);
void halsub_clrs(void);
u8 halsub_gets(void);
u16 halsig_duty(void);
u8 halsig_gerr(void);

u8 main_hal_get(void);
u8 ref_hal_get(void);
#endif /* _MOTORS_H */
