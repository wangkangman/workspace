#ifndef _REGDEF_H
#define _REGDEF_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Port register define                                            */
/*-----------------------------------------------------------------*/
#define GPIO_REG_BASE (0x5000)

#define GPIO_PA_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x00))
#define GPIO_PA_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x01))
#define GPIO_PA_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x02))
#define GPIO_PA_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x03))
#define GPIO_PA_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x04))

#define GPIO_PB_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x05))
#define GPIO_PB_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x06))
#define GPIO_PB_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x07))
#define GPIO_PB_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x08))
#define GPIO_PB_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x09))

#define GPIO_PC_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x0A))
#define GPIO_PC_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x0B))
#define GPIO_PC_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x0C))
#define GPIO_PC_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x0D))
#define GPIO_PC_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x0E))

#define GPIO_PD_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x0F))
#define GPIO_PD_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x10))
#define GPIO_PD_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x11))
#define GPIO_PD_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x12))
#define GPIO_PD_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x13))

#define GPIO_PE_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x14))
#define GPIO_PE_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x15))
#define GPIO_PE_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x16))
#define GPIO_PE_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x17))
#define GPIO_PE_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x18))

#define GPIO_PF_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x19))
#define GPIO_PF_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x1A))
#define GPIO_PF_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x1B))
#define GPIO_PF_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x1C))
#define GPIO_PF_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x1D))

#define GPIO_PG_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x1E))
#define GPIO_PG_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x1F))
#define GPIO_PG_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x20))
#define GPIO_PG_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x21))
#define GPIO_PG_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x22))

#define GPIO_PH_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x23))
#define GPIO_PH_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x24))
#define GPIO_PH_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x25))
#define GPIO_PH_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x26))
#define GPIO_PH_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x27))

#define GPIO_PI_ODR (*(volatile u8 *)(GPIO_REG_BASE + 0x28))
#define GPIO_PI_IDR (*(volatile u8 *)(GPIO_REG_BASE + 0x29))
#define GPIO_PI_DDR (*(volatile u8 *)(GPIO_REG_BASE + 0x2A))
#define GPIO_PI_CR1 (*(volatile u8 *)(GPIO_REG_BASE + 0x2B))
#define GPIO_PI_CR2 (*(volatile u8 *)(GPIO_REG_BASE + 0x2C))

/*-----------------------------------------------------------------*/
/* Clock register define                                           */
/*-----------------------------------------------------------------*/
#define CLK_REG_BASE (0x50C0)

#define CLK_ICKR (*(volatile u8 *)(CLK_REG_BASE + 0x00))
#define CLK_ECKR (*(volatile u8 *)(CLK_REG_BASE + 0x01))
#define CLK_RSV1 (*(volatile u8 *)(CLK_REG_BASE + 0x02))
#define CLK_CMSR (*(volatile u8 *)(CLK_REG_BASE + 0x03))
#define CLK_SWR (*(volatile u8 *)(CLK_REG_BASE + 0x04))
#define CLK_SWCR (*(volatile u8 *)(CLK_REG_BASE + 0x05))
#define CLK_CKDIVR (*(volatile u8 *)(CLK_REG_BASE + 0x06))
#define CLK_PCKENR1 (*(volatile u8 *)(CLK_REG_BASE + 0x07))
#define CLK_CSSR (*(volatile u8 *)(CLK_REG_BASE + 0x08))
#define CLK_CCOR (*(volatile u8 *)(CLK_REG_BASE + 0x09))
#define CLK_PCKENR2 (*(volatile u8 *)(CLK_REG_BASE + 0x0A))
#define CLK_RSV2 (*(volatile u8 *)(CLK_REG_BASE + 0x0B))
#define CLK_HSITRIMR (*(volatile u8 *)(CLK_REG_BASE + 0x0C))
#define CLK_SWIMCCR (*(volatile u8 *)(CLK_REG_BASE + 0x0D))

/*-----------------------------------------------------------------*/
/* Watchdog register define                                        */
/*-----------------------------------------------------------------*/
#define WDG_REG_BASE (0x50D1)
#define WWDG_CR (*(volatile u8 *)(WDG_REG_BASE + 0x00))
#define WWDG_WR (*(volatile u8 *)(WDG_REG_BASE + 0x01))

