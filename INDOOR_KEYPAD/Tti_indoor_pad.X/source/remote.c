#include "remote.h"
#include "pic.h"
#include "keeloq.h"
#include "usart.h"
#include "stdio.h"

static ETST_REMOTE remoter;

const u8 CUSTOMER_code[8]={0x21, 0x18, 0xde, 0x7b, 0x0a, 0x9c, 0xd5, 0x51};


void  remote_init(){
  U08ET index;
  TRISA |= (1<<5);

  remoter.st_last = 0;
  remoter.st_curt = 0;
  remoter.st_remo = ST_REMOTE_TP;
  remoter.recvidx = 0;
  remoter.repeat_pass = 0;
  
  for (index = 0; index < 18; index++)
      ((U08ET*)(&remoter.sn_save[0]))[index] = (U08ET)eeprom_read(index + SN_ADRESS);
  
  if (SAVED != remoter.save_flag) {
      for (index = 0; index < 8; index++)
          remoter.sn_save[index] = 0x0;
      
      remoter.save_flag = SAVED;
  }
  return;
}


void remote_timer_task(void){
    static U08ET lv_cnt, tp_cnt;
    U08ET  byidx,btidx;
    remoter.st_last = remoter.st_curt;
    remoter.st_curt = (U08ET)RA5;    
    if (remoter.st_last == remoter.st_curt) {
        lv_cnt++;
    } else {
        if (ST_REMOTE_TH == remoter.st_remo) {
          if ((1 == remoter.st_curt) && (lv_cnt >= CNT_TH_MIN) && (lv_cnt<=CNT_TH_MAX)) {
            remoter.st_remo = ST_REMOTE_DATA;
            remoter.recvidx = 0;
          } else remoter.st_remo = ST_REMOTE_TP;
          lv_cnt = 0;
        } else if (ST_REMOTE_DATA == remoter.st_remo) {
          if (0 == remoter.st_curt) {
            byidx = (U08ET)(remoter.recvidx / 8);
            btidx = (U08ET)(remoter.recvidx % 8);
            if ((lv_cnt >= CNT_L_MIN) && (lv_cnt <= CNT_L_MAX)) {
              remoter.rbuf[byidx] |= (U08ET)(1<<btidx);
              remoter.recvidx++;
            } else if ((lv_cnt>=CNT_H_MIN) && (lv_cnt<=CNT_H_MAX)) {
              remoter.rbuf[byidx] &= (U08ET)(~(1<<btidx));
              remoter.recvidx++;
            } else remoter.st_remo = ST_REMOTE_TP;
            remoter.hi_cnt = lv_cnt;
            
            if(remoter.recvidx >= RBUF_SIZE * 8) 
                remoter.st_remo = ST_REMOTE_OK;
          }
          if (1 == remoter.st_curt) {
            if ((remoter.hi_cnt + lv_cnt) < CNT_CYCLE_MIN || (remoter.hi_cnt + lv_cnt) > CNT_CYCLE_MAX)
              remoter.st_remo = ST_REMOTE_TP;
          }
          lv_cnt = 0;
        }	
    }
}


