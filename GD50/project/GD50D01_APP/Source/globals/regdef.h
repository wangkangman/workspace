#ifndef _REGDEF_H
#define _REGDEF_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Global controller register define                               */
/*-----------------------------------------------------------------*/
#define GCR_REG_BASE (0x50000000)

#define GCR_PDID (*(volatile U32ET *)(GCR_REG_BASE + 0x0000))
#define GCR_RSTSRC (*(volatile U32ET *)(GCR_REG_BASE + 0x0004))
#define GCR_IPRSTC1 (*(volatile U32ET *)(GCR_REG_BASE + 0x0008))
#define GCR_IPRSTC2 (*(volatile U32ET *)(GCR_REG_BASE + 0x000C))
#define GCR_BODCR (*(volatile U32ET *)(GCR_REG_BASE + 0x0018))
#define GCR_TEMPCR (*(volatile U32ET *)(GCR_REG_BASE + 0x001C))
#define GCR_PORCR (*(volatile U32ET *)(GCR_REG_BASE + 0x0024))
#define GCR_P0MFP (*(volatile U32ET *)(GCR_REG_BASE + 0x0030))
#define GCR_P1MFP (*(volatile U32ET *)(GCR_REG_BASE + 0x0034))
#define GCR_P2MFP (*(volatile U32ET *)(GCR_REG_BASE + 0x0038))
#define GCR_P3MFP (*(volatile U32ET *)(GCR_REG_BASE + 0x003C))
#define GCR_P4MFP (*(volatile U32ET *)(GCR_REG_BASE + 0x0040))
#define GCR_WPRPROT (*(volatile U32ET *)(GCR_REG_BASE + 0x0100))

/*-----------------------------------------------------------------*/
/* Clock register define                                           */
/*-----------------------------------------------------------------*/
#define CLK_REG_BASE (0x50000200)

#define CLK_PWRCON (*(volatile U32ET *)(CLK_REG_BASE + 0x0000))
#define CLK_AHB (*(volatile U32ET *)(CLK_REG_BASE + 0x0004))
#define CLK_APB (*(volatile U32ET *)(CLK_REG_BASE + 0x0008))
#define CLK_STATUS (*(volatile U32ET *)(CLK_REG_BASE + 0x000C))
#define CLK_SEL0 (*(volatile U32ET *)(CLK_REG_BASE + 0x0010))
#define CLK_SEL1 (*(volatile U32ET *)(CLK_REG_BASE + 0x0014))
#define CLK_CLKDIV (*(volatile U32ET *)(CLK_REG_BASE + 0x0018))
#define CLK_SEL2 (*(volatile U32ET *)(CLK_REG_BASE + 0x001C))
#define CLK_PLLCON (*(volatile U32ET *)(CLK_REG_BASE + 0x0020))
#define CLK_FRQDIV (*(volatile U32ET *)(CLK_REG_BASE + 0x0024))

/*-----------------------------------------------------------------*/
/* NVIC register define                                            */
/*-----------------------------------------------------------------*/
#define NVIC_REG_BASE (0xE000E100)
#define NVIC_ISER (*(volatile U32ET *)(NVIC_REG_BASE + 0x0000))
#define NVIC_ICER (*(volatile U32ET *)(NVIC_REG_BASE + 0x0080))
#define NVIC_ISPR (*(volatile U32ET *)(NVIC_REG_BASE + 0x0100))
#define NVIC_ICPR (*(volatile U32ET *)(NVIC_REG_BASE + 0x0180))
#define NVIC_IPR0 (*(volatile U32ET *)(NVIC_REG_BASE + 0x0300))
#define NVIC_IPR1 (*(volatile U32ET *)(NVIC_REG_BASE + 0x0304))
#define NVIC_IPR2 (*(volatile U32ET *)(NVIC_REG_BASE + 0x0308))
#define NVIC_IPR3 (*(volatile U32ET *)(NVIC_REG_BASE + 0x030C))
#define NVIC_IPR4 (*(volatile U32ET *)(NVIC_REG_BASE + 0x0310))
#define NVIC_IPR5 (*(volatile U32ET *)(NVIC_REG_BASE + 0x0314))
#define NVIC_IPR6 (*(volatile U32ET *)(NVIC_REG_BASE + 0x0318))
#define NVIC_IPR7 (*(volatile U32ET *)(NVIC_REG_BASE + 0x031C))

