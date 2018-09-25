#ifndef _KEYSMP_H
#define _KEYSMP_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define KEY status                                               */
/*-----------------------------------------------------------------*/
#define KEY_ST_LOW   (0)
#define KEY_ST_HIGH  (1)
#define KEY_ST_MAX   (2)

/*-----------------------------------------------------------------*/
/* Define KEY ID                                                   */
/*-----------------------------------------------------------------*/
#define KEY_ID_SET   (0)
#define KEY_ID_CODE  (1)
#define KEY_ID_IRAD  (2)
#define KEY_ID_WALL  (3)
#define KEY_ID_LMT0  (4)
#define KEY_ID_LMT1  (5)
#define KEY_ID_LR    (6)
#define KEY_ID_OC    (7)
#define KEY_ID_IRAD_SWITCH    (8)
#define KEY_ID_AUTO_CLOSE    (9)
#define KEY_ID_MAX   (10)


/* 2017/6/19 星期一 下午 4:23:43
KEY_ID_SET:  key
KEY_ID_CODE：key
KEY_ID_IRAD：infrared
KEY_ID_WALL：input
KEY_ID_LMT0：limit  botton
KEY_ID_LMT1：limit  top
KEY_ID_LR：  lift or right
2017/6/19 星期一 下午 4:23:43 */

/*-----------------------------------------------------------------*/
/* Define KEY control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET lstst:2;   /* last key status KEY_ST_HIGH/KEY_ST_LOW */
    U08ET curst:2;   /* current key status KEY_ST_HIGH/KEY_ST_LOW */
    U08ET stabe:2;   /* stable status KEY_ST_HIGH/KEY_ST_LOW */
    U08ET actlv:2;   /* active level KEY_ST_HIGH/KEY_ST_LOW */
    U08ET cycle:8;   /* control cycle, unit of task dispatch 2ms */
    U08ET count:8;   /* cycle count */
}ETST_KEYS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET keysmp_init(VODET);
VODET keysmp_task(VODET);
U08ET getkey_curst(U08ET keys);
U08ET getkey_stabe(U08ET keys);
/*  2017/6/16 星期五 上午 8:48:12 GET driver level ----------------*/
U08ET getdriver_level(VODET);
/*  2017/6/16 星期五 上午 8:48:12 GET driver level ----------------*/

/* 2017/6/16 星期五 下午 2:42:41   OC_detect-----------------------*/
U08ET oc_detect(VODET);
/* 2017/6/16 星期五 下午 2:42:41   OC_detect-----------------------*/



/* 2017/6/17 星期六 上午 8:42:28-limit sensor skew detect----------*/
#if LIMIT_SEN_ERR
#define LIMIT_SEN_ERR_NUM  (15)          /*tolerance of hall missing times*/
#define LIMIT_SEN_HALL_TOLERANCE  (130)  /*tolerance of hall missing counter per time */ 
VODET limit_sen_cnt_plus(VODET);
VODET limit_sen_cnt_min(VODET);
U08ET limit_sen_err_check(VODET);
#endif
/* 2017/6/17 星期六 上午 8:42:28-limit sensor skew detect----------*/


#endif /* _KEYSMP_H */

