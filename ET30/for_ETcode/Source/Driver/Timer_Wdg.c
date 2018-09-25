#pragma interrupt INTTM00 TM00_ISR
#pragma interrupt INTTM01 TM01_ISR
#pragma interrupt INTTM02 TM02_ISR



#include "regdef.h"
#include "ClkSet.h"
#include "MotorCtrl.h"
#include "Timer_Wdg.h"
#include "Remote.h"
#ifdef UART_DEBUG
#include "UartPrin.h"
#endif 

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_TASKSKM tsk_skm;
static ETST_PWMCTRL pwm_ctl;

/*-----------------------------------------------------------------*/
/* Local function declare                                          */
/*-----------------------------------------------------------------*/
static void wdtdog_init(void);
static void timerc_init(void);


/*-----------------------------------------------------------------*/
/* FUNC : Timer and watchdog initialize                            */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void timwdt_init(void)
{
    wdtdog_init();
    timerc_init();

    tsk_skm.skm_cnt = 0;
    pwm_ctl.pwm_flag = ET_FALSE;
    pwm_ctl.pwm_duty = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer initialize                                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static void timerc_init(void)
{
    clkset_ctrl(CLK_ID_TM0, CLK_CT_ENA);

    /* P13 <-> PWM signal */
    PORT_PD1  &= ~(1<<3); /* default output 0 */
    PORT_PMC1 &= ~(1<<3); /* digital port */
    PORT_PM1  &= ~(1<<3); /* output mode */

    /* CK00=24MHz CK01=24MHz CK02=12MHz CK03=93.8KHz */
    TIM_TPS0 = 0x0000;

    /* stop all timer channels */
    TIM_TT0 = 0x0A0F;

    /* disable all channel interrupt */
    INT_MK0H |= (1<<7);  /* TMMK01 */
    INT_MK0H |= (1<<6);  /* TMMK00 */
    INT_MK0H |= (1<<4);  /* TMMK03H */
    INT_MK0H |= (1<<3);  /* TMMK01H */
    INT_MK1L |= (1<<1);  /* TMMK03 */
    INT_MK1L |= (1<<0);  /* TMMK02 */

    /* clear all channel interrupt flag */
    INT_IF0H &= ~(1<<7);  /* TMIF01 */
    INT_IF0H &= ~(1<<6);  /* TMIF00 */
    INT_IF0H &= ~(1<<4);  /* TMIF03H */
    INT_IF0H &= ~(1<<3);  /* TMIF01H */
    INT_IF1L &= ~(1<<1);  /* TMIF03 */
    INT_IF1L &= ~(1<<0);  /* TMIF02 */

    /* set interrupt priority */
    INT_PR10H |=  (1<<6);  /* CH0 <-> 3 */
    INT_PR00H |=  (1<<6);
    INT_PR10H |=  (1<<7);  /* CH1 <-> 2 */
    INT_PR00H &= ~(1<<7);
    INT_PR11L |=  (1<<0);  /* CH2 <-> 3 */
    INT_PR01L |=  (1<<0);

    /* configure timer channel 0 */
    TIM_TMR00 &= ~((3<<14)|(1<<12)); /* select CK00 as clock source */
    TIM_TMR00 &= ~(7<<8);            /* software trigger */
    TIM_TMR00 &= ~(7<<1);            /* interval timer mode */
    TIM_TMR00 &= ~(1<<0);            /* no interrupt when counter start */
    TIM_TDR00  = 0xBB7;              /* set channel 0 frequency to 8KHz */

    /* configure timer channel 1 */
    TIM_TMR01 &= ~((3<<14)|(1<<12)); /* select CK00 as clock source */
    TIM_TMR01 &= ~(1<<11);           /* 16 bit timer */
    TIM_TMR01 &= ~(7<<8);            /* use interrupt signal of master channel as trigger signal */
    TIM_TMR01 |=  (4<<8);
    TIM_TMR01 &= ~(7<<1);            /* single count mode */
    TIM_TMR01 |=  (4<<1);
    TIM_TMR01 &= ~(1<<0);            /* start trigger is effective during counting */

    /* configure timer channel 2 */
    TIM_TMR02 &= ~((3<<14)|(1<<12)); /* select CK00 as clock source */
    TIM_TMR02 &= ~(1<<11);           /* independent channel */
    TIM_TMR02 &= ~(7<<8);            /* software trigger */
    TIM_TMR02 &= ~(7<<1);            /* interval timer mode */
    TIM_TMR02 &= ~(1<<0);            /* no interrupt when counter start */
    TIM_TDR02  = 0x04AF;             /* set channel 2 period to 50us */

    /* enable selected channel interrupt */
    INT_MK0H &= ~(1<<6);  /* TMMK00 */
    INT_MK0H &= ~(1<<7);  /* TMMK01 */
    INT_MK1L &= ~(1<<0);  /* TMMK02 */

     /* enable timer channel 0 */
     TIM_TS0 |=  (1<<0);

     //TIM_TS0 &=  ~(1<<0);
     /* enable timer channel 2 */
     TIM_TS0 |=  (1<<2);
}

/*-----------------------------------------------------------------*/
/* FUNC : Watchdog initialize                                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void wdtdog_init(void)
{
    WDTIMK = 1U;    /* disable INTWDTI interrupt */
    WDTIIF = 0U;    /* clear INTWDTI interrupt flag */
    /* Set INTWDTI low priority */
    WDTIPR1 = 1U;
    WDTIPR0 = 1U;
    WDTIMK = 0U;    /* enable INTWDTI interrupt */
}

