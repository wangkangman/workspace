#include "regdef.h"
#include "dflash.h"
#include "Remote.h"
#include "timwdt.h"
#include "stdio.h"
#include "spi.h"
/**
  ******************************************************************************
  * @file    remote.c
  * @author  ETdoor team
  * @version V0.0.3
  * @date    2018/4/13
  * @brief   This file provides all application of local wireless equip, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + rec rf data from TTI equipment & ET keypad.
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
  * V0.0.1: spi wrong data filter;
  * V0.0.2: indoor keypad add;
  * V0.0.3: reconstruction
  *
  *
  ******************************************************************************
  */
/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static __align(4) ETST_REMOTE_KEYS et_keys;
static ETST_REMOTE remoter;
static ETST_REMOTE_REPEAT repeat;
const U08ET NLF[2][2][2][2][2] = {0, 1, 1, 1,
	                                0, 1, 0, 0,
	                                0, 0, 1, 0,
	                                1, 1, 1, 0,
	                                0, 0, 1, 1,
	                                1, 0, 1, 0,
	                                0, 1, 0, 1,
	                                1, 1, 0, 0};
const U08ET CUSTOMER_code[8] = {0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01};

static VODET repeat_task(VODET);
static TTI_EQUIP_TYPE equip_type_check(U08ET *sn_string);
static U08ET key_illegal_judge(U08ET *pbuf);

/**
  * @brief  Remote controller initialize 
  * @param  NONE
  * @retval NONE
  *
  */
VODET remote_init(VODET)
{
  U08ET index_1;
  /* remote controller  <--> PC2.6 */
  GPIO_P2_PMD &= ~((U32ET)1 << 12);
  GPIO_P2_PMD &= ~((U32ET)1 << 13);
  /* initialize remote control structure */
  remoter.st_last = 0;
  remoter.st_curt = 0;
  remoter.st_remo = ST_REMOTE_TP;
  remoter.recvidx = 0;
  for (index_1 = 0; index_1 < RBUF_SIZE; index_1++){
    remoter.rbuf[index_1] = 0;
    remoter.lbuf[index_1] = 0;
  }
  
  SPI_Slave_Configuration();
  repeat.state = REPEAT_DISABLE;
  repeat.mode = LOCAL_COMMAND_NONE;
  repeat.cnt_0 = 0;
  repeat.cnt_1 = 0;
  repeat.out_flag = LOCAL_COMMAND_NONE;

  wdtdog_feed();
}

/**
  * @brief  init remote_keys 
  * @param  NONE
  * @retval NONE
  *
  */
VODET remote_keys_init(VODET)
{
  U08ET index_1, index_2, size;
  U32ET *pbuf;
  /* inital keys */
  size = sizeof(ETST_REMOTE_KEYS) / 4;
  pbuf = (U32ET *)(&et_keys);
  dflash_read_page(pbuf, DFMC_RKEYS_OFST, size);
  if ((et_keys.keynum > KEYS_NUM) 
     || (et_keys.keyidx > KEYS_NUM))
  {
    for (index_1 = 0; index_1 < KEYS_NUM; index_1++)
      for (index_2 = 0; index_2 < KEY_LENGTH; index_2++)
        et_keys.keys[index_1][index_2] = 0x0;
    et_keys.keynum = 0x0;
    et_keys.keyidx = 0x0;
  }
  wdtdog_feed();
}

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
      if ((1 == remoter.st_curt) 
         && (lv_cnt >= CNT_TH_MIN) 
         && (lv_cnt <= CNT_TH_MAX)) {
        remoter.st_remo = ST_REMOTE_DATA;
        remoter.recvidx = 0;
      }
      else remoter.st_remo = ST_REMOTE_TP;
      lv_cnt = 0;
    } else if (ST_REMOTE_DATA == remoter.st_remo) {
      if (0 == remoter.st_curt) {
        byidx = (U08ET)(remoter.recvidx / 8);
        btidx = (U08ET)(remoter.recvidx % 8);
        if ((lv_cnt >= CNT_L_MIN) 
           && (lv_cnt <= CNT_L_MAX)) {
          remoter.rbuf[byidx] |= (U08ET)(1 << btidx);
          remoter.recvidx++;
        } else if ((lv_cnt >= CNT_H_MIN) 
                  && (lv_cnt <= CNT_H_MAX)) {
          remoter.rbuf[byidx] &= (U08ET)(~(1 << btidx));
          remoter.recvidx++;
        } else remoter.st_remo = ST_REMOTE_TP;
        remoter.hi_cnt = lv_cnt;
        if (remoter.recvidx >= RBUF_SIZE * 8) 
          remoter.st_remo = ST_REMOTE_OK;
      }
      if (1 == remoter.st_curt)
        if (((remoter.hi_cnt + lv_cnt) < CNT_CYCLE_MIN) 
           || ((remoter.hi_cnt + lv_cnt) > CNT_CYCLE_MAX))
          remoter.st_remo = ST_REMOTE_TP;
      lv_cnt = 0;
    }
  }
}