/*-----------------------------------------------------------------*/
/* Watchdog register define                                        */
/*-----------------------------------------------------------------*/
#define WDT_REG_BASE (0x40004000)
#define WDT_WTCR (*(volatile U32ET *)(WDT_REG_BASE + 0x0000))
#define WDT_WTCRALT (*(volatile U32ET *)(WDT_REG_BASE + 0x0004))

/*-----------------------------------------------------------------*/
/* Uart register define                                            */
/*-----------------------------------------------------------------*/
#define UART0_REG_BASE (0x40050000)
#define UART1_REG_BASE (0x40150000)

#define UART0_RBR (*(volatile U32ET *)(UART0_REG_BASE + 0x0000))
#define UART0_THR (*(volatile U32ET *)(UART0_REG_BASE + 0x0000))
#define UART0_IER (*(volatile U32ET *)(UART0_REG_BASE + 0x0004))
#define UART0_FCR (*(volatile U32ET *)(UART0_REG_BASE + 0x0008))
#define UART0_LCR (*(volatile U32ET *)(UART0_REG_BASE + 0x000C))
#define UART0_MCR (*(volatile U32ET *)(UART0_REG_BASE + 0x0010))
#define UART0_MSR (*(volatile U32ET *)(UART0_REG_BASE + 0x0014))
#define UART0_FSR (*(volatile U32ET *)(UART0_REG_BASE + 0x0018))
#define UART0_ISR (*(volatile U32ET *)(UART0_REG_BASE + 0x001C))
#define UART0_TOR (*(volatile U32ET *)(UART0_REG_BASE + 0x0020))
#define UART0_BAUD (*(volatile U32ET *)(UART0_REG_BASE + 0x0024))
#define UART0_IRCR (*(volatile U32ET *)(UART0_REG_BASE + 0x0028))
#define UART0_ALTCSR (*(volatile U32ET *)(UART0_REG_BASE + 0x002C))
#define UART0_FUNSEL (*(volatile U32ET *)(UART0_REG_BASE + 0x0030))

#define UART1_RBR (*(volatile U32ET *)(UART1_REG_BASE + 0x0000))
#define UART1_THR (*(volatile U32ET *)(UART1_REG_BASE + 0x0000))
#define UART1_IER (*(volatile U32ET *)(UART1_REG_BASE + 0x0004))
#define UART1_FCR (*(volatile U32ET *)(UART1_REG_BASE + 0x0008))
#define UART1_LCR (*(volatile U32ET *)(UART1_REG_BASE + 0x000C))
#define UART1_MCR (*(volatile U32ET *)(UART1_REG_BASE + 0x0010))
#define UART1_MSR (*(volatile U32ET *)(UART1_REG_BASE + 0x0014))
#define UART1_FSR (*(volatile U32ET *)(UART1_REG_BASE + 0x0018))
#define UART1_ISR (*(volatile U32ET *)(UART1_REG_BASE + 0x001C))
#define UART1_TOR (*(volatile U32ET *)(UART1_REG_BASE + 0x0020))
#define UART1_BAUD (*(volatile U32ET *)(UART1_REG_BASE + 0x0024))
#define UART1_IRCR (*(volatile U32ET *)(UART1_REG_BASE + 0x0028))
#define UART1_ALTCSR (*(volatile U32ET *)(UART1_REG_BASE + 0x002C))
#define UART1_FUNSEL (*(volatile U32ET *)(UART1_REG_BASE + 0x0030))

/*-----------------------------------------------------------------*/
/* PWM register define                                             */
/*-----------------------------------------------------------------*/
#define PWMA_REG_BASE (0x40040000)
#define PWMB_REG_BASE (0x40140000)