/*-----------------------------------------------------------------*/
/* FUNC : Feed watchdog                                            */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void wdtdog_feed(void)
{
    WDT_WDTE = 0xAC;
}

/*-----------------------------------------------------------------*/
/* FUNC : PWM enable or disable                                    */
/* PARA :                                                          */
/*   flag : enbale or disable flag                                 */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void pwmctl_ctrl(U08ET flag)
{
    if(ET_TRUE == flag) {
        TIM_TS0  |=  (1<<1);
        pwm_ctl.pwm_flag = ET_TRUE;
    } else {
        TIM_TS0  &= ~(1<<1);
        PORT_PD1 &= ~(1<<3);
        pwm_ctl.pwm_flag = ET_FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Set pwm duty                                             */
/* PARA :                                                          */
/*   flag : enbale or disable flag                                 */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void pwmctl_duty(U08ET duty)
{
    if(duty > PWM_DUTY_MAX) {
        return ;
    }
    pwm_ctl.pwm_duty = duty;
    TIM_TDR01 = (U16ET)duty * 12 - 1;
}

/*-----------------------------------------------------------------*/
/* FUNC : Check if ready to skeme task                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   If ready, return ET_TURE, else return ET_FALSE                */
/*-----------------------------------------------------------------*/
U08ET tskskm_rdy(void)
{
    if(tsk_skm.skm_cnt >= TSK_SKM_CNT) {
        tsk_skm.skm_cnt = 0;
        return ET_TRUE;
    } else {
        return ET_FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : timer channel 0 interrupt isr                            */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static void TM00_ISR(void)
{
    /* pwm port set or reset */
    if( ET_TRUE == pwm_ctl.pwm_flag ) {
        if(0 == pwm_ctl.pwm_duty) {
            PORT_PD1  &= ~(1<<3);
        } else {
            PORT_PD1  |=  (1<<3);
        }
    } else {
        PORT_PD1  &= ~(1<<3);
    }
    


    /* excute task skeme count */
    if(tsk_skm.skm_cnt < TSK_SKM_CNT) {
        tsk_skm.skm_cnt++;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : timer channel 1 interrupt isr                            */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static void TM01_ISR(void)
{
    /* pwm port set or reset */
    if( ET_FALSE == pwm_ctl.pwm_flag ) {
        PORT_PD1  &= ~(1<<3);
    } else {
        if(PWM_DUTY_MAX == pwm_ctl.pwm_duty) {
            PORT_PD1  |=  (1<<3);
        } else {
            PORT_PD1  &= ~(1<<3);
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : timer channel 2 interrupt isr                            */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static void TM02_ISR(void)
{
   /* remote process  */
   remote_timer_task();
    
}
