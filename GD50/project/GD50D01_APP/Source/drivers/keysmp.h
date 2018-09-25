#ifndef _KEYSMP_H
#define _KEYSMP_H

#include "global.h"
/**
  ******************************************************************************
  * @file    keysmp.h
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
  * @brief  Define KEYS STATUS
  *
  */
#define KEY_ST_LOW (0)
#define KEY_ST_HIGH (1)
#define KEY_ST_MAX (2)

/**
  * @brief  Define KEYS ID
  *
  */
#define KEY_D_POWDN (0)
#define KEY_D_OVERI (1)
#define KEY_A_SET (2)
#define KEY_A_CODE (3)
#define KEY_A_UP (4)
#define KEY_A_DOWN (5)
#define KEY_P_INFRA (6)
#define KEY_MAX (7)

/**
  * @brief  define adc_val of key 
  *
  */
#define KEY_SET_ADC_VAL (102)
#define KEY_CODE_ADC_VAL (52)
#define KEY_UP_ADC_VAL (206)
#define KEY_DOWN_ADC_VAL (152)
#define KEY_XX_ADC_WRAP (8)

/**
  * @brief  infra parameter;
  *
  */
#define INFRA_CYC (400) // 20ms
#define INFRA_JUDGE (2) // at lest H2L twice

/**
  * @brief  Define KEYS control structure
  *
  */
typedef struct
{
  U08ET lstst : 2; /* last key status KEY_ST_HIGH/KEY_ST_LOW */
  U08ET curst : 2; /* current key status KEY_ST_HIGH/KEY_ST_LOW */
  U08ET stabe : 2; /* stable status KEY_ST_HIGH/KEY_ST_LOW */
  U08ET actlv : 2; /* active level KEY_ST_HIGH/KEY_ST_LOW */
  U08ET cycle : 8; /* control cycle, unit of task dispatch 2ms */
  U08ET count : 8; /* cycle count */
} ETST_KEYS;

/**
  * @brief  Define INFRA control structure 
  *
  */
typedef struct
{
  U16ET time_count;
  U08ET H2L_count;
  U08ET lstst : 2;
  U08ET curst : 2;
  U08ET flag : 2;
} ETST_INFRA;

/**
  * @brief  Function declare
  *
  */
VODET keysmp_init(VODET);
VODET keysmp_task(VODET);
U08ET getkey_curst(U08ET keys);
U08ET getkey_stabe(U08ET keys);
VODET infra_samp(VODET);

#endif /* _KEYSMP_H */
