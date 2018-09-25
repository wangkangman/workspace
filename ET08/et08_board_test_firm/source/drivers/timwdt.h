#ifndef _TIMWDT_H
#define _TIMWDT_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* define PWM parameter                                            */
/*-----------------------------------------------------------------*/
#define PWM_DUTY_MAX (250)

/*-----------------------------------------------------------------*/
/* Define task skeme control structure                             */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 flag;
} ETST_TASKSKM;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void timwdt_init(void);
void wdtdog_feed(void);
u8 tskskm_flag(void);
void timer2_ctrl(u8 ctrl);
void timer2_duty(u8 duty);
@far @interrupt void TIM1_IRQHandler(void);
@far @interrupt void TIM3_IRQHandler(void);

#endif /* _TIMWDT_H */
