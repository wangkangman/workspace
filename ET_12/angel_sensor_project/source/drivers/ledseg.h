#ifndef _LEDSEG_H
#define _LEDSEG_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define LED status                                               */
/*-----------------------------------------------------------------*/
#define LED_ST_OFF  (0)
#define LED_ST_ON   (1)

/*-----------------------------------------------------------------*/
/* Define LED ID                                                   */
/*-----------------------------------------------------------------*/
#define LED_ID_SET  (0)
#define LED_ID_CODE (1)
#define LED_ID_LAMP (2)
#define LED_ID_MAX  (3)

/*-----------------------------------------------------------------*/
/* Define LED control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET stats;      /* led status */
    U16ET period;     /* led control period */
    U16ET count;      /* led period counter */
}ETST_LEDS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET ledseg_init(VODET);
VODET ledseg_task(VODET);
VODET ledlow_ctrl(U08ET ledid, U08ET stats);
VODET ledtsk_ctrl(U08ET ledid, U08ET stats, U32ET period);

#endif /* _LEDSEG_H */
