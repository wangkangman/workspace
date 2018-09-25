#ifndef _SPI_H
#define _SPI_H

#include "global.h"
/**
  ******************************************************************************
  * @file    spi.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/1/11
  * @brief   This file provides all application of spi, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + rec rf data from TTI equipment.
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
#define SPI_LEN_MAX (8)
#define SPI_OVERTIME_CNT (100)
#define SPI_IGNOR_CNT (300)

typedef struct
{
  U08ET buf[SPI_LEN_MAX];
  U08ET garbage;
  U16ET overtime;

  U08ET step;
  U08ET isfull;
  U08ET state;
  U08ET ignore;
} ETST_SPI;

VODET SPI_Slave_Configuration(VODET);
VODET SPI_IRQ_task(VODET);
VODET SPI_overtime_task(VODET);
U08ET SPI_get_data(U08ET *pbuf);
//@far @interrupt VODET SPI_IRQHandler(VODET);

#endif /* _UARTCT_H */
