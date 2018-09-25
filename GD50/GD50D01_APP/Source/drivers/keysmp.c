#include "regdef.h"
#include "keysmp.h"
#include "adcsmp.h"
#include "ledbuzlamp.h"
/**
  ******************************************************************************
  * @file    keysmp.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/1/10
  * @brief   This file provides all application of keys infra, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + all general input process.
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
  * @brief  local variable
  *
  */
static ETST_KEYS et_keys[KEY_MAX - 1];
static ETST_INFRA et_infra;

/**
  * @brief  keyAD Initalize;
  * @param  
  *   none;
  * @retval 
  *   none;
  *
  */
VODET keysmp_init(VODET)
{
  U08ET index;
  /* KEY_INFRA         <--> P0.3 */
  GPIO_P0_PMD &= ~(1 << 6);
  GPIO_P0_PMD &= ~(1 << 7);
  /* KEY_OVER_CURRENT  <--> P3.2 */
  GPIO_P3_PMD &= ~(1 << 4);
  GPIO_P3_PMD &= ~(1 << 5);
  /* KEY_POWER_DOWN    <--> P4.0 */
  GPIO_P4_PMD &= ~(1 << 0);
  GPIO_P4_PMD &= ~(1 << 1);
  /* initialize keys control structure */
  for (index = 0; index < KEY_MAX - 1; index++)
  {
    et_keys[index].lstst = KEY_ST_MAX;
    et_keys[index].curst = KEY_ST_MAX;
    et_keys[index].stabe = KEY_ST_MAX;
    et_keys[index].cycle = 20;
    et_keys[index].count = 0;
  }
  et_keys[KEY_D_POWDN].cycle = 30;
  et_keys[KEY_D_OVERI].cycle = 3;
  et_keys[KEY_D_POWDN].actlv = KEY_ST_HIGH;
  et_keys[KEY_D_OVERI].actlv = KEY_ST_HIGH;
  et_keys[KEY_A_SET].actlv   = KEY_ST_LOW;
  et_keys[KEY_A_CODE].actlv  = KEY_ST_LOW;
  et_keys[KEY_A_UP].actlv    = KEY_ST_LOW;
  et_keys[KEY_A_DOWN].actlv  = KEY_ST_LOW;
  /* initalize parameter for infra contrl */
  et_infra.time_count = 0;
  et_infra.H2L_count = 0;
  et_infra.flag = 0;
}

/**
  * @brief  keyAD Task called by system;
  * @param  
  *   none;
  * @retval
  *   none;
  *
  */
VODET keysmp_task(VODET)
{
  U08ET index, adcKey;
  /* KEY_Digit get value */
  et_keys[KEY_D_POWDN].curst = PORT_READ(ET_PORT4, 0);
  et_keys[KEY_D_OVERI].curst = PORT_READ(ET_PORT3, 2);
  /* KEY_AD get value    */
  adcKey = getadc_data(ADC_CH_KEY);
  et_keys[KEY_A_SET].curst  = KEY_ST_HIGH;
  et_keys[KEY_A_CODE].curst = KEY_ST_HIGH;
  et_keys[KEY_A_UP].curst   = KEY_ST_HIGH;
  et_keys[KEY_A_DOWN].curst = KEY_ST_HIGH;
  
  if ((adcKey <= KEY_SET_ADC_VAL + KEY_XX_ADC_WRAP)
  	 && (adcKey >= KEY_SET_ADC_VAL - KEY_XX_ADC_WRAP))
    et_keys[KEY_A_SET].curst = KEY_ST_LOW;
  else if ((adcKey <= KEY_CODE_ADC_VAL + KEY_XX_ADC_WRAP)
  	      && (adcKey >= KEY_CODE_ADC_VAL - KEY_XX_ADC_WRAP))
    et_keys[KEY_A_CODE].curst = KEY_ST_LOW;
  else if ((adcKey <= KEY_UP_ADC_VAL + KEY_XX_ADC_WRAP)
  	      && (adcKey >= KEY_UP_ADC_VAL - KEY_XX_ADC_WRAP))
    et_keys[KEY_A_UP].curst = KEY_ST_LOW;
  else if ((adcKey <= KEY_DOWN_ADC_VAL + KEY_XX_ADC_WRAP)
  	      && (adcKey >= KEY_DOWN_ADC_VAL - KEY_XX_ADC_WRAP))
    et_keys[KEY_A_DOWN].curst = KEY_ST_LOW;
    
  for (index = 0; index < KEY_MAX - 1; index++)
  {
    if (et_keys[index].curst != et_keys[index].lstst)
    {
      et_keys[index].count = 0;
      et_keys[index].stabe = KEY_ST_MAX;
    }
    else
    {
      if (et_keys[index].count < et_keys[index].cycle)
        et_keys[index].count++;
      else if (et_keys[index].count == et_keys[index].cycle)
      {
        et_keys[index].stabe = et_keys[index].curst;
        et_keys[index].count++;
      }
    }
    et_keys[index].lstst = et_keys[index].curst;
  }
}

/**
  * @brief  GET current keys status;
  * @param  
  *   id : key port  ID
  * @retval 
  *   HIGH or LOW
  *
  */
U08ET getkey_curst(U08ET id)
{
	if (id >= KEY_MAX)
		return ET_FALSE;

  if (KEY_P_INFRA != id)
  {
    if (et_keys[id].curst == et_keys[id].actlv)
      return KEY_ST_HIGH;
    else
      return KEY_ST_LOW;
  }
  else
    return et_infra.flag;
}

/**
  * @brief  GET stable INPUT status;
  * @param  
  *   id : input port  ID
  * @retval 
  *   HIGH or LOW
  *
  */
U08ET getkey_stabe(U08ET id)
{
  if (id >= KEY_MAX)
    return ET_FALSE;	

  if (KEY_P_INFRA != id)
  {
    if (et_keys[id].stabe == et_keys[id].actlv)
    {
      et_keys[id].stabe = KEY_ST_MAX;
      return KEY_ST_HIGH;
    }
    else if (et_keys[id].stabe == KEY_ST_MAX)
      return KEY_ST_MAX;
    else
    {
      et_keys[id].stabe = KEY_ST_MAX;
      return KEY_ST_LOW;
    }
  }
  else
    return et_infra.flag;
}

/**
  * @brief infra task performed by timer<50us>;
  * @param  
  *   none;
  * @retval
  *   none;
  *
  */
VODET infra_samp(VODET)
{
  et_infra.lstst = et_infra.curst;
  et_infra.curst = PORT_READ(ET_PORT0, 3);
  if (et_infra.time_count < INFRA_CYC)
  {
    ++et_infra.time_count;
    if ((KEY_ST_LOW == et_infra.curst)
    	 && (KEY_ST_HIGH == et_infra.lstst))
      if (et_infra.H2L_count < 0x3f)
        ++et_infra.H2L_count;
  }
  else
  {
    if (et_infra.H2L_count > INFRA_JUDGE)
      et_infra.flag = 0;
    else
      et_infra.flag = 1;

    et_infra.time_count = 0;
    et_infra.H2L_count = 0;
  }
}
