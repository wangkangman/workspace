#ifndef _REMOTE_H
#define _REMOTE_H

#include "global.h"
/**
  ******************************************************************************
  * @file    remote.h
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

typedef enum {
  EQUIP_NONE = (0),
  EQUIP_REMOTE,
  CUS_SENSOR
}EQUIP_TYPE;


typedef enum {
  COMMAND_NONE = (0),
  COMMAND_SENSOR,

  ET_REMOTE_CTL,

  ET_REMOTE_TRAIN_ENTRY,

  ET_REMOTE_OPEN_BEGIN,
  ET_REMOTE_OPEN_END,

  ET_REMOTE_CLOSE_BEGIN,
  ET_REMOTE_CLOSE_END,

  ET_REMOTE_SET
}LOCAL_COMMAND_LIST;

typedef enum {   
   REPEAT_DISABLE = (0),
   REPEAT_START,
   REPEAT_KEEP,
}KEYPAD_REPEAT_STATE;

typedef enum {
   ST_REMOTE_TH = (0),
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
#define KEY_LENGTH (8)
#define CLEAR_SENSOR (KEYS_NUM)
#define CLEAR_ALL    (CLEAR_SENSOR + 1)

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

#define BUTTON_CODE_OPEN (0X20)
#define BUTTON_CODE_CLOSE (0X40)
#define BUTTON_CODE_SET (0X80)
#define BUTTON_CODE_TRAIN_ENTRY (0X60)

#if 0
#define EQUIP_REMOTE_RANGE_LOWER (0x3800000)
#define EQUIP_REMOTE_RANGE_UPPER (0x3ffffff)
#define EQUIP_KEYPAD_RANGE_LOWER (0x7c00000)
#define EQUIP_KEYPAD_RANGE_UPPER (0x7ffffff)
#define EQUIP_HOMELK_RANGE_LOWER (0x5050f80)
#define EQUIP_HOMELK_RANGE_UPPER (0x68287c0)
#endif

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
  LOCAL_COMMAND_LIST command;
  LOCAL_COMMAND_LIST out_flag;
  U16ET cnt_0;
  U16ET cnt_1;
} ETST_DATA_PROCESS;

/**
  * @brief  Define keys saved structure 
  *
  */
__packed typedef struct {
  U08ET keys[KEYS_NUM][KEY_LENGTH];
  U08ET key_ciphers[KEYS_NUM][KEY_LENGTH];

  U08ET cus_sensor[KEY_LENGTH];
  U08ET sensor_cipher[KEY_LENGTH];

  U16ET keynum;
  U16ET keyidx;
} ETST_REMOTE_KEYS;

/**
  * @brief  Function declare 
  *
  */
VODET remote_init(VODET);
VODET remote_samp(VODET);
VODET remote_clr(U08ET index);
VODET remote_save(U08ET* puf);

U32ET* remote_get_keys_addr(VODET);

VODET remote_Write_FlashMemory(VODET);
VODET remote_keys_init(VODET);
LOCAL_COMMAND_LIST remote_hits(ENUM_MOTOR_IFLEARN_nMODE(*motor_state)(VODET));
U08ET remote_learn(U08ET* puf);

#endif /* _REMOTE_H */
