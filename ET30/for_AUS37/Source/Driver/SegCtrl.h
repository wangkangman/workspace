#ifndef _LEDSEG_H
#define _LEDSEG_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define LED status                                               */
/*-----------------------------------------------------------------*/
#define LED_ST_OFF  (0)
#define LED_ST_ON   (1)

/*-----------------------------------------------------------------*/
/* Define LED ID                                                   */
/*-----------------------------------------------------------------*/
#define LED_ID_LAMP (0)
#define LED_ID_MAX  (1)

/*-----------------------------------------------------------------*/
/* Define SEG control code                                         */
/*-----------------------------------------------------------------*/
#define SEG_NONE  (0xFF)
#define SEG_DEC0  (0x03)
#define SEG_DEC1  (0x9F)
#define SEG_DEC2  (0x25)
#define SEG_DEC3  (0x0D)
#define SEG_DEC4  (0x99)
#define SEG_DEC5  (0x49)
#define SEG_DEC6  (0x41)
#define SEG_DEC7  (0x1F)
#define SEG_DEC8  (0x01)
#define SEG_DEC9  (0x09)
#define SEG_LINE  (0xFD)
#define SEG_DOWN  (0xC7)
#define SEG_UPUP  (0x3B)
//#define SEG_STOP  (0x3B)
#define SEG_STOP  (0xD7)
#define SEG_CLRS  (0x63)
#define SEG_INF0  (0x93)
#define SEG_INF1  (0x91)

#define SEG_DLIN  (0x93)
#define SEG_LETH  (0x91)
#define SEG_EROR  (0x61)
#define SEG_EROR_3LINE (0x6D)


#define UP_SEG_FLICKER_PERIOD  (200)
#define DOWN_SEG_FLICKER_PERIOD  (200)

/*-----------------------------------------------------------------*/
/* Define SEG control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET byseg;      /* segment byte code */
    U08ET ispnt;      /* is point need to show */
    U08ET stats;      /* current status */
    U16ET period;     /* led control period */
    U16ET count;      /* led period counter */
}ETST_SEGS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void SegCtrl_init(void);
void seglow_ctrl(U08ET cseg, U08ET ispnt);
void segtsk_ctrl(U08ET isdig, U08ET cseg, U08ET ispnt, U16ET period);
void SegCtrl_task(void);
#endif /* _LEDSEG_H */
