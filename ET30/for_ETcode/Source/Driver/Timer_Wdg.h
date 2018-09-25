#ifndef _TIMER_WDG_H
#define _TIMER_WDG_H
#include "global.h"


/*-----------------------------------------------------------------*/
/* Define timer control parameter                                  */
/*-----------------------------------------------------------------*/
#define PWM_DUTY_MAX (250)
#define TSK_SKM_CNT  (16)

/*-----------------------------------------------------------------*/
/* Define task skeme control structure                             */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET skm_cnt;
}ETST_TASKSKM;

typedef struct
{
    U08ET pwm_flag;
    U08ET pwm_duty;
}ETST_PWMCTRL;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void timwdt_init(void);
void wdtdog_feed(void);
void pwmctl_ctrl(U08ET flag);
void pwmctl_duty(U08ET duty);
U08ET tskskm_rdy(void);


#endif

