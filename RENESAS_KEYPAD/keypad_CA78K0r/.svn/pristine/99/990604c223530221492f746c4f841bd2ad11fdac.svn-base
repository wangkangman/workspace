#ifndef _KEY_H
#define _KEY_H

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
#define KEY_ID_0 (0)
#define KEY_ID_1 (1)
#define KEY_ID_2 (2)
#define KEY_ID_3 (3)
#define KEY_ID_4 (4)
#define KEY_ID_5 (5)
#define KEY_ID_6 (6)
#define KEY_ID_7 (7)
#define KEY_ID_8 (8)
#define KEY_ID_9 (9)
#define KEY_ID_X (10)
#define KEY_ID_ENTER (11)
#define KEY_ID_MAX (12)


/*********INPUT*************/
/*#define KEY_1 (P1_bit.no0)
#define KEY_2 (P1_bit.no1)
#define KEY_3 (P1_bit.no2)
#define KEY_4 (P1_bit.no3)
#define KEY_5 (P1_bit.no4)
#define KEY_6 (P6_bit.no1)
#define KEY_7 (P6_bit.no0)
#define KEY_8 (P12_bit.no1)
#define KEY_9 (P12_bit.no2)
#define KEY_X (P13_bit.no7)
#define KEY_0 (P4_bit.no1)
#define KEY_ENTER (P4_bit.no2)*/

/*-----------------------------------------------------------------*/
/* Define KEY control structure                                    */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 lstst:2;   /* last key status KEY_ST_HIGH/KEY_ST_LOW */
    u8 curst:2;   /* current key status KEY_ST_HIGH/KEY_ST_LOW */
    u8 stabe:2;   /* stable status KEY_ST_HIGH/KEY_ST_LOW */
    u8 actlv:2;   /* active level KEY_ST_HIGH/KEY_ST_LOW */
    u8 cycle:8;   /* control cycle, unit of task dispatch 2ms */
    u8 count:8;   /* cycle count */
}ETST_KEYS;

void key_init(void);
void key_task(void);
u8 getkey(void);

#endif