#ifndef _LEDSEG_H
#define _LEDSEG_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define LED status                                               */
/*-----------------------------------------------------------------*/
#define LED_ST_OFF (0)
#define LED_ST_ON (1)

/*-----------------------------------------------------------------*/
/* Define LED ID                                                   */
/*-----------------------------------------------------------------*/
#define LED_ID_LAMP (0)
#define LED_ID_MAX (1)

/*-----------------------------------------------------------------*/
/* Define SEG control code                                         */
/*-----------------------------------------------------------------*/
#define SEG_NONE (0xFF)
#define SEG_DEC0 (0x03)
#define SEG_DEC1 (0x9F)
#define SEG_DEC2 (0x25)
#define SEG_DEC3 (0x0D)
#define SEG_DEC4 (0x99)
#define SEG_DEC5 (0x49)
#define SEG_DEC6 (0x41)
#define SEG_DEC7 (0x1F)
#define SEG_DEC8 (0x01)
#define SEG_DEC9 (0x09)
#define SEG_LINE (0xFD)
#define SEG_DOWN (0xC7)
#define SEG_UPUP (0x3B)
#define SEG_STOP (0xD7)
#define SEG_CLRS (0x63)
#define SEG_DLIN (0x93)
#define SEG_LETH (0x91)
#define SEG_EROR (0x61)

/*-----------------------------------------------------------------*/
/* Define LED control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 stats;  /* led status */
    u16 period; /* led control period */
    u16 count;  /* led period counter */
} ETST_LEDS;

/*-----------------------------------------------------------------*/
/* Define SEG control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 byseg;  /* segment byte code */
    u8 ispnt;  /* is point need to show */
    u8 stats;  /* current status */
    u16 period; /* led control period */
    u16 count;  /* led period counter */
} ETST_SEGS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void ledseg_init(void);
void ledseg_task(void);
void ledlow_ctrl(u8 ledid, u8 stats);
void ledtsk_ctrl(u8 ledid, u8 stats, u32 period);
void seglow_ctrl(u8 cseg, u8 ispnt);
void segtsk_ctrl(u8 isdig, u8 cseg, u8 ispnt, u16 period);

#endif /* _LEDSEG_H */
