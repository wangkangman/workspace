/* 
 * File:   remote.h
 * Author: Administrator
 *
 * Created on November 8, 2017, 7:58 PM
 */

#ifndef REMOTE_H
#define	REMOTE_H


#include "type.h"


#define ST_REMOTE_TP   (0)
#define ST_REMOTE_TH   (1)
#define ST_REMOTE_DATA (2)
#define ST_REMOTE_OK   (3)

#define RBUF_SIZE      (8)
#define ENCY_SIZE      (4)
#define REID_SIZE      (4)

#define CNT_TP_TE      (23)
#define CNT_TE_MIN     (4)
#define CNT_TE_MAX     (12)
#define CNT_TH_MIN     (70)
#define CNT_TH_MAX     (90)
#define CNT_L_MIN      (4)
#define CNT_L_MAX      (10)
#define CNT_H_MIN      (12)
#define CNT_H_MAX      (20)


#define KEY_FALSE (0)
#define KEY_1     (1)
#define KEY_2     (2)
#define KEY_3     (3)
#define KEY_4     (4)
#define KEY_5     (5)
#define KEY_6     (6)
#define KEY_7     (7)
#define KEY_LOCK  (8)
#define KEY_LED   (9)
#define KEY_DOOR  (10)




typedef struct{
    U08ET st_last:1;          /* last status */
    U08ET st_curt:1;          /* current status */
    U08ET st_remo:6;          /* remote controller identification status */
    U08ET recvidx;            /* receive buff index, unit of bit */
    U08ET rbuf[RBUF_SIZE];    /* receive buff */
    U08ET lbuf[4];    /* last receive buff */
}ETST_REMOTE;

typedef struct{
    U08ET delock;
}DECODE;

void remote_timer_task();
void remote_init();
U08ET remote_gkey(U08ET *pbuf);
void de_code_open(void);
U08ET remote_hits(U08ET* buf);

#endif	/* REMOTE_H */

