#ifndef _CLKRSTS_H
#define _CLKRSTS_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define system clock frequency                                   */
/*-----------------------------------------------------------------*/
#define CLK_CPU_FREQ (16000000)
#define CLK_PERH_FREQ (16000000)

/*-----------------------------------------------------------------*/
/* Define clock control code                                       */
/*-----------------------------------------------------------------*/
#define CLK_CT_ENA (ET_TRUE)
#define CLK_CT_DIS (ET_FALSE)

/*-----------------------------------------------------------------*/
/* Define clock ID                                                 */
/*-----------------------------------------------------------------*/
#define CLK_ID_ALL 0
#define CLK_ID_ADC 1
#define CLK_ID_UART2 2
#define CLK_ID_TIME1 3
#define CLK_ID_TIME2 4
#define CLK_ID_TIME3 5

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/
void sysclk_init(void);
void sysclk_ctrl(u8 clkid, u8 ctrls);

#endif /* _CLKRSTS_H */
