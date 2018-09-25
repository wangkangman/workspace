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
#define KEY_ID_REMOTE    (0)
#define KEY_ID_MAX       (1)
#if 0
#define KEY_ID_K1    (0)
#define KEY_ID_K2    (1)
#define KEY_ID_K3    (2)
#define KEY_ID_K4    (3)
#define KEY_ID_MAX   (4)
#endif

/*-----------------------------------------------------------------*/
/* Define KEY parameter                                            */
/*-----------------------------------------------------------------*/
#define KEY_K1_ADC_VAL (205)
#define KEY_K2_ADC_VAL (151)
#define KEY_K3_ADC_VAL (100)
#define KEY_K4_ADC_VAL (50)
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
}ETST_KEYS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET keysmp_init(VODET);
VODET keysmp_task(VODET);
U08ET getkey_curst(U08ET keys);
U08ET getkey_stabe(U08ET keys);

#endif /* _KEYSMP_H */
