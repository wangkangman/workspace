#include "regdef.h"
#include "clkrst.h"
#include "adcsmp.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_ADCS et_adcs;

/*-----------------------------------------------------------------*/
/* FUNC : ADC initialize                                           */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET adcsmp_init(VODET)
{
    /* disable pin digital input path */
    GPIO_P1_OFFD |= (1<<20);

    /* AIN4 <--> P1.4 */
    GCR_P1MFP |=  (1<<4);
    GCR_P1MFP &= ~(1<<12);

    /* select HIRC as ADC clock source */
    CLK_SEL1 |= (3<<2);

    /* set ADC clock divider to 22 */
    CLK_CLKDIV &= ~(0xFF<<16);
    CLK_CLKDIV |=  (21<<16);

    /* enable ADC clock */
    CLK_APB |=  (1<<28);

    /* select single end mode */
    ADC_ADCR &= ~(1<<10);

    /* select single conversion mode */
    ADC_ADCR &= ~(3<<2);

    /* select channel 4 */
    ADC_ADCHER |= (1<<4);

    /* enable ADC unit */
    ADC_ADCR |= (1<<0);

    /* clear ADC conversion end flag */
    ADC_ADSR |= (1<<0);

    /* start ADC conversion */
    ADC_ADCR |= (1<<11);
}

/*-----------------------------------------------------------------*/
/* FUNC : ADC task function                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET adcsmp_task(VODET)
{
    U16ET value;
    U16ET min,max,index;

    /* ADC conversion not complete */
    if( 0 == (ADC_ADSR & (1<<0)) ) {
        return ;
    }

    /* get adc convert value */
    et_adcs.value[et_adcs.index] = ADC_ADDR4;

    /* check adc filt ring buff if first full after adc_reset_buff */
    if( (0 == et_adcs.ready) && (ADC_FILT_MAX-1 == et_adcs.index) ) {
        et_adcs.ready = 1;
    }
    et_adcs.index = (et_adcs.index + 1)%ADC_FILT_MAX;

    /* adc filtering */
    min = et_adcs.value[0];
    max = et_adcs.value[0];
    value = et_adcs.value[0];
    for(index=1; index<ADC_FILT_MAX; index++) {
        if( et_adcs.value[index] < min ) {
            min = et_adcs.value[index];
        }
        if( et_adcs.value[index] > max ) {
            max = et_adcs.value[index];
        }
        value += et_adcs.value[index];
    }
    value -= min;
    value -= max;
    et_adcs.result = value / (ADC_FILT_MAX-2);

    /* if adc filt ring buff is not full, filt result should be ignored */
    if(0 == et_adcs.ready) {
        et_adcs.result = 0;
    }

    /* clear ADC conversion end flag */
    ADC_ADSR |= (1<<0);

    /* start ADC conversion */
    ADC_ADCR |= (1<<11);
}

/*-----------------------------------------------------------------*/
/* FUNC : Get current adc value                                    */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return current adc value                                      */
/*-----------------------------------------------------------------*/
U16ET getadc_data(VODET)
{
    return et_adcs.result;
}
