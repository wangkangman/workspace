/* 
 * File:   remote.h
 * Author: Administrator
 *
 * Created on November 8, 2017, 7:58 PM
 */

#ifndef REMOTE_H
#define	REMOTE_H


#include "type.h"


#define SN_ADRESS      ((U08ET)(0))
#define LRN_ADRESS     ((U08ET)(20))
#define SAVED     (9527)

#define GD200 (0)
#define GD125 (1)
#define EQ_NONE (2)

#define DIFFER_CNT     (50)

//#define ST_REMOTE_TP   (0)
#define ST_REMOTE_TP   (1)
#define ST_REMOTE_TH   (1)
#define ST_REMOTE_DATA (2)
#define ST_REMOTE_OK   (3)

#define RBUF_SIZE      (8)
#define ENCY_SIZE      (4)
#define REID_SIZE      (4)

#define CNT_TP_TE      (23)
#define CNT_TE_MIN     (4)
#define CNT_TE_MAX     (12)
#define CNT_TH_MIN     (52)
#define CNT_TH_MAX     (132)
#define CNT_L_MIN      (4)
#define CNT_L_MAX      (10)
#define CNT_H_MIN      (12)
#define CNT_H_MAX      (20)
#define CNT_CYCLE_MIN      (18)
#define CNT_CYCLE_MAX      (38)

#define KEY_NONE  (0x11)
#define KEY_1     (0x00)
#define KEY_2     (0x01)
#define KEY_3     (0x02)
#define KEY_4     (0x03)
#define KEY_5     (0x04)
#define KEY_6     (0x05)
#define KEY_7     (0x06)
#define KEY_LOCK  (0x07)
#define KEY_LED   (0x08)
#define KEY_DOOR  (0x09)


typedef struct{
    U08ET st_last:1;          /* last status */
    U08ET st_curt:1;          /* current status */
    U08ET st_remo:6;          /* remote controller identification status */
    U08ET recvidx;            /* receive buff index, unit of bit */
    U08ET repeat_pass;
    U08ET hi_cnt;
    
    U08ET rbuf[RBUF_SIZE];    /* receive buff */
    U08ET lbuf[4];    /* last receive buff */    
    
    U08ET sn_save[RBUF_SIZE];
    U08ET lrn_key_save[8];
    U16ET save_flag;
}ETST_REMOTE;

VODET  remote_timer_task(VODET);
VODET  remote_init(VODET);
U08ET  remote_gkey(U08ET* buf);
VODET  de_code_open(VODET);
U08ET  remote_hits(VODET);
U08ET  remote_learn(U08ET* rf_buf, U08ET flag);
VODET  remote_clear(VODET);
void   repeat_lock(void);
U08ET  equipment_type(VODET);
#endif	/* REMOTE_H */

