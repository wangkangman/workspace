#include "regdef.h"
#include "uartct.h"
#include "timwdt.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_TASKSKM tsk_skm;

/*-----------------------------------------------------------------*/
/* FUNC : Timer initialize                                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET timwdt_init(VODET)
{
    /* select HIRC as TIMER0 clock source */
    CLK_SEL1 |= (7<<8);

    /* enable TIMER0 clock */
    CLK_APB |=  (1<<2);

    /* set TIMER0 to periodic mode */
    TIM0_TCSR &= ~(3<<27);
    TIM0_TCSR |=  (1<<27);

    /* set TIMER0 prescale to 22 */
    TIM0_TCSR &= ~(0xFF<<0);
    TIM0_TCSR |=  (21<<0);

    /* set timeout value to 50us */
    TIM0_TCMPR = 50;

    /* enable TIMER0 interrupt */
    TIM0_TCSR |= (1<<29);

    /* enable TIMER0 NVIC interrupt */
    NVIC_ISER = (1<<8);

    /* enable TIMER0 */
    TIM0_TCSR |= (1<<30);

    /* initialize timer control structure */
    tsk_skm.count = 0;
    tsk_skm.cycle = 40;
}

/*-----------------------------------------------------------------*/
/* FUNC : Stop timer and disable its interrupt                     */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET timwdt_stop(VODET)
{
    /* disable TIMER0 clock */
    CLK_APB &= ~(1<<2);

    /* disable TIMER0 */
    TIM0_TCSR &= ~(1<<30);

    /* disable TIMER0 interrupt */
    TIM0_TCSR &= ~(1<<29);

    /* disable TIMER0 NVIC interrupt */
    NVIC_ICER = (1<<8);
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
    if( tsk_skm.count >= tsk_skm.cycle ) {
        tsk_skm.count = 0;
        return ET_TRUE;
    }
    else {
        return ET_FALSE;
    }
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
    TIM0_TISR |= (1<<0);

    /* uart receive overtime control */
    uartct_time();

    /* 2ms task dispatch */
    if( tsk_skm.count < tsk_skm.cycle ) {
        tsk_skm.count++;
    }
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
    CLK_SEL1 |=  (3<<0);
    /* enable WDT clock */
    CLK_APB |= (1<<0);
    /* set WDT timeout value to 2^12 clocks  about 409.6ms*/
    WDT_WTCR &= ~(7<<8);
    WDT_WTCR |=  (2<<8);
    /* enable WDT timeout reset */
    WDT_WTCR |= (1<<1);
    /* enable WDT timeout irq */
    WDT_WTCR |= (1<<6);  
    /* enable WDT unit */
    WDT_WTCR |= (1<<7);
    /* lock register */
    GCR_WPRPROT = 0x00;
}

/*-----------------------------------------------------------------*/
/* FUNC : Watchdog initialize                                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static VODET wdtdog_disable(VODET)
{
    /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;

    /* enable WDT unit */
    WDT_WTCR &= ~(1<<7);
    /* lock register */
    GCR_WPRPROT = 0x00;
}

VODET wdt_reset(VODET){
    if (GCR_RSTSRC & (1<<2)) 
	    wdtdog_disable();
		else 
			wdtdog_init();
}