/*-----------------------------------------------------------------*/
/* Uart2 register define                                           */
/*-----------------------------------------------------------------*/
#define UART2_REG_BASE (0x5240)
#define UART2_SR (*(volatile u8 *)(UART2_REG_BASE + 0x00))
#define UART2_DR (*(volatile u8 *)(UART2_REG_BASE + 0x01))
#define UART2_BRR1 (*(volatile u8 *)(UART2_REG_BASE + 0x02))
#define UART2_BRR2 (*(volatile u8 *)(UART2_REG_BASE + 0x03))
#define UART2_CR1 (*(volatile u8 *)(UART2_REG_BASE + 0x04))
#define UART2_CR2 (*(volatile u8 *)(UART2_REG_BASE + 0x05))
#define UART2_CR3 (*(volatile u8 *)(UART2_REG_BASE + 0x06))
#define UART2_CR4 (*(volatile u8 *)(UART2_REG_BASE + 0x07))
#define UART2_CR5 (*(volatile u8 *)(UART2_REG_BASE + 0x08))
#define UART2_CR6 (*(volatile u8 *)(UART2_REG_BASE + 0x09))
#define UART2_GTR (*(volatile u8 *)(UART2_REG_BASE + 0x0A))
#define UART2_PSCR (*(volatile u8 *)(UART2_REG_BASE + 0x0B))

/*-----------------------------------------------------------------*/
/* Timer1 register define                                          */
/*-----------------------------------------------------------------*/
#define TIM1_REG_BASE (0x5250)
#define TIM1_CR1 (*(volatile u8 *)(TIM1_REG_BASE + 0x00))
#define TIM1_CR2 (*(volatile u8 *)(TIM1_REG_BASE + 0x01))
#define TIM1_SMCR (*(volatile u8 *)(TIM1_REG_BASE + 0x02))
#define TIM1_ETR (*(volatile u8 *)(TIM1_REG_BASE + 0x03))
#define TIM1_IER (*(volatile u8 *)(TIM1_REG_BASE + 0x04))
#define TIM1_SR1 (*(volatile u8 *)(TIM1_REG_BASE + 0x05))
#define TIM1_SR2 (*(volatile u8 *)(TIM1_REG_BASE + 0x06))
#define TIM1_EGR (*(volatile u8 *)(TIM1_REG_BASE + 0x07))
#define TIM1_CCMR1 (*(volatile u8 *)(TIM1_REG_BASE + 0x08))
#define TIM1_CCMR2 (*(volatile u8 *)(TIM1_REG_BASE + 0x09))
#define TIM1_CCMR3 (*(volatile u8 *)(TIM1_REG_BASE + 0x0A))
#define TIM1_CCMR4 (*(volatile u8 *)(TIM1_REG_BASE + 0x0B))
#define TIM1_CCER1 (*(volatile u8 *)(TIM1_REG_BASE + 0x0C))
#define TIM1_CCER2 (*(volatile u8 *)(TIM1_REG_BASE + 0x0D))
#define TIM1_CNTRH (*(volatile u8 *)(TIM1_REG_BASE + 0x0E))
#define TIM1_CNTRL (*(volatile u8 *)(TIM1_REG_BASE + 0x0F))
#define TIM1_PSCRH (*(volatile u8 *)(TIM1_REG_BASE + 0x10))
#define TIM1_PSCRL (*(volatile u8 *)(TIM1_REG_BASE + 0x11))
#define TIM1_ARRH (*(volatile u8 *)(TIM1_REG_BASE + 0x12))
#define TIM1_ARRL (*(volatile u8 *)(TIM1_REG_BASE + 0x13))
#define TIM1_RCR (*(volatile u8 *)(TIM1_REG_BASE + 0x14))
#define TIM1_CCR1H (*(volatile u8 *)(TIM1_REG_BASE + 0x15))
#define TIM1_CCR1L (*(volatile u8 *)(TIM1_REG_BASE + 0x16))
#define TIM1_CCR2H (*(volatile u8 *)(TIM1_REG_BASE + 0x17))
#define TIM1_CCR2L (*(volatile u8 *)(TIM1_REG_BASE + 0x18))
#define TIM1_CCR3H (*(volatile u8 *)(TIM1_REG_BASE + 0x19))
#define TIM1_CCR3L (*(volatile u8 *)(TIM1_REG_BASE + 0x1A))
#define TIM1_CCR4H (*(volatile u8 *)(TIM1_REG_BASE + 0x1B))
#define TIM1_CCR4L (*(volatile u8 *)(TIM1_REG_BASE + 0x1C))
#define TIM1_BKR (*(volatile u8 *)(TIM1_REG_BASE + 0x1D))
#define TIM1_DTR (*(volatile u8 *)(TIM1_REG_BASE + 0x1E))
#define TIM1_OISR (*(volatile u8 *)(TIM1_REG_BASE + 0x1F))

