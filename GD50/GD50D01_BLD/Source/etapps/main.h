#ifndef _MAIN_H
#define _MAIN_H

#include "global.h"
#include "clkrst.h"
#include "flashs.h"
#include "ledseg.h"
#include "adcsmp.h"
#include "keysmp.h"
#include "timwdt.h"
#include "uartct.h"

/*--------------------------------------------------------------*/
/* Define system control structure                              */
/*--------------------------------------------------------------*/
typedef enum
{
    SYS_ST_WAIT_EVT       = 0,        /* wait firmware program key event */
    SYS_ST_FILE_GET,                  /* get firmware file from programer */
    SYS_ST_PROG_END,                  /* firmware program finished */
    SYS_ST_INVALID
}SYS_STATUS;

typedef enum
{
    SYS_ST_SUB0           = 0,
    SYS_ST_SUB1,
    SYS_ST_SUB2,
    SYS_ST_SUB3,
    SYS_ST_SUB4,
    SYS_ST_SUB5,
    SYS_ST_SUB6,
    SYS_ST_SUB7,
    SYS_ST_SUB8,
    SYS_ST_SUB9,
}SUB_STATUS;

typedef void (FUNC_PTR)(void);

/*-----------------------------------------------------------------*/
/* Define system control structure                                 */
/*-----------------------------------------------------------------*/
typedef struct
{
    SYS_STATUS sys_sts;               /* current system status */
    SUB_STATUS sub_sts;               /* current system sub status */

    U32ET delay0;                     /* time delay */
    U32ET dis_sts;                    /* display status */

    U08ET rcv_pkt[640];               /* receive packet buff */
    U08ET snd_pkt[32];                /* send packet buff */
}SYS_CTRL;

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/
VODET sys_pre_init(VODET);
VODET sys_wait_evt(VODET);
VODET sys_file_get(VODET);
VODET sys_file_wrt(VODET);
VODET sys_prog_end(VODET);

#endif /* _MAIN_H */
