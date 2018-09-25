#include "regdef.h"
#include "clkrst.h"
#include "uartct.h"

/*-----------------------------------------------------------------*/
/* FUNC : Uart initialize                                          */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET uartct_init(VODET)
{
    U16ET baud;

    sysclk_ctrl(CLK_ID_UART2, CLK_CT_ENA);

    /* TXD  <--> PD5 */
//    GPIO_PD_DDR |=  (1<<5);  /* direction output */
//    GPIO_PD_CR1 |=  (1<<2);  /* push-pull        */
//    GPIO_PD_CR2 &= ~(1<<2);  /* max speed 2MHz   */
    
    GPIO_PD_DDR |=  (1<<5);  /* direction output */
    GPIO_PD_CR1 |=  (1<<5);  /* push-pull        */
    GPIO_PD_CR2 &= ~(1<<5);  /* max speed 2MHz   */


/* 2017/6/16 星期五 上午 11:55:32-----used for oc detect- */
//    /* RXD  <--> PD6 */
//    GPIO_PD_DDR &= ~(1<<6);  /* direction input  */
//    GPIO_PD_CR1 &= ~(1<<6);  /* float input      */
//    GPIO_PD_CR2 &= ~(1<<6);  /* max speed 2MHz   */
/* 2017/6/16 星期五 上午 11:55:32 ----used for oc detect- */

    /* data bits <-> 8 start bits <-> 1 */
    UART2_CR1 &= ~(1<<4);

    /* stop bits <-> 1 */
    UART2_CR3 &= ~(3<<4);

    /* no parity */
    UART2_CR1 &= ~(3<<1);

    /* set baudrate */
    UART2_BRR1 = 0x00;
    UART2_BRR2 = 0x00;
    baud = CLK_PERH_FREQ / BAUDRATE;
    UART2_BRR2 |= (baud&0x000F) >> 0;
    UART2_BRR2 |= (baud&0xF000) >> 8;
    UART2_BRR1 |= (baud&0x0FF0) >> 4;

    /* enable TX and RX, enable RX interrupt */
//    UART2_CR2 |= (3<<2) | (1<<5);
		UART2_CR2 |= (1<<3);
}

/*-----------------------------------------------------------------*/
/* FUNC : Retargets the C library printf function to the UART      */
/* PARA :                                                          */
/*   ch : a character to be send                                   */
/* RETN :                                                          */
/*   return the original character                                 */
/*-----------------------------------------------------------------*/
S08ET putchar(S08ET ch)
{
    UART2_DR = ch;
    while( 0 == (UART2_SR & (1<<7)) );

    return ch;
}

/*-----------------------------------------------------------------*/
/* FUNC : UART2 IRQ handler                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
@far @interrupt VODET UART2_IRQHandler(VODET)
{
    U08ET trash;

    if( UART2_SR & (1<<5) ) {
        UART2_CR2 &= ~(1<<5); /* disable interrupt */

        trash = UART2_DR;

        UART2_CR2 |= (1<<5);  /* enable interrupt */
    }
}
