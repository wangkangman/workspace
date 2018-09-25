#include "regdef.h"
#include "eeprom.h"
#include "remote.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_REMOTE remoter;

/*-----------------------------------------------------------------*/
/* FUNC : Remote controller initialize                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void remote_init(void)
{
    u8 index;

    /* remote controller  <--> PD3 */
    GPIO_PD_DDR &= ~(1 << 3); /* direction input  */
    GPIO_PD_CR1 &= ~(1 << 3); /* float input      */
    GPIO_PD_CR2 &= ~(1 << 3); /* max speed 2MHz   */

    /* initialize remote control structure */
    remoter.st_last = 0;
    remoter.st_curt = 0;
    remoter.st_remo = ST_REMOTE_TP;
    remoter.recvidx = 0;
    for (index = 0; index < RBUF_SIZE; index++)
    {
        remoter.rbuf[index] = 0;
        remoter.lbuf[index] = 0;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Remote controller signal sampling                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void remote_samp(void)
{
    static u8 lv_cnt, tp_cnt;
    u8 byidx, btidx, idx;

    remoter.st_last = remoter.st_curt;
    remoter.st_curt = (GPIO_PD_IDR & 0x08) ? 1 : 0;
    if (remoter.st_last == remoter.st_curt)
    {
        lv_cnt++;
    }

    /* identify TP */
    if (ST_REMOTE_TP == remoter.st_remo)
    {
        if ((remoter.st_last != remoter.st_curt))
        {
            if ((lv_cnt >= CNT_TE_MIN) && (lv_cnt <= CNT_TE_MAX))
            {
                tp_cnt++;
                if (CNT_TP_TE == tp_cnt)
                {
                    remoter.st_remo = ST_REMOTE_TH;
                }
            }
            else
            {
                tp_cnt = 0;
            }
            lv_cnt = 0;
        }
    }
    /* identify TH */
    else if (ST_REMOTE_TH == remoter.st_remo)
    {
        if ((remoter.st_last != remoter.st_curt))
        {
            if ((1 == remoter.st_curt) && (lv_cnt >= CNT_TH_MIN) && (lv_cnt <= CNT_TH_MAX))
            {
                remoter.st_remo = ST_REMOTE_DATA;
                remoter.recvidx = 0;
            }
            else
            {
                remoter.st_remo = ST_REMOTE_TP;
            }
            lv_cnt = 0;
        }
    }
    /* receive data */
    else if (ST_REMOTE_DATA == remoter.st_remo)
    {
        if ((remoter.st_last != remoter.st_curt))
        {
            if (1 == remoter.st_curt)
            {
                byidx = remoter.recvidx / 8;
                btidx = remoter.recvidx % 8;
                if ((lv_cnt >= CNT_L_MIN) && (lv_cnt <= CNT_L_MAX))
                {
                    remoter.rbuf[byidx] &= ~(1 << btidx);
                    remoter.recvidx++;
                }
                else if ((lv_cnt >= CNT_H_MIN) && (lv_cnt <= CNT_H_MAX))
                {
                    remoter.rbuf[byidx] |= (1 << btidx);
                    remoter.recvidx++;
                }
                else
                {
                    remoter.st_remo = ST_REMOTE_TP;
                }

                if (remoter.recvidx >= RBUF_SIZE * 8)
                {
                    remoter.st_remo = ST_REMOTE_OK;
                }
            }
            lv_cnt = 0;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Get remote controller key                                */
/* PARA :                                                          */
/*   pbuf : buff to store key value                                */
/* RETN :                                                          */
/*   return ET_TURE if receive a key code, else return ET_FALSE    */
/*-----------------------------------------------------------------*/
u8 remote_gkey(u8 *pbuf)
{
    u8 index;

    if (ST_REMOTE_OK != remoter.st_remo)
    {
        return ET_FALSE;
    }

    /* ignore all 0x00 remote controller */
    if ((0x00 == remoter.rbuf[4]) && (0x00 == remoter.rbuf[5]) && (0x00 == remoter.rbuf[6]) && (0x00 == (remoter.rbuf[7] & 0x0F)))
    {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    /* ignore all 0xFF remote controller */
    if ((0xFF == remoter.rbuf[4]) && (0xFF == remoter.rbuf[5]) && (0xFF == remoter.rbuf[6]) && (0x0F == (remoter.rbuf[7] & 0x0F)))
    {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    /* check if the remote controller is keep pressed */
    for (index = 0; index < ENCY_SIZE; index++)
    {
        if (remoter.rbuf[index] != remoter.lbuf[index])
        {
            break;
        }
    }
    if (ENCY_SIZE == index)
    {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    /* record current remote code */
    for (index = 0; index < RBUF_SIZE; index++)
    {
        remoter.lbuf[index] = remoter.rbuf[index];
    }

    pbuf[0] = remoter.rbuf[4];
    pbuf[1] = remoter.rbuf[5];
    pbuf[2] = remoter.rbuf[6];
    pbuf[3] = remoter.rbuf[7];

    remoter.st_remo = ST_REMOTE_TP;

    return ET_TRUE;
}

/*-----------------------------------------------------------------*/
/* FUNC : clear remote key saved in data falsh                     */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void remote_clrs(void)
{
    eepbyt_writ(252, 0);
    eepbyt_writ(253, 0);
}

/*-----------------------------------------------------------------*/
/* FUNC : Save remote key to eeprom                                */
/* PARA :                                                          */
/*   peep : pointer of eeprom control structure                    */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
u8 remote_save(u8 key[4])
{
    u8 sav_key[4], index;
    u8 keynum, keyidx;

    keynum = eepbyt_read(252);
    keyidx = eepbyt_read(253);

    for (index = 0; index < keynum; index++)
    {
        sav_key[0] = eepbyt_read(128 + 4 * index + 0);
        sav_key[1] = eepbyt_read(128 + 4 * index + 1);
        sav_key[2] = eepbyt_read(128 + 4 * index + 2);
        sav_key[3] = eepbyt_read(128 + 4 * index + 3);
        if ((key[0] == sav_key[0]) && (key[1] == sav_key[1]) && (key[2] == sav_key[2]) && ((key[3] & 0x0F) == (sav_key[3] & 0x0F)))
        {
            break;
        }
    }
    if (keynum != index)
    {
        eepbyt_writ(128 + 4 * index + 3, key[3]);
    }
    else
    {
        eepbyt_writ(128 + 4 * keyidx + 0, key[0]);
        eepbyt_writ(128 + 4 * keyidx + 1, key[1]);
        eepbyt_writ(128 + 4 * keyidx + 2, key[2]);
        eepbyt_writ(128 + 4 * keyidx + 3, key[3]);
        if (keynum < 20)
        {
            keynum++;
        }
        keyidx = (keyidx + 1) % 20;
    }

    eepbyt_writ(252, keynum);
    eepbyt_writ(253, keyidx);

    return ET_TRUE;
}

/*-----------------------------------------------------------------*/
/* FUNC : Get a saved remote key                                   */
/* PARA :                                                          */
/*   peep : pointer of eeprom control structure                    */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
u8 remote_hits(void)
{
    u8 sav_key[4], key[4], index;
    u8 keynum, keyidx;

    if (ET_FALSE == remote_gkey(key))
    {
        return ET_FALSE;
    }

    keynum = eepbyt_read(252);

    keyidx = RMT_KEY_NONE;
    for (index = 0; index < keynum; index++)
    {
        sav_key[0] = eepbyt_read(128 + 4 * index + 0);
        sav_key[1] = eepbyt_read(128 + 4 * index + 1);
        sav_key[2] = eepbyt_read(128 + 4 * index + 2);
        sav_key[3] = eepbyt_read(128 + 4 * index + 3);

        if ((key[0] == sav_key[0]) && (key[1] == sav_key[1]) && (key[2] == sav_key[2]))
        {
            if ((key[3] != sav_key[3]) && (1 != (key[3] >> 4)))
            {
                continue;
            }
            switch (key[3] >> 4)
            {
            case 1:
                keyidx = RMT_KEY_HOPN;
                break;
            case 2:
                keyidx = RMT_KEY_CTRL;
                break;
            case 4:
                keyidx = RMT_KEY_CTRL;
                break;
            case 8:
                keyidx = RMT_KEY_CTRL;
                break;
            default:
                keyidx = RMT_KEY_NONE;
                break;
            }
        }
    }

    return keyidx;
}
