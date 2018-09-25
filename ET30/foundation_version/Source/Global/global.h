
#ifndef _GLOBAL_H
#define _GLOBAL_H

#pragma sfr
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP

typedef signed   char  S08ET;
typedef signed   short S16ET;
typedef signed   long  S32ET;
typedef unsigned char  U08ET;
typedef unsigned short U16ET;
typedef unsigned long  U32ET;
typedef void   (*FUNC)(void);

#define ET_TRUE        (1)
#define ET_FALSE       (0)

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


#define UP_FORCE_SAMP         (128)   /* open door force sampling points */
#define DN_FORCE_SAMP         (256)   /* close door force sampling points */
//#define UP_FORCE_SAMP         (1)   /* open door force sampling points */
//#define DN_FORCE_SAMP         (1)   /* close door force sampling points */

//#define DN_FORCE_SAMP         (128)
typedef struct
{
    U16ET save_flg;                   /* saved indication flag */
    U16ET posval_top;                 /* top side Encoder value */
    U16ET posval_bot;                 /* bottom side Encoder value */
    U08ET inertia_top;		      /* the inertia*2/3 of top side */
    U08ET inertia_bot;		      /* the inertia/2/3 of bot side */
    
//    U16ET posval_cur;               /* current position hall value */
    U08ET uforce_max;                 /* max force value in open duration */
    U08ET dforce_max;                 /* max force value in close duration */
    U08ET force_lv;                   /* force control level */
    U08ET auto_clo;                   /* auto close minutes */
    U08ET infra;		      /* If the infra fuction if valid*/
    U08ET LR_set;    		      /* If the install mode is left or right */
    
    U08ET upForce[UP_FORCE_SAMP];     /* record open  force value */
    U08ET dnForce[DN_FORCE_SAMP];     /* record close force value */
}GLB_CTRL;

extern GLB_CTRL glb_ctrl;

//#define UART_DEBUG
#define EEP_SAVE_FLG (9527)
#define PRG_MAIN_VER (0)

#endif