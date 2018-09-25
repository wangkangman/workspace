#include "regdef.h"
#include "ClkSet.h"
#include "UartPrin.h"
/*-----------------------------------------------------------------*/
/* FUNC : Uart initialize, use SAU0 channel 0 as uart TX           */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void uartct_init(void)
{
    /* enable uart clock */
    clkset_ctrl(CLK_ID_UART, CLK_CT_ENA);
    NOP();
    NOP();
    NOP();
    NOP();
    /* UART TX PIN - P12 */
    PORT_PMC1 &= ~(1<<2);
    PORT_PD1  |=  (1<<2);
    PORT_PM1  &= ~(1<<2);

    /* set division of ckm0 and ckm1 to 2^4 */
 //   UART_SPS0 = 0x0044;
   UART_SPS0 = 0x0002;

    /* disable uart TX and RX */
    UART_ST0  = 0x03;

    /* clear TX complete flag */
    INT_IF0H &= ~(1<<0);

    /* use CK00 as clock source */
    UART_SMR00 &= ~(3<<14);

    /* only use software trigger */
    UART_SMR00 &= ~(1<<8);

    /* SAU0 channel 0 act as uart mode */
    UART_SMR00 &= ~(3<<1);
    UART_SMR00 |=  (1<<1);

    /* SAU0 channel 0 act in TX mode */
    UART_SCR00 &= ~(3<<14);
    UART_SCR00 |=  (2<<14);

    /* disable error interrupt */
    UART_SCR00 &= ~(1<<10);
 
    /* no parity */
    UART_SCR00 &= ~(3<<8);

    /* LSB mode */
    UART_SCR00 |=  (1<<7);

    /* stop bits <-> 1 */
    UART_SCR00 &= ~(3<<4);
    UART_SCR00 |=  (1<<4);

    /* data bits <-> 8 */
    UART_SCR00 |=  (3<<0);

    /* baudrate 9600 */
    UART_SDR00 = 0x9A00;

    /* enable uart unit */
    UART_SOE0 |= (1<<0);

    /* enable TX */
    UART_SS0 |= (1<<0);
}

/*-----------------------------------------------------------------*/
/* FUNC : Retargets the C library printf function to the UART      */
/* PARA :                                                          */
/*   ch : a character to be send                                   */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
int putchar(int ch)
{
    UART_TXD0 = (U08ET)ch;
    while( 0 == (INT_IF0H & (1<<0)) );
    INT_IF0H &= ~(1<<0);
    return ch;
}

void putint(unsigned int n)
{
    char buf[5], index = 0;    
    for (index = 0; index < 5; ++index){
    	buf[index] = 0;
   }    
    index = 0;    

    while(n != 0){
    	buf[index] = (U08ET)n % 10;
    	n /= 10;
	++index;
    }
    for (index = 0; index < 5; ++index){
    	buf[index] += '0';
    } 
    putchar(buf[4]);
    putchar(buf[3]);
    putchar(buf[2]);
    putchar(buf[1]);
    putchar(buf[0]);
    putchar(' ');
}

/**
  * @brief  Send a byte as format Hex.
  * @param
  *	n : The byte which should be sent.
  * @retval None
  *
  */
void puthex(unsigned char n)
{
    char buf[2], index = 0;    
    for (index = 0; index < 2; ++index){
    	buf[index] = 0;
   }    
    index = 0;    

    while(n != 0){
    	buf[index] = n % 16;
    	n /= 16;
	++index;
    }
    for (index = 0; index < 2; ++index){
	if(buf[index] < 10)    
    	   buf[index] += '0';
	else
	   buf[index] = 'A' + buf[index] - 10;
    } 
    putchar('0');
    putchar('x');
    putchar(buf[1]);
    putchar(buf[0]);
    putchar(' ');
}

/**
  * @brief  Send a string through Uart_1.
  * @param
  *	fmt:Piont to the string which should be sent.
  * @retval None
  *
  */
void PutString(char* fmt)
{
  while(*fmt) {
    putchar(*fmt++);
  }
}

/*-----------------------------------------------------------------*/
/* FUNC : Send data through UART                                   */
/* PARA :                                                          */
/*   sbuf : buff stored data that to be send                       */
/*   size : buff size                                              */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void uartct_send(U08ET *sbuf, U08ET size)
{
    U08ET index;

    for(index=0; index<size; index++) {
        UART_TXD0 = sbuf[index];
        while( 0 == (INT_IF0H & (1<<0)) );
        INT_IF0H &= ~(1<<0);
    }
}



