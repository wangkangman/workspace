#include "Remote.h"
#include "dflash.h"
#include "regdef.h"
#include "stdio.h"
#include "timwdt.h"

/**
  ******************************************************************************
  * @file    remote.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/8/12
  * @brief   This file provides all application of rf module, 
  *          functionalities of the rf signal process:
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
/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static __align(4) ETST_REMOTE_KEYS et_keys;
static ETST_REMOTE remoter;
static ETST_DATA_PROCESS data_process;
const U08ET NLF[2][2][2][2][2] = {0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
                                  0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
                                  1, 0, 0, 1, 0, 1, 1, 1, 0, 0};
//const U08ET CUSTOMER_code[8] = {0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01};
//const U08ET CUSTOMER_code[8] = {'h', 'i', 's', 'c', 'o', '4', '5', '3'};
const U08ET CUSTOMER_code[8] = {'3', '5', '4', 'o', 'c', 's', 'i', 'h'};
// customer code "hicso453"


//static VODET rf_data_process_task(VODET);
static U08ET *DEcrypt(U08ET *source, U08ET *_key);

/**
 * @brief  Remote controller initialize
 * @param  NONE
 * @retval NONE
 *
 */
VODET remote_init(VODET) {
  U08ET index_1;
  /* remote controller  <--> PC2.6 */
  GPIO_P2_PMD &= ~((U32ET)1 << 12);
  GPIO_P2_PMD &= ~((U32ET)1 << 13);
  /* initialize rf module control structure */
  remoter.st_last = 0;
  remoter.st_curt = 0;
  remoter.st_remo = ST_REMOTE_TH;
  remoter.recvidx = 0;
  for (index_1 = 0; index_1 < RBUF_SIZE; index_1++) {
    remoter.rbuf[index_1] = 0;
    remoter.lbuf[index_1] = 0;
  }

  /* rf data process struct init */
  data_process.state = REPEAT_DISABLE;
  data_process.command = COMMAND_NONE;
  data_process.cnt_0 = 0;
  data_process.cnt_1 = 0;
  data_process.out_flag = COMMAND_NONE;
}

/**
 * @brief  init remote_keys from flash rom
 * @param  NONE
 * @retval NONE
 *
 */
VODET remote_keys_init(VODET) {
  /* illegal data process */
  if ((et_keys.keynum > KEYS_NUM) || (et_keys.keyidx > KEYS_NUM)) {
    et_keys.keynum = 0x0;
    et_keys.keyidx = 0x0;
  }
}


U32ET* remote_get_keys_addr(VODET) {return (U32ET*)(&(et_keys.keys[0][0]));}

/**
 * @brief  Remote controller signal sampling
 * @param  NONE
 * @retval NONE
 *
 */
VODET remote_samp(VODET) {
  static U08ET lv_cnt;
  U08ET byidx, btidx;
  remoter.st_last = remoter.st_curt;
  remoter.st_curt = PORT_READ(ET_PORT2, 6);
  if (remoter.st_last == remoter.st_curt) lv_cnt++;
  else {
    if (ST_REMOTE_TH == remoter.st_remo) {
      if ((1 == remoter.st_curt) && (lv_cnt >= CNT_TH_MIN) && (lv_cnt <= CNT_TH_MAX)) {
        remoter.st_remo = ST_REMOTE_DATA;
        remoter.recvidx = 0;
      } else remoter.st_remo = ST_REMOTE_TH;
      lv_cnt = 0;
    } else if (ST_REMOTE_DATA == remoter.st_remo) {
      if (0 == remoter.st_curt) {
        byidx = (U08ET)(remoter.recvidx / 8);
        btidx = (U08ET)(remoter.recvidx % 8);
        if ((lv_cnt >= CNT_L_MIN) && (lv_cnt <= CNT_L_MAX)) {
          remoter.rbuf[byidx] |= (U08ET)(1 << btidx);
          remoter.recvidx++;
        } else if ((lv_cnt >= CNT_H_MIN) && (lv_cnt <= CNT_H_MAX)) {
          remoter.rbuf[byidx] &= (U08ET)(~(1 << btidx));
          remoter.recvidx++;
        } else remoter.st_remo = ST_REMOTE_TH;
        remoter.hi_cnt = lv_cnt;
        if (remoter.recvidx >= RBUF_SIZE * 8) remoter.st_remo = ST_REMOTE_OK;
      }
      if (1 == remoter.st_curt)
        if (((remoter.hi_cnt + lv_cnt) < CNT_CYCLE_MIN) || ((remoter.hi_cnt + lv_cnt) > CNT_CYCLE_MAX))
          remoter.st_remo = ST_REMOTE_TH;
      lv_cnt = 0;
    }
  }
}

