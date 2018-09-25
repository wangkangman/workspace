#include "remote.h"
#include "pic.h"
#include "keeloq.h"
#include "usart.h"
#include "stdio.h"

static ETST_REMOTE remoter;
static DECODE de_code;
const U08ET sav_key[4]={
0x01, 0x00, 0x00, 0x00
}; 
//const u8 CUSTOMER_code[8]={0x21, 0x18, 0xde, 0x7b, 0x0a, 0x9c, 0xd5, 0x51};
const u8 CUSTOMER_code[8]={0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01};
//const u8 CUSTOMER_code[8]={0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
void  remote_init(){
  U08ET index;
  WPUC |= (1<<5);
  TRISC |= (1<<5);
  remoter.st_last = 0;
  remoter.st_curt = 0;
  remoter.st_remo = ST_REMOTE_TP;
  remoter.recvidx = 0;
  for (index = 0; index < RBUF_SIZE; index++) {
    remoter.rbuf[index] = 0;
    remoter.lbuf[index] = 0;
  }
  de_code.delock = 0;
  return;
}

void remote_timer_task(void){
  static U08ET lv_cnt, tp_cnt;
  U08ET  byidx,btidx;
  remoter.st_last = remoter.st_curt;
  remoter.st_curt = (U08ET)RC5;

  if (remoter.st_last == remoter.st_curt) {
    lv_cnt++;
  } else {
    if (ST_REMOTE_TP == remoter.st_remo) {
      if ((lv_cnt >= CNT_TE_MIN) && (lv_cnt <= CNT_TE_MAX)) {
        tp_cnt++;
        if (CNT_TP_TE == tp_cnt) remoter.st_remo = ST_REMOTE_TH;
      } else tp_cnt = 0;
      lv_cnt = 0;
    } else if (ST_REMOTE_TH == remoter.st_remo) {
      if ((1 == remoter.st_curt) && (lv_cnt>=CNT_TH_MIN) && (lv_cnt<=CNT_TH_MAX)) {
        remoter.st_remo = ST_REMOTE_DATA;
        remoter.recvidx = 0;
      } else remoter.st_remo = ST_REMOTE_TP;
      lv_cnt = 0;
    } else if (ST_REMOTE_DATA == remoter.st_remo) {
      if (1==remoter.st_curt) {
        byidx = remoter.recvidx/8;
        btidx = remoter.recvidx%8;
        if ((lv_cnt>=CNT_L_MIN) && (lv_cnt<=CNT_L_MAX)) {
          remoter.rbuf[byidx] &= (U08ET)(~(1<<btidx));
          remoter.recvidx++;
        } else if ((lv_cnt>=CNT_H_MIN) && (lv_cnt<=CNT_H_MAX)) {
          remoter.rbuf[byidx] |= (U08ET)(1<<btidx);
          remoter.recvidx++;
        } else remoter.st_remo = ST_REMOTE_TP;

        if(remoter.recvidx >= RBUF_SIZE*8) remoter.st_remo = ST_REMOTE_OK;
      }
      lv_cnt = 0;
    }	
  }
}

U08ET remote_gkey(U08ET *pbuf){
  U08ET index;

  if (ST_REMOTE_OK != remoter.st_remo) return ET_FALSE;

  if ((0x00 == remoter.rbuf[4]) && (0x00 == remoter.rbuf[5]) && (0x00 == remoter.rbuf[6]) && (0x00 == (remoter.rbuf[7]&0x0F))) {
    remoter.st_remo = ST_REMOTE_TP;
    return ET_FALSE;
  }

  if ((0xFF == remoter.rbuf[4]) && (0xFF == remoter.rbuf[5]) && (0xFF == remoter.rbuf[6]) && (0x0F == (remoter.rbuf[7]&0x0F))) {
    remoter.st_remo = ST_REMOTE_TP;
    return ET_FALSE;
  }
  
  /* check if the remote controller is keep pressed */
  for (index = 0; index < 4; index++) {
      if (remoter.rbuf[index] != remoter.lbuf[index])
          break;
  }
  if (index == 4) {
      remoter.st_remo = ST_REMOTE_TP;
      return ET_FALSE;
  }
  
  /* copy the current data into last_buffer and parameter_buffer*/
  for (index = 0; index < 4; index++) {
      remoter.lbuf[index] = remoter.rbuf[index];
      pbuf[index] = remoter.rbuf[index];
  }  
  pbuf[4] = remoter.rbuf[4];
  pbuf[5] = remoter.rbuf[5];
  pbuf[6] = remoter.rbuf[6];
  pbuf[7] = remoter.rbuf[7];
 
  remoter.st_remo = ST_REMOTE_TP;
  return ET_TRUE;
}

/*-----------------------------------------------------------------*/
/* FUNC : Get a saved remote key                                   */
/* PARA :                                                          */
/*   peep : pointer of eeprom control structure                    */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
U08ET remote_hits(U08ET* buf) {
    U08ET rf_buf[8], lrn_buf[8], index;
  
    if (ET_FALSE == remote_gkey(rf_buf)) return ET_FALSE;
    lrn_buf[3] = rf_buf[7] & 0x0F | 0x20;             
    lrn_buf[2] = rf_buf[6];
    lrn_buf[1] = rf_buf[5];
    lrn_buf[0] = rf_buf[4];
    DEcrypt((u8*)lrn_buf, (u8*)CUSTOMER_code);
    lrn_buf[7] = rf_buf[7] & 0x0F | 0x60;          
    lrn_buf[6] = rf_buf[6];
    lrn_buf[5] = rf_buf[5];
    lrn_buf[4] = rf_buf[4];
    CLRWDT();
    DEcrypt(((u8*)lrn_buf) + 4, (u8*)CUSTOMER_code);
    CLRWDT();
    DEcrypt((u8*)rf_buf, (u8*)lrn_buf);
    

    if((rf_buf[2] != rf_buf[4]) || ((rf_buf[3]&0xF0) != (rf_buf[7]&0xF0))) return ET_FALSE;
    for (index = 0; index < 8; index++) {
        buf[index] = rf_buf[index];
    }
    return ET_TRUE;
}