#define PWMA_PPR (*(volatile U32ET *)(PWMA_REG_BASE + 0x0000))
#define PWMA_CSR (*(volatile U32ET *)(PWMA_REG_BASE + 0x0004))
#define PWMA_PCR (*(volatile U32ET *)(PWMA_REG_BASE + 0x0008))
#define PWMA_CNR0 (*(volatile U32ET *)(PWMA_REG_BASE + 0x000C))
#define PWMA_CMR0 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0010))
#define PWMA_PDR0 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0014))
#define PWMA_CNR1 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0018))
#define PWMA_CMR1 (*(volatile U32ET *)(PWMA_REG_BASE + 0x001C))
#define PWMA_PDR1 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0020))
#define PWMA_CNR2 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0024))
#define PWMA_CMR2 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0028))
#define PWMA_PDR2 (*(volatile U32ET *)(PWMA_REG_BASE + 0x002C))
#define PWMA_CNR3 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0030))
#define PWMA_CMR3 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0034))
#define PWMA_PDR3 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0038))
#define PWMA_PIER (*(volatile U32ET *)(PWMA_REG_BASE + 0x0040))
#define PWMA_PIIR (*(volatile U32ET *)(PWMA_REG_BASE + 0x0044))
#define PWMA_CCR0 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0050))
#define PWMA_CCR2 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0054))
#define PWMA_CRLR0 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0058))
#define PWMA_CFLR0 (*(volatile U32ET *)(PWMA_REG_BASE + 0x005C))
#define PWMA_CRLR1 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0060))
#define PWMA_CFLR1 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0064))
#define PWMA_CRLR2 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0068))
#define PWMA_CFLR2 (*(volatile U32ET *)(PWMA_REG_BASE + 0x006C))
#define PWMA_CRLR3 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0070))
#define PWMA_CFLR3 (*(volatile U32ET *)(PWMA_REG_BASE + 0x0074))
#define PWMA_CAPENR (*(volatile U32ET *)(PWMA_REG_BASE + 0x0078))
#define PWMA_POE (*(volatile U32ET *)(PWMA_REG_BASE + 0x007C))
#define PWMA_TCON (*(volatile U32ET *)(PWMA_REG_BASE + 0x0080))
#define PWMA_TSTATUS (*(volatile U32ET *)(PWMA_REG_BASE + 0x0084))
#define PWMA_PSCR (*(volatile U32ET *)(PWMA_REG_BASE + 0x0098))

#define PWMB_PPR (*(volatile U32ET *)(PWMB_REG_BASE + 0x0000))
#define PWMB_CSR (*(volatile U32ET *)(PWMB_REG_BASE + 0x0004))
#define PWMB_PCR (*(volatile U32ET *)(PWMB_REG_BASE + 0x0008))
#define PWMB_CNR0 (*(volatile U32ET *)(PWMB_REG_BASE + 0x000C))
#define PWMB_CMR0 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0010))
#define PWMB_PDR0 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0014))
#define PWMB_CNR1 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0018))
#define PWMB_CMR1 (*(volatile U32ET *)(PWMB_REG_BASE + 0x001C))
#define PWMB_PDR1 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0020))
#define PWMB_CNR2 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0024))
#define PWMB_CMR2 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0028))
#define PWMB_PDR2 (*(volatile U32ET *)(PWMB_REG_BASE + 0x002C))
#define PWMB_CNR3 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0030))
#define PWMB_CMR3 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0034))
#define PWMB_PDR3 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0038))
#define PWMB_PIER (*(volatile U32ET *)(PWMB_REG_BASE + 0x0040))
#define PWMB_PIIR (*(volatile U32ET *)(PWMB_REG_BASE + 0x0044))
#define PWMB_CCR0 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0050))
#define PWMB_CCR2 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0054))
#define PWMB_CRLR0 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0058))
#define PWMB_CFLR0 (*(volatile U32ET *)(PWMB_REG_BASE + 0x005C))
#define PWMB_CRLR1 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0060))
#define PWMB_CFLR1 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0064))
#define PWMB_CRLR2 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0068))
#define PWMB_CFLR2 (*(volatile U32ET *)(PWMB_REG_BASE + 0x006C))
#define PWMB_CRLR3 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0070))
#define PWMB_CFLR3 (*(volatile U32ET *)(PWMB_REG_BASE + 0x0074))
#define PWMB_CAPENR (*(volatile U32ET *)(PWMB_REG_BASE + 0x0078))
#define PWMB_POE (*(volatile U32ET *)(PWMB_REG_BASE + 0x007C))
#define PWMB_TCON (*(volatile U32ET *)(PWMB_REG_BASE + 0x0080))
#define PWMB_TSTATUS (*(volatile U32ET *)(PWMB_REG_BASE + 0x0084))
#define PWMB_PSCR (*(volatile U32ET *)(PWMB_REG_BASE + 0x0098))

