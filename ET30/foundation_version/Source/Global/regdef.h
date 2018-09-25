#ifndef _REGDEF_H
#define _REGDEF_H

#include "global.h"



typedef struct
{
    unsigned char no0:1;
    unsigned char no1:1;
    unsigned char no2:1;
    unsigned char no3:1;
    unsigned char no4:1;
    unsigned char no5:1;
    unsigned char no6:1;
    unsigned char no7:1;
} __bitf_T;

typedef struct
{
    unsigned char no0:1;
    unsigned char no1:1;
    unsigned char no2:1;
    unsigned char no3:1;
    unsigned char no4:1;
    unsigned char no5:1;
    unsigned char no6:1;
    unsigned char no7:1;
    unsigned char no8:1;
    unsigned char no9:1;
    unsigned char no10:1;
    unsigned char no11:1;
    unsigned char no12:1;
    unsigned char no13:1;
    unsigned char no14:1;
    unsigned char no15:1;
} __bitf_T2;
/*-----------------------------------------------------------------*/
/* dataflash register define                                            */
/*-----------------------------------------------------------------*/
#define FDL_DFLCTL             (*(volatile U08ET *)(0x0090))

/*-----------------------------------------------------------------*/
/* Port register define                                            */
/*-----------------------------------------------------------------*/
#define PORT_PM1               (*(volatile U08ET *)(0xFF21))
#define PORT_PM2               (*(volatile U08ET *)(0xFF22))
#define PORT_PM4               (*(volatile U08ET *)(0xFF24))
#define PORT_PM6               (*(volatile U08ET *)(0xFF26))
#define PORT_PD1               (*(volatile U08ET *)(0xFF01))
#define PORT_PD2               (*(volatile U08ET *)(0xFF02))
#define PORT_PD4               (*(volatile U08ET *)(0xFF04))
#define PORT_PD6               (*(volatile U08ET *)(0xFF06))
#define PORT_PD12              (*(volatile U08ET *)(0xFF0C))
#define PORT_PD13              (*(volatile U08ET *)(0xFF0D))
#define PORT_PU1               (*(volatile U08ET *)(0x0031))
#define PORT_PU4               (*(volatile U08ET *)(0x0034))
#define PORT_PU12              (*(volatile U08ET *)(0x003C))
#define PORT_PIM1              (*(volatile U08ET *)(0x0041))
#define PORT_POM1              (*(volatile U08ET *)(0x0051))
#define PORT_POM4              (*(volatile U08ET *)(0x0054))
#define PORT_PMC1              (*(volatile U08ET *)(0x0061))
#define PORT_PMC4              (*(volatile U08ET *)(0x0064))
#define PORT_ADPC              (*(volatile U08ET *)(0x0076))
#define PORT_PIOR              (*(volatile U08ET *)(0x0077))

/*-----------------------------------------------------------------*/
/* Clock and reset register define                                 */
/*-----------------------------------------------------------------*/
#define CLK_CMC                (*(volatile U08ET *)(0xFFA0))
#define CLK_CSC                (*(volatile U08ET *)(0xFFA1))
#define CLK_OSTC               (*(volatile U08ET *)(0xFFA2))
#define CLK_OSTS               (*(volatile U08ET *)(0xFFA3))
#define CLK_CKC                (*(volatile U08ET *)(0xFFA4))
#define CLK_CKS0               (*(volatile U08ET  *)0xFFA5)
#define CLK_HIOTRM             (*(volatile U08ET *)(0x00A0))
#define CLK_HOCODIV            (*(volatile U08ET *)(0x00A8))
#define CLK_PER0               (*(volatile U08ET *)(0x00F0))
#define CLK_OSMC               (*(volatile U08ET *)(0x00F3))
#define RST_RESF               (*(volatile U08ET *)(0xFFA8))

/*-----------------------------------------------------------------*/
/* Watchdog register define                                        */
/*-----------------------------------------------------------------*/
#define WDT_WDTE               (*(volatile U08ET *)(0xFFAB))

