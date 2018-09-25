#ifndef _ADCFUNC_H
#define _ADCFUNC_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define ADC parameter                                            */
/*-----------------------------------------------------------------*/
#define ADC_CH_XXX   (0)
#define ADC_CH_MOTOR (1)
#define ADC_CH_KEY   (2)
#define ADC_CH_POS   (3)
#define ADC_CH_MAX   (4)

#define ADC_FILT_MAX (5)

/*-----------------------------------------------------------------*/
/* Define ADC control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    U16ET result;               /* ADC result after filtering */
    U16ET value[ADC_FILT_MAX];  /* latest ADC_FILT_MAX times convert value */
    U16ET ready;
    U16ET index;
}ETST_ADCS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void adcsmp_init(void);
void adcsmp_task(void);
U16ET getadc_data(U16ET chan);
U08ET force_get(void);
#endif /* _ADCSMP_H */