/*-----------------------------------------------------------------*/
/* Timer register define                                           */
/*-----------------------------------------------------------------*/
#define TM01A_REG_BASE (0x40010000)
#define TM23B_REG_BASE (0x40110000)

#define TIM0_TCSR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0000))
#define TIM0_TCMPR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0004))
#define TIM0_TISR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0008))
#define TIM0_TDR (*(volatile U32ET *)(TM01A_REG_BASE + 0x000C))
#define TIM0_TCAP (*(volatile U32ET *)(TM01A_REG_BASE + 0x0010))
#define TIM0_TEXCON (*(volatile U32ET *)(TM01A_REG_BASE + 0x0014))
#define TIM0_TEXISR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0018))
#define TIM1_TCSR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0020))
#define TIM1_TCMPR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0024))
#define TIM1_TISR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0028))
#define TIM1_TDR (*(volatile U32ET *)(TM01A_REG_BASE + 0x002C))
#define TIM1_TCAP (*(volatile U32ET *)(TM01A_REG_BASE + 0x0030))
#define TIM1_TEXCON (*(volatile U32ET *)(TM01A_REG_BASE + 0x0034))
#define TIM1_TEXISR (*(volatile U32ET *)(TM01A_REG_BASE + 0x0038))
#define TIM2_TCSR (*(volatile U32ET *)(TM023_REG_BASE + 0x0000))
#define TIM2_TCMPR (*(volatile U32ET *)(TM023_REG_BASE + 0x0004))
#define TIM2_TISR (*(volatile U32ET *)(TM023_REG_BASE + 0x0008))
#define TIM2_TDR (*(volatile U32ET *)(TM023_REG_BASE + 0x000C))
#define TIM2_TCAP (*(volatile U32ET *)(TM023_REG_BASE + 0x0010))
#define TIM2_TEXCON (*(volatile U32ET *)(TM023_REG_BASE + 0x0014))
#define TIM2_TEXISR (*(volatile U32ET *)(TM023_REG_BASE + 0x0018))
#define TIM3_TCSR (*(volatile U32ET *)(TM023_REG_BASE + 0x0020))
#define TIM3_TCMPR (*(volatile U32ET *)(TM023_REG_BASE + 0x0024))
#define TIM3_TISR (*(volatile U32ET *)(TM023_REG_BASE + 0x0028))
#define TIM3_TDR (*(volatile U32ET *)(TM023_REG_BASE + 0x002C))
#define TIM3_TCAP (*(volatile U32ET *)(TM023_REG_BASE + 0x0030))
#define TIM3_TEXCON (*(volatile U32ET *)(TM023_REG_BASE + 0x0034))
#define TIM3_TEXISR (*(volatile U32ET *)(TM023_REG_BASE + 0x0038))

/*-----------------------------------------------------------------*/
/* ADC register define                                             */
/*-----------------------------------------------------------------*/
#define ADC_REG_BASE (0x400E0000)

#define ADC_ADDR0 (*(volatile U32ET *)(ADC_REG_BASE + 0x0000))
#define ADC_ADDR1 (*(volatile U32ET *)(ADC_REG_BASE + 0x0004))
#define ADC_ADDR2 (*(volatile U32ET *)(ADC_REG_BASE + 0x0008))
#define ADC_ADDR3 (*(volatile U32ET *)(ADC_REG_BASE + 0x000C))
#define ADC_ADDR4 (*(volatile U32ET *)(ADC_REG_BASE + 0x0010))
#define ADC_ADDR5 (*(volatile U32ET *)(ADC_REG_BASE + 0x0014))
#define ADC_ADDR6 (*(volatile U32ET *)(ADC_REG_BASE + 0x0018))
#define ADC_ADDR7 (*(volatile U32ET *)(ADC_REG_BASE + 0x001C))
#define ADC_ADCR (*(volatile U32ET *)(ADC_REG_BASE + 0x0020))
#define ADC_ADCHER (*(volatile U32ET *)(ADC_REG_BASE + 0x0024))
#define ADC_ADCMPR0 (*(volatile U32ET *)(ADC_REG_BASE + 0x0028))
#define ADC_ADCMPR1 (*(volatile U32ET *)(ADC_REG_BASE + 0x002C))
#define ADC_ADSR (*(volatile U32ET *)(ADC_REG_BASE + 0x0030))
#define ADC_ADTDCR (*(volatile U32ET *)(ADC_REG_BASE + 0x0044))

