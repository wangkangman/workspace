#include "pwm_capture.h"
#include "regdef.h"

/**
  ******************************************************************************
  * @file    pwm capture.c
  * @author  ETdoor team
  * @version V0.0.2
  * @date    2018/8/11
  * @brief   This file provides all application of pwm capture,
  *          functionalities of the pwm capture:
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

static ETST_POSITION st_position;

/**
 * @brief       PWMB capture initial
 *
 * @param       None
 *
 * @return      None
 *
 * @details     PWMB capture for hall sensor
 */
void pwm_capture_init(void) {
  U08ET index;

  /* unlock register */
  GCR_WPRPROT = 0x59;
  GCR_WPRPROT = 0x16;
  GCR_WPRPROT = 0x88;

  /* PWM6  <--> P2.6 */
  GCR_P2MFP &= ~((U32ET)1 << 4);
  GCR_P2MFP |= ((U32ET)1 << 12);

  /* select HCLK as PWM67 clock source */
  // CLK_SEL2 |= (CLK_SEL2 & ~(0X3 << 6)) | (2 << 6);
  CLK_SEL2 |= ((U32ET)2 << 4);
  /* enable PWM67 clock */
  CLK_APB |= ((U32ET)1 << 22);

  /*Set Pwm mode*/
  PWMB_PCR |= (1 << 3);
#if 0
	/* set PWM23 clock pre divider to 11 */
  PWMB_PPR &= 0x00ff;
  PWMB_PPR |= ((U32ET)10 << 0);
#endif
  /*Set PWM Timer clock prescaler*/
  PWMB_PPR = (PWMB_PPR & ~(0XFF << 0)) | (1 << 0);
  /*Set PWM Timer clock divider 1 select*/
  PWMB_CSR = (PWMB_CSR & ~(7 << 0)) | (4 << 0);
  /*Set PWM Timer loaded value*/
  PWMB_CNR0 = 0xFFFF;
  // PWMB_CNR2 = 0x7000;
  /* Enable capture falling edge interrupt for PWMB channel 2 */
  PWMB_CCR0 = (PWMB_CCR0 & 0x000F000F) | (1 << 2);
  /* Enable capture pwm6 input reverse */
  PWMB_CCR0 = (PWMB_CCR0 & 0x000F000F) | (1 << 0);
  /* Enable PWMB NVIC interrupt */
  NVIC_ISER |= (1 << 7);

//  PWMB_PIER |= (1 << 10);
  PWMB_PIER |= (1 << 8);

  /* Enable Capture Function for PWMB channel 2 */
  PWMB_CCR0 = (PWMB_CCR0 & 0x000F000F) | (1 << 3);
  /* Enable Timer for PWMB channel 2  */
  PWMB_PCR |= (1 << 0);

  /* Wait until PWMB channel 2 Timer start to count */
  // while(PWMB_PDR2 == 0);

  /* Enable capture input path for PWMB channel 2 */
  PWMB_CAPENR |= (1 << 0);

  /* lock register */
  GCR_WPRPROT = 0x00;

  /* initial struct data*/
  st_position.step = 0;
  for (index = 0; index < 8; index++) {
    st_position.pulse[index] = 0;
    st_position.cycle[index] = 0;
  }
  st_position.overflow_cnt = 0;
}

/**
 * @brief       PWMB IRQ Handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     ISR to handle PWMB interrupt event
 */
void PWMB_IRQHandler(void) {
  U32ET u32CapIntFlag1;

  if (PWMB_PIIR & (1 << 8)) {
    if (st_position.overflow_cnt < 0xff) st_position.overflow_cnt++;
    PWMB_PIIR |= (1 << 8);
  }

  /* Handle PWMB Capture function */
  u32CapIntFlag1 = PWMB_CCR0;
  /* PWMB channel 2 Capture interrupt */
  if (u32CapIntFlag1 & (1 << 4)) {
    st_position.pulse[st_position.step] = 0xffff - PWMB_CRLR0;
    st_position.cycle[st_position.step] = 0xffff - PWMB_CFLR0;
    st_position.step = (st_position.step + 1) % PWM_FILTER_NUM;
    if (st_position.overflow_cnt > 0) st_position.overflow_cnt--;
    PWMB_CCR0 &= (0x000F000F | (1 << 4));
  }
}

// U16ET sensor_val_get(void) { return 11; }

/**
 * @brief       get pulse buf data
 *
 * @param       None
 *
 * @return      None
 *
 * @details     get buf data <pulse>
 */
VODET position_buf_get(U16ET *pbuf) {
  U08ET index;
  for (index = 0; index < PWM_FILTER_NUM; index++) pbuf[index] = st_position.pulse[index];
}

/**
 * @brief       get cycle buf data
 *
 * @param       None
 *
 * @return      None
 *
 * @details     get buf data <cycle>
 */
VODET cycle_buf_get(U16ET *pbuf) {
  U08ET index;
  for (index = 0; index < PWM_FILTER_NUM; index++) pbuf[index] = st_position.cycle[index];
}

/**
 * @brief       get pwm capture over flow cnt
 *
 * @param       None
 *
 * @return      None
 *
 * @details     get buf data <over flow>
 */
U16ET overflow_cnt_get(void) { return st_position.overflow_cnt; }
