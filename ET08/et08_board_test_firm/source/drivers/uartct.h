#ifndef _UARTCT_H
#define _UARTCT_H

#include "global.h"

#define USART_REC_LEN (200)
/*-----------------------------------------------------------------*/
/* Define UART parameter                                           */
/*-----------------------------------------------------------------*/
#define BAUDRATE (115200)

/**
  * for usmart
  * @brief  rec buf.
  * @param  length:200
  *
  */
extern u8 USART_RX_BUF[USART_REC_LEN];

/**
  * for usmart
  * @brief  rec flag.
  * @param  
  *   bit15:   rec over
  *   bit14:   rec 0x0d
  *   bit13~0: valid cnt
  */
extern u16 USART_RX_STA;



/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void uartct_init(void);
@far @interrupt void UART2_IRQHandler(void);

#endif /* _UARTCT_H */
