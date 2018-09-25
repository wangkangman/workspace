#ifndef _REMOTE_H
#define _REMOTE_H

#include "global.h"
/**
  ******************************************************************************
  * @file    remote.h
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/1/12
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
  *
  *
  ******************************************************************************
  */

typedef enum {
   EQUIP_REMOTE = (1),
   EQUIP_KEYPAD,
   EQUIP_HOMELK,
   EQUIP_ILLEGAL,
}TTI_EQUIP_TYPE;

typedef enum {
   KEY_NONE = (0),
   KEY_TTI_SPI,
   KEY_ET_KEYPAD,
}LOCAL_KEY_TYPE;

typedef enum {
   LOCAL_COMMAND_NONE = (0),
   TTI_REMOTE,
   TTI_KEYPAD,
   TTI_HOMELIK,
   ET_KEYPAD_DOOR,
   ET_KEYPAD_LOCK,
   ET_KEYPAD_LAMP,
   ET_KEYPAD_DOOR_HOLD_ENABLE,
   ET_KEYPAD_DOOR_HOLD_DISABLE,
}LOCAL_COMMAND_LIST;

typedef enum {
   REPEAT_DISABLE = (0),
   REPEAT_START,
   REPEAT_KEEP,
}KEYPAD_REPEAT_STATE;

typedef enum {
   ST_REMOTE_TP = (1),
   ST_REMOTE_TH = (1),
   ST_REMOTE_DATA,
   ST_REMOTE_OK,
}RF_STATE;



/**
  * @brief  Define remote controller parameter 
  *
  */
#define RBUF_SIZE (8)
#define ENCY_SIZE (4)
#define REID_SIZE (4)

#define KEYS_NUM (10)
#define KEY_PAD_INDEX (KEYS_NUM)
#define KEY_LENGTH (8)
#define CLEAR_KEYPAD (KEYS_NUM)
#define CLEAR_ALL    (CLEAR_KEYPAD + 1)

#define CNT_TP_TE (23)
#define CNT_TE_MIN (4)
#define CNT_TE_MAX (12)
#define CNT_TH_MIN (52)
#define CNT_TH_MAX (132)
#define CNT_L_MIN (4)
#define CNT_L_MAX (10)
#define CNT_H_MIN (12)
#define CNT_H_MAX (20)
#define CNT_CYCLE_MIN (18)
#define CNT_CYCLE_MAX (30)

#define CNT_OVERFLOW (0xffff)
#define CNT_CLEAR_NUM (50)
#define CNT_REACT_NUM (3)

#define EQUIP_REMOTE_RANGE_LOWER (0x3800000)
#define EQUIP_REMOTE_RANGE_UPPER (0x3ffffff)
#define EQUIP_KEYPAD_RANGE_LOWER (0x7c00000)
#define EQUIP_KEYPAD_RANGE_UPPER (0x7ffffff)
#define EQUIP_HOMELK_RANGE_LOWER (0x5050f80)
#define EQUIP_HOMELK_RANGE_UPPER (0x68287c0)

/**
  * @brief  Define remote controller control structure 
  *
  */
__packed typedef struct
{
  U08ET st_last : 4;     /* last status */
  U08ET st_curt : 4;     /* current status */
  RF_STATE st_remo;     /* remote controller identification status */
  U08ET recvidx;         /* receive buff index, unit of bit */
  U08ET rbuf[RBUF_SIZE]; /* receive buff */
  U08ET lbuf[RBUF_SIZE]; /* last receive buff */
  U08ET hi_cnt;
} ETST_REMOTE;

typedef struct{
  KEYPAD_REPEAT_STATE state;
  LOCAL_COMMAND_LIST mode;
  LOCAL_COMMAND_LIST out_flag;
  U16ET cnt_0;
  U16ET cnt_1;
} ETST_REMOTE_REPEAT;

/**
  * @brief  Define keys saved structure 
  *
  */
typedef struct {
  U08ET keys[KEYS_NUM][KEY_LENGTH];
  U08ET key_pad[KEY_LENGTH];
  U08ET lrn_key[KEY_LENGTH];
  U16ET keynum;
  U16ET keyidx;
} ETST_REMOTE_KEYS;

extern const U08ET CUSTOMER_code[8];
/**
  * @brief  Function declare 
  *
  */
VODET remote_init(VODET);
VODET remote_samp(VODET);
U08ET get_TTI_spi(U08ET *pbuf);
U08ET get_ET_keypad(U08ET *pbuf);
VODET remote_clr(U08ET index);
U08ET remote_save(U08ET key[8], LOCAL_KEY_TYPE key_mode);
VODET remote_Write_FlashMemory(VODET);
LOCAL_COMMAND_LIST remote_hits(VODET);
VODET remote_sys_task(VODET);
U08ET *DEcrypt(U08ET *source, U08ET *_key);
VODET remote_keys_init(VODET);

#endif /* _REMOTE_H */
