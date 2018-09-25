#include "regdef.h"
#include "clkrst.h"
#include "adcsmp.h"

/**
  ******************************************************************************
  * @file    adcsmp.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/1/10
  * @brief   This file provides all application of clock, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + adc val capture and filter.
  *
 ===============================================================================
  
  ******************************************************************************
  * @attention
  *
  * &copy; COPYRIGHT 2018 ETdoor
  *
  *
  ******************************************************************************
 ===============================================================================
  
  ******************************************************************************
  * @verlog
  *
  * V0.0.0: original version;
  *
  *
  ******************************************************************************
  */
/**
  * @brief  Local function declare
  *
  */
static U08ET getadc_chan(VODET);
static VODET adcena_chan(U08ET chan);
static U16ET getadc_addr(U08ET chan);
/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_ADCS et_adcs[ADC_CH_MAX];
/**
  * @brief  ADC initialize 
  * @param  
  *   none;
  * @retval
  *   none;
  *
  */
VODET adcsmp_init(VODET)
{
  U08ET index_1, index_2;
  /* disable pin digital input path */
  GPIO_P1_OFFD |= (1 << 16);
  GPIO_P1_OFFD |= (1 << 17);
  GPIO_P1_OFFD |= (1 << 20);
  /* AIN0 <--> P1.0 */
  GCR_P1MFP |= (1 << 0);
  GCR_P1MFP &= ~(1 << 8);
  /* AIN1 <--> P1.1 */
  GCR_P1MFP |= (1 << 1);
  GCR_P1MFP &= ~(1 << 9);
  /* AIN4 <--> P1.4 */
  GCR_P1MFP |= (1 << 4);
  GCR_P1MFP &= ~(1 << 12);
  /* select HCLK as ADC clock source */
  CLK_SEL1 |= (2 << 2);
  /* set ADC clock divider to 22 */
  CLK_CLKDIV &= ~(0xFF << 16);
  CLK_CLKDIV |= (21 << 16);
  /* enable ADC clock */
  CLK_APB |= (1 << 28);
  /* select single end mode */
  ADC_ADCR &= ~(1 << 10);
  /* select single conversion mode */
  ADC_ADCR &= ~(3 << 2);
  /* enable ADC unit */
  ADC_ADCR |= (1 << 0);
  /* clear ADC conversion end flag */
  ADC_ADSR |= (1 << 0);
  /* reset adc sample structure */
  for (index_1 = 0; index_1 < ADC_CH_MAX; ++index_1)
  {
    et_adcs[index_1].result = 0;
    et_adcs[index_1].index = 0;
    et_adcs[index_1].ready = 0;
    for (index_2 = 0; index_2 < ADC_FILT_MAX; ++index_2)
      et_adcs[index_1].value[index_2] = 0;
  }
  adcena_chan(ADC_CH_MOT);
  /* start ADC conversion */
  ADC_ADCR |= (1 << 11);
}

/**
  * @brief  ADC task function 
  * @param  
  *   none;
  * @retval
  *   none;
  *
  */
VODET adcsmp_task(VODET)
{
  U16ET value;
  U16ET chan, min, max, index;
  /* ADC conversion not complete */
  if (0 == (ADC_ADSR & (1 << 0)))
    return;
  /* get current adc channel */
  chan = getadc_chan();
  /* get adc convert value, we ignore the 2 LSB bit */
  et_adcs[chan].value[et_adcs[chan].index] = getadc_addr(chan);
  /* check adc filt ring buff if first full after adc_reset_buff */
  if ((0 == et_adcs[chan].ready)
     && ((ADC_FILT_MAX - 1) == et_adcs[chan].index))
    et_adcs[chan].ready = 1;
	
  et_adcs[chan].index = (et_adcs[chan].index + 1)
                        % ADC_FILT_MAX;
  /* adc filtering */
  min = et_adcs[chan].value[0];
  max = et_adcs[chan].value[0];
  value = et_adcs[chan].value[0];
  for (index = 1; index < ADC_FILT_MAX; index++)
  {
    if (et_adcs[chan].value[index] < min)
      min = et_adcs[chan].value[index];
		
    if (et_adcs[chan].value[index] > max)
      max = et_adcs[chan].value[index];
		
    value += et_adcs[chan].value[index];
  }
  value -= min;
  value -= max;
  et_adcs[chan].result = value / (ADC_FILT_MAX - 2);
  /* if adc filt ring buff is not full, filt result should be ignored */
  if (0 == et_adcs[chan].ready)
    et_adcs[chan].result = 0;
  /* select new channel */
  adcena_chan((chan + 1) % ADC_CH_MAX);
  /* clear ADC conversion end flag */
  ADC_ADSR |= (1 << 0);
  /* start ADC conversion */
  ADC_ADCR |= (1 << 11);
}

/**
  * @brief  Get current adc value  
  * @param  
  *   none;
  * @retval 
  *   current adc value 
  *
  */
U08ET getadc_data(U08ET chan)
{
  return (et_adcs[chan].result >> 4);
}

/**
  * @brief  Get current adc channel
  * @param
  *   none;
  * @retval 
  *   U08ET: current adc channel, can be ADC_CH_MOT or ADC_CH_KEY 
  *
  */
static U08ET getadc_chan(void)
{
  if ((1 << 0) == (ADC_ADCHER & 0xFF))
    return ADC_CH_MOT;
  else if ((1 << 4) == (ADC_ADCHER & 0xFF))
    return ADC_CH_KEY;
  else if ((1 << 1) == (ADC_ADCHER & 0xFF))
    return ADC_CH_VOL;
  else
    return ADC_CH_MAX;
}

/**
  * @brief  Get current adc ADDR data
  * @param
  *   chan;
  * @retval 
  *   U16ET: current adr addr data
  *
  */
static U16ET getadc_addr(U08ET chan)
{
  if (ADC_CH_MOT == chan)
    return ADC_ADDR0;
  else if (ADC_CH_KEY == chan)
    return ADC_ADDR4;
  else if (ADC_CH_VOL == chan)
    return ADC_ADDR1;
  else
    return ADC_CH_MAX;
}

/**
  * @brief  Enable adc channel 
  * @param
  *   chan : adc channel, can be ADC_CH_MOT or ADC_CH_KEY;
  * @retval 
  *   none;
  *
  */
static void adcena_chan(U08ET chan)
{
  /* disable convert*/
  ADC_ADCR &= ~(1 << 11);
  /* select channel */
  ADC_ADCHER &= ~(0xff);
  switch (chan)
  {
  case ADC_CH_MOT:
    ADC_ADCHER |= (1 << 0);
    break;
  case ADC_CH_KEY:
    ADC_ADCHER |= (1 << 4);
    break;
  case ADC_CH_VOL:
    ADC_ADCHER |= (1 << 1);
    break;
  default:
    break;
  }
  /* enable convert */
  ADC_ADCR |= (1 << 11);
}
