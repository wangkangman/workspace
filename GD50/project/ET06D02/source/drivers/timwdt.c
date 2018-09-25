#include "regdef.h"
#include "clkrst.h"
#include "motors.h"
#include "remote.h"
#include "timwdt.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_TASKSKM tsk_skm;

/*-----------------------------------------------------------------*/
/* Local function declare                                          */
/*-----------------------------------------------------------------*/
static VODET timer1_init(VODET);
static VODET timer2_init(VODET);
static VODET timer3_init(VODET);
static VODET wdtdog_init(VODET);

/*-----------------------------------------------------------------*/
/* FUNC : Timer and watchdog initialize                            */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET timwdt_init(VODET)
{
    timer1_init();
    timer2_init();
    timer3_init();
    wdtdog_init();
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer1 initialize for system task dispatch               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static VODET timer1_init(VODET)
{
    /* enbale clock of TIMER1 */
    sysclk_ctrl(CLK_ID_TIME1, CLK_CT_ENA);

    /* set prescale clock division to 16, so timer1 clock is 1MHz */
    TIM1_PSCRH = 0x00;
    TIM1_PSCRL = 0x0F;

    /* set auto load value to 2000, so timer1 cycle is 2ms */
    TIM1_ARRH = 0x07;
    TIM1_ARRL = 0xD0;

    /* set timer1 control mode */
    TIM1_CR1  = 0x00;    /* disable timer */
    TIM1_CR1 |= (1<<7);  /* auto reload enable */
    TIM1_CR1 |= (1<<2);  /* send interrupt request only when overflow */

    /* disable all interrupt */
    TIM1_IER = 0x00;

    /* clear all interrupt flag */
    TIM1_SR1 = 0x00;
    TIM1_SR2 = 0x00;

    /* enable update interrupt */
    TIM1_IER |= (1<<0);

    /* enable timer */
    TIM1_CR1 |= (1<<0);

    /* set task scheme flag to default busy */
    tsk_skm.flag = ET_FALSE;
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer2 initialize for motor PWM control                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static VODET timer2_init(VODET)
{
    /* enbale clock of TIMER2 */
    sysclk_ctrl(CLK_ID_TIME2, CLK_CT_ENA);

    /* set prescale clock division to 2^3=8, so timer2 clock is 2MHz */
    TIM2_PSCR = 0x03;

    /* PWM1 mode, preload disable, CC1 channel as ouput */
    TIM2_CCMR1 = 0x60;

    /* OC1 signal is output on the corresponding output pin */
    TIM2_CCER1 = 0x01;

    /* set pwm frequency to 8KHz */
    TIM2_ARRH = (PWM_DUTY_MAX & 0xFF00) >> 8;
    TIM2_ARRL = (PWM_DUTY_MAX & 0x00FF) >> 0;

    /* set pwm duty default to 0 */
    TIM2_CCR1H = 0x00;
    TIM2_CCR1L = 0x00;

    /* enable counter */
    TIM2_CR1 |= (1<<0);
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer3 initialize for GPIO check                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static VODET timer3_init(VODET)
{
    /* enbale clock of TIMER3 */
    sysclk_ctrl(CLK_ID_TIME3, CLK_CT_ENA);

    /* set prescale clock division to 2^4=16, so timer3 clock is 1MHz */
    TIM3_PSCR = 0x04;

    /* set auto load value to 50, so timer3 cycle is 50us */
    TIM3_ARRH = 0x00;
    TIM3_ARRL = 0x32;

    /* set timer3 control mode */
    TIM3_CR1  = 0x00;    /* disable timer */
    TIM3_CR1 |= (1<<7);  /* auto reload enable */
    TIM3_CR1 |= (1<<2);  /* send interrupt request only when overflow */

    /* disable all interrupt */
    TIM3_IER = 0x00;

    /* clear all interrupt flag */
    TIM3_SR1 = 0x00;
    TIM3_SR2 = 0x00;

    /* enable update interrupt */
    TIM3_IER |= (1<<0);

    /* enable timer */
    TIM3_CR1 |= (1<<0);
}

/*-----------------------------------------------------------------*/
/* FUNC : Watchdog initialize                                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static VODET wdtdog_init(VODET)
{
    /* enable IWDG */
    IWDG_KR  = 0xCC;

    /* register access enable */
    IWDG_KR  = 0x55;

    /* timeout value 255ms */
    IWDG_PR  = 4;
    IWDG_RLR = 0xFF;

    /* register access disable */
    IWDG_KR  = 0xAA;
}

/*-----------------------------------------------------------------*/
/* FUNC : Feed watchdog                                            */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET wdtdog_feed(VODET)
{
    IWDG_KR = 0xAA;
}

/*-----------------------------------------------------------------*/
/* FUNC : get task skeme flag                                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return current task skeme flag                                */
/*-----------------------------------------------------------------*/
U08ET tskskm_flag(VODET)
{
    if(ET_TRUE == tsk_skm.flag) {
        tsk_skm.flag = ET_FALSE;
        return ET_TRUE;
    }
    else {
        return ET_FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : timer2 control, enable or disable                        */
/* PARA :                                                          */
/*   ctrl : control code, ET_TRUE or ET_FALSE                      */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET timer2_ctrl(U08ET ctrl)
{
    switch(ctrl) {
        case ET_TRUE:
            TIM2_CR1 |= (1<<0);
            break;
        case ET_FALSE:
            TIM2_CR1 &= ~(1<<0);
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Set duty of timer2                                       */
/* PARA :                                                          */
/*   duty : pwm duty [0,PWM_DUTY_MAX]                              */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET timer2_duty(U08ET duty)
{
    if(duty >= PWM_DUTY_MAX) {
        duty = PWM_DUTY_MAX + 1;
    }

    TIM2_CCR1H = (duty & 0xFF00) >> 8;
    TIM2_CCR1L = (duty & 0x00FF) >> 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer1 IRQ handler                                       */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
@far @interrupt VODET TIM1_IRQHandler(VODET)
{
    /* disable interrupt */
    TIM1_IER &= ~(1<<0);

    /* set task scheme flag READY */
    tsk_skm.flag = ET_TRUE;

    /* clear interrupt flag */
    TIM1_SR1 &= ~(1<<0);

    /* enable update interrupt */
    TIM1_IER |= (1<<0);
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer3 IRQ handler                                       */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
@far @interrupt VODET TIM3_IRQHandler(VODET)
{
    /* disable interrupt */
    TIM3_IER &= ~(1<<0);

    /* sampling motor hall signal */
    halsig_samp();

    /* sampling remote controller signal */
    remote_samp();

    /* clear interrupt flag */
    TIM3_SR1 &= ~(1<<0);

    /* enable update interrupt */
    TIM3_IER |= (1<<0);
}
