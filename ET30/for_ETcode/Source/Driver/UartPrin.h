#ifndef _UARTPRIN_H
#define _UARTPRIN_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define UART parameter                                           */
/*-----------------------------------------------------------------*/
#define BAUDRATE     115200

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void uartct_init(void);
void uartct_send(U08ET *sbuf, U08ET size);
void putint(unsigned int n);
int  putchar(int ch); 
void PutString(char* fmt);
void puthex(unsigned char c);
#endif /* _UARTCT_H */
