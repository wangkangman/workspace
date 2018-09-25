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
#define KEY_ID_IRAD  (0)
#define KEY_ID_WALL  (1)
#define KEY_ID_DOOR  (2)
#define KEY_ID_POW   (3)
#define KEY_ID_K1    (4)
#define KEY_ID_K2    (5)
#define KEY_ID_K3    (6)
#define KEY_ID_K4    (7)
#define KEY_ID_MAX   (8)

/*-----------------------------------------------------------------*/
/* Define KEY simulation                                           */
/*-----------------------------------------------------------------*/
#define KEY_ID_SIM   (99)
#define KEY_TM_SIM   (2500)

/*-----------------------------------------------------------------*/
/* Define KEY parameter                                            */
/*-----------------------------------------------------------------*/
#define KEY_K1_ADC_VAL (183)
#define KEY_K2_ADC_VAL (201)
#define KEY_K3_ADC_VAL (168)
#define KEY_K4_ADC_VAL (234)
#define KEY_X_ADC_WRAP (5)

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

    U08ET oshot:8;   /* one shot key KEY_ST_HIGH/KEY_ST_LOW */
    U16ET ocnts:16;  /* one shot count */
}ETST_KEYS;

typedef struct
{
    U08ET sims[8];
    U08ET keys[8];
    U16ET tims;
}ETST_SIMS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET keysmp_init(VODET);
VODET keysmp_task(VODET);
U08ET getkey_curst(U08ET keys);
U08ET getkey_stabe(U08ET keys);
U08ET getkey_oshot(U08ET keys);
VODET simkey_clear(VODET);

#endif /* _KEYSMP_H */
