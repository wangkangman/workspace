#ifndef _GLOBALS_H
#define _GLOBALS_H

/*-----------------------------------------------------------------*/
/* Define our own data types xxxET(ET Door data types)             */
/*-----------------------------------------------------------------*/
#ifndef _XXXET_
#define _XXXET_

typedef void   (*FUNC)(void);

typedef signed   char  S08ET;
typedef signed   short S16ET;
typedef signed   long  S32ET;
typedef unsigned char  U08ET;
typedef unsigned short U16ET;
typedef unsigned long  U32ET;
typedef signed   int   INTET;
typedef float          FLTET;
#define VODET          void

#define ET_TRUE        (1)
#define ET_FALSE       (0)

#endif

/*-----------------------------------------------------------------*/
/* Define our own common structure xxxET(ET Door common structure) */
/*-----------------------------------------------------------------*/
typedef struct
{
    U32ET bit0:1;
    U32ET bit1:1;
    U32ET bit2:1;
    U32ET bit3:1;
    U32ET bit4:1;
    U32ET bit5:1;
    U32ET bit6:1;
    U32ET bit7:1;
}BITET;

/*-----------------------------------------------------------------*/
/* Define eeprom parameter structure                               */
/*-----------------------------------------------------------------*/

#define UP_FORCE_SAMP         (256)   /* open door force sampling points */
#define DN_FORCE_SAMP         (512)   /* close door force sampling points */

typedef struct
{
    U32ET save_flg;                   /* saved indication flag */
    U32ET hall_top;                   /* top side hall value */
    U32ET hall_bot;                   /* bottom side hall value */
    U32ET hall_cur;                   /* current position hall value */
    U32ET hsub_oft;                   /* hall position when sub hall detect */
    U32ET hsub_dif;                   /* hall differ between two sub hall */
    U32ET ufor_max;                   /* max force value in open duration */
    U32ET dfor_max;                   /* max force value in close duration */
    U32ET force_lv;                   /* force control level */
    U32ET irad_ena;                   /* IRAD enable or disable */
    U32ET auto_clo;                   /* auto close minutes */
    U32ET save_end;                   /* misc data save end */

    U32ET rkey_num;                   /* remote key total number */
    U32ET rkey_idx;                   /* remote key save index */
    U32ET rkey[20];                   /* remote key save buff */
    U32ET rkey_end;                   /* remote key save end */

    U16ET upForce[UP_FORCE_SAMP];     /* record open force value */
    U16ET dnForce[DN_FORCE_SAMP];     /* record close force value */
}GLB_CTRL;

extern GLB_CTRL glb_ctrl;

/*-----------------------------------------------------------------*/
/* Define system debug mode                                        */
/*-----------------------------------------------------------------*/
#define DBG_LV_NONE     0
#define DBG_LV_ADCVAL   (1<<0)
#define DBG_LV_PROCESS  (1<<1)
#define DBG_LV_CURRENT  (DBG_LV_PROCESS)

#endif /* _GLOBALS_H */