/**
  * @brief  Get ET keypad data   
  * @param  
  *   pbuf : buff to store key value
  * @retval
  *   return ET_TURE if receive a key code, else return ET_FALSE
  *
  */
U08ET get_key(U08ET *pbuf) {
  U08ET index, res;
  if (ST_REMOTE_OK != remoter.st_remo) res = ET_FALSE;
  else {
    /* 0xff & 0x00 filter */
    if ((0 == remoter.rbuf[4]) && (0 == remoter.rbuf[5]) && (0 == remoter.rbuf[6]) && (0 == (remoter.rbuf[7] & 0x0f))) res = ET_FALSE; 
    else if ((0xff == remoter.rbuf[4]) && (0xff == remoter.rbuf[5]) && (0xff == remoter.rbuf[6]) && (0x0f == (remoter.rbuf[7] & 0x0f))) res = ET_FALSE;
    /* valid data */
    else {
      for (index = 0; index < RBUF_SIZE; index++) pbuf[index] = remoter.rbuf[index];
      res = ET_TRUE;
    }
    remoter.st_remo = ST_REMOTE_TH;
  }
  return res;
}

/**
 * @brief  clear remote key saved in data falsh
 * @param
 *   mode : the index of the keys which should be cleared,
 *          + if mode < KEYS_NUM, clear the current key,
 *          + if mode == CLEAR_KEYPAD, clear keypad,
 *          + if mode == CLEAR_ALL, clear all keys.
 * @retval
 *   none;
 *
 */
VODET remote_clr(U08ET mode) {
  U08ET index;
  if (mode < KEYS_NUM) {
    for (index = 0; index < KEY_LENGTH; index++) et_keys.keys[mode][index] = 0x0;
  } else if (mode == CLEAR_SENSOR) {
    for (index = 0; index < KEY_LENGTH; index++) et_keys.cus_sensor[index] = 0x0;
  } else if (mode == CLEAR_ALL) {
    et_keys.keyidx = 0;
    et_keys.keynum = 0;
    for (index = 0; index < KEY_LENGTH; index++) et_keys.cus_sensor[index] = 0x0;
  }

#if 0
  dflash_writ_page((U32ET *)(&et_keys),
                   DFMC_RKEYS_OFST,
                   sizeof(ETST_REMOTE_KEYS) / 4);
  wdtdog_feed();
#endif
}

/**
 * @brief  learn key and cipher from data
 * @param
 *   peep : pointer of eeprom control structure
 * @retval
 *   always return ET_TRUE
 *
 */
U08ET remote_learn(U08ET* puf) {
  U08ET res, index, key[8], lrn_buf[8];

  if (ET_FALSE == get_key(key)) res = ET_FALSE;
  else {
    /* judge if code is reapeat */
    for (index = 0; index < ENCY_SIZE; index++)    
      if (key[index] != remoter.lbuf[index])
        break;

    /* cipher text of data is reapeat */
    if (ENCY_SIZE != index ) {
      /* write last buf */
      for (index = 0; index < RBUF_SIZE; index++) remoter.lbuf[index] = key[index];
      #ifdef REMOTE_DEBUG
      printf("get key\r\n");
      #endif

      lrn_buf[3] = key[7] & 0x0F;
      lrn_buf[3] |= 0x20;
      lrn_buf[2] = key[6];
      lrn_buf[1] = key[5];
      lrn_buf[0] = key[4];
      wdtdog_feed();
      DEcrypt((U08ET *)lrn_buf, (U08ET *)CUSTOMER_code);
      lrn_buf[7] = key[7] & 0x0F;
      lrn_buf[7] |= 0x60;
      lrn_buf[6] = key[6];
      lrn_buf[5] = key[5];
      lrn_buf[4] = key[4];
      wdtdog_feed();
      DEcrypt(((U08ET *)lrn_buf) + 4, (U08ET *)CUSTOMER_code);
      wdtdog_feed();
      DEcrypt((U08ET *)key, (U08ET *)lrn_buf);
      if ((key[2] != key[4]) || ((key[3] & 0xf) != (key[5] & 0xf))) res = ET_FALSE;  
      else {
        for (index = 0; index < KEY_LENGTH; index++) puf[index] = key[index];
        res = ET_TRUE;
      }
    }
  }
  return res;
}

