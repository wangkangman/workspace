#ifndef _KEYSMP_H
#define _KEYSMP_H

#include "global.h"
/**
  ******************************************************************************
  * @file    keysmp.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/8/10
  * @brief   This file provides all application of keys infra, 
  *          functionalities of the inital and listen for uart:
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
#define KEY_BUTTON (0)
#define KEY_INFRA     (1)
#define KEY_POWER_DOWN (2)
#define KEY_PB_WALL (3)
#define KEY_MAX (4)

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
  * @brief  Function declare
  *
  */
VODET keysmp_init(VODET);
VODET keysmp_task(VODET);
U08ET getkey_curst(U08ET keys);
U08ET getkey_stabe(U08ET keys);
VODET infra_samp(VODET);

#endif /* _KEYSMP_H */
