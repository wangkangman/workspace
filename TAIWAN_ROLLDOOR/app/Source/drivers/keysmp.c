#include "keysmp.h"
#include "adcsmp.h"
#include "ledbuzlamp.h"
#include "regdef.h"

/**
  ******************************************************************************
  * @file    keysmp.c
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
 * @brief  local variable
 *
 */
static ETST_KEYS et_keys[KEY_MAX];

/**
 * @brief  keyAD Initalize;
 * @param
 *   none;
 * @retval
 *   none;
 *
 */
VODET keysmp_init(VODET) {
  U08ET index;

  /* key button <--> P0.2 */
  GPIO_P0_PMD &= ~((U32ET)1 << 4);
  GPIO_P0_PMD &= ~((U32ET)1 << 5);
  /* press button__wall <--> P2.3 */
  GPIO_P2_PMD &= ~((U32ET)1 << 6);
  GPIO_P2_PMD &= ~((U32ET)1 << 7);
  /* KEY_INFRA         <--> P2.2 */
  GPIO_P2_PMD &= ~((U32ET)1 << 4);
  GPIO_P2_PMD &= ~(U32ET)(1 << 5);
  /* KEY_POWER_DOWN    <--> P1.3 */
  GPIO_P1_PMD &= ~((U32ET)1 << 6);
  GPIO_P1_PMD &= ~((U32ET)1 << 7);
  /* initialize keys control structure */
  for (index = 0; index < KEY_MAX; index++) {
    et_keys[index].lstst = KEY_ST_MAX;
    et_keys[index].curst = KEY_ST_MAX;
    et_keys[index].stabe = KEY_ST_MAX;
    et_keys[index].count = 0;
  }
  et_keys[KEY_BUTTON].cycle = 20;
  et_keys[KEY_PB_WALL].cycle = 20;
  et_keys[KEY_INFRA].cycle = 10;
  et_keys[KEY_POWER_DOWN].cycle = 4;

  et_keys[KEY_BUTTON].actlv = KEY_ST_LOW;
  et_keys[KEY_PB_WALL].actlv = KEY_ST_LOW;
  et_keys[KEY_INFRA].actlv = KEY_ST_HIGH;
  et_keys[KEY_POWER_DOWN].actlv = KEY_ST_HIGH;
}

/**
 * @brief  keyAD Task called by system;
 * @param
 *   none;
 * @retval
 *   none;
 *
 */
VODET keysmp_task(VODET) {
  U08ET index;
  /* KEY_Digit get value */
  et_keys[KEY_BUTTON].curst = PORT_READ(ET_PORT0, 2);
  et_keys[KEY_PB_WALL].curst = PORT_READ(ET_PORT2, 3);
  et_keys[KEY_INFRA].curst = PORT_READ(ET_PORT2, 2);
  et_keys[KEY_POWER_DOWN].curst = PORT_READ(ET_PORT1, 3);
  for (index = 0; index < KEY_MAX; index++) {
    if (et_keys[index].curst != et_keys[index].lstst) {
      et_keys[index].count = 0;
      et_keys[index].stabe = KEY_ST_MAX;
    } else {
      if (et_keys[index].count < et_keys[index].cycle) et_keys[index].count++;
      else if (et_keys[index].count == et_keys[index].cycle) {
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
U08ET getkey_curst(U08ET id) {
  U08ET res;
  if (id >= KEY_MAX) res = ET_FALSE;
  else {
    if (et_keys[id].curst == et_keys[id].actlv) res = KEY_ST_HIGH;
    else res = KEY_ST_LOW;
  }
  return res;
}

/**
 * @brief  GET stable INPUT status;
 * @param
 *   id : input port  ID
 * @retval
 *   HIGH or LOW
 *
 */
U08ET getkey_stabe(U08ET id) {
  U08ET res;
  if (id >= KEY_MAX) res = ET_FALSE;
  else {
    if (et_keys[id].stabe == et_keys[id].actlv) {
      et_keys[id].stabe = (id != KEY_INFRA) ? KEY_ST_MAX : et_keys[id].stabe;
      res = KEY_ST_HIGH;
    } else if (et_keys[id].stabe == KEY_ST_MAX) res = KEY_ST_MAX;
    else {
      et_keys[id].stabe = KEY_ST_MAX;
      res = KEY_ST_LOW;
    }
  }
  return res;
}
