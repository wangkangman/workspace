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

#define LED_ID_REMOTE (0)
#define LED_ID_MAX    (1)

#if 0
#define LED_ID_LAMP (0)
#define LED_ID_LED1 (1)
#define LED_ID_LED2 (2)
#define LED_ID_LED3 (3)
#define LED_ID_LED4 (4)
#define LED_ID_MAX  (5)
#endif

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
