#include "regdef.h"
#include "SegCtrl.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_SEGS et_segs;

/*-----------------------------------------------------------------*/
/* FUNC : Led and Segment initialize                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void SegCtrl_init(void)
{

    /* SEG_SDI <--> P11 */
    PORT_PMC1 &= ~(1<<1);
    PORT_PM1  &= ~(1<<1);
    PORT_PD1  &= ~(1<<1);

    /* SEG_SCLK <--> P12 */
    PORT_PMC1 &= ~(1<<2);
    PORT_PM1  &= ~(1<<2);
    PORT_PD1  &= ~(1<<2);

    /* SEG_RCLK <--> P21 */
    PORT_PD2  &= ~(1<<1);
    PORT_PM2  &= ~(1<<1);


    /* initialize segment control structure */
    et_segs.period = 0;
    et_segs.count  = 0;
    et_segs.stats  = LED_ST_OFF;
    et_segs.byseg  = SEG_NONE;
    et_segs.ispnt  = ET_FALSE;

    /* initialize SEG to display none */
 //   seglow_ctrl(SEG_NONE, ET_FALSE);
}
/**
  * @brief  Seg direct control.
  * @param  
  *	cseg:  Drive wave of seg;
  *	ispnt: If the piont should be open.
  * @retval None
  *
  */
void seglow_ctrl(U08ET cseg, U08ET ispnt)
{
    U08ET index,delay;

    /* modify control code according point stats */
    if( ET_TRUE == ispnt ) {
        cseg &= ~(1<<0);
    }
    else {
        cseg |=  (1<<0);
    }

    /* shift data to MC74VHC595D */
    for(index=0; index<8; index++) {
        if( 0 == (cseg&(1<<index)) ) {
            PORT_PD1  &= ~ (1<<1);
        }
        else {
            PORT_PD1  |=   (1<<1);
        }
        for(delay=0; delay < 10; delay++);
        PORT_PD1  |=  (1<<2);
        for(delay=0; delay < 10; delay++);
        PORT_PD1  &= ~(1<<2);
    }

    /* lock data to MC74VHC595D */
    PORT_PD2  |=  (1<<1);
    for(delay=0; delay<10; delay++);
    PORT_PD2  &= ~(1<<1);
    for(delay=0; delay<10; delay++);
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
void segtsk_ctrl(U08ET isdig, U08ET cseg, U08ET ispnt, U16ET period)
{
    if( ET_TRUE == isdig ) {
        switch(cseg) {
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
    et_segs.count  = 0;
    et_segs.stats  = LED_ST_OFF;
    et_segs.byseg  = cseg;
    et_segs.ispnt  = ispnt;
}

/*-----------------------------------------------------------------*/
/* FUNC : LED task dispatch function, it is called every system    */
/*        heart tick                                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void SegCtrl_task(void)
{
    /* segment control */
    if(0 != et_segs.period) {
        et_segs.count++;
        if(et_segs.count == et_segs.period) {
            if( LED_ST_ON == et_segs.stats ) {
                seglow_ctrl(et_segs.byseg, et_segs.ispnt);
                et_segs.stats = LED_ST_OFF;
            } else {
                seglow_ctrl(SEG_NONE, ET_FALSE);
                et_segs.stats = LED_ST_ON;
            }
            et_segs.count = 0;
        }
    }
}

