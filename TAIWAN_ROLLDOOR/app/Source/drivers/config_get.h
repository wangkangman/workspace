#ifndef _CONFIG_GET_H
#define _CONFIG_GET_H

#include "global.h"
/**
  ******************************************************************************
  * @file    keysmp.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/8/10
  * @brief   This file provides all application of config_get,
  *          functionalities of the install mode and force level:
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
#define CONFIG_ST_LOW (0)
#define CONFIG_ST_HIGH (1)
#define CONFIG_ST_MAX (2)

/**
  * @brief  Define KEYS ID
  *
  */
#define CONFIG_R_F (0)
#define CONFIG_LEVEL2 (1)
#define CONFIG_LEVEL1 (2)
#define CONFIG_LEVEL0 (3)
#define CONFIG_MAX (4)


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
} ETST_CONFIGS;

/**
  * @brief  Function declare
  *
  */
VODET configs_init(VODET);
VODET configs_task(VODET);
ENUM_MOTOR_INSTALL_nMODE config_install_rf(VODET);
U08ET config_force_level_set(VODET);

#endif /* _KEYSMP_H */
