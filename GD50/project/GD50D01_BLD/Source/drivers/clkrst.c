#include "regdef.h"
#include "clkrst.h"

/*-----------------------------------------------------------------*/
/* FUNC : System clock initialize                                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sysclk_init(VODET)
{
    /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;

    /* enable HIRC 22.1148MHz */
    CLK_PWRCON |= (1<<2);
    while( 0 == (CLK_STATUS & 0x00000010) );

    /* select HIRC as CPUCLK/HCLK/PCLK source and divider to 1 */
    CLK_SEL0   |=  (7<<0);
    CLK_CLKDIV &= ~(15<<0);

    /* lock register */
    GCR_WPRPROT = 0x00;
}


/*-----------------------------------------------------------------*/
/* FUNC : CORTEX RESET                                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET mcu_reset(VODET)
{
	 /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;
	
	
   GCR_IPRSTC1 |= (1<<3);
}
