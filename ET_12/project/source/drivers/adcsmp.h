#ifndef _ADCSMP_H
#define _ADCSMP_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define ADC control parameter                                    */
/*-----------------------------------------------------------------*/
#define ADC_FILT_MAX (5)

#define ADC_CH_MOT   (0)
#define ADC_CH_MAX   (1)

/*-----------------------------------------------------------------*/
/* Define ADC control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET result;
    U08ET value[ADC_FILT_MAX];
    U08ET ready;
    U08ET index;
}ETST_ADCS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET adcsmp_init(VODET);
VODET adcsmp_task(VODET);
U08ET getadc_data(U08ET chan);

#endif /* _ADCSMP_H */
