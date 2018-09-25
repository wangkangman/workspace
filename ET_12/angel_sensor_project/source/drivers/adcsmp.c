#include "regdef.h"
#include "clkrst.h"
#include "adcsmp.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_ADCS et_adcs[ADC_CH_MAX];

/*-----------------------------------------------------------------*/
/* Local function declare                                          */
/*-----------------------------------------------------------------*/
static U08ET getadc_chan(VODET);
static VODET adcena_chan(U08ET chan);

/*-----------------------------------------------------------------*/
/* FUNC : ADC initialize                                           */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET adcsmp_init(VODET)
{
    U08ET index;

    /* ADC0  <--> PB0 */
    GPIO_PB_DDR &= ~(1<<0);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<0);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<0);  /* max speed 2MHz   */

    /* enable ADC clock */
    sysclk_ctrl(CLK_ID_ADC, CLK_CT_ENA);

    /* data left alignment <-> 8 MSB bits on ADC_DRH, 2 LSB bits on ADC_DRL */
    ADC_CR2 &= ~(1<<3);

    /* disable schmitt trigger of channel 0 */
    ADC_TDRL |= (1<<0);

    /* use maximum prescaler division of adc clock */
    ADC_CR1 |= (7<<4);

    /* reset adc sample structure */
    et_adcs[0].result = 0;
    et_adcs[0].index  = 0;
    et_adcs[0].ready  = 0;
    for (index = 0; index < ADC_FILT_MAX; index++)
    {
        et_adcs[0].value[index] = 0;
    }

    /* enable motor adc channel as default */
    adcena_chan(ADC_CH_MOT);
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
    U08ET chan,min,max,index;

    /* ADC conversion not complete */
    if (0 == (ADC_CSR & (1<<7)))
    {
        return;
    }

    /* get current adc channel */
    chan = getadc_chan();

    /* get adc convert value, we ignore the 2 LSB bit */
    et_adcs[chan].value[et_adcs[chan].index] = ADC_DRH;

    /* check adc filt ring buff if first full after adc_reset_buff */
    if ((0 == et_adcs[chan].ready) && \
    	  (ADC_FILT_MAX-1 == et_adcs[chan].index)) {
        et_adcs[chan].ready = 1;
    }
    et_adcs[chan].index = (et_adcs[chan].index + 1)%ADC_FILT_MAX;

    /* adc filtering */
    min = et_adcs[chan].value[0];
    max = et_adcs[chan].value[0];
    value = et_adcs[chan].value[0];
    for (index = 1; index < ADC_FILT_MAX; index++) {
        if (et_adcs[chan].value[index] < min) {
            min = et_adcs[chan].value[index];
        }
        if (et_adcs[chan].value[index] > max) {
            max = et_adcs[chan].value[index];
        }
        value += et_adcs[chan].value[index];
    }
    value -= min;
    value -= max;
    et_adcs[chan].result = value / (ADC_FILT_MAX-2);

    /* if adc filt ring buff is not full, filt result should be ignored */
    if (0 == et_adcs[chan].ready) {
        et_adcs[chan].result = 0;
    }

    /* clear ADC complete flag */
    ADC_CSR &= ~(1<<7);

    /* select new channel */
    adcena_chan(ADC_CH_MOT);
}

/*-----------------------------------------------------------------*/
/* FUNC : Get current adc value                                    */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return current adc value                                      */
/*-----------------------------------------------------------------*/
U08ET getadc_data(U08ET chan)
{
    /* undefined ADC channel */
    if (chan >= ADC_CH_MAX)
    {
        return ;
    }

    return et_adcs[chan].result;
}

/*-----------------------------------------------------------------*/
/* FUNC : Get current adc channel                                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return adc channel, can be ADC_CH_MOT or ADC_CH_KEY           */
/*-----------------------------------------------------------------*/
static U08ET getadc_chan(void)
{
    if (0x00 == (ADC_CSR&0x0F))
    {
        return ADC_CH_MOT;
    }
    else
    {
        return ADC_CH_MAX;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Enable adc channel                                       */
/* PARA :                                                          */
/*   chan : adc channel, can be ADC_CH_MOT or ADC_CH_KEY           */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static void adcena_chan(U08ET chan)
{
    U08ET delay;

    /* select channel */
    ADC_CSR &= ~(0x0F);
    switch (chan)
    {
        case ADC_CH_MOT:
            ADC_CSR |=  (0x00);
            break;
        default:
            break;
    }

    /* enbale adc */
    if (0 == (ADC_CR1 & 0x01))
    {
        ADC_CR1 |= (1<<0);
        for(delay=0; delay<40; delay++);
    }
    ADC_CR1 |= (1<<0);
}
