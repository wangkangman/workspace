#ifndef _REMOTE_H
#define _REMOTE_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define remote controller dectection status                      */
/*-----------------------------------------------------------------*/
#define ST_REMOTE_TP   (0)
#define ST_REMOTE_TH   (1)
#define ST_REMOTE_DATA (2)
#define ST_REMOTE_OK   (3)

/*-----------------------------------------------------------------*/
/* Define remote controller parameter                              */
/*-----------------------------------------------------------------*/
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

#define RMT_KEY_NONE   (0)
#define RMT_KEY_CTRL   (1)

/*-----------------------------------------------------------------*/
/* Define remote controller control structure                      */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET st_last:1;          /* last status */
    U08ET st_curt:1;          /* current status */
    U08ET st_remo:6;          /* remote controller identification status */
    U08ET recvidx;            /* receive buff index, unit of bit */
    U08ET rbuf[RBUF_SIZE];    /* receive buff */
    U08ET lbuf[RBUF_SIZE];    /* last receive buff */
    U08ET skey[RBUF_SIZE];    /* encryption key */
}ETST_REMOTE;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET remote_init(VODET);
VODET remote_samp(VODET);
U08ET remote_gkey(U08ET *pbuf);
VODET remote_clrs(VODET);
U08ET remote_save(U08ET key[4]);
U08ET remote_hits(VODET);

#endif /* _REMOTE_H */
