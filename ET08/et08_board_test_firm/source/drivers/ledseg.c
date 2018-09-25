#include "regdef.h"
#include "ledseg.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_LEDS et_leds[LED_ID_MAX];
static ETST_SEGS et_segs;

/*-----------------------------------------------------------------*/
/* FUNC : Led and Segment initialize                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void ledseg_init(void)
{
    u8 index;

    /* set LED default status */
    ledlow_ctrl(LED_ID_LAMP, LED_ST_OFF);

    /* LED_LAMP <--> PC1 */
    GPIO_PC_DDR |= (1 << 1);  /* direction output */
    GPIO_PC_CR1 |= (1 << 1);  /* push-pull        */
    GPIO_PC_CR2 &= ~(1 << 1); /* max speed 2MHz   */

    /* SEG_SDI <--> PC5 */
    GPIO_PC_DDR |= (1 << 5);  /* direction output */
    GPIO_PC_CR1 |= (1 << 5);  /* push-pull        */
    GPIO_PC_CR2 &= ~(1 << 5); /* max speed 2MHz   */
    GPIO_PC_ODR &= ~(1 << 5); /* default low      */

    /* SEG_SCLK <--> PC6 */
    GPIO_PC_DDR |= (1 << 6);  /* direction output */
    GPIO_PC_CR1 |= (1 << 6);  /* push-pull        */
    GPIO_PC_CR2 &= ~(1 << 6); /* max speed 2MHz   */
    GPIO_PC_ODR &= ~(1 << 6); /* default low      */

    /* SEG_RCLK <--> PC7 */
    GPIO_PC_DDR |= (1 << 7);  /* direction output */
    GPIO_PC_CR1 |= (1 << 7);  /* push-pull        */
    GPIO_PC_CR2 &= ~(1 << 7); /* max speed 2MHz   */
    GPIO_PC_ODR &= ~(1 << 7); /* default low      */

    /* initialize led control structure */
    for (index = 0; index < LED_ID_MAX; index++)
    {
        et_leds[index].stats = LED_ST_OFF;
        et_leds[index].period = 0;
        et_leds[index].count = 0;
    }

    /* initialize segment control structure */
    et_segs.period = 0;
    et_segs.count = 0;
    et_segs.stats = LED_ST_OFF;
    et_segs.byseg = SEG_NONE;
    et_segs.ispnt = ET_FALSE;

    /* initialize SEG to display none */
    seglow_ctrl(SEG_NONE, ET_FALSE);
}

/*-----------------------------------------------------------------*/
/* FUNC : LED task dispatch function, it is called every system    */
/*        heart tick                                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void ledseg_task(void)
{
    u8 index;

    /* led control */
    for (index = 0; index < LED_ID_MAX; index++)
    {
        if (0 != et_leds[index].period)
        {
            et_leds[index].count++;
            if (et_leds[index].count == et_leds[index].period)
            {
                if (LED_ST_ON == et_leds[index].stats)
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

    /* segment control */
    if (0 != et_segs.period)
    {
        et_segs.count++;
        if (et_segs.count == et_segs.period)
        {
            if (LED_ST_ON == et_segs.stats)
            {
                seglow_ctrl(et_segs.byseg, et_segs.ispnt);
                et_segs.stats = LED_ST_OFF;
            }
            else
            {
                seglow_ctrl(SEG_NONE, ET_FALSE);
                et_segs.stats = LED_ST_ON;
            }
            et_segs.count = 0;
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
void ledlow_ctrl(u8 ledid, u8 stats)
{
    switch (ledid)
    {
    case LED_ID_LAMP: /* Active high */
        if (LED_ST_ON == stats)
        {
            GPIO_PC_ODR |= (1 << 1);
        }
        if (LED_ST_OFF == stats)
        {
            GPIO_PC_ODR &= ~(1 << 1);
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
void ledtsk_ctrl(u8 ledid, u8 stats, u32 period)
{
    ledlow_ctrl(ledid, stats);
    et_leds[ledid].stats = stats;
    et_leds[ledid].period = period;
    et_leds[ledid].count = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : SEG low level control                                    */
/* PARA :                                                          */
/*   cseg  : SEG code                                              */
/*   ispnt : is point display or not                               */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void seglow_ctrl(u8 cseg, u8 ispnt)
{
    u8 index, delay;

    /* modify control code according point stats */
    if (ET_TRUE == ispnt)
    {
        cseg &= ~(1 << 0);
    }
    else
    {
        cseg |= (1 << 0);
    }

    /* shift data to MC74VHC595D */
    for (index = 0; index < 8; index++)
    {
        if (0 == (cseg & (1 << index)))
        {
            GPIO_PC_ODR &= ~(1 << 5);
        }
        else
        {
            GPIO_PC_ODR |= (1 << 5);
        }
        for (delay = 0; delay < 10; delay++)
            ;
        GPIO_PC_ODR |= (1 << 6);
        for (delay = 0; delay < 10; delay++)
            ;
        GPIO_PC_ODR &= ~(1 << 6);
    }

    /* lock data to MC74VHC595D */
    GPIO_PC_ODR |= (1 << 7);
    for (delay = 0; delay < 10; delay++)
        ;
    GPIO_PC_ODR &= ~(1 << 7);
    for (delay = 0; delay < 10; delay++)
        ;
}

/*-----------------------------------------------------------------*/
/* FUNC : SEG display digtal number                                */
/* PARA :                                                          */
/*   isdig  : if parameter cseg is digital [0-9]                   */
/*   cseg   : segment code                                         */
/*   ispnt  : is point display or not                              */
/*   period : segment control period                               */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void segtsk_ctrl(u8 isdig, u8 cseg, u8 ispnt, u16 period)
{
    if (ET_TRUE == isdig)
    {
        switch (cseg)
        {
        case 0:
            cseg = SEG_DEC0;
            break;
        case 1:
            cseg = SEG_DEC1;
            break;
        case 2:
            cseg = SEG_DEC2;
            break;
        case 3:
            cseg = SEG_DEC3;
            break;
        case 4:
            cseg = SEG_DEC4;
            break;
        case 5:
            cseg = SEG_DEC5;
            break;
        case 6:
            cseg = SEG_DEC6;
            break;
        case 7:
            cseg = SEG_DEC7;
            break;
        case 8:
            cseg = SEG_DEC8;
            break;
        case 9:
            cseg = SEG_DEC9;
            break;
        default:
            break;
        }
    }

    seglow_ctrl(cseg, ispnt);
    et_segs.period = period;
    et_segs.count = 0;
    et_segs.stats = LED_ST_OFF;
    et_segs.byseg = cseg;
    et_segs.ispnt = ispnt;
}
