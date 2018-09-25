#ifndef _WIFI_H
#define _WIFI_H
#include "global.h"
#include "main.h"

/**
  ******************************************************************************
  * @file    wifi.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/4/11
  * @brief   This file provides all functions of wifi adapter, 
  *          functionalities of the inital and listen for wifi adapter:
  *           + Interface seting, the current Interface is usart_1
  *           + command list and info list set
  *           + error data process
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

#define STATE_STANDBY    (0)
#define STATE_RECING     (1)
#define STATE_RECOVER    (2)


#define WIFI_OVERTIME    (30 * 500)

#define WIFI_UNLOCK      (0)
#define WIFI_PRELOCK     (1)
#define WIFI_LOCKED      (2)

#define REC_LENGTH       (9)
#define SEND_LENGTH      (12)


#define DOOR_CLOSED      (0x0)
#define DOOR_OPEN        (0x1)
#define DOOR_CLOSING     (0x2)
#define DOOR_OPENING     (0x3)
#define DOOR_FAULT       (0x4)


#define DOOR_COMMAND_CLOSED    (0x0)
#define DOOR_COMMAND_OPEN      (0x1)
#define DOOR_COMMAND_PRESET    (0x2)
#define DOOR_TOGGLE            (0x3)

#define NOT_REMOTE_PROGRAM     (0x0)
#define REMOTE_PROGRAM         (0x1)

__packed typedef struct{
  U08ET module_id;
  U08ET clear_flag;
  U08ET vacation;
  U08ET lamp_state;
  U08ET lamp_time;
  U08ET door_command;
  U08ET preset;
  U08ET program;
  U08ET checksum;
}WIFI_COMMAND;

__packed typedef struct{
  U08ET module_id;
  U08ET snesor_flag;
  U08ET lamp_state;
  U08ET lamp_time;
  U08ET door_state;
  U08ET operation_mode;
  U08ET door_position;
  U08ET preset_position;
  U08ET door_max_position;
  U08ET vacation;
  U08ET program;
  U08ET checksum;
}WIFI_INFO;

typedef union{
  __packed WIFI_COMMAND commandlist;
  __packed U08ET buf[9];
}REC_UNION;

typedef struct{
  REC_UNION data;
  U08ET state;
  U16ET byte_cnt;
  U08ET outtime_cnt;
}REC_ST;

typedef union{
  WIFI_INFO infolist;
  __packed U08ET buf[12];
}SEND_UNION;

VODET wifi_init(VODET);
VODET wifi_irq_task(U08ET data);
VODET wifi_sys_task(VODET);
U08ET wifi_get_data(REC_UNION* prec);
VODET wifi_send_string(U08ET *pbuf, U16ET size);

#endif
