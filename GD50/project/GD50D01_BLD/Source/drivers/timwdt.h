#ifndef _TIMWDT_H
#define _TIMWDT_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define task skeme control structure                             */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET count;
    U08ET cycle;
}ETST_TASKSKM;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET timwdt_init(VODET);
VODET timwdt_stop(VODET);
U08ET tskskm_flag(VODET);
VODET wdt_reset(VODET);

#endif /* _TIMWDT_H */
