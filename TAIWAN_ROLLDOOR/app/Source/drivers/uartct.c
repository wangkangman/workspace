#include <stdio.h>
#include "regdef.h"
#include "uartct.h"
/**
  ******************************************************************************
  * @file    uartct.c
  * @author  ETdoor team
  * @version V0.0.2
  * @date    2018/4/11
  * @brief   This file provides all application of uart module, 
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
  * @brief  Uart_0 initialize   
  * @param
  *   none;
  * @retval 
  *   none;
  *
  */
VODET uartct0_init(VODET)
{
  /* UART0_TXD <--> P3.1 */
  GCR_P3MFP |= (1 << 1);
  GCR_P3MFP &= ~(1 << 9);
  /* UART0_RXD <--> P3.0 */
  GCR_P3MFP |= (1 << 0);
  GCR_P3MFP &= ~(1 << 8);
  /* select HIRC as UART clock source */
  CLK_SEL1 |= (3 << 24);
  /* set UART clock divider to 1 */
  CLK_CLKDIV &= ~(15 << 8);
  /* enable UART clock */
  CLK_APB |= (1 << 16);
  /* select UART0 as UART mode */
  UART0_FUNSEL &= ~(3 << 0);
  /* data bits <-> 8 */
  UART0_LCR |= (3 << 0);
  /* stop bits <-> 1 */
  UART0_LCR &= ~(1 << 2);
  /* no parity */
  UART0_LCR &= ~(1 << 3);
  /* set baudrate to 115200 */
  UART0_BAUD = 0x0F00000A;
  /* enbale rx interrupt */
  UART0_IER |= (1 << 0);
  /* enable UART0 NVIC interrupt */
  NVIC_ISER = (1 << 12);
}

/**
  * @brief  Uart_1 initialize   
  * @param
  *   none;
  * @retval 
  *   none;
  *
  */
VODET uartct1_init(VODET)
{
    /* UART1_TXD <--> P1.3 */
    GCR_P1MFP &= ~(1<<3);
    GCR_P1MFP |=  (1<<11);
    /* UART1_RXD <--> P1.2 */
    GCR_P1MFP &= ~(1<<2);
    GCR_P1MFP |=  (1<<10);
    /* select HIRC as UART clock source */
    CLK_SEL1 |= (3<<24);
    /* set UART clock divider to 1 */
    CLK_CLKDIV &= ~(15<<8);
    /* enable UART clock */
    CLK_APB |=  (1<<17);
    /* select UART1 as UART mode */
    UART1_FUNSEL &= ~(3<<0);
    /* data bits <-> 8 */
    UART1_LCR |= (3<<0);
    /* stop bits <-> 1 */
    UART1_LCR &= ~(1<<2);
    /* no parity */
    UART1_LCR &= ~(1<<3);
    /* set baudrate to 9600 */
    UART1_BAUD = 0x0F00008E;
    /* enbale rx interrupt */
    UART1_IER |= (1<<0);
    /* enable UART1 NVIC interrupt */
    NVIC_ISER = (1<<13);
}

/**
  * @brief  derction of printf  
  * @param
  *   ch: char which should be send;
  * @retval 
  *   ch: char which has been sent;
  *
  */
INTET fputc(INTET ch, FILE *stream)
{
  while (UART0_FSR & (1 << 23));
  UART0_THR = ch;
  return ch;
}
/**
  * @brief  UART0_IRQHandler 
  * @param
  *   none;
  * @retval 
  *   none;
  *
  */
VODET UART0_IRQHandler(VODET)
{
  volatile U08ET trash;
  if (UART0_ISR & (1 << 0))
    trash = UART0_RBR;
}

/**
  * @brief  UART1_IRQHandler 
  * @param
  *   none;
  * @retval 
  *   none;
  *
  */
VODET UART1_IRQHandler(VODET){
  volatile U08ET data;
  if( UART1_ISR & (1<<0) ) {
    data = UART1_RBR;
  }
}

/**
  * @brief  UART1 send byte 
  * @param
  *   data : char
  * @retval 
  *   none;
  *
  */
VODET uart1_send_byte(U08ET data) {
  while (UART1_FSR & (1 << 23));
  UART1_THR = data;
}

/**
  * @brief  UART1 send packet 
  * @param
  *   pbuf : data buff
  *   size : data size 
  * @retval 
  *   none;
  *
  */
VODET uart1_send_packet(U08ET *pbuf, U16ET size) {
  U16ET index;
  for (index = 0; index < size; index++) {
    while (UART1_FSR & (1 << 23));
    UART1_THR = *pbuf++;
  }
}

