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

#define ET_TRUE        (1)
#define ET_FALSE       (0)

#define OFFSETOF(TYPE,MEMBER) ((U08ET)(&((TYPE*)0)->MEMBER))

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
    U08ET bit6:1;   // limit pos charge (when dn sensor trigger  0->1  then up pos can be changed by limit sensor trigger)
    U08ET bit7:1;   // if limit pos change when stop moving (1: limit sensor) or not (0: remote or wall switch)
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
#define DN_FORCE_SAMP         (640)   /* close door force sampling points */

typedef struct
{
    U16ET save_flg;                   /* saved indication flag */
    U16ET hall_top;                   /* top side hall value */
    U16ET hall_bot;                   /* bottom side hall value */
    U16ET hall_cur;                   /* current position hall value */
    U08ET ufor_max;                   /* max force value in open duration */
    U08ET dfor_max;                   /* max force value in close duration */
    U08ET force_lv;                   /* force control level */
    U08ET auto_clo;                   /* auto close minutes */
    U08ET LorR_Set;
    U08ET save_end;                   /* misc data save end */

    U08ET upForce[UP_FORCE_SAMP];     /* record open  force value */
    U08ET dnForce[DN_FORCE_SAMP];     /* record close force value */
}GLB_CTRL;

extern GLB_CTRL glb_ctrl;

/*-----------------------------------------------------------------*/
/* Define system debug mode                                        */
/*-----------------------------------------------------------------*/
#define DBG_LV_NONE     0
#define DBG_LV_ADCVAL   (1<<0)
#define DBG_LV_PROCESS  (0<<0)
#define DBG_LV_CURRENT  (DBG_LV_PROCESS)


/* 2017/6/17 星期六 上午 9:15:48 limit sensor skew detect----------*/
#define LIMIT_SEN_ERR   (1)
/* 2017/6/17 星期六 上午 9:15:48 limit sensor skew detect----------*/
#define SYS_ESTIMATE   (0)
#define AUTO_STOP      (30000) /*define  auto stop time */

#endif /* _GLOBALS_H */