/**
 * @brief  learn key and cipher from data
 * @param
 *   peep : pointer of eeprom control structure
 * @retval
 *   always return ET_TRUE
 *
 */
VODET remote_save(U08ET *puf) {
  U08ET index_1, index_2, lrn_buf[8];

  lrn_buf[3] = puf[7] & 0x0F;
  lrn_buf[3] |= 0x20;
  lrn_buf[2] = puf[6];
  lrn_buf[1] = puf[5];
  lrn_buf[0] = puf[4];
  DEcrypt((U08ET *)lrn_buf, (U08ET *)CUSTOMER_code);
  lrn_buf[7] = puf[7] & 0x0F;
  lrn_buf[7] |= 0x60;
  lrn_buf[6] = puf[6];
  lrn_buf[5] = puf[5];
  lrn_buf[4] = puf[4];
  DEcrypt(((U08ET *)lrn_buf) + 4, (U08ET *)CUSTOMER_code);

  for (index_1 = 0; index_1 < et_keys.keynum; index_1++) {
    if ((et_keys.keys[index_1][4] == puf[4])
       && (et_keys.keys[index_1][5] == puf[5])
       && (et_keys.keys[index_1][6] == puf[6])
       && ((et_keys.keys[index_1][7] & 0xf) == (puf[7] & 0xf)))
       break;

    if ((et_keys.keys[index_1][4] == 0x00)
       && (et_keys.keys[index_1][5] == 0x00)
       && (et_keys.keys[index_1][6] == 0x00)
       && (et_keys.keys[index_1][7] == 0x00))
       break;
  }

  if (index_1 < et_keys.keynum) {
    for (index_2 = 0; index_2 < KEY_LENGTH; index_2++) {
      et_keys.keys[index_1][index_2] = puf[index_2];
      et_keys.key_ciphers[index_1][index_2] = lrn_buf[index_2];
    }
  } else {
    for (index_2 = 0; index_2 < KEY_LENGTH; index_2++) {
        et_keys.keys[et_keys.keyidx][index_2] = puf[index_2];
        et_keys.key_ciphers[et_keys.keyidx][index_2] = lrn_buf[index_2];
    }

    et_keys.keynum = (et_keys.keynum + 1) > KEYS_NUM ? KEYS_NUM : (et_keys.keynum + 1);
    et_keys.keyidx = (et_keys.keyidx + 1) % KEYS_NUM;
  }
}

/**
 * @brief  Get a saved remote key
 * @param
 *   none;
 * @retval
 *   COMMAND_LIST : the current command of local equipment.
 *
 */
