#include <stdio.h>
#include "regdef.h"
#include "clkrst.h"
#include "uartct.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ET_UART et_uart;

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

    /* UART0_RXD <--> P3.0 */
    GCR_P3MFP |=  (1<<0);
    GCR_P3MFP &=  ~(1<<8);

    /* UART0_TXD <--> P3.1 */
    GCR_P3MFP |=  (1<<1);
    GCR_P3MFP &=  ~(1<<9);

    /* select HIRC as UART clock source */
    CLK_SEL1 |= (3<<24);

    /* set UART clock divider to 1 */
    CLK_CLKDIV &= ~(15<<8);

    /* enable UART clock */
    CLK_APB |=  (1<<16);

    /* select UART0 as UART mode */
    UART0_FUNSEL &= ~(3<<0);

    /* data bits <-> 8 */
    UART0_LCR |= (3<<0);

    /* stop bits <-> 1 */
    UART0_LCR &= ~(1<<2);

    /* no parity */
    UART0_LCR &= ~(1<<3);

    /* set baudrate to 115200 */
    baud = CLK_HIRC_FREQ / 16 / BAUDRATE - 2;
    UART0_BAUD = 0x0F000000 | baud;

    /* enbale rx interrupt */
    UART0_IER |= (1<<0);

    /* enable UART1 NVIC interrupt */
    NVIC_ISER = (1<<12);

    /* initialize uart control structure */
    et_uart.recv_indx = 0;
    et_uart.recv_time = 0;
    et_uart.recv_flag = ET_FALSE;
}

#if 0
/**
  * @brief  Uart initialize   
  * @param
  *   none;
  * @retval 
  *   none;
  *
  */
VODET uartct0_init(VODET){
    /* UART0_TXD <--> P3.1 */
    GCR_P3MFP |=  (1<<1);
    GCR_P3MFP &= ~(1<<9);
    /* UART0_RXD <--> P3.0 */
    GCR_P3MFP |=  (1<<0);
    GCR_P3MFP &= ~(1<<8);
    /* select HIRC as UART clock source */
    CLK_SEL1 |= (3<<24);
    /* set UART clock divider to 1 */
    CLK_CLKDIV &= ~(15<<8);
    /* enable UART clock */
    CLK_APB |=  (1<<16);
    /* select UART0 as UART mode */
    UART0_FUNSEL &= ~(3<<0);
    /* data bits <-> 8 */
    UART0_LCR |= (3<<0);
    /* stop bits <-> 1 */
    UART0_LCR &= ~(1<<2);
    /* no parity */
    UART0_LCR &= ~(1<<3);
    /* set baudrate to 115200 */
    UART0_BAUD = 0x0F00000A;
    /* enbale rx interrupt */
    UART0_IER |= (1<<0);
    /* enable UART0 NVIC interrupt */
    NVIC_ISER = (1<<12);
}

VODET uartct0_send(U08ET *pbuf, U16ET size)
{
    U08ET index;

    for(index=0; index<size; index++) {
        UART0_THR = pbuf[index];
        while( UART0_FSR & (1<<23) );
    }
}
#endif

/*-----------------------------------------------------------------*/
/* FUNC : Stop uart and disable its interrupt                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET uartct_stop(VODET)
{
    /* disable UART clock */
    CLK_APB &= ~(1<<16);

    /* disable rx interrupt */
    UART0_IER &= ~(1<<0);

    /* disable UART0 NVIC interrupt */
    NVIC_ICER |= (1<<12);
}

/*-----------------------------------------------------------------*/
/* FUNC : Uart send packet                                         */
/* PARA :                                                          */
/*   pbuf : data buff                                              */
/*   size : data size                                              */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET uartct_send(U08ET *pbuf, U16ET size)
{
    U08ET index;

    for(index=0; index<size; index++) {
        UART0_THR = pbuf[index];
        while( UART0_FSR & (1<<23) );
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Uart recvive packet                                      */
/* PARA :                                                          */
/*   pbuf : data buff                                              */
/*   size : data size                                              */
/* RETN :                                                          */
/*   If success, return ET_TRUE; else return ET_FALSE              */
/*-----------------------------------------------------------------*/
U08ET uartct_recv(U08ET *pbuf, U16ET size)
{
    U16ET index;

    if( ET_FALSE == et_uart.recv_flag ) {
        return ET_FALSE;
    }

    if( size < et_uart.recv_indx ) {
        return ET_FALSE;
    }

    for(index=0; index < et_uart.recv_indx; index++) {
        pbuf[index] = et_uart.recv_buff[index];
    }
    et_uart.recv_indx = 0;
    et_uart.recv_time = 0;
    et_uart.recv_flag = ET_FALSE;

    return ET_TRUE;
}

/*-----------------------------------------------------------------*/
/* FUNC : Uart recvive overtime                                    */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET uartct_time(VODET)
{
    if( ET_TRUE == et_uart.recv_flag ) {
        return ;
    }

    if( et_uart.recv_time < 200 ) {
        et_uart.recv_time++;
    }
    else {
        et_uart.recv_time = 0;
        et_uart.recv_indx = 0;
        et_uart.recv_flag = ET_FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : UART0 IRQ handler                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET UART0_IRQHandler(VODET)
{
    volatile U08ET trash;
    U16ET pktsize;

    if( UART0_ISR & (1<<0) ) {
        trash = UART0_RBR;
        et_uart.recv_time = 0;

        /* check if receive ok */
        if( ET_TRUE == et_uart.recv_flag ) {
            return ;
        }

        /* receive one packet */
        switch(et_uart.recv_indx) {
            case PKT_FLG_IDX:
                if( 0xFF == trash ) {
                    et_uart.recv_buff[et_uart.recv_indx++] = trash;
                }
                else {
                    et_uart.recv_indx = PKT_FLG_IDX;
                }
                break;
            case PKT_DEV_IDX:
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                break;
            case PKT_SIZ_IDX:
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                break;
            case PKT_SIZ_IDX+1: 
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                break;
            default:
                pktsize = (et_uart.recv_buff[PKT_SIZ_IDX]<<8) + et_uart.recv_buff[PKT_SIZ_IDX+1];
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                if( et_uart.recv_indx == pktsize ) {
                    et_uart.recv_flag = ET_TRUE;
                }
                break;
        }
    }
}

#if 0
/*-----------------------------------------------------------------*/
/* FUNC : UART0 IRQ handler                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET UART1_IRQHandler(VODET)
{
    volatile U08ET trash;
    U16ET pktsize;

    if( UART1_ISR & (1<<0) ) {
        trash = UART1_RBR;
        et_uart.recv_time = 0;

        /* check if receive ok */
        if( ET_TRUE == et_uart.recv_flag ) {
            return ;
        }

        /* receive one packet */
        switch(et_uart.recv_indx) {
            case PKT_FLG_IDX:
                if( 0xFF == trash ) {
                    et_uart.recv_buff[et_uart.recv_indx++] = trash;
                }
                else {
                    et_uart.recv_indx = PKT_FLG_IDX;
                }
                break;
            case PKT_DEV_IDX:
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                break;
            case PKT_SIZ_IDX:
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                break;
            case PKT_SIZ_IDX+1: 
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                break;
            default:
                pktsize = (et_uart.recv_buff[PKT_SIZ_IDX]<<8) + et_uart.recv_buff[PKT_SIZ_IDX+1];
                et_uart.recv_buff[et_uart.recv_indx++] = trash;
                if( et_uart.recv_indx == pktsize ) {
                    et_uart.recv_flag = ET_TRUE;
                }
                break;
        }
    }
}
#endif


