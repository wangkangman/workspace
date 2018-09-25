#ifndef _REMOTE_H
#define _REMOTE_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define remote controller dectection status                      */
/*-----------------------------------------------------------------*/
#define ST_REMOTE_TP (0)
#define ST_REMOTE_TH (1)
#define ST_REMOTE_DATA (2)
#define ST_REMOTE_OK (3)

/*-----------------------------------------------------------------*/
/* Define remote controller parameter                              */
/*-----------------------------------------------------------------*/
#define RBUF_SIZE (8)
#define ENCY_SIZE (4)
#define REID_SIZE (4)

#define CNT_TP_TE (23)
#define CNT_TE_MIN (4)
#define CNT_TE_MAX (12)
#define CNT_TH_MIN (70)
#define CNT_TH_MAX (90)
#define CNT_L_MIN (4)
#define CNT_L_MAX (10)
#define CNT_H_MIN (12)
#define CNT_H_MAX (20)

#define RMT_KEY_NONE (0)
#define RMT_KEY_HOPN (1)
#define RMT_KEY_CTRL (2)

/*-----------------------------------------------------------------*/
/* Define remote controller control structure                      */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 st_last : 1;     /* last status */
    u8 st_curt : 1;     /* current status */
    u8 st_remo : 6;     /* remote controller identification status */
    u8 recvidx;         /* receive buff index, unit of bit */
    u8 rbuf[RBUF_SIZE]; /* receive buff */
    u8 lbuf[RBUF_SIZE]; /* last receive buff */
} ETST_REMOTE;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void remote_init(void);
void remote_samp(void);
u8 remote_gkey(u8 *pbuf);
void remote_clrs(void);
u8 remote_save(u8 key[4]);
u8 remote_hits(void);

#endif /* _REMOTE_H */