/*-----------------------------------------------------------------*/
/* Flash register define                                           */
/*-----------------------------------------------------------------*/
#define FMC_REG_BASE (0x5000C000)
#define FMC_ISPCON (*(volatile U32ET *)(FMC_REG_BASE + 0x0000))
#define FMC_ISPADR (*(volatile U32ET *)(FMC_REG_BASE + 0x0004))
#define FMC_ISPDAT (*(volatile U32ET *)(FMC_REG_BASE + 0x0008))
#define FMC_ISPCMD (*(volatile U32ET *)(FMC_REG_BASE + 0x000C))
#define FMC_ISPTRG (*(volatile U32ET *)(FMC_REG_BASE + 0x0010))
#define FMC_DFBADR (*(volatile U32ET *)(FMC_REG_BASE + 0x0014))
#define FMC_FATCON (*(volatile U32ET *)(FMC_REG_BASE + 0x0018))
#define FMC_ISPSTA (*(volatile U32ET *)(FMC_REG_BASE + 0x0040))

/*-----------------------------------------------------------------*/
/* GPIO register define                                            */
/*-----------------------------------------------------------------*/
#define GPIO_REG_BASE (0x50004000)

#define GPIO_P0_PMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0000))
#define GPIO_P0_OFFD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0004))
#define GPIO_P0_DOUT (*(volatile U32ET *)(GPIO_REG_BASE + 0x0008))
#define GPIO_P0_DMASK (*(volatile U32ET *)(GPIO_REG_BASE + 0x000C))
#define GPIO_P0_PIN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0010))
#define GPIO_P0_DBEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0014))
#define GPIO_P0_IMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0018))
#define GPIO_P0_IEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x001C))
#define GPIO_P0_ISRC (*(volatile U32ET *)(GPIO_REG_BASE + 0x0020))

#define GPIO_P1_PMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0040))
#define GPIO_P1_OFFD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0044))
#define GPIO_P1_DOUT (*(volatile U32ET *)(GPIO_REG_BASE + 0x0048))
#define GPIO_P1_DMASK (*(volatile U32ET *)(GPIO_REG_BASE + 0x004C))
#define GPIO_P1_PIN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0050))
#define GPIO_P1_DBEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0054))
#define GPIO_P1_IMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0058))
#define GPIO_P1_IEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x005C))
#define GPIO_P1_ISRC (*(volatile U32ET *)(GPIO_REG_BASE + 0x0060))

#define GPIO_P2_PMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0080))
#define GPIO_P2_OFFD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0084))
#define GPIO_P2_DOUT (*(volatile U32ET *)(GPIO_REG_BASE + 0x0088))
#define GPIO_P2_DMASK (*(volatile U32ET *)(GPIO_REG_BASE + 0x008C))
#define GPIO_P2_PIN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0090))
#define GPIO_P2_DBEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0094))
#define GPIO_P2_IMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0098))
#define GPIO_P2_IEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x009C))
#define GPIO_P2_ISRC (*(volatile U32ET *)(GPIO_REG_BASE + 0x00A0))

#define GPIO_P3_PMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x00C0))
#define GPIO_P3_OFFD (*(volatile U32ET *)(GPIO_REG_BASE + 0x00C4))
#define GPIO_P3_DOUT (*(volatile U32ET *)(GPIO_REG_BASE + 0x00C8))
#define GPIO_P3_DMASK (*(volatile U32ET *)(GPIO_REG_BASE + 0x00CC))
#define GPIO_P3_PIN (*(volatile U32ET *)(GPIO_REG_BASE + 0x00D0))
#define GPIO_P3_DBEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x00D4))
#define GPIO_P3_IMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x00D8))
#define GPIO_P3_IEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x00DC))
#define GPIO_P3_ISRC (*(volatile U32ET *)(GPIO_REG_BASE + 0x00E0))

