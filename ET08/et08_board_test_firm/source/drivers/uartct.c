#include "regdef.h"
#include "clkrst.h"
#include "uartct.h"

/**
  * @brief  rec buf.
  * @param  length:200
  *
  */
u8 USART_RX_BUF[USART_REC_LEN];

/**
  * @brief  rec flag.
  * @param  
  *   bit15:   rec over
  *   bit14:   rec 0x0d
  *   bit13~0: valid cnt
  */
u16 USART_RX_STA = 0;

/*-----------------------------------------------------------------*/
/* FUNC : Uart initialize                                          */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void uartct_init(void)
{
    u16 baud;

    sysclk_ctrl(CLK_ID_UART2, CLK_CT_ENA);

    /* TXD  <--> PD5 */
    GPIO_PD_DDR |= (1 << 5);  /* direction output */
    GPIO_PD_CR1 |= (1 << 2);  /* push-pull        */
    GPIO_PD_CR2 &= ~(1 << 2); /* max speed 2MHz   */

    /* RXD  <--> PD6 */
    GPIO_PD_DDR &= ~(1 << 6); /* direction input  */
    GPIO_PD_CR1 &= ~(1 << 6); /* float input      */
    GPIO_PD_CR2 &= ~(1 << 6); /* max speed 2MHz   */

    /* data bits <-> 8 start bits <-> 1 */
    UART2_CR1 &= ~(1 << 4);

    /* stop bits <-> 1 */
    UART2_CR3 &= ~(3 << 4);

    /* no parity */
    UART2_CR1 &= ~(3 << 1);

    /* set baudrate */
    UART2_BRR1 = 0x00;
    UART2_BRR2 = 0x00;
    baud = CLK_PERH_FREQ / BAUDRATE;
    UART2_BRR2 |= (baud & 0x000F) >> 0;
    UART2_BRR2 |= (baud & 0xF000) >> 8;
    UART2_BRR1 |= (baud & 0x0FF0) >> 4;

    /* enable TX and RX, enable RX interrupt */
    UART2_CR2 |= (3 << 2) | (1 << 5);
    
    USART_RX_STA = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Retargets the C library printf function to the UART      */
/* PARA :                                                          */
/*   ch : a character to be send                                   */
/* RETN :                                                          */
/*   return the original character                                 */
/*-----------------------------------------------------------------*/
s8 putchar(s8 ch)
{
    UART2_DR = ch;
    while (0 == (UART2_SR & (1 << 7)))
        ;

    return ch;
}

/**
  * @brief  uart2 irq handler, updata rec buf, 
  *         service for usmart.
  * @param  none
  * @retval none
  *
  */
@far @interrupt void UART2_IRQHandler(void) //uart2 interrupt
{
    u8 Res;
    if (UART2_SR & (1 << 5))
    {
        UART2_CR2 &= ~(1 << 5);
        Res = UART2_DR;

        if ((USART_RX_STA & 0x8000) == 0) //receiving
        {
            if (USART_RX_STA & 0x4000) //rec:0x0d
            {
                if (Res != 0x0a)
                    USART_RX_STA = 0; //rec error, reset buf
                else
                    USART_RX_STA |= 0x8000; //rec over
            }
            else //ecpect : 0X0D
            {
                if (Res == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                        USART_RX_STA = 0; //invalid data, reset buf
                }
            }
        }
        UART2_CR2 |= (1 << 5);
    }
}
