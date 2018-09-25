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

    /* set LED default status */
    ledlow_ctrl(LED_ID_SET,  LED_ST_OFF);
    ledlow_ctrl(LED_ID_CODE, LED_ST_OFF);
    ledlow_ctrl(LED_ID_LAMP, LED_ST_OFF);

    /* LED_SET  <--> PC2 */
    GPIO_PC_DDR |=  (1<<2);  /* direction output */
    GPIO_PC_CR1 |=  (1<<2);  /* push-pull        */
    GPIO_PC_CR2 &= ~(1<<2);  /* max speed 2MHz   */

    /* LED_CODE <--> PC6 */
    GPIO_PC_DDR |=  (1<<6);  /* direction output */
    GPIO_PC_CR1 |=  (1<<6);  /* push-pull        */
    GPIO_PC_CR2 &= ~(1<<6);  /* max speed 2MHz   */

    /* LED_LAMP <--> PD7 */
    GPIO_PD_DDR |=  (1<<7);  /* direction output */
    GPIO_PD_CR1 |=  (1<<7);  /* push-pull        */
    GPIO_PD_CR2 &= ~(1<<7);  /* max speed 2MHz   */

    /* initialize led control structure */
    for(index=0; index<LED_ID_MAX; index++)
    {
        et_leds[index].stats  = LED_ST_OFF;
        et_leds[index].period = 0;
        et_leds[index].count  = 0;
    }
}

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
    for(index=0; index<LED_ID_MAX; index++)
    {
        if(0 != et_leds[index].period)
        {
            et_leds[index].count++;
            if(et_leds[index].count == et_leds[index].period)
            {
                if( LED_ST_ON == et_leds[index].stats )
                {
                    ledlow_ctrl(index, LED_ST_ON);
                    et_leds[index].stats = LED_ST_OFF;
                }
                else
                {
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
    switch(ledid)
    {
        case LED_ID_SET: /* Active low */
            if(LED_ST_ON == stats)
            {
                GPIO_PC_ODR &= ~(1<<2);
            }
            if(LED_ST_OFF == stats)
            {
                GPIO_PC_ODR |=  (1<<2);
            }
            break;
        case LED_ID_CODE: /* Active low */
            if(LED_ST_ON == stats)
            {
                GPIO_PC_ODR &= ~(1<<6);
            }
            if(LED_ST_OFF == stats)
            {
                GPIO_PC_ODR |=  (1<<6);
            }
            break;
        case LED_ID_LAMP: /* Active high */
            if(LED_ST_ON == stats)
            {
                GPIO_PD_ODR |=  (1<<7);
            }
            if(LED_ST_OFF == stats)
            {
                GPIO_PD_ODR &= ~(1<<7);
            }
            break;
        default:
            break;
    }
}

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