#define GPIO_P4_PMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0100))
#define GPIO_P4_OFFD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0104))
#define GPIO_P4_DOUT (*(volatile U32ET *)(GPIO_REG_BASE + 0x0108))
#define GPIO_P4_DMASK (*(volatile U32ET *)(GPIO_REG_BASE + 0x010C))
#define GPIO_P4_PIN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0110))
#define GPIO_P4_DBEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x0114))
#define GPIO_P4_IMD (*(volatile U32ET *)(GPIO_REG_BASE + 0x0118))
#define GPIO_P4_IEN (*(volatile U32ET *)(GPIO_REG_BASE + 0x011C))
#define GPIO_P4_ISRC (*(volatile U32ET *)(GPIO_REG_BASE + 0x0120))

#define GPIO_DBNCECON (*(volatile U32ET *)(GPIO_REG_BASE + 0x0180))

#define GPIO_P0_PDIO0 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0200))
#define GPIO_P0_PDIO1 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0204))
#define GPIO_P0_PDIO2 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0208))
#define GPIO_P0_PDIO3 (*(volatile U32ET *)(GPIO_REG_BASE + 0x020C))
#define GPIO_P0_PDIO4 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0210))
#define GPIO_P0_PDIO5 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0214))
#define GPIO_P0_PDIO6 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0218))
#define GPIO_P0_PDIO7 (*(volatile U32ET *)(GPIO_REG_BASE + 0x021C))

#define GPIO_P1_PDIO0 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0220))
#define GPIO_P1_PDIO1 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0224))
#define GPIO_P1_PDIO2 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0228))
#define GPIO_P1_PDIO3 (*(volatile U32ET *)(GPIO_REG_BASE + 0x022C))
#define GPIO_P1_PDIO4 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0230))
#define GPIO_P1_PDIO5 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0234))
#define GPIO_P1_PDIO6 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0238))
#define GPIO_P1_PDIO7 (*(volatile U32ET *)(GPIO_REG_BASE + 0x023C))

#define GPIO_P2_PDIO0 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0240))
#define GPIO_P2_PDIO1 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0244))
#define GPIO_P2_PDIO2 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0248))
#define GPIO_P2_PDIO3 (*(volatile U32ET *)(GPIO_REG_BASE + 0x024C))
#define GPIO_P2_PDIO4 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0250))
#define GPIO_P2_PDIO5 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0254))
#define GPIO_P2_PDIO6 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0258))
#define GPIO_P2_PDIO7 (*(volatile U32ET *)(GPIO_REG_BASE + 0x025C))

#define GPIO_P3_PDIO0 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0260))
#define GPIO_P3_PDIO1 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0264))
#define GPIO_P3_PDIO2 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0268))
#define GPIO_P3_PDIO3 (*(volatile U32ET *)(GPIO_REG_BASE + 0x026C))
#define GPIO_P3_PDIO4 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0270))
#define GPIO_P3_PDIO5 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0274))
#define GPIO_P3_PDIO6 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0278))
#define GPIO_P3_PDIO7 (*(volatile U32ET *)(GPIO_REG_BASE + 0x027C))

#define GPIO_P4_PDIO0 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0280))
#define GPIO_P4_PDIO1 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0284))
#define GPIO_P4_PDIO2 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0288))
#define GPIO_P4_PDIO3 (*(volatile U32ET *)(GPIO_REG_BASE + 0x028C))
#define GPIO_P4_PDIO4 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0290))
#define GPIO_P4_PDIO5 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0294))
#define GPIO_P4_PDIO6 (*(volatile U32ET *)(GPIO_REG_BASE + 0x0298))
#define GPIO_P4_PDIO7 (*(volatile U32ET *)(GPIO_REG_BASE + 0x029C))

/**
  * @brief  spi register;
  *
  */
#define SPI0_BASE (0x40030000) /*!< SPI0 Base Address                                */
#define SPI1_BASE (0x40034000)

