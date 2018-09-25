#include "config_get.h"
#include "regdef.h"

/**
  ******************************************************************************
  * @file    config_get.c
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
 * @brief  local variable
 *
 */
static ETST_CONFIGS et_configs[CONFIG_MAX];

/**
 * @brief  keyAD Initalize;
 * @param
 *   none;
 * @retval
 *   none;
 *
 */
VODET configs_init(VODET) {
  U08ET index;

  /* install_rf_set  <--> P1.1 */
  GPIO_P1_PMD &= ~((U32ET)1 << 2);
  GPIO_P1_PMD &= ~((U32ET)1 << 3);
  /* force_level_2   <--> P1.0 */
  GPIO_P1_PMD &= ~((U32ET)1 << 0);
  GPIO_P1_PMD &= ~((U32ET)1 << 1);
  /* force_level_1   <--> P0.0 */
  GPIO_P0_PMD &= ~((U32ET)1 << 0);
  GPIO_P0_PMD &= ~(U32ET)(1 << 1);
  /* force_level_1   <--> P0.1 */
  GPIO_P0_PMD &= ~((U32ET)1 << 2);
  GPIO_P0_PMD &= ~((U32ET)1 << 3);
  /* initialize keys control structure */
  for (index = 0; index < CONFIG_MAX; index++) {
    et_configs[index].lstst = CONFIG_ST_MAX;
    et_configs[index].curst = CONFIG_ST_MAX;
    et_configs[index].stabe = CONFIG_ST_MAX;
    et_configs[index].count = 0;
  }
  et_configs[CONFIG_R_F].cycle = 20;
  et_configs[CONFIG_LEVEL2].cycle = 20;
  et_configs[CONFIG_LEVEL1].cycle = 20;
  et_configs[CONFIG_LEVEL0].cycle = 20;

  et_configs[CONFIG_R_F].actlv = CONFIG_ST_LOW;
  et_configs[CONFIG_LEVEL2].actlv = CONFIG_ST_LOW;
  et_configs[CONFIG_LEVEL1].actlv = CONFIG_ST_LOW;
  et_configs[CONFIG_LEVEL0].actlv = CONFIG_ST_LOW;
}

/**
 * @brief  keyAD Task called by system;
 * @param
 *   none;
 * @retval
 *   none;
 *
 */
VODET configs_task(VODET) {
  U08ET index;
  /* KEY_Digit get value */
  et_configs[CONFIG_R_F].curst = PORT_READ(ET_PORT1, 1);
  et_configs[CONFIG_LEVEL2].curst = PORT_READ(ET_PORT1, 0);
  et_configs[CONFIG_LEVEL1].curst = PORT_READ(ET_PORT0, 0);
  et_configs[CONFIG_LEVEL0].curst = PORT_READ(ET_PORT0, 1);

  for (index = 0; index < CONFIG_MAX; index++) {
    if (et_configs[index].curst != et_configs[index].lstst) et_configs[index].count = 0;
    else {
      if (et_configs[index].count < et_configs[index].cycle) et_configs[index].count++;
      else if (et_configs[index].count == et_configs[index].cycle) {
        et_configs[index].stabe = et_configs[index].curst;
        et_configs[index].count++;
      }
    }
    et_configs[index].lstst = et_configs[index].curst;
  }
}

/**
 * @brief  GET stable INPUT status;
 * @param
 *   id : input port  ID
 * @retval
 *   HIGH or LOW
 *
 */
static U08ET getkey_stabe(U08ET id) {
  U08ET res;
  if (id >= CONFIG_MAX) res = CONFIG_ST_LOW;
  else {
    if (et_configs[id].stabe == et_configs[id].actlv) res = CONFIG_ST_HIGH;
    else res = CONFIG_ST_LOW;
  }
  return res;
}

/**
 * @brief  GET Install mode left or right;
 * @param
 *   NONE
 * @retval
 *   mode_left or mode_right
 *
 */
ENUM_MOTOR_INSTALL_nMODE config_install_rf(VODET){
  return ((CONFIG_ST_HIGH == getkey_stabe(CONFIG_R_F)) ? INSTALL_RIGHT : INSTALL_LEFT);
}



/**
 * @brief  GET force level;
 * @param
 *   NONE
 * @retval
 *   force level 0~7
 *
 */
U08ET config_force_level_set(VODET){
  return ((U08ET)(getkey_stabe(CONFIG_LEVEL2) << 2) | (U08ET)(getkey_stabe(CONFIG_LEVEL1) << 1) | (U08ET)getkey_stabe(CONFIG_LEVEL0));
}

