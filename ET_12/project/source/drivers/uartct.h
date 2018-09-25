#ifndef _UARTCT_H
#define _UARTCT_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define UART parameter                                           */
/*-----------------------------------------------------------------*/
#define BAUDRATE     (115200)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET uartct_init(VODET);
@far  @interrupt VODET UART2_IRQHandler(VODET);
//S08ET putchar(S08ET ch);
#endif /* _UARTCT_H */