#define SPI0_CNTRL (*(volatile U32ET *)(SPI0_BASE + 0x00))
#define SPI0_DIVIDER (*(volatile U32ET *)(SPI0_BASE + 0x04))
#define SPI0_SSR (*(volatile U32ET *)(SPI0_BASE + 0x08))
#define SPI0_RX0 (*(volatile U32ET *)(SPI0_BASE + 0x10))
#define SPI0_RX1 (*(volatile U32ET *)(SPI0_BASE + 0x14))
#define SPI0_TX0 (*(volatile U32ET *)(SPI0_BASE + 0x20))
#define SPI0_TX1 (*(volatile U32ET *)(SPI0_BASE + 0x24))
#define SPI0_VARCLK (*(volatile U32ET *)(SPI0_BASE + 0x34))
#define SPI0_CNTRL2 (*(volatile U32ET *)(SPI0_BASE + 0x3c))
#define SPI0_FIFO_CTL (*(volatile U32ET *)(SPI0_BASE + 0x40))
#define SPI0_STATUS (*(volatile U32ET *)(SPI0_BASE + 0x44))
#define SPI1_CNTRL (*(volatile U32ET *)(SPI1_BASE + 0x00))
#define SPI1_DIVIDER (*(volatile U32ET *)(SPI1_BASE + 0x04))
#define SPI1_SSR (*(volatile U32ET *)(SPI1_BASE + 0x08))
#define SPI1_RX0 (*(volatile U32ET *)(SPI1_BASE + 0x10))
#define SPI1_RX1 (*(volatile U32ET *)(SPI1_BASE + 0x14))
#define SPI1_TX0 (*(volatile U32ET *)(SPI1_BASE + 0x20))
#define SPI1_TX1 (*(volatile U32ET *)(SPI1_BASE + 0x24))
#define SPI1_VARCLK (*(volatile U32ET *)(SPI1_BASE + 0x34))
#define SPI1_CNTRL2 (*(volatile U32ET *)(SPI1_BASE + 0x3c))
#define SPI1_FIFO_CTL (*(volatile U32ET *)(SPI1_BASE + 0x40))
#define SPI1_STATUS (*(volatile U32ET *)(SPI1_BASE + 0x44))

//typedef struct {
//    __IO uint32_t CNTRL;         /* Offset: 0x00  SPI Control and Status Register                                    */
//    __IO uint32_t DIVIDER;       /* Offset: 0x04  SPI Clock Divider Register                                         */
//    __IO uint32_t SSR;           /* Offset: 0x08  SPI Slave Select Register                                          */
//    __I  uint32_t RESERVE0;
//    __I  uint32_t RX0;           /* Offset: 0x10  Data Receive Register                                              */
//    __I  uint32_t RX1;           /* Offset: 0x14  Data Receive Register                                              */
//    __I  uint32_t RESERVE1[2];
//    __O  uint32_t TX0;           /* Offset: 0x20  Data Transmit Register                                             */
//    __O  uint32_t TX1;           /* Offset: 0x24  Data Transmit Register                                             */
//    __I  uint32_t RESERVE2[3];
//    __IO uint32_t VARCLK;        /* Offset: 0x34  Variable Clock Pattern Register                                    */
//    __I  uint32_t RESERVE3;
//    __IO uint32_t CNTRL2;        /* Offset: 0x3C  SPI Control and Status Register 2                                  */
//    __IO uint32_t FIFO_CTL;      /* Offset: 0x40  SPI FIFO Control Register                                          */
//    __IO uint32_t STATUS;        /* Offset: 0x44  SPI Status Register                                                */

//} SPI_T;

#define LOW (0)
#define HIGH (1)

#define ET_PORT0 (0)
#define ET_PORT1 (1)
#define ET_PORT2 (2)
#define ET_PORT3 (3)
#define ET_PORT4 (4)

//GP_BA+0x200 + 0x04 * n
#define PORT_READ(PORT, PIN) (((*((volatile U32ET *)(GPIO_REG_BASE + 0x200 + (PORT * 0x20) + 0x04 * PIN))) & 0x01) ? 1 : 0)

#define PORT_SET(PORT, PIN, STATUS) (STATUS ? (*((volatile U32ET *)(GPIO_REG_BASE + 0x200 + (PORT * 0x20) + 0x04 * PIN)) = 0x1) : (*((volatile U32ET *)(GPIO_REG_BASE + 0x200 + (PORT * 0x20) + 0x04 * PIN)) = 0x0))

#endif /* _REGDEF_H */
