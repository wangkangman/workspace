#ifndef _CLKSET_H
#define _CLKSET_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define system clock frequency                                   */
/*-----------------------------------------------------------------*/
#define CLK_IH_FREQ 24000000
#define CLK_IL_FREQ    15000

/*-----------------------------------------------------------------*/
/* Define clock control code                                       */
/*-----------------------------------------------------------------*/
#define CLK_CT_XXX    0
#define CLK_CT_DIS    1
#define CLK_CT_ENA    2
#define CLK_CT_MAX    3

/*-----------------------------------------------------------------*/
/* Define clock unit ID                                            */
/*-----------------------------------------------------------------*/
#define CLK_ID_XXX    0
#define CLK_ID_TM0    1
#define CLK_ID_UART   2
#define CLK_ID_ADC    3
#define CLK_ID_MAX    4

/*-----------------------------------------------------------------*/
/* Define clock and reset control structure                        */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET rstFlag;
}ETST_CLKRST;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void clkrst_init(void);
void clkset_ctrl(U08ET clkid, U08ET ctrl);

#endif /* _CLKRST_H */
