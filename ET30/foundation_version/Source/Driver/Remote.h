#ifndef __REMOTE_H
#define __REMOTE_H

#include "regdef.h"
#include "global.h"
#include "dataflash.h"
#include "Timer_Wdg.h"
/*******REMOTE*************/


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

#define REMOTE_SAVE_FLAG      (9527)

#define KEY_AMOUNT   (20)

typedef struct{
    U08ET store_key[KEY_AMOUNT][4];    /* receive buff */
    U08ET store_step;    /* last receive buff */
    U08ET store_count;
    U16ET save_flag;
}KEY_ST;

typedef struct{
    U08ET st_last:1;          /* last status */
    U08ET st_curt:1;          /* current status */
    U08ET st_remo:6;          /* remote controller identification status */
    U08ET recvidx;            /* receive buff index, unit of bit */
    U08ET rbuf[RBUF_SIZE];    /* receive buff */
    U08ET lbuf[RBUF_SIZE];    /* last receive buff */
}ETST_REMOTE;

void  RemoteKey_Read(void);
void  remote_init(void);
void  remote_timer_task(void);
void  remote_sys_task(void);
U08ET remote_gkey(U08ET *pbuf);
U08ET remote_hits(void);
void  remote_save(void);
void  remote_clrs(void);
void  remote_insert_key(U08ET *pbuf);

#endif