#include "regdef.h"
#include "ledseg.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_LEDS et_leds[LED_ID_MAX];

/*-----------------------------------------------------------------*/
/* FUNC : Led and Segment initialize                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET ledseg_init(VODET)
{
    U08ET index;

    /* set LED and SEG default status */
    ledlow_ctrl(LED_ID_REMOTE, LED_ST_OFF);

    /* LED_REMOTE <--> P0.3 */
    GPIO_P0_PMD |=  (1<<6);
    GPIO_P0_PMD &= ~(1<<7);

    /* initialize led control structure */
    for(index=0; index<LED_ID_MAX; index++) {
        et_leds[index].stats  = LED_ST_OFF;
        et_leds[index].period = 0;
        et_leds[index].count  = 0;
    }
}
#if 0
VODET ledseg_init(VODET)
{
    U08ET index;

    /* set LED and SEG default status */
    ledlow_ctrl(LED_ID_LAMP, LED_ST_OFF);
    ledlow_ctrl(LED_ID_LED1, LED_ST_OFF);
    ledlow_ctrl(LED_ID_LED2, LED_ST_OFF);
    ledlow_ctrl(LED_ID_LED3, LED_ST_OFF);
    ledlow_ctrl(LED_ID_LED4, LED_ST_OFF);

    /* LED_LAMP <--> P2.4 */
    GPIO_P2_PMD |=  (1<<8);
    GPIO_P2_PMD &= ~(1<<9);

    /* LED_LED1 <--> P0.0 */
    GPIO_P0_PMD |=  (1<<0);
    GPIO_P0_PMD &= ~(1<<1);

    /* LED_LED2 <--> P0.1 */
    GPIO_P0_PMD |=  (1<<2);
    GPIO_P0_PMD &= ~(1<<3);

    /* LED_LED3 <--> P0.2 */
    GPIO_P0_PMD |=  (1<<4);
    GPIO_P0_PMD &= ~(1<<5);

    /* LED_LED4 <--> P4.1 */
    GPIO_P4_PMD |=  (1<<2);
    GPIO_P4_PMD &= ~(1<<3);

    /* initialize led control structure */
    for(index=0; index<LED_ID_MAX; index++) {
        et_leds[index].stats  = LED_ST_OFF;
        et_leds[index].period = 0;
        et_leds[index].count  = 0;
    }
}
#endif

/*-----------------------------------------------------------------*/
/* FUNC : LED task dispatch function, it is called every system    */
/*        heart tick                                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET ledseg_task(VODET)
{
    U08ET index;

    /* led control */
    for(index=0; index<LED_ID_MAX; index++) {
        if(0 != et_leds[index].period) {
            et_leds[index].count++;
            if(et_leds[index].count == et_leds[index].period) {
                if( LED_ST_ON == et_leds[index].stats ) {
                    ledlow_ctrl(index, LED_ST_ON);
                    et_leds[index].stats = LED_ST_OFF;
                }
                else {
                    ledlow_ctrl(index, LED_ST_OFF);
                    et_leds[index].stats = LED_ST_ON;
                }
                et_leds[index].count = 0;
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : LED low level control, can set led on/off directly       */
/* PARA :                                                          */
/*   ledid : LED ID                                                */
/*   stats : LED status                                            */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET ledlow_ctrl(U08ET ledid, U08ET stats)
{
    switch(ledid) {
        case LED_ID_REMOTE:  /* Active High */
            if(LED_ST_ON == stats) {
                GPIO_P0_PDIO3 &=  ~(1<<0);
            }
            if(LED_ST_OFF == stats) {
                GPIO_P0_PDIO3 |=  (1<<0);
            }
            break;
        default:
            break;
    }
}
#if 0
VODET ledlow_ctrl(U08ET ledid, U08ET stats)
{
    switch(ledid) {
        case LED_ID_LAMP:  /* Active High */
            if(LED_ST_ON == stats) {
                GPIO_P2_PDIO4 |=  (1<<0);
            }
            if(LED_ST_OFF == stats) {
                GPIO_P2_PDIO4 &= ~(1<<0);
            }
            break;
        case LED_ID_LED1:  /* Active Low */
            if(LED_ST_ON == stats) {
                GPIO_P0_PDIO0 &= ~(1<<0);
            }
            if(LED_ST_OFF == stats) {
                GPIO_P0_PDIO0 |=  (1<<0);
            }
            break;
        case LED_ID_LED2:  /* Active Low */
            if(LED_ST_ON == stats) {
                GPIO_P0_PDIO1 &= ~(1<<0);
            }
            if(LED_ST_OFF == stats) {
                GPIO_P0_PDIO1 |=  (1<<0);
            }
            break;
        case LED_ID_LED3: /* Active Low */
            if(LED_ST_ON == stats) {
                GPIO_P0_PDIO2 &= ~(1<<0);
            }
            if(LED_ST_OFF == stats) {
                GPIO_P0_PDIO2 |=  (1<<0);
            }
            break;
        case LED_ID_LED4: /* Active Low */
            if(LED_ST_ON == stats) {
                GPIO_P4_PDIO1 &= ~(1<<0);
            }
            if(LED_ST_OFF == stats) {
                GPIO_P4_PDIO1 |=  (1<<0);
            }
            break;
        default:
            break;
    }
}
#endif
/*-----------------------------------------------------------------*/
/* FUNC : LED logic level control, can set on/off/blink mode       */
/* PARA :                                                          */
/*   ledid  : LED ID                                               */
/*   ctrls  : LED control code                                     */
/*   stats  : LED default status                                   */
/*   period : LED control period                                   */
/*   timlst : LED continous control time                           */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET ledtsk_ctrl(U08ET ledid, U08ET stats, U32ET period)
{
    ledlow_ctrl(ledid, stats);
    et_leds[ledid].stats  = stats;
    et_leds[ledid].period = period;
    et_leds[ledid].count  = 0;
}
