#include "regdef.h"
#include "motors.h"
#include "remote.h"
#include "keysmp.h"
#include "timwdt.h"
/**
  ******************************************************************************
  * @file    timwdt.c
  * @author  ETdoor team
  * @version V0.0.2
  * @date    2018/4/11
  * @brief   This file provides all application of timer and wcg, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + system period setting.
  *
 ===============================================================================
  
  ******************************************************************************
  * @attention
  *
  * &copy; COPYRIGHT 2018 ETdoor
  *
  *
  ******************************************************************************
 ===============================================================================
  
  ******************************************************************************
  * @verlog
  *
  * V0.0.0: original version;
  * V0.0.1: wcg self reset for spi1_error;
  * V0.0.2: wcg self reset delete;
  *
  *
  ******************************************************************************
  */

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_TASKSKM tsk_skm;

/*-----------------------------------------------------------------*/
/* Local function declare                                          */
/*-----------------------------------------------------------------*/
static VODET timer0_init(VODET);
static VODET wdtdog_init(VODET);

/*-----------------------------------------------------------------*/
/* FUNC : Timer0 50us                                              */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET timwdt_init(VODET)
{
    timer0_init();
    wdtdog_init();

    tsk_skm.count = 0;
    tsk_skm.cycle = 40;
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer1 initialize for system task dispatch               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static VODET timer0_init(VODET)
{
    /* select HIRC as TIMER0 clock source */
    CLK_SEL1 |= (7 << 8);
    /* enable TIMER0 clock */
    CLK_APB |= (1 << 2);
    /* set TIMER0 to periodic mode */
    TIM0_TCSR &= ~(3 << 27);
    TIM0_TCSR |= (1 << 27);
    /* set TIMER0 prescale to 22 */
    TIM0_TCSR &= ~(0xFF << 0);
    TIM0_TCSR |= (21 << 0);
    /* set timeout value to 50us */
    TIM0_TCMPR = 50;
    /* enable TIMER0 interrupt */
    TIM0_TCSR |= (1 << 29);
    /* enable TIMER0 NVIC interrupt */
    NVIC_ISER = (1 << 8);
    /* enable TIMER0 */
    TIM0_TCSR |= (1 << 30);
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
    /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;
    /* select LIRC as WDT clock source */
    CLK_SEL1 |= (3 << 0);
    /* enable WDT clock */
    CLK_APB |= (1 << 0);
    /* set WDT timeout value to 2^12 clocks  about 409.6ms*/
    WDT_WTCR &= ~(7 << 8);
    WDT_WTCR |= (4 << 8);
    /* enable WDT timeout reset */
    WDT_WTCR |= (1 << 1);
    /* enable WDT timeout irq */
    WDT_WTCR |= (1 << 6);
    /* enable WDT unit */
    WDT_WTCR |= (1 << 7);
    /* lock register */
    GCR_WPRPROT = 0x00;
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
    /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;
    //    if (GCR_RSTSRC & (1<<2))
    WDT_WTCR |= (1 << 0);
    /* lock register */
    GCR_WPRPROT = 0x00;
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
    if (tsk_skm.count >= tsk_skm.cycle)
    {
        tsk_skm.count = 0;
        return ET_TRUE;
    }
    else
        return ET_FALSE;
}

/*-----------------------------------------------------------------*/
/* FUNC : Timer0 IRQ handler                                       */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET TMR0_IRQHandler(VODET)
{
    /* clear interrupt flag */
    TIM0_TISR |= (1 << 0);
    /* 2ms task dispatch */
    if (tsk_skm.count < tsk_skm.cycle)
        tsk_skm.count++;
    /* sampling motor hall signal */
    halsig_samp();
    /* sampling remote controller signal */
    remote_samp();
    /* sampling AC signal keys */
    infra_samp();
}
