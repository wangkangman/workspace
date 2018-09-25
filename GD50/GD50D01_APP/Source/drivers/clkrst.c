#include "regdef.h"
#include "clkrst.h"
/**
  ******************************************************************************
  * @file    keysmp.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/1/10
  * @brief   This file provides all application of clock, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + peripheral clock enabel & disable.
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
/*-----------------------------------------------------------------*/
/* FUNC : System clock initialize                                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sysclk_init(VODET)
{
#if 0
  /* unlock register */
  GCR_WPRPROT = 0x59;
  GCR_WPRPROT = 0x16;
  GCR_WPRPROT = 0x88;
  /* enable HIRC 22.1148MHz */
  CLK_PWRCON |= (1<<2);
  while( 0 == (CLK_STATUS & 0x00000010) );
  /* enable LIRC 10KHz */
  CLK_PWRCON |= (1<<3);
  while( 0 == (CLK_STATUS & 0x00000008) );
  /* select HIRC as CPUCLK/HCLK/PCLK source and divider to 1 */
  CLK_SEL0   |=  (7<<0);
  CLK_CLKDIV &= ~(0xf<<0);  
  
  /* set PLL to power down mode */
  CLK_PLLCON |= (1<<16);
  CLK_PLLCON |= (1<<19);
  
  CLK_PLLCON &= ~(1<<17);
  /* configure PLL source and frequency */
//  CLK_PLLCON = 0x00095639;
  CLK_PLLCON &= ~(0x3 << 14);
  CLK_PLLCON |= (0x1 << 14);  
  CLK_PLLCON &= ~(0x1f << 9);
  CLK_PLLCON |= (0x4 << 9);  
  CLK_PLLCON &= ~(0x1ff << 0);
  CLK_PLLCON |= (23 << 0);
  CLK_PLLCON &= ~(0x1 << 18);
  /* set PLL to normal mode */
  CLK_PLLCON &= ~(1<<16);
  while( 0 == (CLK_STATUS & 0x00000004) );

  /* set HIRC as CPUCLK/HCLK/PCLK source and divider to 1 */
  CLK_SEL0 &= ~(0x07);
  CLK_SEL0 |=  (0x02);
  CLK_CLKDIV &= ~(0xf<<0);
  /* lock register */
  GCR_WPRPROT = 0x00;
#endif

  /* unlock register */
  GCR_WPRPROT = 0x59;
  GCR_WPRPROT = 0x16;
  GCR_WPRPROT = 0x88;
  /* enable HIRC 22.1148MHz */
  CLK_PWRCON |= (1 << 2);
  while (0 == (CLK_STATUS & 0x00000010))
    ;
  /* enable LIRC 10KHz */
  CLK_PWRCON |= (1 << 3);
  while (0 == (CLK_STATUS & 0x00000008))
    ;
  /* select HIRC as CPUCLK/HCLK/PCLK source and divider to 1 */
  CLK_SEL0 |= (7 << 0);
  CLK_CLKDIV &= ~(0xf << 0);
  /* set PLL to power down mode */
  CLK_PLLCON |= (1 << 16);
  /* configure PLL source and frequency */
  CLK_PLLCON = 0x00095639;
  /* set PLL to normal mode */
  CLK_PLLCON &= ~(1 << 16);
  while (0 == (CLK_STATUS & 0x00000004))
    ;

  /* set HIRC as CPUCLK/HCLK/PCLK source and divider to 1 */
  CLK_SEL0 &= ~(0x0F);
  CLK_SEL0 |= (0x07);
  CLK_CLKDIV &= ~(0xf << 0);
  /* lock register */
  GCR_WPRPROT = 0x00;
}

/**
  * @brief  Unlock reg
  * @param  
  *   none;
  * @retval 
  *   none;
  *
  */
VODET sysunlock_reg(VODET)
{
  GCR_WPRPROT = 0x59;
  GCR_WPRPROT = 0x16;
  GCR_WPRPROT = 0x88;
}

/**
  * @brief  lock reg
  * @param  
  *   none;
  * @retval 
  *   none;
  *
  */
VODET syslock_reg(VODET)
{
  GCR_WPRPROT = 0x00;
}
