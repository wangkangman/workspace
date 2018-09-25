#include "regdef.h"
#include "eeprom.h"
#include "timwdt.h"
#include "remote.h"
#include "si_4355.h"

/*-----------------------------------------------------------------*/
/* Const variable define                                           */
/*-----------------------------------------------------------------*/
const U08ET NLF[2][2][2][2][2] = 
{
    0,1,1,1,0,1,0,0,
    0,0,1,0,1,1,1,0,
    0,0,1,1,1,0,1,0,
    0,1,0,1,1,1,0,0,
};

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_REMOTE remoter;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
static U08ET getsbit(U08ET *psrc, U16ET bidx);
static VODET decrypt(U08ET pbuf[4], U08ET nkey[8]);

/*-----------------------------------------------------------------*/
/* FUNC : Remote controller initialize                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET remote_init(VODET)
{
    U08ET index;

    /* remote controller  <--> PD3 */
    GPIO_PD_DDR &= ~(1<<3);  /* direction input  */
    GPIO_PD_CR1 &= ~(1<<3);  /* float input      */
    GPIO_PD_CR2 &= ~(1<<3);  /* max speed 2MHz   */

    /* initialize remote control structure */
    remoter.st_last = 0;
    remoter.st_curt = 0;
    remoter.st_remo = ST_REMOTE_TP;
    remoter.recvidx = 0;
    for(index=0; index<RBUF_SIZE; index++) {
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
VODET remote_samp(VODET)
{
    static U08ET lv_cnt, tp_cnt;
    U08ET byidx,btidx,idx;

    remoter.st_last = remoter.st_curt;
    remoter.st_curt = (GPIO_PD_IDR & 0x08) ? 1:0;
    if( remoter.st_last == remoter.st_curt ) {
        lv_cnt++;
    }

    /* identify TP */
    if(ST_REMOTE_TP == remoter.st_remo) {
        if( (remoter.st_last != remoter.st_curt) ) {
            if( (lv_cnt >= CNT_TE_MIN) && (lv_cnt <= CNT_TE_MAX) ) {
                tp_cnt++;
                if(CNT_TP_TE == tp_cnt) {
                    remoter.st_remo = ST_REMOTE_TH;
                }
            }
            else {
                tp_cnt = 0;
            }
            lv_cnt = 0;
        }
    }
    /* identify TH */
    else if(ST_REMOTE_TH == remoter.st_remo) {
        if( (remoter.st_last != remoter.st_curt) ) {
            if( (1==remoter.st_curt) && (lv_cnt>=CNT_TH_MIN) && (lv_cnt<=CNT_TH_MAX) ) {
                remoter.st_remo = ST_REMOTE_DATA;
                remoter.recvidx = 0;
            }
            else {
                remoter.st_remo = ST_REMOTE_TP;
            }
            lv_cnt = 0;
        }
    }
    /* receive data */
    else if(ST_REMOTE_DATA == remoter.st_remo) {
        if( (remoter.st_last != remoter.st_curt) ) {
            if(1==remoter.st_curt) {
                byidx = remoter.recvidx/8;
                btidx = remoter.recvidx%8;
                if( (lv_cnt>=CNT_L_MIN) && (lv_cnt<=CNT_L_MAX) ) {
                    remoter.rbuf[byidx] &= ~(1<<btidx);
                    remoter.recvidx++;
                }
                else if( (lv_cnt>=CNT_H_MIN) && (lv_cnt<=CNT_H_MAX) ) {
                    remoter.rbuf[byidx] |= (1<<btidx);
                    remoter.recvidx++;
                }
                else {
                    remoter.st_remo = ST_REMOTE_TP;
                }

                if(remoter.recvidx >= RBUF_SIZE*8) {
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

U08ET remote_gkey(U08ET *pbuf) {
  return si4355_getkey(pbuf);
}
#if 0
U08ET remote_gkey(U08ET *pbuf)
{
    U08ET index;

    if(ST_REMOTE_OK != remoter.st_remo) {
        return ET_FALSE;
    }

    /* ignore all 0x00 remote controller */
    if( (0x00==remoter.rbuf[4]) && (0x00==remoter.rbuf[5]) && (0x00==remoter.rbuf[6]) && (0x00==(remoter.rbuf[7]&0x0F)) ) {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    /* ignore all 0xFF remote controller */
    if( (0xFF==remoter.rbuf[4]) && (0xFF==remoter.rbuf[5]) && (0xFF==remoter.rbuf[6]) && (0x0F==(remoter.rbuf[7]&0x0F)) ) {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    /* check if the remote controller is keep pressed */
    for(index=0; index<ENCY_SIZE; index++) {
        if(remoter.rbuf[index] != remoter.lbuf[index]) {
            break;
        }
    }
    if(ENCY_SIZE == index) {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    /* record current remote code */
    for(index=0; index<RBUF_SIZE; index++) {
        remoter.lbuf[index] = remoter.rbuf[index];
    }

    switch(glb_cfgs.rmt_type) {
        case 0: /* open code */
            break;
        case 1: /* serial number detect */
            if( (0x70!=(remoter.rbuf[6]&0xF0)) && (0x05!=(remoter.rbuf[7]&0x0F)) ) {
                remoter.st_remo = ST_REMOTE_TP;
                return ET_FALSE;
            }
            break;
        case 2: /* easy mode rolling code */
            decrypt(remoter.rbuf, glb_cfgs.rmt_vids);

            if( (remoter.rbuf[2] != remoter.rbuf[4]) || ((remoter.rbuf[3]&0xF0) != (remoter.rbuf[7]&0xF0)) ) {
                remoter.st_remo = ST_REMOTE_TP;
                return ET_FALSE;
            }
            break;
        case 3: /* normal mode rolling code */
            remoter.skey[0] = remoter.rbuf[4];
            remoter.skey[1] = remoter.rbuf[5];
            remoter.skey[2] = remoter.rbuf[6];
            remoter.skey[3] = remoter.rbuf[7] & 0x0F | 0x20;
            decrypt(remoter.skey, glb_cfgs.rmt_vids);

            remoter.skey[4] = remoter.rbuf[4];
            remoter.skey[5] = remoter.rbuf[5];
            remoter.skey[6] = remoter.rbuf[6];
            remoter.skey[7] = remoter.rbuf[7] & 0x0F | 0x60;
            decrypt(remoter.skey+4, glb_cfgs.rmt_vids);

            decrypt(remoter.rbuf, remoter.skey);

            if( (remoter.rbuf[2] != remoter.rbuf[4]) || ((remoter.rbuf[3]&0xF0) != (remoter.rbuf[7]&0xF0)) ) {
                remoter.st_remo = ST_REMOTE_TP;
                return ET_FALSE;
            }
            break;
        default:
            break;
    }

    pbuf[0] = remoter.rbuf[4];
    pbuf[1] = remoter.rbuf[5];
    pbuf[2] = remoter.rbuf[6];
    pbuf[3] = remoter.rbuf[7];

    remoter.st_remo = ST_REMOTE_TP;

    return ET_TRUE;
}
#endif

/*-----------------------------------------------------------------*/
/* FUNC : clear remote key saved in data falsh                     */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET remote_clrs(VODET)
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
U08ET remote_save(U08ET key[4])
{
    U08ET sav_key[4], index;
    U08ET keynum, keyidx;

    keynum = eepbyt_read(252);
    keyidx = eepbyt_read(253);

    for(index=0; index<keynum; index++) {
        sav_key[0] = eepbyt_read(128+4*index+0);
        sav_key[1] = eepbyt_read(128+4*index+1);
        sav_key[2] = eepbyt_read(128+4*index+2);
        sav_key[3] = eepbyt_read(128+4*index+3);
        if( (key[0]==sav_key[0]) && (key[1]==sav_key[1]) && (key[2]==sav_key[2]) && ((key[3]&0x0F)==(sav_key[3]&0x0F)) ) {
            break;
        }
    }
    if(keynum != index) {
        eepbyt_writ(128+4*index+3, key[3]);
    }
    else {
        eepbyt_writ(128+4*keyidx+0, key[0]);
        eepbyt_writ(128+4*keyidx+1, key[1]);
        eepbyt_writ(128+4*keyidx+2, key[2]);
        eepbyt_writ(128+4*keyidx+3, key[3]);
        if(keynum < 20) {
            keynum++;
        }
        keyidx = (keyidx+1)%20;
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
U08ET remote_hits(VODET)
{
    U08ET sav_key[4], key[4], index;
    U08ET keynum, keyidx;

    if( ET_FALSE == remote_gkey(key) ) {
        return ET_FALSE;
    }

    keynum = eepbyt_read(252);

    keyidx = RMT_KEY_NONE;
    for(index=0; index<keynum; index++) {
        sav_key[0] = eepbyt_read(128+4*index+0);
        sav_key[1] = eepbyt_read(128+4*index+1);
        sav_key[2] = eepbyt_read(128+4*index+2);
        sav_key[3] = eepbyt_read(128+4*index+3);

        if( (key[0]==sav_key[0]) && (key[1]==sav_key[1]) && (key[2]==sav_key[2]) && (key[3]==sav_key[3]) ) {
            keyidx = RMT_KEY_CTRL;
        }
    }

    return keyidx;
}

/*-----------------------------------------------------------------*/
/* FUNC : Get one bit of from array                                */
/* PARA :                                                          */
/*   psrc : array data buff                                        */
/*   bidx : target bit index                                       */
/* RETN :                                                          */
/*   return bit value                                              */
/*-----------------------------------------------------------------*/
static U08ET getsbit(U08ET *psrc, U16ET bidx)
{
    U08ET byidx, btidx;

    byidx = bidx/8;
    btidx = bidx%8;

    if( 0 == (psrc[byidx] & (1<<btidx)) ) {
        return 0;
    }
    else{
        return 1;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Simple mode KEELOQ decryption                            */
/* PARA :                                                          */
/*   pbuf : orginal encrypted data                                 */
/*   nkey : nlf key                                                */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static VODET decrypt(U08ET pbuf[4], U08ET nkey[8])
{
    U08ET bitk[4], res;
    U16ET index;

    for(index=0; index<528; index++) {
        bitk[0] = NLF[getsbit(pbuf,30)][getsbit(pbuf,25)][getsbit(pbuf,19)][getsbit(pbuf, 8)][getsbit(pbuf,0)];
        bitk[1] = getsbit(pbuf, 15);
        bitk[2] = getsbit(pbuf, 31);
        bitk[3] = getsbit(nkey, (527-index)%64);
        res = bitk[0]^bitk[1]^bitk[2]^bitk[3];
        pbuf[3] = (pbuf[3]<<1) | (pbuf[2]>>7);
        pbuf[2] = (pbuf[2]<<1) | (pbuf[1]>>7);
        pbuf[1] = (pbuf[1]<<1) | (pbuf[0]>>7);
        pbuf[0] = (pbuf[0]<<1) | (res);
    }
}
