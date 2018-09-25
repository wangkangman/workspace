#include "wifi.h"
#include "uartct.h"
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

static REC_ST receive;

/**
  * @brief  WiFi module inital;
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET wifi_init(VODET) {
  receive.state = STATE_STANDBY;
  receive.byte_cnt = 0;
  receive.outtime_cnt = 0;
}

/**
  * @brief  WiFi irq task, called uart1_irq;
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET wifi_irq_task(U08ET data) {
  U08ET index, sum;

  if (STATE_STANDBY == receive.state) {
    if ((0xFE == data) || (0xFF == data)) {
      receive.data.buf[0] = data;
      receive.state = STATE_RECOVER;
      receive.byte_cnt = 0;
    } else {
      receive.data.buf[receive.byte_cnt++] = data;
      receive.state = STATE_RECING;
    }
  } else if (STATE_RECING == receive.state) {
    receive.outtime_cnt = 0;
    receive.data.buf[receive.byte_cnt++] = data;
    if (REC_LENGTH == receive.byte_cnt) {
      for (sum = 0, index = 0; index < (REC_LENGTH - 1); index++)
        sum += receive.data.buf[index];

      receive.state = (sum == receive.data.commandlist.checksum)
                          ? STATE_RECOVER
                          : STATE_STANDBY;
      receive.byte_cnt = 0;
    }
  }
}

/**
  * @brief  WiFi system task, called by man cycle;
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET wifi_sys_task(VODET) {
  if (STATE_RECING != receive.state)
    return;

  receive.outtime_cnt++;
  if (receive.outtime_cnt > 50) {
    receive.state = STATE_STANDBY;
    receive.byte_cnt = 0;
    receive.outtime_cnt = 0;
  }
}

/**
  * @brief  get data from WiFi buf;
  * @param
  *   prec : piont to buf which will save the current data;
  * @retval
  *   ET_TRUE or ET_FALSE;
  *
  */
U08ET wifi_get_data(REC_UNION *prec) {
  U08ET index;
  if (STATE_RECOVER != receive.state)
    return ET_FALSE;

  for (index = 0; index < REC_LENGTH; index++)
    prec->buf[index] = receive.data.buf[index];

  receive.state = STATE_STANDBY;
  receive.byte_cnt = 0;
  return ET_TRUE;
}

/**
  * @brief  send data through uart1;
  * @param
  *   pbuf : piont string;
  *   size : length of string;
  * @retval
  *   none;
  *
  */
VODET wifi_send_string(U08ET *pbuf, U16ET size) 
{ 
  U16ET sum, index;  
  for (sum = 0, index = 0; index < size; index++){
    uart1_send_byte(pbuf[index]);
    sum += pbuf[index];
  }  
  
  uart1_send_byte((U08ET)sum);
}
