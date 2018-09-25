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
typedef float          FLTET;
typedef void           VODET;

typedef union UU16
{
    U16ET U16;
    S16ET S16;
    U08ET U8[2];
    S08ET S8[2];
} UU16;

typedef union UU32
{
    U32ET U32;
    S32ET S32;
    UU16 UU16[2];
    U16ET U16[2];
    S16ET S16[2];
    U08ET U8[4];
    S08ET S8[4];
} UU32;

#define ET_TRUE        (1)
#define ET_FALSE       (0)

#endif

/*-----------------------------------------------------------------*/
/* Define our own common structure xxxET(ET Door common structure) */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET bit0:1;
    U08ET bit1:1;
    U08ET bit2:1;
    U08ET bit3:1;
    U08ET bit4:1;
    U08ET bit5:1;
    U08ET bit6:1;
    U08ET bit7:1;
}BITET;

/*-----------------------------------------------------------------*/
/* Define our own instructions xxxET(ET Door instructions)         */
/*-----------------------------------------------------------------*/
#define ET_DI()      {_asm("sim\n");}  /* disable interrupt */
#define ET_EI()      {_asm("rim\n");}  /* enable interrupt */
#define ET_NOP()     {_asm("nop\n");}  /* no operation */

/*-----------------------------------------------------------------*/
/* Define eeprom parameter structure                               */
/*-----------------------------------------------------------------*/

#define UP_FORCE_SAMP         (128)   /* open door force sampling points */
#define DN_FORCE_SAMP         (512)   /* close door force sampling points */

typedef struct
{
    U16ET save_flg;                   /* saved indication flag */
    U32ET save_sum;                   /* saved data checksum */
    U16ET hall_top;                   /* top side hall value */
    U16ET hall_bot;                   /* bottom side hall value */
    U16ET hall_cur;                   /* current position hall value */
    U16ET hsub_oft;                   /* hall position when sub hall detect */
    U16ET hsub_dif;                   /* hall differ between two sub hall */
    U16ET hsub_lst;                   /* last sub hall verify value */
    U08ET ufor_max;                   /* max force value in open duration */
    U08ET dfor_max;                   /* max force value in close duration */
    U08ET force_lv;                   /* force control level */
    U08ET irad_ena;                   /* IRAD enable or disable */
    U08ET auto_clo;                   /* auto close minutes */
    U08ET save_end;                   /* misc data save end */

    U08ET upForce[UP_FORCE_SAMP];     /* record open force value */
    U08ET dnForce[DN_FORCE_SAMP];     /* record close force value */
}GLB_CTRL;

typedef struct
{
    U08ET flg_cfgs;                   /* configuration indication flag */
    U08ET prg_vers;                   /* software release version */
    U08ET rmt_type;                   /* remote controller type */
    U08ET rmt_vids[8];                /* remote controller VID */
    U08ET clo_auto;                   /* auto close seconds */
    U16ET siz_ropn;                   /* open height after obstable */
    U08ET max_levl;                   /* resistance maximum level */
    U08ET dft_levl;                   /* resistance default level */
    U08ET end_cfgs;                   /* misc data configuration end */
}GLB_CFGS;

extern GLB_CTRL glb_ctrl;
extern GLB_CFGS glb_cfgs;

/*-----------------------------------------------------------------*/
/* Define system debug mode                                        */
/*-----------------------------------------------------------------*/
#define DBG_LV_NONE     (0)
#define DBG_LV_ADCVAL   (1<<0)
#define DBG_LV_PROCESS  (1<<1)
#define DBG_LV_CURRENT  (DBG_LV_PROCESS)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET glbcfg_init(VODET);

#endif /* _GLOBALS_H */
