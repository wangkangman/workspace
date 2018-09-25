#include "regdef.h"
#include "clkrst.h"
#include "spi.h"
#include "stdio.h"
#include "timwdt.h"

/**
  ******************************************************************************
  * @file    spi.c
  * @author  ETdoor team
  * @version V0.0.2
  * @date    2018/3/11
  * @brief   This file provides all application of spi, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + rec rf data from TTI equipment.
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
  * V0.0.1: filter wrong data;
  * V0.0.2: add over time process;
  *
  *
  ******************************************************************************
  */

static ETST_SPI et_spi;
/**
  * @brief  spi module inital;
  * @param  
  *   none;
  * @retval
  *   none;
  *
  */
VODET SPI_Slave_Configuration(VODET)
{
  U32ET index;
  /* unlock register */
  GCR_WPRPROT = 0x59;
  GCR_WPRPROT = 0x16;
  GCR_WPRPROT = 0x88;
  /* SPI1_CLK   <--> P0.7 */
  GCR_P0MFP &= ~((U32ET)1 << 7);
  GCR_P0MFP |= ((U32ET)1 << 15);
  /* SPI1_MOSI  <--> P0.5 */
  GCR_P0MFP &= ~((U32ET)1 << 5);
  GCR_P0MFP |= ((U32ET)1 << 13);
  /* SPI1_CS    <--> P0.4 */
  GCR_P0MFP &= ~((U32ET)1 << 4);
  GCR_P0MFP |= ((U32ET)1 << 12);
  /* select HCLK as SPI1 clock source */
  CLK_SEL1 |= (1 << 5);
  /* enable SPI1 clock */
  CLK_APB |= (1 << 13);
  /* slave mode */
  SPI1_CNTRL |= (1 << 18);
  /* bit length == 8 */
  SPI1_CNTRL &= ~(0x1f << 3);
  SPI1_CNTRL |= (0x08 << 3);
  /* spi mode */
  /* cpol = 0*/
  SPI1_CNTRL &= ~(1 << 11);
  /* cha = 1*/
  SPI1_CNTRL |= (1 << 1);
  SPI1_CNTRL &= ~(1 << 2);
  /* MSB first */
  SPI1_CNTRL &= ~(1 << 10);
  /* enable fifo */
  SPI1_CNTRL |= (1 << 21);
  /* slave select signal is low level active */
  SPI1_SSR |= 1 << 4;
  SPI1_SSR |= 1 << 3;
  SPI1_SSR |= 1 << 0;
  /* enable SPI interrupt */
  SPI1_FIFO_CTL = 0;
  SPI1_FIFO_CTL |= (1 << 2);
  SPI1_DIVIDER = 5;
  for (index = 0; index < SPI_LEN_MAX; index++)
    et_spi.buf[index] = 0x0;
		
  et_spi.step = 0;
  et_spi.overtime = 0;
  et_spi.state = 0;
  et_spi.isfull = 0;
  et_spi.ignore = 0;
  /* unit irq enable */
  SPI1_CNTRL |= (1 << 17);
  NVIC_ISER = (1 << 15);
  /* lock register */
  GCR_WPRPROT = 0x00;
}

/**
  * @brief  spi irq task;
  * @param  NONE
  * @retval NONE
  *
  */
VODET SPI1_IRQHandler(VODET)
{
  if (SPI1_CNTRL & (1 << 16)) {
    SPI1_CNTRL |= (1 << 16);
    if (0 == et_spi.ignore) {
      et_spi.overtime = 0;
      et_spi.buf[et_spi.step++] = SPI1_RX0;
      if (8 == et_spi.step) {
        et_spi.step = 0;
        if (0 == et_spi.isfull) 
					et_spi.isfull = 1;
      }
    } else {
      et_spi.garbage = SPI1_RX0;
    }
  }
}

/**
  * @brief  spi rx data over time;
  * @param  NONE
  * @retval NONE
  *
  */
VODET SPI_overtime_task(VODET)
{
  if (1 == et_spi.isfull 
		 || et_spi.step > 0)
    et_spi.overtime++;

  if (SPI_OVERTIME_CNT == et_spi.overtime) {
    if ((1 == et_spi.isfull)
       && (0 == et_spi.step)) {
      et_spi.state = 1;
      et_spi.ignore = 1;
    } else {
      et_spi.state = 0;
      et_spi.isfull = 0;
      et_spi.step = 0;
      et_spi.overtime = 0;
      et_spi.ignore = 0;
    }
  } else if (SPI_IGNOR_CNT == et_spi.overtime) {
    et_spi.state = 0;
    et_spi.isfull = 0;
    et_spi.step = 0;
    et_spi.overtime = 0;
    et_spi.ignore = 0;
  }
}

/**
  * @brief  get data from spi buf;
  * @param  
  *   pbuf: piont to the app buf
  * @retval NONE
  *   ET_TRUE: buf is full
  *   ET_TRUE: buf is empty
  *
  */
U08ET SPI_get_data(U08ET *pbuf)
{
  if (0 == et_spi.state)
    return ET_FALSE;

  pbuf[0] = et_spi.buf[6];
  pbuf[1] = et_spi.buf[7];
  pbuf[2] = et_spi.buf[4];
  pbuf[3] = et_spi.buf[5];
  pbuf[4] = et_spi.buf[0];
  pbuf[5] = et_spi.buf[1];
  pbuf[6] = et_spi.buf[2];
  pbuf[7] = et_spi.buf[3];

  /*  disable the data buf */
  et_spi.state = 0;
  et_spi.overtime += 5;
  return ET_TRUE;
}
