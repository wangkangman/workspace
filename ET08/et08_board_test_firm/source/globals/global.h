#ifndef _GLOBALS_H
#define _GLOBALS_H
#include "typedef.h"
/*-----------------------------------------------------------------*/
/* Define our own data types xxxET(ET Door data types)             */
/*-----------------------------------------------------------------*/
#ifndef _XXXET_
#define _XXXET_


typedef void (*FUNC)(void);

//typedef signed char s8;
//typedef signed short s16;
//typedef signed long s32;
//typedef unsigned char u8;
//typedef unsigned short u16;
//typedef unsigned long u32;


#define ET_TRUE (1)
#define ET_FALSE (0)

#endif

/*-----------------------------------------------------------------*/
/* Define our own common structure xxxET(ET Door common structure) */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
} BITET;

/*-----------------------------------------------------------------*/
/* Define our own instructions xxxET(ET Door instructions)         */
/*-----------------------------------------------------------------*/
#define ET_DI()        \
    {                  \
        _asm("sim\n"); \
    } /* disable interrupt */
#define ET_EI()        \
    {                  \
        _asm("rim\n"); \
    } /* enable interrupt */
#define ET_NOP()       \
    {                  \
        _asm("nop\n"); \
    } /* no operation */

/*-----------------------------------------------------------------*/
/* Define eeprom parameter structure                               */
/*-----------------------------------------------------------------*/

#define UP_FORCE_SAMP (128) /* open door force sampling points */
#define DN_FORCE_SAMP (640) /* close door force sampling points */

typedef struct
{
    u16 save_flg; /* saved indication flag */
    u16 hall_top; /* top side hall value */
    u16 hall_bot; /* bottom side hall value */
    u16 hall_cur; /* current position hall value */
    u16 hsub_oft; /* hall position when sub hall detect */
    u16 hsub_dif; /* hall differ between two sub hall */
    u16 hsub_lst; /* last sub hall verify value */
    u8 ufor_max; /* max force value in open duration */
    u8 dfor_max; /* max force value in close duration */
    u8 force_lv; /* force control level */
    u8 irad_ena; /* IRAD enable or disable */
    u8 auto_clo; /* auto close minutes */
    u8 half_opn; /* half open level */
    u8 just_opn; /* just open function */
    u8 save_end; /* misc data save end */

    u8 upForce[UP_FORCE_SAMP]; /* record open force value */
    u8 dnForce[DN_FORCE_SAMP]; /* record close force value */
} GLB_CTRL;

extern GLB_CTRL glb_ctrl;

/*-----------------------------------------------------------------*/
/* Define system debug mode                                        */
/*-----------------------------------------------------------------*/
#define DBG_LV_NONE 0
#define DBG_LV_ADCVAL (1 << 0)
#define DBG_LV_PROCESS (1 << 1)
#define DBG_LV_CURRENT (DBG_LV_NONE)

#endif /* _GLOBALS_H */