LOCAL_COMMAND_LIST remote_hits(ENUM_MOTOR_IFLEARN_nMODE(*motor_state)(VODET)) {
  U08ET key[8], index;
  U16ET local_cnt, remote_cnt, differ;

  LOCAL_COMMAND_LIST command;
  EQUIP_TYPE equipment;
  ENUM_MOTOR_IFLEARN_nMODE iflearn_nmode;

  /* process data */
  if (ET_TRUE == get_key(key)) {
    iflearn_nmode = motor_state();

#if 0
    #ifdef REMOTE_DEBUG
    printf("key:");
    for (index = 0; index < 8; index++)  
      printf("0x%x", key[index]);
    printf("\r\n");
    #endif
#endif
    /* judge if code is reapeat */
    for (index = 0; index < ENCY_SIZE; index++)    
      if (key[index] != remoter.lbuf[index])
        break;

    /* cipher text of data is reapeat */
    if (ENCY_SIZE == index ) {
      if (REPEAT_DISABLE != data_process.state)
        data_process.cnt_0 = 150;
    }
    else {
      /* write last buf */
      for (index = 0; index < RBUF_SIZE; index++)
        remoter.lbuf[index] = key[index];

      /* judge if clear sn is same as memory data */
      for (index = 0; index < et_keys.keynum; index++) {
        if ((key[4] == et_keys.keys[index][4]) 
          && (key[5] == et_keys.keys[index][5])
          && (key[6] == et_keys.keys[index][6])
          && ((key[7] & 0xf) == (et_keys.keys[index][7] & 0xf)))
          break;
      }
      if (index < et_keys.keynum) //keys 
        equipment = EQUIP_REMOTE;
      else {//sensor
        if ((key[4] == et_keys.cus_sensor[4]) 
          && (key[5] == et_keys.cus_sensor[5])
          && (key[6] == et_keys.cus_sensor[6])
          && (key[7] == et_keys.cus_sensor[7]))
          equipment = CUS_SENSOR;
        else
          equipment = EQUIP_NONE;
      }

      /*--------------------------------- decode -------------------------------*/
      if (equipment == EQUIP_REMOTE){
        if (((MOTOR_NORMAL == iflearn_nmode) && 
             ((key[7] == et_keys.keys[index][7]) || (BUTTON_CODE_TRAIN_ENTRY== (key[7] & 0xf0))))
           || ((MOTOR_LEARN == iflearn_nmode) && 
             ((BUTTON_CODE_OPEN== (key[7] & 0xf0)) || (BUTTON_CODE_CLOSE== (key[7] & 0xf0)) || (BUTTON_CODE_SET== (key[7] & 0xf0)))))
        {
          #ifdef REMOTE_DEBUG
          printf("dec\r\n");
          #endif
          DEcrypt((U08ET *)key, (U08ET *)(et_keys.key_ciphers[index]));
          /* cipher sn compare with clear sn **12bits */
          if ((key[2] == key[4]) && ((key[3] & 0xf) == (key[5] & 0xf))) {
            local_cnt = ((U16ET)et_keys.keys[index][1] << 8) + et_keys.keys[index][0];
            remote_cnt = ((U16ET)key[1] << 8) + key[0];

            /* 16bit value overflow process */
            if ((local_cnt > (CNT_OVERFLOW - CNT_CLEAR_NUM)) &&
                    (remote_cnt < CNT_CLEAR_NUM))
              differ = (CNT_OVERFLOW - local_cnt + remote_cnt) + 1;
            else
              differ = remote_cnt - local_cnt;

            /* differ is bigger than 50 */
            if (differ > CNT_CLEAR_NUM) {
              remote_clr(index);
            }
            else {
              /* updata local cnt */
              et_keys.keys[index][0] = key[0];
              et_keys.keys[index][1] = key[1];
              /* set current command */
              if ((differ < CNT_REACT_NUM) && (differ != 0)) {
                if (MOTOR_NORMAL == iflearn_nmode) {
                  if (BUTTON_CODE_TRAIN_ENTRY == (key[7] & 0xf0)) {
                    data_process.state = REPEAT_START;
                    data_process.cnt_0 = 150;
                    data_process.cnt_1 = 1500;
                    data_process.command = ET_REMOTE_TRAIN_ENTRY;
                    data_process.out_flag = COMMAND_NONE;
                  } else if(et_keys.keys[index][7] == key[7]){
                    data_process.out_flag = ET_REMOTE_CTL;
                  }
                } 
                else if (MOTOR_LEARN == iflearn_nmode) {
                  switch (key[7] & 0xf0) {
                  case BUTTON_CODE_SET:
                    data_process.out_flag = (COMMAND_NONE == data_process.out_flag
                                            && REPEAT_DISABLE == data_process.state)
                                          ? ET_REMOTE_SET
                                          : data_process.out_flag;
									break;
                  case BUTTON_CODE_OPEN:
                    data_process.state = REPEAT_START;
                    data_process.cnt_0 = 150;
                    data_process.command = ET_REMOTE_OPEN_BEGIN;
                    data_process.out_flag = ET_REMOTE_OPEN_BEGIN;
									break;
                  case BUTTON_CODE_CLOSE:
                    data_process.state = REPEAT_START;
                    data_process.cnt_0 = 150;
                    data_process.command = ET_REMOTE_CLOSE_BEGIN;
                    data_process.out_flag = ET_REMOTE_CLOSE_BEGIN;
									break;
                  }
                }
              }
            }
          }
        }
      }
       
      else if (CUS_SENSOR == equipment) {
        #if 0
        DEcrypt((U08ET *)key, (U08ET *)(et_keys.sensor_cipher));
        /* cipher sn compare with clear sn **12bits */
        if ((key[2] == key[4]) && ((key[3] & 0xf) == (key[5] & 0xf))) {
          local_cnt = ((U16ET)et_keys.cus_sensor[1] << 8) + et_keys.cus_sensor[0];
          remote_cnt = ((U16ET)key[1] << 8) + key[0];
          /* 16bit value overflow process */
          if ((local_cnt > (CNT_OVERFLOW - CNT_CLEAR_NUM)) &&
                  (remote_cnt < CNT_CLEAR_NUM))
            differ = (CNT_OVERFLOW - local_cnt + remote_cnt) + 1;
          else
            differ = remote_cnt - local_cnt;

          /* differ is bigger than 50 */
          if (differ > CNT_CLEAR_NUM) {
            remote_clr(CLEAR_SENSOR);
          }
        }
        #endif
      }
    }
  }
  /* repeat process */
  if (REPEAT_START == data_process.state) {
    if (ET_REMOTE_TRAIN_ENTRY == data_process.command) {
      if (data_process.cnt_0 > 0) data_process.cnt_0--;
      else data_process.state = REPEAT_DISABLE;

      if (data_process.cnt_1 > 0) data_process.cnt_1--;
      else{
        data_process.state = REPEAT_DISABLE;
        data_process.out_flag = ET_REMOTE_TRAIN_ENTRY;
      }
    }
    
    else if (ET_REMOTE_OPEN_BEGIN == data_process.command) {
      if (data_process.cnt_0 > 0) data_process.cnt_0--;
      else {
        data_process.state = REPEAT_DISABLE;
        data_process.out_flag = ET_REMOTE_OPEN_END;
      }
    }

    else if (ET_REMOTE_CLOSE_BEGIN == data_process.command) {
      if (data_process.cnt_0 > 0) data_process.cnt_0--;
      else {
        data_process.state = REPEAT_DISABLE;
        data_process.out_flag = ET_REMOTE_CLOSE_END;
      }
    }
  }

  command = data_process.out_flag;
  data_process.out_flag = COMMAND_NONE;
  return command;
}