/*-----------------------------------------------------------------*/
/* Timer2 register define                                          */
/*-----------------------------------------------------------------*/
#define TIM2_REG_BASE (0x5300)
#define TIM2_CR1 (*(volatile u8 *)(TIM2_REG_BASE + 0x00))
#define TIM2_IER (*(volatile u8 *)(TIM2_REG_BASE + 0x01))
#define TIM2_SR1 (*(volatile u8 *)(TIM2_REG_BASE + 0x02))
#define TIM2_SR2 (*(volatile u8 *)(TIM2_REG_BASE + 0x03))
#define TIM2_EGR (*(volatile u8 *)(TIM2_REG_BASE + 0x04))
#define TIM2_CCMR1 (*(volatile u8 *)(TIM2_REG_BASE + 0x05))
#define TIM2_CCMR2 (*(volatile u8 *)(TIM2_REG_BASE + 0x06))
#define TIM2_CCMR3 (*(volatile u8 *)(TIM2_REG_BASE + 0x07))
#define TIM2_CCER1 (*(volatile u8 *)(TIM2_REG_BASE + 0x08))
#define TIM2_CCER2 (*(volatile u8 *)(TIM2_REG_BASE + 0x09))
#define TIM2_CNTRH (*(volatile u8 *)(TIM2_REG_BASE + 0x0A))
#define TIM2_CNTRL (*(volatile u8 *)(TIM2_REG_BASE + 0x0B))
#define TIM2_PSCR (*(volatile u8 *)(TIM2_REG_BASE + 0x0C))
#define TIM2_ARRH (*(volatile u8 *)(TIM2_REG_BASE + 0x0D))
#define TIM2_ARRL (*(volatile u8 *)(TIM2_REG_BASE + 0x0E))
#define TIM2_CCR1H (*(volatile u8 *)(TIM2_REG_BASE + 0x0F))
#define TIM2_CCR1L (*(volatile u8 *)(TIM2_REG_BASE + 0x10))
#define TIM2_CCR2H (*(volatile u8 *)(TIM2_REG_BASE + 0x11))
#define TIM2_CCR2L (*(volatile u8 *)(TIM2_REG_BASE + 0x12))
#define TIM2_CCR3H (*(volatile u8 *)(TIM2_REG_BASE + 0x13))
#define TIM2_CCR3L (*(volatile u8 *)(TIM2_REG_BASE + 0x14))

