#ifndef _ADCSMP_H
#define _ADCSMP_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define ADC control parameter                                    */
/*-----------------------------------------------------------------*/
#define ADC_FILT_MAX (5)

#define ADC_CH_MOT (0)
#define ADC_CH_KEY (1)
#define ADC_CH_MAX (2)

/*-----------------------------------------------------------------*/
/* Define ADC control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 result;
    u8 value[ADC_FILT_MAX];
    u8 ready;
    u8 index;
} ETST_ADCS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void adcsmp_init(void);
void adcsmp_task(void);
u8 getadc_data(u8 chan);

#endif /* _ADCSMP_H */