/*-----------------------------------------------------------------*/
/* Timer register define                                           */
/*-----------------------------------------------------------------*/
#define TIM_TPS0               (*(volatile U16ET *)(0x01B6))
#define TIM_TMR00              (*(volatile U16ET *)(0x0190))
#define TIM_TMR01              (*(volatile U16ET *)(0x0192))
#define TIM_TMR02              (*(volatile U16ET *)(0x0194))
#define TIM_TMR03              (*(volatile U16ET *)(0x0196))
#define TIM_TSR0               (*(volatile U16ET *)(0x01A0))
#define TIM_TSR1               (*(volatile U16ET *)(0x01A2))
#define TIM_TSR2               (*(volatile U16ET *)(0x01A4))
#define TIM_TSR3               (*(volatile U16ET *)(0x01A6))
#define TIM_TE0                (*(volatile U16ET *)(0x01B0))
#define TIM_TS0                (*(volatile U16ET *)(0x01B2))
#define TIM_TT0                (*(volatile U16ET *)(0x01B4))
#define TIM_TIS0               (*(volatile U08ET *)(0x0074))
#define TIM_TOE0               (*(volatile U16ET *)(0x01BA))
#define TIM_TO0                (*(volatile U16ET *)(0x01B8))
#define TIM_TOL0               (*(volatile U16ET *)(0x01BC))
#define TIM_TOM0               (*(volatile U16ET *)(0x01BE))
#define TIM_NFEN1              (*(volatile U08ET *)(0x0071))
#define TIM_TDR00              (*(volatile U16ET *)(0xFF18))
#define TIM_TDR01              (*(volatile U16ET *)(0xFF1A))
#define TIM_TDR02              (*(volatile U16ET *)(0xFF64))
#define TIM_TDR03              (*(volatile U16ET *)(0xFF66))
#define TIM_TCR00              (*(volatile U16ET *)(0x0180))
#define TIM_TCR01              (*(volatile U16ET *)(0x0182))
#define TIM_TCR02              (*(volatile U16ET *)(0x0184))
#define TIM_TCR03              (*(volatile U16ET *)(0x0186))

/*-----------------------------------------------------------------*/
/* Uart register define                                            */
/*-----------------------------------------------------------------*/
#define UART_SPS0              (*(volatile U16ET *)(0x0126))
#define UART_SMR00             (*(volatile U16ET *)(0x0110))
#define UART_SMR01             (*(volatile U16ET *)(0x0112))
#define UART_SCR00             (*(volatile U16ET *)(0x0118))
#define UART_SCR01             (*(volatile U16ET *)(0x011A))
#define UART_SDR00             (*(volatile U16ET *)(0xFF10))
#define UART_SDR01             (*(volatile U16ET *)(0xFF12))
#define UART_SIR00             (*(volatile U16ET *)(0x0108))
#define UART_SIR01             (*(volatile U16ET *)(0x010A))
#define UART_SSR00             (*(volatile U16ET *)(0x0100))
#define UART_SSR01             (*(volatile U16ET *)(0x0102))
#define UART_SS0               (*(volatile U16ET *)(0x0122))
#define UART_ST0               (*(volatile U16ET *)(0x0124))
#define UART_SE0               (*(volatile U16ET *)(0x0120))
#define UART_SOE0              (*(volatile U16ET *)(0x012A))
#define UART_SO0               (*(volatile U16ET *)(0x0128))
#define UART_SOL0              (*(volatile U16ET *)(0x0134))
#define UART_SSC0              (*(volatile U16ET *)(0x0138))
#define UART_NFEN0             (*(volatile U16ET *)(0x0070))
#define UART_TXD0              (*(volatile U08ET *)(0xFF10))
#define UART_RXD0              (*(volatile U08ET *)(0xFF12))