#if 0
/**
 * @brief  Remote sys task
 * @param
 *   none;
 * @retval
 *   none;
 *
 */
VODET remote_sys_task(VODET) { data_process_task(); }
#endif

U08ET getBit(U08ET *source, U16ET n) {
  return (source[n / 8] & (1 << (n % 8))) ? 1 : 0;
}

U08ET *DEcrypt(U08ET *source, U08ET *_key) {
  U08ET bitk[4], res;
  U16ET index;
  for (index = 0; index < 528; index++) {
    bitk[0] = NLF[getBit(source, 30)][getBit(source, 25)][getBit(source, 19)]
                 [getBit(source, 8)][getBit(source, 0)];
    bitk[1] = getBit(source, 15);
    bitk[2] = getBit(source, 31);
    bitk[3] = getBit(_key, (527 - index) % 64);
    res = bitk[0] ^ bitk[1] ^ bitk[2] ^ bitk[3];
    source[3] = (source[3] << 1) | (source[2] >> 7);
    source[2] = (source[2] << 1) | (source[1] >> 7);
    source[1] = (source[1] << 1) | (source[0] >> 7);
    source[0] = (source[0] << 1) | (res);
  }
  return source;
}

#if 0
/**
 * @brief  data_process calculation task
 * @param
 *   none;
 * @retval
 *   LOCK DOOR or FORCE_DOOR
 *
 */
VODET rf_data_process_task(VODET) {
  if (data_process_DISABLE == data_process.state)
    return;

  else if (data_process_START == data_process.state) {
    if (ET_KEYPAD_DOOR == data_process.mode) {
      data_process.cnt_0--;
      if (0 == data_process.cnt_0) {
        data_process.state = data_process_DISABLE;
        data_process.out_flag = ET_KEYPAD_DOOR;
        return;
      }
      data_process.cnt_1--;
      if (0 == data_process.cnt_1) {
        data_process.state = data_process_KEEP;
        data_process.out_flag = ET_KEYPAD_DOOR_HOLD_ENABLE;
      }
    } else if (ET_KEYPAD_LOCK == data_process.mode) {
      data_process.cnt_0--;
      if (0 == data_process.cnt_0) {
        data_process.state = data_process_DISABLE;
        return;
      }
      data_process.cnt_1--;
      if (0 == data_process.cnt_1) {
        data_process.state = data_process_DISABLE;
        data_process.out_flag = ET_KEYPAD_LOCK;
      }
    } else {
      data_process.state = data_process_DISABLE;
      return;
    }
  } else if (data_process_KEEP == data_process.state) {
    data_process.cnt_0--;
    if (0 == data_process.cnt_0) {
      data_process.state = data_process_DISABLE;
      data_process.out_flag = ET_KEYPAD_DOOR_HOLD_DISABLE;
    }
  }
}
#endif

/**
 * @brief  Save current remote key
 * @param
 *   none;
 * @retval
 *   none;
 *
 */
VODET remote_Write_FlashMemory(VODET) {
  dflash_writ_page((U32ET *)(&et_keys), DFMC_RKEYS_OFST,
                   sizeof(ETST_REMOTE_KEYS) / 4);
}