/*-----------------------------------------------------------------*/
/* Timer3 register define                                          */
/*-----------------------------------------------------------------*/
#define TIM3_REG_BASE (0x5320)
#define TIM3_CR1 (*(volatile u8 *)(TIM3_REG_BASE + 0x00))
#define TIM3_IER (*(volatile u8 *)(TIM3_REG_BASE + 0x01))
#define TIM3_SR1 (*(volatile u8 *)(TIM3_REG_BASE + 0x02))
#define TIM3_SR2 (*(volatile u8 *)(TIM3_REG_BASE + 0x03))
#define TIM3_EGR (*(volatile u8 *)(TIM3_REG_BASE + 0x04))
#define TIM3_CCMR1 (*(volatile u8 *)(TIM3_REG_BASE + 0x05))
#define TIM3_CCMR2 (*(volatile u8 *)(TIM3_REG_BASE + 0x06))
#define TIM3_CCER1 (*(volatile u8 *)(TIM3_REG_BASE + 0x07))
#define TIM3_CNTRH (*(volatile u8 *)(TIM3_REG_BASE + 0x08))
#define TIM3_CNTRL (*(volatile u8 *)(TIM3_REG_BASE + 0x09))
#define TIM3_PSCR (*(volatile u8 *)(TIM3_REG_BASE + 0x0A))
#define TIM3_ARRH (*(volatile u8 *)(TIM3_REG_BASE + 0x0B))
#define TIM3_ARRL (*(volatile u8 *)(TIM3_REG_BASE + 0x0C))
#define TIM3_CCR1H (*(volatile u8 *)(TIM3_REG_BASE + 0x0D))
#define TIM3_CCR1L (*(volatile u8 *)(TIM3_REG_BASE + 0x0E))
#define TIM3_CCR2H (*(volatile u8 *)(TIM3_REG_BASE + 0x0F))
#define TIM3_CCR2L (*(volatile u8 *)(TIM3_REG_BASE + 0x10))

/*-----------------------------------------------------------------*/
/* Adc register define                                             */
/*-----------------------------------------------------------------*/
#define ADC_REG_BASE (0x53E0)
#define ADC_DBXR (*(volatile u8 *)(ADC_REG_BASE + 0x00))
#define ADC_CSR (*(volatile u8 *)(ADC_REG_BASE + 0x20))
#define ADC_CR1 (*(volatile u8 *)(ADC_REG_BASE + 0x21))
#define ADC_CR2 (*(volatile u8 *)(ADC_REG_BASE + 0x22))
#define ADC_CR3 (*(volatile u8 *)(ADC_REG_BASE + 0x23))
#define ADC_DRH (*(volatile u8 *)(ADC_REG_BASE + 0x24))
#define ADC_DRL (*(volatile u8 *)(ADC_REG_BASE + 0x25))
#define ADC_TDRH (*(volatile u8 *)(ADC_REG_BASE + 0x26))
#define ADC_TDRL (*(volatile u8 *)(ADC_REG_BASE + 0x27))
#define ADC_HTRH (*(volatile u8 *)(ADC_REG_BASE + 0x28))
#define ADC_HTRL (*(volatile u8 *)(ADC_REG_BASE + 0x29))
#define ADC_LTRH (*(volatile u8 *)(ADC_REG_BASE + 0x2A))
#define ADC_LTRL (*(volatile u8 *)(ADC_REG_BASE + 0x2B))
#define ADC_AWSRH (*(volatile u8 *)(ADC_REG_BASE + 0x2C))
#define ADC_AWSRL (*(volatile u8 *)(ADC_REG_BASE + 0x2D))
#define ADC_AWCRH (*(volatile u8 *)(ADC_REG_BASE + 0x2E))
#define ADC_AWCRL (*(volatile u8 *)(ADC_REG_BASE + 0x2F))

/*-----------------------------------------------------------------*/
/* Eeprom register define                                          */
/*-----------------------------------------------------------------*/
#define FLASH_REG_BASE (0x505A)
#define FLASH_CR1 (*(volatile u8 *)(FLASH_REG_BASE + 0x00))
#define FLASH_CR2 (*(volatile u8 *)(FLASH_REG_BASE + 0x01))
#define FLASH_NCR2 (*(volatile u8 *)(FLASH_REG_BASE + 0x02))
#define FLASH_FPR (*(volatile u8 *)(FLASH_REG_BASE + 0x03))
#define FLASH_NFPR (*(volatile u8 *)(FLASH_REG_BASE + 0x04))
#define FLASH_IAPSR (*(volatile u8 *)(FLASH_REG_BASE + 0x05))
#define FLASH_PUKR (*(volatile u8 *)(FLASH_REG_BASE + 0x08))
#define FLASH_DUKR (*(volatile u8 *)(FLASH_REG_BASE + 0x0A))

#endif /* _REGDEF_H */