/*-----------------------------------------------------------------*/
/* ADC register define                                             */
/*-----------------------------------------------------------------*/
#define ADC_ADM0               (*(volatile U08ET *)(0xFF30))
#define ADC_ADM1               (*(volatile U08ET *)(0xFF32))
#define ADC_ADM2               (*(volatile U08ET *)(0x0010))
#define ADC_ADCR               (*(volatile U16ET *)(0xFF1E))
#define ADC_ADS                (*(volatile U08ET *)(0xFF31))
#define ADC_ADUL               (*(volatile U08ET *)(0x0011))
#define ADC_ADLL               (*(volatile U08ET *)(0x0012))
#define ADC_ADTES              (*(volatile U08ET *)(0x0013))

/*-----------------------------------------------------------------*/
/* Interrupt register define                                       */
/*-----------------------------------------------------------------*/
#define INT_IF0L               (*(volatile U08ET *)(0xFFE0))
#define INT_IF0H               (*(volatile U08ET *)(0xFFE1))
#define INT_IF1L               (*(volatile U08ET *)(0xFFE2))
#define INT_MK0L               (*(volatile U08ET *)(0xFFE4))
#define INT_MK0H               (*(volatile U08ET *)(0xFFE5))
#define INT_MK1L               (*(volatile U08ET *)(0xFFE6))
#define INT_PR00L              (*(volatile U08ET *)(0xFFE8))
#define INT_PR00H              (*(volatile U08ET *)(0xFFE9))
#define INT_PR01L              (*(volatile U08ET *)(0xFFEA))
#define INT_PR10L              (*(volatile U08ET *)(0xFFEC))
#define INT_PR10H              (*(volatile U08ET *)(0xFFED))
#define INT_PR11L              (*(volatile U08ET *)(0xFFEE))
#define INT_EGP0               (*(volatile U08ET *)(0xFF38))
#define INT_EGN0               (*(volatile U08ET *)(0xFF39))

/*-----------------------------------------------------------------*/
/* Interrupt vector define                                         */
/*-----------------------------------------------------------------*/
#define INT_TBL_WDTI           0x0004
#define INT_TBL_LVI            0x0006
#define INT_TBL_P0             0x0008
#define INT_TBL_P1             0x000A
#define INT_TBL_P2             0x000C
#define INT_TBL_P3             0x000E
#define INT_TBL_DMA0           0x0010
#define INT_TBL_DMA1           0x0012
#define INT_TBL_CSI00          0x0014
#define INT_TBL_IIC00          0x0014
#define INT_TBL_ST0            0x0014
#define INT_TBL_CSI01          0x0016
#define INT_TBL_IIC01          0x0016
#define INT_TBL_SR0            0x0016
#define INT_TBL_SRE0           0x0018
#define INT_TBL_TM01H          0x001A
#define INT_TBL_TM03H          0x001C
#define INT_TBL_IICA0          0x001E
#define INT_TBL_TM00           0x0020
#define INT_TBL_TM01           0x0022
#define INT_TBL_TM02           0x0024
#define INT_TBL_TM03           0x0026
#define INT_TBL_AD             0x0028
#define INT_TBL_IT             0x002A
#define INT_TBL_KR             0x002C
#define INT_TBL_MD             0x002E
#define INT_TBL_FL             0x0030

