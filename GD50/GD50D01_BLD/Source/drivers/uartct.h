#ifndef _UARTCT_H
#define _UARTCT_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define UART parameter                                           */
/*-----------------------------------------------------------------*/
#define BAUDRATE     (9600)

/*-----------------------------------------------------------------*/
/* Define firmware update parameter                                */
/*-----------------------------------------------------------------*/
#define PKT_FLG_IDX (0)
#define PKT_DEV_IDX (1)
#define PKT_SIZ_IDX (2)
#define PKT_CRC_IDX (4)

#define PKT_CMD_IDX (6)

#define PKT_CMD_STA (0x01) /* firmware update start command */
#define PKT_ACK_STA (0x02) /* firmware update start acknowledge */
#define PKT_CMD_END (0x03) /* firmware update finish command */
#define PKT_ACK_END (0x04) /* firmware update finish acknowledge */
#define PKT_CMD_PKT (0x05) /* firmware update packet command */
#define PKT_ACK_PKT (0x06) /* firmware update packet acknowledge */
#define PKT_ERR_CRC (0xFF) /* firmware update packet error */
#define PKT_ERR_DEV (0xFE) /* firmware update device error */
#define PKT_ERR_CMD (0xFD) /* firmware update command error */
#define PKT_ERR_PAR (0xFC) /* firmware update parameter error */
#define PKT_ERR_SUM (0xFB) /* firmware update checksum error */

#define PKT_DEV_ID  (0x01) /* firmware update device */

/*-----------------------------------------------------------------*/
/* Define uart control structure                                   */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET recv_buff[640];
    U16ET recv_indx;

    U16ET recv_time;
    U08ET recv_flag;
}ET_UART;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET uartct_init(VODET);
VODET uartct_stop(VODET);
VODET uartct_send(U08ET *pbuf, U16ET size);
U08ET uartct_recv(U08ET *pbuf, U16ET size);
VODET uartct_time(VODET);

VODET uartct0_send(U08ET *pbuf, U16ET size);
VODET uartct0_init(VODET);

#endif /* _UARTCT_H */
