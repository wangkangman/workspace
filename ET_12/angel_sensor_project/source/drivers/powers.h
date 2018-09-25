#ifndef _POWERS_H
#define _POWERS_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define AC power parameter                                       */
/*-----------------------------------------------------------------*/
#define AC_POW_PEROID (5) /* 50Hz 60Hz signal max period is 20ms */

/*-----------------------------------------------------------------*/
/* Define power control structure                                  */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET lstst:1;   /* last AC power dectect pin status */
    U08ET curst:1;   /* current AC power dectect pin status */
    U08ET count:6;   /* AC power status counter */
}ETST_ACS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET acpowr_task(VODET);
U08ET powsts_flag(VODET);
VODET powers_init(VODET);

#endif /* _POWERS_H */