#if 1
#define ADTYP        (((volatile __near __bitf_T *)0x10)->no0)
#define AWC          (((volatile __near __bitf_T *)0x10)->no2)
#define ADRCK        (((volatile __near __bitf_T *)0x10)->no3)
#define ISC          (*(volatile __near unsigned char  *)0x73)
#define ISC_bit      (*(volatile __near __bitf_T *)0x73)
#define IAWCTL       (*(volatile __near unsigned char  *)0x78)
#define DFLCTL       (*(volatile __near unsigned char  *)0x90)
#define DFLCTL_bit   (*(volatile __near __bitf_T *)0x90)
#define DFLEN        (((volatile __near __bitf_T *)0x90)->no0)
#define TEMPCAL0     (*(volatile __near unsigned char  *)0xAC)
#define TEMPCAL1     (*(volatile __near unsigned char  *)0xAD)
#define TEMPCAL2     (*(volatile __near unsigned char  *)0xAE)
#define TEMPCAL3     (*(volatile __near unsigned char  *)0xAF)
#define MDCL         (*(volatile __near unsigned short *)0xE0)
#define MDCH         (*(volatile __near unsigned short *)0xE2)
#define MDUC         (*(volatile __near unsigned char  *)0xE8)
#define MDUC_bit     (*(volatile __near __bitf_T *)0xE8)
#define DIVST        (((volatile __near __bitf_T *)0xE8)->no0)
#define MACSF        (((volatile __near __bitf_T *)0xE8)->no1)
#define MACOF        (((volatile __near __bitf_T *)0xE8)->no2)
#define MDSM         (((volatile __near __bitf_T *)0xE8)->no3)
#define MACMODE      (((volatile __near __bitf_T *)0xE8)->no6)
#define DIVMODE      (((volatile __near __bitf_T *)0xE8)->no7)
#define TAU0EN       (((volatile __near __bitf_T *)0xF0)->no0)
#define SAU0EN       (((volatile __near __bitf_T *)0xF0)->no2)
#define IICA0EN      (((volatile __near __bitf_T *)0xF0)->no4)
#define ADCEN        (((volatile __near __bitf_T *)0xF0)->no5)
#define TMKAEN       (((volatile __near __bitf_T *)0xF0)->no7)
#define RMC          (*(volatile __near unsigned char  *)0xF4)
#define RMC_bit      (*(volatile __near __bitf_T *)0xF4)
#define WDVOL        (((volatile __near __bitf_T *)0xF4)->no7)
#define RPECTL       (*(volatile __near unsigned char  *)0xF5)
#define RPECTL_bit   (*(volatile __near __bitf_T *)0xF5)
#define RPEF         (((volatile __near __bitf_T *)0xF5)->no0)
#define RPERDIS      (((volatile __near __bitf_T *)0xF5)->no7)
#define BCDADJ       (*(volatile __near unsigned char  *)0xFE)
#define IICCTL00     (*(volatile __near unsigned char  *)0x230)
#define IICCTL00_bit (*(volatile __near __bitf_T *)0x230)
#define SPT0         (((volatile __near __bitf_T *)0x230)->no0)
#define STT0         (((volatile __near __bitf_T *)0x230)->no1)
#define ACKE0        (((volatile __near __bitf_T *)0x230)->no2)
#define WTIM0        (((volatile __near __bitf_T *)0x230)->no3)
#define SPIE0        (((volatile __near __bitf_T *)0x230)->no4)
#define WREL0        (((volatile __near __bitf_T *)0x230)->no5)
#define LREL0        (((volatile __near __bitf_T *)0x230)->no6)
#define IICE0        (((volatile __near __bitf_T *)0x230)->no7)
#define IICCTL01     (*(volatile __near unsigned char  *)0x231)
#define IICCTL01_bit (*(volatile __near __bitf_T *)0x231)
#define PRS0         (((volatile __near __bitf_T *)0x231)->no0)
#define DFC0         (((volatile __near __bitf_T *)0x231)->no2)
#define SMC0         (((volatile __near __bitf_T *)0x231)->no3)
#define DAD0         (((volatile __near __bitf_T *)0x231)->no4)
#define CLD0         (((volatile __near __bitf_T *)0x231)->no5)
#define WUP0         (((volatile __near __bitf_T *)0x231)->no7)
#define IICWL0       (*(volatile __near unsigned char  *)0x232)
#define IICWH0       (*(volatile __near unsigned char  *)0x233)
#define SVA0         (*(volatile __near unsigned char  *)0x234)
#define CRCD         (*(volatile __near unsigned short *)0x2FA)
#define SIO00        (*(volatile __near unsigned char  *)0xFF10)
#define SIO01        (*(volatile __near unsigned char  *)0xFF12)
#define KRCTL        (*(volatile __near unsigned char  *)0xFF34)
#define KRCTL_bit    (*(volatile __near __bitf_T *)0xFF34)
#define KRF          (*(volatile __near unsigned char  *)0xFF35)
#define KRM0         (*(volatile __near unsigned char  *)0xFF37)
#define KRM0_bit     (*(volatile __near __bitf_T *)0xFF37)
#define IICA0        (*(volatile __near unsigned char  *)0xFF50)
#define IICS0        (*(volatile __near unsigned char  *)0xFF51)
#define IICS0_bit    (*(volatile __near __bitf_T *)0xFF51)
#define SPD0         (((volatile __near __bitf_T *)0xFF51)->no0)
#define STD0         (((volatile __near __bitf_T *)0xFF51)->no1)
#define ACKD0        (((volatile __near __bitf_T *)0xFF51)->no2)
#define TRC0         (((volatile __near __bitf_T *)0xFF51)->no3)
#define COI0         (((volatile __near __bitf_T *)0xFF51)->no4)
#define EXC0         (((volatile __near __bitf_T *)0xFF51)->no5)
#define ALD0         (((volatile __near __bitf_T *)0xFF51)->no6)
#define MSTS0        (((volatile __near __bitf_T *)0xFF51)->no7)
#define IICF0        (*(volatile __near unsigned char  *)0xFF52)
#define IICF0_bit    (*(volatile __near __bitf_T *)0xFF52)
#define IICRSV0      (((volatile __near __bitf_T *)0xFF52)->no0)
#define STCEN0       (((volatile __near __bitf_T *)0xFF52)->no1)
#define IICBSY0      (((volatile __near __bitf_T *)0xFF52)->no6)
#define STCF0        (((volatile __near __bitf_T *)0xFF52)->no7)
#define ITMC         (*(volatile __near unsigned short *)0xFF90)
#define LVIM         (*(volatile __near unsigned char  *)0xFFA9)
#define LVIM_bit     (*(volatile __near __bitf_T *)0xFFA9)
#define LVIF         (((volatile __near __bitf_T *)0xFFA9)->no0)
#define LVIOMSK      (((volatile __near __bitf_T *)0xFFA9)->no1)
#define LVISEN       (((volatile __near __bitf_T *)0xFFA9)->no7)
#define LVIS         (*(volatile __near unsigned char  *)0xFFAA)
#define LVIS_bit     (*(volatile __near __bitf_T *)0xFFAA)
#define LVILV        (((volatile __near __bitf_T *)0xFFAA)->no0)
#define LVIMD        (((volatile __near __bitf_T *)0xFFAA)->no7)
#define CRCIN        (*(volatile __near unsigned char  *)0xFFAC)
#define DSA0         (*(volatile __near unsigned char  *)0xFFB0)
#define DSA1         (*(volatile __near unsigned char  *)0xFFB1)
#define DRA0         (*(volatile __near unsigned short *)0xFFB2)
#define DRA0L        (*(volatile __near unsigned char  *)0xFFB2)
#define DRA0H        (*(volatile __near unsigned char  *)0xFFB3)
#define DRA1         (*(volatile __near unsigned short *)0xFFB4)
#define DRA1L        (*(volatile __near unsigned char  *)0xFFB4)
#define DRA1H        (*(volatile __near unsigned char  *)0xFFB5)
#define DBC0         (*(volatile __near unsigned short *)0xFFB6)
#define DBC0L        (*(volatile __near unsigned char  *)0xFFB6)
#define DBC0H        (*(volatile __near unsigned char  *)0xFFB7)
#define DBC1         (*(volatile __near unsigned short *)0xFFB8)
#define DBC1L        (*(volatile __near unsigned char  *)0xFFB8)
#define DBC1H        (*(volatile __near unsigned char  *)0xFFB9)
#define DMC0         (*(volatile __near unsigned char  *)0xFFBA)
#define DMC0_bit     (*(volatile __near __bitf_T *)0xFFBA)
#define DWAIT0       (((volatile __near __bitf_T *)0xFFBA)->no4)
#define DS0          (((volatile __near __bitf_T *)0xFFBA)->no5)
#define DRS0         (((volatile __near __bitf_T *)0xFFBA)->no6)
#define STG0         (((volatile __near __bitf_T *)0xFFBA)->no7)
#define DMC1         (*(volatile __near unsigned char  *)0xFFBB)
#define DMC1_bit     (*(volatile __near __bitf_T *)0xFFBB)
#define DWAIT1       (((volatile __near __bitf_T *)0xFFBB)->no4)
#define DS1          (((volatile __near __bitf_T *)0xFFBB)->no5)
#define DRS1         (((volatile __near __bitf_T *)0xFFBB)->no6)
#define STG1         (((volatile __near __bitf_T *)0xFFBB)->no7)
#define DRC0         (*(volatile __near unsigned char  *)0xFFBC)
#define DRC0_bit     (*(volatile __near __bitf_T *)0xFFBC)
#define DST0         (((volatile __near __bitf_T *)0xFFBC)->no0)
#define DEN0         (((volatile __near __bitf_T *)0xFFBC)->no7)
#define DRC1         (*(volatile __near unsigned char  *)0xFFBD)
#define DRC1_bit     (*(volatile __near __bitf_T *)0xFFBD)
#define DST1         (((volatile __near __bitf_T *)0xFFBD)->no0)
#define DEN1         (((volatile __near __bitf_T *)0xFFBD)->no7)
#define WDTIIF       (((volatile __near __bitf_T *)0xFFE0)->no0)
#define LVIIF        (((volatile __near __bitf_T *)0xFFE0)->no1)
#define PIF0         (((volatile __near __bitf_T *)0xFFE0)->no2)
#define PIF1         (((volatile __near __bitf_T *)0xFFE0)->no3)
#define PIF2         (((volatile __near __bitf_T *)0xFFE0)->no4)
#define PIF3         (((volatile __near __bitf_T *)0xFFE0)->no5)
#define DMAIF0       (((volatile __near __bitf_T *)0xFFE0)->no6)
#define DMAIF1       (((volatile __near __bitf_T *)0xFFE0)->no7)
#define CSIIF00      (((volatile __near __bitf_T *)0xFFE1)->no0)
#define IICIF00      (((volatile __near __bitf_T *)0xFFE1)->no0)
#define STIF0        (((volatile __near __bitf_T *)0xFFE1)->no0)
#define CSIIF01      (((volatile __near __bitf_T *)0xFFE1)->no1)
#define IICIF01      (((volatile __near __bitf_T *)0xFFE1)->no1)
#define SRIF0        (((volatile __near __bitf_T *)0xFFE1)->no1)
#define SREIF0       (((volatile __near __bitf_T *)0xFFE1)->no2)
#define TMIF01H      (((volatile __near __bitf_T *)0xFFE1)->no3)
#define TMIF03H      (((volatile __near __bitf_T *)0xFFE1)->no4)
#define IICAIF0      (((volatile __near __bitf_T *)0xFFE1)->no5)
#define TMIF00       (((volatile __near __bitf_T *)0xFFE1)->no6)
#define TMIF01       (((volatile __near __bitf_T *)0xFFE1)->no7)
#define TMIF02       (((volatile __near __bitf_T *)0xFFE2)->no0)
#define TMIF03       (((volatile __near __bitf_T *)0xFFE2)->no1)
#define ADIF         (((volatile __near __bitf_T *)0xFFE2)->no2)
#define TMKAIF       (((volatile __near __bitf_T *)0xFFE2)->no3)
#define KRIF         (((volatile __near __bitf_T *)0xFFE2)->no4)
#define MDIF         (((volatile __near __bitf_T *)0xFFE2)->no5)
#define FLIF         (((volatile __near __bitf_T *)0xFFE2)->no6)
#define WDTIMK       (((volatile __near __bitf_T *)0xFFE4)->no0)
#define LVIMK        (((volatile __near __bitf_T *)0xFFE4)->no1)
#define PMK0         (((volatile __near __bitf_T *)0xFFE4)->no2)
#define PMK1         (((volatile __near __bitf_T *)0xFFE4)->no3)
#define PMK2         (((volatile __near __bitf_T *)0xFFE4)->no4)
#define PMK3         (((volatile __near __bitf_T *)0xFFE4)->no5)
#define DMAMK0       (((volatile __near __bitf_T *)0xFFE4)->no6)
#define DMAMK1       (((volatile __near __bitf_T *)0xFFE4)->no7)
#define CSIMK00      (((volatile __near __bitf_T *)0xFFE5)->no0)
#define IICMK00      (((volatile __near __bitf_T *)0xFFE5)->no0)
#define STMK0        (((volatile __near __bitf_T *)0xFFE5)->no0)
#define CSIMK01      (((volatile __near __bitf_T *)0xFFE5)->no1)
#define IICMK01      (((volatile __near __bitf_T *)0xFFE5)->no1)
#define SRMK0        (((volatile __near __bitf_T *)0xFFE5)->no1)
#define SREMK0       (((volatile __near __bitf_T *)0xFFE5)->no2)
#define TMMK01H      (((volatile __near __bitf_T *)0xFFE5)->no3)
#define TMMK03H      (((volatile __near __bitf_T *)0xFFE5)->no4)
#define IICAMK0      (((volatile __near __bitf_T *)0xFFE5)->no5)
#define TMMK00       (((volatile __near __bitf_T *)0xFFE5)->no6)
#define TMMK01       (((volatile __near __bitf_T *)0xFFE5)->no7)
#define TMMK02       (((volatile __near __bitf_T *)0xFFE6)->no0)
#define TMMK03       (((volatile __near __bitf_T *)0xFFE6)->no1)
#define ADMK         (((volatile __near __bitf_T *)0xFFE6)->no2)
#define TMKAMK       (((volatile __near __bitf_T *)0xFFE6)->no3)
#define KRMK         (((volatile __near __bitf_T *)0xFFE6)->no4)
#define MDMK         (((volatile __near __bitf_T *)0xFFE6)->no5)
#define FLMK         (((volatile __near __bitf_T *)0xFFE6)->no6)
#define WDTIPR0      (((volatile __near __bitf_T *)0xFFE8)->no0)
#define LVIPR0       (((volatile __near __bitf_T *)0xFFE8)->no1)
#define PPR00        (((volatile __near __bitf_T *)0xFFE8)->no2)
#define PPR01        (((volatile __near __bitf_T *)0xFFE8)->no3)
#define PPR02        (((volatile __near __bitf_T *)0xFFE8)->no4)
#define PPR03        (((volatile __near __bitf_T *)0xFFE8)->no5)
#define DMAPR00      (((volatile __near __bitf_T *)0xFFE8)->no6)
#define DMAPR01      (((volatile __near __bitf_T *)0xFFE8)->no7)
#define CSIPR000     (((volatile __near __bitf_T *)0xFFE9)->no0)
#define IICPR000     (((volatile __near __bitf_T *)0xFFE9)->no0)
#define STPR00       (((volatile __near __bitf_T *)0xFFE9)->no0)
#define CSIPR001     (((volatile __near __bitf_T *)0xFFE9)->no1)
#define IICPR001     (((volatile __near __bitf_T *)0xFFE9)->no1)
#define SRPR00       (((volatile __near __bitf_T *)0xFFE9)->no1)
#define SREPR00      (((volatile __near __bitf_T *)0xFFE9)->no2)
#define TMPR001H     (((volatile __near __bitf_T *)0xFFE9)->no3)
#define TMPR003H     (((volatile __near __bitf_T *)0xFFE9)->no4)
#define IICAPR00     (((volatile __near __bitf_T *)0xFFE9)->no5)
#define TMPR000      (((volatile __near __bitf_T *)0xFFE9)->no6)
#define TMPR001      (((volatile __near __bitf_T *)0xFFE9)->no7)
#define TMPR002      (((volatile __near __bitf_T *)0xFFEA)->no0)
#define TMPR003      (((volatile __near __bitf_T *)0xFFEA)->no1)
#define ADPR0        (((volatile __near __bitf_T *)0xFFEA)->no2)
#define TMKAPR0      (((volatile __near __bitf_T *)0xFFEA)->no3)
#define KRPR0        (((volatile __near __bitf_T *)0xFFEA)->no4)
#define MDPR0        (((volatile __near __bitf_T *)0xFFEA)->no5)
#define FLPR0        (((volatile __near __bitf_T *)0xFFEA)->no6)
#define WDTIPR1      (((volatile __near __bitf_T *)0xFFEC)->no0)
#define LVIPR1       (((volatile __near __bitf_T *)0xFFEC)->no1)
#define PPR10        (((volatile __near __bitf_T *)0xFFEC)->no2)
#define PPR11        (((volatile __near __bitf_T *)0xFFEC)->no3)
#define PPR12        (((volatile __near __bitf_T *)0xFFEC)->no4)
#define PPR13        (((volatile __near __bitf_T *)0xFFEC)->no5)
#define DMAPR10      (((volatile __near __bitf_T *)0xFFEC)->no6)
#define DMAPR11      (((volatile __near __bitf_T *)0xFFEC)->no7)
#define CSIPR100     (((volatile __near __bitf_T *)0xFFED)->no0)
#define IICPR100     (((volatile __near __bitf_T *)0xFFED)->no0)
#define STPR10       (((volatile __near __bitf_T *)0xFFED)->no0)
#define CSIPR101     (((volatile __near __bitf_T *)0xFFED)->no1)
#define IICPR101     (((volatile __near __bitf_T *)0xFFED)->no1)
#define SRPR10       (((volatile __near __bitf_T *)0xFFED)->no1)
#define SREPR10      (((volatile __near __bitf_T *)0xFFED)->no2)
#define TMPR101H     (((volatile __near __bitf_T *)0xFFED)->no3)
#define TMPR103H     (((volatile __near __bitf_T *)0xFFED)->no4)
#define IICAPR10     (((volatile __near __bitf_T *)0xFFED)->no5)
#define TMPR100      (((volatile __near __bitf_T *)0xFFED)->no6)
#define TMPR101      (((volatile __near __bitf_T *)0xFFED)->no7)
#define TMPR102      (((volatile __near __bitf_T *)0xFFEE)->no0)
#define TMPR103      (((volatile __near __bitf_T *)0xFFEE)->no1)
#define ADPR1        (((volatile __near __bitf_T *)0xFFEE)->no2)
#define TMKAPR1      (((volatile __near __bitf_T *)0xFFEE)->no3)
#define KRPR1        (((volatile __near __bitf_T *)0xFFEE)->no4)
#define MDPR1        (((volatile __near __bitf_T *)0xFFEE)->no5)
#define FLPR1        (((volatile __near __bitf_T *)0xFFEE)->no6)
#define MDAL         (*(volatile __near unsigned short *)0xFFF0)
#define MULA         (*(volatile __near unsigned short *)0xFFF0)
#define MDAH         (*(volatile __near unsigned short *)0xFFF2)
#define MULB         (*(volatile __near unsigned short *)0xFFF2)
#define MDBH         (*(volatile __near unsigned short *)0xFFF4)
#define MULOH        (*(volatile __near unsigned short *)0xFFF4)
#define MDBL         (*(volatile __near unsigned short *)0xFFF6)
#define MULOL        (*(volatile __near unsigned short *)0xFFF6)
#define PMC          (*(volatile __near unsigned char  *)0xFFFE)
#define PMC_bit      (*(volatile __near __bitf_T *)0xFFFE)
#define MAA          (((volatile __near __bitf_T *)0xFFFE)->no0)

#endif

#endif /* _REGDEF_H */
