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
    U08ET flag;
}ETST_TASKSKM;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET timwdt_init(VODET);
VODET wdtdog_feed(VODET);
U08ET tskskm_flag(VODET);
VODET timer2_ctrl(U08ET ctrl);
VODET timer2_duty(U08ET duty);
@far @interrupt VODET TIM1_IRQHandler(VODET);
@far @interrupt VODET TIM3_IRQHandler(VODET);


#endif /* _TIMWDT_H */
