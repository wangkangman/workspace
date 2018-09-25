#include "regdef.h"
#include "clkrst.h"
#include "powers.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_ACS  ac_power;

/*-----------------------------------------------------------------*/
/* FUNC : Power initialize                                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void powers_init(void) {
    /* AC POWER  <--> PA1 */
    GPIO_PA_DDR &= ~(1<<1);  /* direction input  */
    GPIO_PA_CR1 &= ~(1<<1);  /* float input      */
    GPIO_PA_CR2 &= ~(1<<1);  /* max speed 2MHz   */

    /* initialize power control structure */
    ac_power.lstst = 0;
    ac_power.curst = 0;
    ac_power.count = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : AC power detect task                                     */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET acpowr_task(VODET){
	if (GPIO_PA_IDR & 0x02){
			ac_power.count++;
	} else ac_power.count = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Check if AC power is off                                 */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   If power off, return ET_TRUE, else return ET_FALSE            */
/*-----------------------------------------------------------------*/
U08ET powsts_flag(VODET){
    if (ac_power.count >= AC_POW_PEROID) {
        return ET_TRUE;
    } else {
        return ET_FALSE;
    }
}