/**
  * @brief  Get TTI spi data   
  * @param  
  *   pbuf : buff to store key value
  * @retval
  *   return ET_TURE if receive a key code, else return ET_FALSE
  *
  */
U08ET get_TTI_spi(U08ET *pbuf) {
  if (ET_FALSE == SPI_get_data(pbuf)) 
    return ET_FALSE;
  
  if (ET_FALSE == key_illegal_judge(pbuf)
     || EQUIP_ILLEGAL == equip_type_check(pbuf)) 
    return ET_FALSE;
  
  return ET_TRUE;
}

/**
  * @brief  Get ET keypad data   
  * @param  
  *   pbuf : buff to store key value
  * @retval
  *   return ET_TURE if receive a key code, else return ET_FALSE
  *
  */
U08ET get_ET_keypad(U08ET *pbuf) {
  U08ET index;
  if (ST_REMOTE_OK != remoter.st_remo)
    return ET_FALSE;
  
  if (ET_FALSE == key_illegal_judge(remoter.rbuf)) {
    remoter.st_remo = ST_REMOTE_TP;
    return ET_FALSE;
  }
  
  for (index = 0; index < RBUF_SIZE; index++)
    pbuf[index] = remoter.rbuf[index];
  
  remoter.st_remo = ST_REMOTE_TP;
  return ET_TRUE;
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
VODET remote_clr(U08ET mode){
  U08ET index;
  if (mode < KEYS_NUM) {
    for (index = 0; index < KEY_LENGTH; index++) 
      et_keys.keys[mode][index] = 0x0;
  } else if (mode == CLEAR_KEYPAD){
    for (index = 0; index < KEY_LENGTH; index++) 
      et_keys.key_pad[index] = 0x0;
  } else if (mode == CLEAR_ALL) {
    et_keys.keyidx = 0;
    et_keys.keynum = 0;
    for (index = 0; index < KEY_LENGTH; index++) 
      et_keys.key_pad[index] = 0x0;
  }
  dflash_writ_page((U32ET *)(&et_keys),
                   DFMC_RKEYS_OFST,
                   sizeof(ETST_REMOTE_KEYS) / 4);
  wdtdog_feed();
}

/**
  * @brief  Save remote key to eeprom 
  * @param  
  *   peep : pointer of eeprom control structure 
  * @retval 
  *   always return ET_TRUE 
  *
  */
U08ET remote_save(U08ET key[8], LOCAL_KEY_TYPE key_mode)
{
  U08ET index_1, index_2, lrn_buf[8];

  /* current key is illegal */
  if (ET_FALSE == key_illegal_judge(key)) 
    goto ERROR;
  // save remote from pic
  if (KEY_TTI_SPI == key_mode) { 
    /* equip mode is illegal */
    if (EQUIP_ILLEGAL == equip_type_check(key))
      goto ERROR;
    /* compare the current key with all saved */
    for (index_1 = 0; index_1 < et_keys.keynum; index_1++)
      if ((key[4] == et_keys.keys[index_1][4])
         && (key[5] == et_keys.keys[index_1][5])
         && (key[6] == et_keys.keys[index_1][6])
         && ((key[7] & 0x0F) == (et_keys.keys[index_1][7] & 0x0F)))
        break;

    /* the current key is saved */
    if (et_keys.keynum != index_1) {
      for (index_2 = 0; index_2 < KEY_LENGTH; index_2++)
        et_keys.keys[index_1][index_2] = key[index_2];
      /* current key is saved, updata counter and button value */
      goto SAVE;
    }

    if (et_keys.keynum < KEYS_NUM) {
      for (index_2 = 0; index_2 < KEY_LENGTH; index_2++)
        et_keys.keys[et_keys.keyidx][index_2] = key[index_2];
      et_keys.keynum = et_keys.keynum < KEYS_NUM 
                           ? (et_keys.keynum + 1)
                           : et_keys.keynum;
      et_keys.keyidx = (et_keys.keyidx + 1) % KEYS_NUM;
      /* memory is not full, store the current key */
      goto SAVE;      
    }
    /* scan the illegal keys of memory sequence,
       + for example 0x00000000 which has been 
       + signle cleared. */
    for (index_1 = 0; index_1 < KEYS_NUM; index_1++) 
      if (EQUIP_ILLEGAL == equip_type_check(et_keys.keys[(et_keys.keyidx + index_1) % KEYS_NUM]))
        break;
    if (index_1 != KEYS_NUM) {
      for (index_2 = 0; index_2 < KEY_LENGTH; index_2++)
        et_keys.keys[(et_keys.keyidx + index_1) % KEYS_NUM][index_2] = key[index_2];

      et_keys.keyidx = ((et_keys.keyidx + index_1 + 1) % KEYS_NUM) % KEYS_NUM;
      /* replace an illegal key */
      goto SAVE;
    }
        
    /* scan the remote keys of memory sequence */
    for (index_1 = 0; index_1 < KEYS_NUM; index_1++)
      if (EQUIP_REMOTE 
          == equip_type_check(et_keys.keys[(et_keys.keyidx + index_1) % KEYS_NUM]))
        break;      
    if (index_1 != KEYS_NUM) {
      for (index_2 = 0; index_2 < KEY_LENGTH; index_2++)
        et_keys.keys[(et_keys.keyidx + index_1) % KEYS_NUM][index_2] = key[index_2];

      et_keys.keyidx = ((et_keys.keyidx + index_1 + 1) % KEYS_NUM) % KEYS_NUM;
      /* replace an remote key */
      goto SAVE;
    }
    goto ERROR;
  }
  /* key pad save */
  else if (KEY_ET_KEYPAD == key_mode){ 
    // save key pad
    /* judge if the code is repeat */
    for (index_1 = 0; index_1 < ENCY_SIZE; index_1++)
      if (key[index_1] != remoter.lbuf[index_1])
        break;
    if (ENCY_SIZE == index_1) {
      remoter.st_remo = ST_REMOTE_TP;
      goto ERROR;
    }
    /* write last buf */
    for (index_1 = 0; index_1 < RBUF_SIZE; index_1++) 
      remoter.lbuf[index_1] = key[index_1];

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

    if ((key[2] != key[4])
       || ((key[3] & 0xf) != (key[5] & 0xf)))
      goto ERROR;

    for (index_2 = 0; index_2 < KEY_LENGTH; index_2++) {
      et_keys.lrn_key[index_2] = lrn_buf[index_2];
      et_keys.key_pad[index_2] = key[index_2];
    }
    goto SAVE;
  }


ERROR:
  return ET_FALSE;

SAVE:
  remote_Write_FlashMemory();
  return ET_TRUE;
}


/**
  * @brief  Get a saved remote key 
  * @param  
  *   none;
  * @retval
  *   COMMAND_LIST : the current command of local equipment.
  *
  */
LOCAL_COMMAND_LIST remote_hits(VODET)
{
  U08ET key[8], index;
  U16ET local_cnt, remote_cnt, differ;
  
  TTI_EQUIP_TYPE     equip_type;  
  LOCAL_COMMAND_LIST command;
  
/*-------------------for keypad repeat---------------------------*/
/* can be ET_KEYPAD_LOCK & ET_KEYPAD_DOOR_HOLD_ENABLE & ET_KEYPAD_DOOR_HOLD_DISABLE */
  if (LOCAL_COMMAND_NONE != repeat.out_flag) {
    command = repeat.out_flag;
    repeat.out_flag = LOCAL_COMMAND_NONE;
    /* indoor keypad repeat */ 
    goto COMMAND_VALID;
  }
/******************************************************************/
/*-------------------------TTI equipment--------------------------*/
/******************************************************************/
  if (ET_TRUE == get_TTI_spi(key)) {
    equip_type = equip_type_check(key);
    
     /* illegal equipment */
    if (EQUIP_ILLEGAL == equip_type)     
      goto COMMAND_NONE;
    
    for (index = 0; index < et_keys.keynum; index++) {
      if ((key[2] == et_keys.keys[index][2])
         && ((key[3] & 0x0f) == (et_keys.keys[index][3] & 0x0f))
         && (key[4] == et_keys.keys[index][4])
         && (key[5] == et_keys.keys[index][5])
         && (key[6] == et_keys.keys[index][6])
         && (key[7] == et_keys.keys[index][7]))
        break;
    }

    if (index == et_keys.keynum) 
      /* can't match the stored keys */
      goto COMMAND_NONE;
    
    local_cnt = ((U16ET)et_keys.keys[index][1] << 8)
                + et_keys.keys[index][0];
    remote_cnt = ((U16ET)key[1] << 8) + key[0];

/*----------------- judge if the counter is satisfy ----------------------*/
    /* 2 counter is same */
    if (local_cnt == remote_cnt)        
      goto COMMAND_NONE;
        
    /* 16bit value overflow process */
    if ((local_cnt > (CNT_OVERFLOW - CNT_CLEAR_NUM))
       && (remote_cnt < CNT_CLEAR_NUM))
      differ = (CNT_OVERFLOW - local_cnt + remote_cnt) + 1;
    else
      differ = remote_cnt - local_cnt;

    /* more than 50, clear the current key */
    if (differ > CNT_CLEAR_NUM) {
      remote_clr(index);
      /* out of 50 */
      goto COMMAND_NONE;
    }

    et_keys.keys[index][0] = key[0];
    et_keys.keys[index][1] = key[1];
    if ((differ < CNT_REACT_NUM)
       && ((key[3] & 0xf0) == (et_keys.keys[index][3] & 0xf0))){
      if (EQUIP_REMOTE == equip_type)
        command = TTI_REMOTE;
      else if (EQUIP_KEYPAD == equip_type)
        command = TTI_KEYPAD;
      else if (EQUIP_HOMELK == equip_type)
        command = TTI_HOMELIK;
      else  
        command = LOCAL_COMMAND_NONE;
      /* TTI command */  
      goto COMMAND_VALID;
    }

    /* match nothing */
    goto COMMAND_NONE;
  }
/******************************************************************/
/*----------------------------ET keypad---------------------------*/
/******************************************************************/
  if (ET_TRUE == get_ET_keypad(key)) {
    /* judge if code is repeat */
    for (index = 0; index < ENCY_SIZE; index++)
      if (key[index] != remoter.lbuf[index])
        break;

    /* cipher text of data is reapeat */
    if (ENCY_SIZE == index) {
      if (REPEAT_DISABLE != repeat.state) 
        repeat.cnt_0 = 150;
      
      goto COMMAND_NONE;
    }

    /* write last buf */
    for (index = 0; index < RBUF_SIZE; index++)
      remoter.lbuf[index] = key[index];

    /* judge if clear sn is same as memory data */
    if ((key[4] != et_keys.key_pad[4])
       || (key[5] != et_keys.key_pad[5])
       || (key[6] != et_keys.key_pad[6])
       || ((key[7] & 0xf) != (et_keys.key_pad[7] & 0xf)))
      goto COMMAND_NONE;

    wdtdog_feed();

/*--------------------------------- decode -------------------------------*/
    DEcrypt((U08ET *)key, (U08ET *)(et_keys.lrn_key));

    /* cipher sn compare with clear sn **12bits */
    if ((key[2] != key[4]) 
       || ((key[3] & 0xf) != (key[5] & 0xf)))
      goto COMMAND_NONE;
      
    local_cnt = ((U16ET)et_keys.key_pad[1] << 8)
                + et_keys.key_pad[0];
    remote_cnt = ((U16ET)key[1] << 8) + key[0];
    
    /* counter is same */
    if (local_cnt == remote_cnt)
      goto COMMAND_NONE;

    /* 16bit value overflow process */
    if ((local_cnt > (CNT_OVERFLOW - CNT_CLEAR_NUM))
       && (remote_cnt < CNT_CLEAR_NUM))
      differ = (CNT_OVERFLOW - local_cnt + remote_cnt) + 1;
    else
      differ = remote_cnt - local_cnt;

    /* differ is bigger than 50 */
    if (differ > CNT_CLEAR_NUM) {
      remote_clr(CLEAR_KEYPAD);
      goto COMMAND_NONE;
    }

    /* updata local cnt */
    et_keys.key_pad[0] = key[0];
    et_keys.key_pad[1] = key[1];

    /* return current command */
    if (differ < CNT_REACT_NUM) {
      switch (key[7] & 0xf0) {
        case 0x20:
          repeat.state = REPEAT_START;
          repeat.mode = ET_KEYPAD_DOOR;
          repeat.cnt_0 = 150;
          repeat.cnt_1 = 400;
          repeat.out_flag = LOCAL_COMMAND_NONE;
        break;
        case 0x50:
          repeat.state = REPEAT_START;
          repeat.mode = ET_KEYPAD_LOCK;
          repeat.cnt_0 = 150;
          repeat.cnt_1 = 300;
          repeat.out_flag = LOCAL_COMMAND_NONE;
        break;
        case 0xc0:
          command = ET_KEYPAD_LAMP;
          /* lamp command */
          goto COMMAND_VALID;
        default:
          /* button val is wrong */
          goto COMMAND_NONE;
      }
    }
  }

COMMAND_NONE:
  command = LOCAL_COMMAND_NONE;
  return command;
  
COMMAND_VALID:
  return command;
}




/**
  * @brief  Remote sys task 
  * @param  
  *   none;
  * @retval
  *   none;
  *
  */
VODET remote_sys_task(VODET)
{
#if SPI == 1
  SPI_overtime_task();
#endif
  repeat_task();
}

U08ET getBit(U08ET *source, U16ET n)
{
  return (source[n / 8] & (1 << (n % 8))) ? 1 : 0;
}

U08ET *DEcrypt(U08ET *source, U08ET *_key)
{
  U08ET bitk[4], res;
  U16ET index;
  for (index = 0; index < 528; index++)
  {
    bitk[0] = NLF[getBit(source, 30)]
                 [getBit(source, 25)]
                 [getBit(source, 19)]
                 [getBit(source, 8)]
                 [getBit(source, 0)];
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

/**
  * @brief  repeat calculation task 
  * @param  
  *   none; 
  * @retval 
  *   LOCK DOOR or FORCE_DOOR 
  *
  */
VODET repeat_task(VODET)
{
  if (REPEAT_DISABLE == repeat.state)
    return;

  else if (REPEAT_START == repeat.state) {
    if (ET_KEYPAD_DOOR == repeat.mode) {
      repeat.cnt_0--;
      if (0 == repeat.cnt_0) {
        repeat.state = REPEAT_DISABLE;
        repeat.out_flag = ET_KEYPAD_DOOR;
        return;
      }
      repeat.cnt_1--;
      if (0 == repeat.cnt_1) {
        repeat.state = REPEAT_KEEP;
        repeat.out_flag = ET_KEYPAD_DOOR_HOLD_ENABLE;
      }
    } else if (ET_KEYPAD_LOCK == repeat.mode) {
      repeat.cnt_0--;
      if (0 == repeat.cnt_0) {
        repeat.state = REPEAT_DISABLE;
        return;
      }
      repeat.cnt_1--;
      if (0 == repeat.cnt_1) {
        repeat.state = REPEAT_DISABLE;
        repeat.out_flag = ET_KEYPAD_LOCK;
      }
    }
    else {
      repeat.state = REPEAT_DISABLE;
      return;
    }
  }  
  else if (REPEAT_KEEP == repeat.state) {
    repeat.cnt_0--;
    if (0 == repeat.cnt_0) {
      repeat.state = REPEAT_DISABLE;
      repeat.out_flag = ET_KEYPAD_DOOR_HOLD_DISABLE;
    }
  }
}

/**
  * @brief  Save current remote key 
  * @param  
  *   none; 
  * @retval 
  *   none; 
  *
  */
VODET remote_Write_FlashMemory(VODET) {
  dflash_writ_page((U32ET *)(&et_keys), 
                   DFMC_RKEYS_OFST,
                   sizeof(ETST_REMOTE_KEYS) / 4);
}

/**
  * @brief  judge if the current key is legal;
  * @param  
  *   pbuf: piont to the sn array; 
  * @retval 
  *   return the result; 
  *
  */
U08ET key_illegal_judge(U08ET *pbuf) {
  if ((0x00 == pbuf[4]) 
     && (0x00 == pbuf[5])
     && (0x00 == pbuf[6])
     && (0x00 == (pbuf[7] & 0x0F)))
    return ET_FALSE;
  
  if ((0xFF == pbuf[4])
     && (0xFF == pbuf[5])
     && (0xFF == pbuf[6])
     && (0x0F == (pbuf[7] & 0x0F)))
    return ET_FALSE;
  
  return ET_TRUE;
}

/**
  * @brief  judge the type of the current equipment;
  * @param  
  *   sn_string: piont to the sn array; 
  * @retval 
  *   return current type; 
  *
  */
TTI_EQUIP_TYPE equip_type_check(U08ET *sn_string)
{
  U32ET sn = 0;
  TTI_EQUIP_TYPE equip_type;

  sn += ((U32ET)sn_string[4]);
  sn += ((U32ET)sn_string[5]) << 8;
  sn += ((U32ET)sn_string[6]) << 16;
  sn += (((U32ET)sn_string[7]) & 0x0f) << 24;

  if ((sn > EQUIP_REMOTE_RANGE_LOWER)
     && (sn < EQUIP_REMOTE_RANGE_UPPER))
    equip_type = EQUIP_REMOTE;
  else if ((sn > EQUIP_KEYPAD_RANGE_LOWER)
          && (sn < EQUIP_KEYPAD_RANGE_UPPER))
    equip_type = EQUIP_KEYPAD;
  else if ((sn > EQUIP_HOMELK_RANGE_LOWER)
          && (sn < EQUIP_HOMELK_RANGE_UPPER))
    equip_type = EQUIP_HOMELK;
  else equip_type = EQUIP_ILLEGAL;

  return  equip_type;
}
