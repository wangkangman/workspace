/* 
 * File:   usart.h
 * Author: Kimi
 *
 * Created on November 7, 2017, 6:34 PM
 */

#ifndef USART_H
#define	USART_H
#include "type.h"

//------Fcpu=32M??????------------
#define     UART_RATE_1200  (uint16)6661
#define     UART_RATE_2400  (uint16)3331
#define     UART_RATE_4800  (uint16)1666    
#define     UART_RATE_9600  (uint16)833 
#define     UART_RATE_19200  (uint16)417

void uart_init(void);
u8 even_check(u8 data);
void Init_Uart();
 
void Uart1SendByte(u8 Byte);
void Uart1SendString(u8 *a ,u8 DatLong);
void puthex(unsigned char n);
void PutString(char* fmt);






























#endif	/* USART_H */

