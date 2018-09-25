#ifndef _ADCSMP_H
#define _ADCSMP_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define ADC control parameter                                    */
/*-----------------------------------------------------------------*/
#define ADC_FILT_MAX (5)

/*-----------------------------------------------------------------*/
/* Define ADC control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    U16ET result;
    U16ET value[ADC_FILT_MAX];
    U16ET ready;
    U16ET index;
}ETST_ADCS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET adcsmp_init(VODET);
VODET adcsmp_task(VODET);
U16ET getadc_data(VODET);

#endif /* _ADCSMP_H */
