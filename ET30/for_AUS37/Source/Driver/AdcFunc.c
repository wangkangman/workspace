#include "regdef.h"
#include "ClkSet.h"
#include "AdcFunc.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_ADCS et_adcs[ADC_CH_MAX-1];

/*-----------------------------------------------------------------*/
/* Local function declare                                          */
/*-----------------------------------------------------------------*/
static void  adcsmp_chan(U16ET chan);
static U16ET getadc_chan(void);

/*-----------------------------------------------------------------*/
/* FUNC : ADC initialize                                           */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void adcsmp_init(void)
{
    U16ET index;

    /* enable ADC clock */
    clkset_ctrl(CLK_ID_ADC, CLK_CT_ENA);
    
    /* KEY channel <-> P22 ANI2 */
    PORT_PM2 |= 0x04U;
    
    /* KEY channel <-> P41 ANI22 */
    PORT_PMC4 |= (1<<1); /* analog port */
    PORT_PM4  |= (1<<1); /* input mode */

    /* KEY channel <-> P42 ANI21 */
    PORT_PMC4 |= (1<<2); /* analog port */
    PORT_PM4  |= (1<<2); /* input mode */


    /* stop AD conversion */
    ADC_ADM0 = 0x00;

    /* disable interrupt */
    INT_MK1L |= (1<<2);

    /* clear interrupt flag */
    INT_IF1L &= ~(1<<2);

    /* ADC clock 24MHz/16 = 1.5MHz */
    ADC_ADM0 |=  (2<<3);

    /* select standard mode 1, so convert time is 12.67us */
    ADC_ADM0 &= ~(3<<1);

    /* ADC select mode */
    ADC_ADM0 &= ~(1<<6);

    /* software trigger mode */
    ADC_ADM1 &= (U08ET)(~(3<<6));

    /* single conversion mode */
    ADC_ADM1 |= (1<<5);

    /* set ADC positive reference voltage to VDD */
    ADC_ADM2 &= (U08ET)(~(3<<6));

    /* set ADC negative reference voltage to VSS */
    ADC_ADM2 &= ~(1<<5);

    /* when ADLL<=ADCR<=ADUL, generate interrupt */
    ADC_ADM2 &= ~(1<<3);

    /* no snooze mode */
    ADC_ADM2 &= ~(1<<2);

    /* 10 bit ADC mode */
    ADC_ADM2 &= ~(1<<0);

    /* set maximum ADC convertion range */
    ADC_ADUL = 0xFF;
    ADC_ADLL = 0x00;

    /* reset adc sample structure */
    et_adcs[0].result = 0;
    et_adcs[0].index  = 0;
    et_adcs[0].ready  = 0;
    for(index=0; index<ADC_FILT_MAX; index++) {
        et_adcs[0].value[index] = 0;
    }
    et_adcs[1].result = 0;
    et_adcs[1].index  = 0;
    et_adcs[1].ready  = 0;
    for(index=0; index<ADC_FILT_MAX; index++) {
        et_adcs[1].value[index] = 0;
    }
    et_adcs[2].result = 0;
    et_adcs[2].index  = 0;
    et_adcs[2].ready  = 0;
    for(index=0; index<ADC_FILT_MAX; index++) {
        et_adcs[2].value[index] = 0;
    }

    /* start to sample motor channel */
    adcsmp_chan(ADC_CH_MOTOR);
}

/*-----------------------------------------------------------------*/
/* FUNC : ADC task dispatch function                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void adcsmp_task(void)
{
    U16ET value;
    U16ET chan,min,max,index;

    /* ADC conversion not complete */
    if( 0 != (ADC_ADM0 & 0x80) ) {
        return;
    }

    /* get current adc channel */
    chan = getadc_chan() - 1;

    /* get adc convert value, 10 bit */
    et_adcs[chan].value[et_adcs[chan].index] = ADC_ADCR >> 6;

    /* check adc filt ring buff if first full after adc_reset_buff */
    if( (0 == et_adcs[chan].ready) && (ADC_FILT_MAX-1 == et_adcs[chan].index) ) {
        et_adcs[chan].ready = 1;
    }
    et_adcs[chan].index = (et_adcs[chan].index + 1)%ADC_FILT_MAX;

    /* adc filtering */
    min = et_adcs[chan].value[0];
    max = et_adcs[chan].value[0];
    value = et_adcs[chan].value[0];
    for(index=1; index<ADC_FILT_MAX; index++) {
        if( et_adcs[chan].value[index] < min ) {
            min = et_adcs[chan].value[index];
        }
        if( et_adcs[chan].value[index] > max ) {
            max = et_adcs[chan].value[index];
        }
        value += et_adcs[chan].value[index];
    }
    value -= min;
    value -= max;
    et_adcs[chan].result = value / (ADC_FILT_MAX-2);

    /* if adc filt ring buff is not full, filt result should be ignored */
    if(0 == et_adcs[chan].ready) {
        et_adcs[chan].result = 0;
    }

    /* select new channel */
    if(ADC_CH_MOTOR == chan+1) {
        adcsmp_chan(ADC_CH_KEY);
    } 
    else if (ADC_CH_KEY == chan+1){
    	adcsmp_chan(ADC_CH_POS);    
    }
    else {
        adcsmp_chan(ADC_CH_MOTOR);
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Get current adc value                                    */
/* PARA :                                                          */
/*   chan : adc channel, can be ADC_CH_MOTOR or ADC_CH_KEY         */
/* RETN :                                                          */
/*   return adc result value                                       */
/*-----------------------------------------------------------------*/
U16ET getadc_data(U16ET chan) 
{
    /* undefined ADC channel */
    if( (chan<=ADC_CH_XXX) || (chan>=ADC_CH_MAX) ) {
        return 0x00;
    }

    return et_adcs[chan-1].result;
}

/*-----------------------------------------------------------------*/
/* FUNC : Enable adc channel                                       */
/* PARA :                                                          */
/*   chan : adc channel, can be ADC_CH_MOTOR or ADC_CH_KEY         */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static void adcsmp_chan(U16ET chan) 
{
    U16ET delay;

    /* disable adc */
    ADC_ADM0 &= (U08ET)(~(1<<7));
    ADC_ADM0 &= (U08ET)(~(1<<0));

    /* select target adc channel */
    switch(chan) {
        case ADC_CH_MOTOR:
            ADC_ADS = 0x15;
            break;
        case ADC_CH_KEY:
            ADC_ADS = 0x16;
            break;
	case ADC_CH_POS:
	    ADC_ADS = 0x02;
	    break;
        default:
            break;
    }

    /* enbale adc */
    ADC_ADM0 |= (1<<0);
    for(delay=0; delay<10; delay++);
    ADC_ADM0 |= (1<<7);
}

/*-----------------------------------------------------------------*/
/* FUNC : Get current adc channel                                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return adc channel, can be ADC_CH_MOTOR or ADC_CH_KEY         */
/*-----------------------------------------------------------------*/
static U16ET getadc_chan(void)
{
    if( 0x15 == ADC_ADS ) {
        return ADC_CH_MOTOR;
    }
    else if( 0x16 == ADC_ADS ) {
        return ADC_CH_KEY;
    }
    else if( 0x02 == ADC_ADS ) {
        return ADC_CH_POS;
    }
    else {
        return ADC_CH_XXX;
    }
}

U08ET force_get(void){
    return getadc_data(ADC_CH_MOTOR) >> 2;
}