#ifndef _UARTCT_H
#define _UARTCT_H

#include "global.h"
/**
  ******************************************************************************
  * @file    uartct.c
  * @author  ETdoor team
  * @version V0.0.2
  * @date    2018/1/13
  * @brief   This file provides all application of uart module, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + call irq task of other module.
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
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET uartct0_init(VODET);
VODET uartct1_init(VODET);
VODET uart1_send_packet(U08ET *pbuf, U16ET size);
VODET uart1_send_byte(U08ET data);

#endif /* _UARTCT_H */
