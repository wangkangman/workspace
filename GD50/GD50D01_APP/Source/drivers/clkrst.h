#ifndef _CLKRSTS_H
#define _CLKRSTS_H

#include "global.h"
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
/* Define system clock frequency                                   */
/*-----------------------------------------------------------------*/
#define CLK_CPU_FREQ (50000000)
#define CLK_HCLK_FREQ (50000000)
#define CLK_HIRC_FREQ (22118400)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET sysclk_init(VODET);
VODET sysunlock_reg(VODET);
VODET syslock_reg(VODET);

#endif /* _CLKRSTS_H */
