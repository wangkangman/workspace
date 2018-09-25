#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "errlist.h"
#include "debug.h"

/*-----------------------------------------------------------------*/
/* Define our own data types xxxET(ET Door data types)             */
/*-----------------------------------------------------------------*/
#ifndef _XXXET_
#define _XXXET_

typedef void (*FUNC)(void);

typedef signed char S08ET;
typedef signed short S16ET;
typedef signed long S32ET;
typedef unsigned char U08ET;
typedef unsigned short U16ET;
typedef unsigned long U32ET;
typedef signed int INTET;
typedef float FLTET;
#define VODET void
#endif

#define ET_TRUE (1)
#define ET_FALSE (0)

#define RF (1)
#define SPI (1)
#define REMOTE_TYPE (SPI)

#define OFFSETOF(TYPE, MEMBER) ((U32ET)(&((TYPE *)0)->MEMBER))

typedef enum {
   MOTOR_NORMAL = (0),
   MOTOR_LEARN
}ENUM_MOTOR_IFLEARN_nMODE;

typedef enum {
   INSTALL_LEFT = (0),
   INSTALL_RIGHT
}ENUM_MOTOR_INSTALL_nMODE;


/*-----------------------------------------------------------------*/
/* Define our own common structure xxxET(ET Door common structure) */
/*-----------------------------------------------------------------*/
typedef struct
{
    U32ET bit0 : 1; //read flag
    U32ET bit1 : 1;
    U32ET bit2 : 1;
    U32ET bit3 : 1;
    U32ET bit4 : 1;
    U32ET bit5 : 1;
    U32ET bit6 : 1;
    U32ET bit7 : 1;
} BITET;

/*-----------------------------------------------------------------*/
/* Define eeprom parameter structure                               */
/*-----------------------------------------------------------------*/

#define UP_FORCE_SAMP (256) /* open door force sampling points */
#define DN_FORCE_SAMP (512) /* close door force sampling points */

__packed typedef struct
{
    U16ET save_flg;   /* saved indication flag */
    U16ET hall_top;   /* top side hall value */
    U16ET hall_bot;   /* bottom side hall value */
    U08ET ufor_max;   /* max force value in open duration */
    U08ET dfor_max;   /* max force value in close duration */
    ENUM_MOTOR_INSTALL_nMODE install_lr_mode;
    U08ET reserve_0;
    U08ET reserve_1;
    U08ET save_end;   /* misc data save end */

    U08ET upForce[UP_FORCE_SAMP]; /* record open  force value */
    U08ET dnForce[DN_FORCE_SAMP];
} GLB_CTRL;

extern GLB_CTRL glb_ctrl;

/*-----------------------------------------------------------------*/
/* Define system debug mode                                        */
/*-----------------------------------------------------------------*/
#define DBG_LV_NONE 0
#define DBG_LV_ADCVAL (1 << 0)
#define DBG_LV_PROCESS (1 << 1)
#define DBG_LV_CURRENT (DBG_LV_PROCESS)
#define UART2_FUNCTION

#endif /* _GLOBALS_H */