U08ET remote_gkey(U08ET* buf){
  U08ET index;

  if (ST_REMOTE_OK != remoter.st_remo) return ET_FALSE;
  
  /* ignore all 0x00 remote controller */
  if ((0x00==remoter.rbuf[4]) && (0x00==remoter.rbuf[5]) && (0x00==remoter.rbuf[6]) && (0x00==(remoter.rbuf[7]&0x0F)) ) {
    remoter.st_remo = ST_REMOTE_TP;
    return ET_FALSE;
  }
  
  /* ignore all 0xFF remote controller */
  if( (0xFF==remoter.rbuf[4]) && (0xFF==remoter.rbuf[5]) && (0xFF==remoter.rbuf[6]) && (0x0F==(remoter.rbuf[7]&0x0F)) ) {
    remoter.st_remo = ST_REMOTE_TP;
    return ET_FALSE;
  }
  
  for (index = 0; index < 8; index++) {
      buf[index] = remoter.rbuf[index];
  }
  
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
U08ET remote_hits(VODET) {
    U08ET rf_buf[8], index;
    U16ET counter_rf, counter_save;
    
    if (ET_FALSE == remote_gkey(rf_buf)) return KEY_NONE;
    
    if ((rf_buf[4] != remoter.sn_save[4]) || \
        (rf_buf[5] != remoter.sn_save[5]) || \
        (rf_buf[6] != remoter.sn_save[6]) || \
        ((rf_buf[7] & 0x0f) != (remoter.sn_save[7] & 0x0f)))
        return KEY_NONE;
    
    /* judge if the current data is repeat */
    for (index = 0; index < 4; index++){
        if (rf_buf[index] != remoter.lbuf[index])
            break;
    } 
    
    if (4 == index) {
        if (((((U08ET)(rf_buf[7])) & 0xf0) == 0x20) || \
            ((((U08ET)(rf_buf[7])) & 0xf0) == 0x30) || \
            ((((U08ET)(rf_buf[7])) & 0xf0) == 0x90) || \
            ((((U08ET)(rf_buf[7])) & 0xf0) == 0x50) ) {
            if (remoter.repeat_pass) {
                if ((((U08ET)(rf_buf[7])) & 0xf0) == 0x20)
                    return KEY_DOOR;
                else if ((((U08ET)(rf_buf[7])) & 0xf0) == 0x90)
                    return KEY_7;
                else if ((((U08ET)(rf_buf[7])) & 0xf0) == 0x30)
                    return KEY_6;
                else if ((((U08ET)(rf_buf[7])) & 0xf0) == 0x50)
                    return KEY_LOCK;
                else return KEY_NONE;
            }
            else return KEY_NONE;
        } else return KEY_NONE;
    }

    for (index = 0; index < 4; index++){
        remoter.lbuf[index] = rf_buf[index];
    }
    /* not repeat  decrypt the current data*/  
    DEcrypt((u8*)rf_buf, (u8*)remoter.lrn_key_save);
    
    /* judge if the decrypt_data is wrong */
    if((rf_buf[2] != rf_buf[4]) || ((rf_buf[3]&0xF0) != (rf_buf[7]&0xF0))) 
        return KEY_NONE;
    
    /* judge if the counter is not satisfy*/
    counter_rf = ((U16ET)rf_buf[1] << 8) + rf_buf[0]; //calculate received counter
    counter_save = ((U16ET)remoter.sn_save[1] << 8) + remoter.sn_save[0];//calculate local counter
    /*judge if the current counter less than local */
    if ((counter_rf < DIFFER_CNT) && (counter_save > (0xffff - DIFFER_CNT))) {
        counter_save = counter_rf + (0xffff - counter_save) + DIFFER_CNT;
        if (counter_save > DIFFER_CNT) {
            remote_clear();
            return KEY_NONE;        
        }
    } else {
        if (counter_rf <= counter_save) { //should be changed 
            remote_clear();
            return KEY_NONE; 
        }
        counter_save = counter_rf - counter_save; //calculate the differ between these two counter
        /* judge if the differ is bigger than 50 */
        if (counter_save > DIFFER_CNT) {
            remote_clear();
            return KEY_NONE;
        }
    }
    
    /* save the current counter */
    remoter.sn_save[0] = rf_buf[0];
    remoter.sn_save[1] = rf_buf[1];
    eeprom_write(SN_ADRESS, remoter.sn_save[0]);
    eeprom_write(SN_ADRESS + 1, remoter.sn_save[1]);    
        
    /* judge if differ > 2 */
    if (counter_save > 3) {
        return KEY_NONE;
    }
    
    /* return the current button-id */
    switch( rf_buf[7] & 0xf0){
        case 0x60:
            return KEY_1;
        case 0xa0:
            return KEY_2;
        case 0x40:
            return KEY_3;
        case 0x80:
            return KEY_4;
        case 0x10:
            return KEY_5;
        case 0x30:
            if (GD125 == equipment_type()) {
                remoter.repeat_pass = 1;
            }
            return KEY_6;
        case 0x90:
            if (GD125 == equipment_type()) {
                remoter.repeat_pass = 1;
            }
            return KEY_7;
        case 0x50:
            remoter.repeat_pass = 1;
            return KEY_LOCK;
        case 0xc0:
            return KEY_LED;
        case 0x20:
            remoter.repeat_pass = 1;
            return KEY_DOOR;
        default:
            return KEY_NONE;
    }
}


U08ET  remote_learn(U08ET* pbuf, U08ET flag){
    U08ET index;
    U08ET rf_buf[8], lrn_buf[8];

    if (ET_FALSE == remote_gkey(rf_buf)) return ET_FALSE;
    /* judge if the current data is repeat */
    for (index = 0; index < 4; index++){
        if (rf_buf[index] != remoter.lbuf[index]) break;
    }    
    if (4 == index) return ET_FALSE;
    
    for (index = 0; index < 4; index++){
        remoter.lbuf[index] = rf_buf[index];
    }

    lrn_buf[3] = rf_buf[7] & 0x0F | 0x20;             
    lrn_buf[2] = rf_buf[6];
    lrn_buf[1] = rf_buf[5];
    lrn_buf[0] = rf_buf[4];
    DEcrypt((u8*)lrn_buf, (u8*)CUSTOMER_code);  
    lrn_buf[7] = rf_buf[7] & 0x0F | 0x60;          
    lrn_buf[6] = rf_buf[6];
    lrn_buf[5] = rf_buf[5];
    lrn_buf[4] = rf_buf[4];
    DEcrypt(((u8*)lrn_buf) + 4, (u8*)CUSTOMER_code);
    DEcrypt((u8*)rf_buf, (u8*)lrn_buf);
    /* judge if the current data is right */
    if((rf_buf[2] != rf_buf[4]) || ((rf_buf[3] & 0xF0) != (rf_buf[7] & 0xF0))) return ET_FALSE;
  
    if (1 == flag) {
        for (index = 0; index < 4; index++){
            pbuf[index] = rf_buf[index + 4];
        }
        return ET_TRUE;
    }
  
  /*  
    else if (2 == flag) {
        if ((pbuf[0] != rf_buf[4]) ||\
            (pbuf[1] != rf_buf[5]) ||\
            (pbuf[2] != rf_buf[6]) ||\
            ((pbuf[3] & 0xf) != (rf_buf[7] & 0xf))){*/    
    
    else if (2 == flag) {
        if (pbuf[0] != rf_buf[4] ||\
            pbuf[1] != rf_buf[5] ||\
            pbuf[2] != rf_buf[6] ||\
            pbuf[3] & 0xf != rf_buf[7] & 0xf ){
            return ET_FALSE;
        }

        for (index = 0; index < 8; index++){
            remoter.lrn_key_save[index] = lrn_buf[index];
            remoter.sn_save[index] = rf_buf[index];
        }
        
        for (index = 0; index < 18; index++)
            eeprom_write((index + SN_ADRESS), ((U08ET*)(&remoter.sn_save[0]))[index]);

        return ET_TRUE;
    }

    return ET_FALSE;

/*  for (index = 0; index < 8; index++){
    remoter.lrn_key_save[index] = lrn_buf[index];
    remoter.sn_save[index] = rf_buf[index];
  }
  
  for (index = 0; index < 8; index++) {
    eeprom_write(index + LRN_ADRESS, remoter.lrn_key_save[index]);
    eeprom_write(index + SN_ADRESS , remoter.lbuf[index]);
  }*/
}

void repeat_lock(void){
    remoter.repeat_pass = 0;
}

U08ET equipment_type(VODET){
    if ((0x00 == remoter.sn_save[4]) && (0x00 == remoter.sn_save[5]) && (0x00 == remoter.sn_save[6]) && (0x00 == remoter.sn_save[7])) {
      return EQ_NONE;
    }
    
    if ((remoter.sn_save[7] & 0x0f) < 8) return GD200;
    else return GD125;
}

VODET  remote_clear(VODET){
    U08ET index;
    for (index = 0; index < 8; index++){
        remoter.lrn_key_save[index] = 0x0;
        remoter.sn_save[index] = 0x0;
    }
        
    for (index = 0; index < 18; index++)
        eeprom_write((index + SN_ADRESS), ((U08ET*)(&remoter.sn_save[0]))[index]);
}