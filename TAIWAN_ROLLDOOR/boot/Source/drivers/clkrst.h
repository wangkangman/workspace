#ifndef _CLKRSTS_H
#define _CLKRSTS_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define system clock frequency                                   */
/*-----------------------------------------------------------------*/
#define CLK_CPU_FREQ  (22118400)
#define CLK_HCLK_FREQ (22118400)
#define CLK_HIRC_FREQ (22118400)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET sysclk_init(VODET);
VODET mcu_reset(VODET);
#endif /* _CLKRSTS_H */
