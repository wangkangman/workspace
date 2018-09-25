#include "regdef.h"
#include "ClkRst.h"

/**
  * @brief  System clock initialize
  * @param  NONE 
  * @retval NONE
  *
  */
VODET sysclk_init(VODET) {
  /* HSI prescale = 1, CPU clock prescale = 1 */
  CLK_CKDIVR = (0<<3) + (0<<0);
  /* enable HSI & LSI oscillator <LSI for iwdg>*/
  CLK_ICKR = (1<<0) | (1<<3);
  /* disable all peripheral clock */
  CLK_PCKENR1 = 0x00;
  CLK_PCKENR2 = 0x00;
}


/**
  * @brief  System clock control(enable or disable)
  * @param  
  *   clkid : clock ID
  *   ctrls : clock control code 
  * @retval NONE
  *
  */
VODET sysclk_ctrl(U08ET clkid, U08ET ctrls) {
  switch (clkid) {
    case CLK_ID_ALL:
      if (CLK_CT_DIS == ctrls) {
        CLK_PCKENR1 = 0x00;
        CLK_PCKENR2 = 0x00;
      }
      
      if (CLK_CT_ENA == ctrls) {
        CLK_PCKENR1 = 0xFF;
        CLK_PCKENR2 = 0xFF;
      }
      break;
    case CLK_ID_ADC:
      if (CLK_CT_DIS == ctrls) {
        CLK_PCKENR2 &= ~(1<<3);
      }
      if (CLK_CT_ENA == ctrls) {
        CLK_PCKENR2 |=  (1<<3);
      }
      break;
    case CLK_ID_UART2:
      if (CLK_CT_DIS == ctrls) {
        CLK_PCKENR1 &= ~(1<<3);
      }

      if (CLK_CT_ENA == ctrls) {
        CLK_PCKENR1 |=  (1<<3);
      }
      break;
    case CLK_ID_TIME1:
      if (CLK_CT_DIS == ctrls) {
        CLK_PCKENR1 &= ~(1<<7);
      }

      if (CLK_CT_ENA == ctrls) {
        CLK_PCKENR1 |=  (1<<7);
      }
      break;
    case CLK_ID_TIME2:
      if (CLK_CT_DIS == ctrls) {
        CLK_PCKENR1 &= ~(1<<5);
      }

      if (CLK_CT_ENA == ctrls) {
        CLK_PCKENR1 |=  (1<<5);
      }
      break;
    case CLK_ID_TIME3:
      if (CLK_CT_DIS == ctrls) {
        CLK_PCKENR1 &= ~(1<<6);
      }
      if (CLK_CT_ENA == ctrls) {
        CLK_PCKENR1 |=  (1<<6);
      }
      break;
    case CLK_ID_SPI:
      if (CLK_CT_DIS == ctrls) {
        CLK_PCKENR1 &= ~(1<<1);
      }
      if (CLK_CT_ENA == ctrls) {
        CLK_PCKENR1 |=  (1<<1);
      }
      break;
    default:
      break;
  }
}
