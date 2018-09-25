#ifndef __KEY_AANDD_H
#define __KEY_AANDD_H

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
#define KEY_ID_XXX     (0)
#define KEY_ID_K1      (1)
#define KEY_ID_K2      (2)
#define KEY_ID_K3      (3)
#define KEY_ID_K4      (4)
#define KEY_ID_K5      (5)
#define KEY_ID_INFRA   (6)
#define KEY_ID_MAX     (7)

/*-----------------------------------------------------------------*/
/* Define KEY parameter                                            */
/*-----------------------------------------------------------------*/
#define KEY_SMP_PERIOD  (7)
#define KEY_K1_ADC_VAL  (184)
#define KEY_K2_ADC_VAL  (202)
#define KEY_K3_ADC_VAL  (169)
#define KEY_K4_ADC_VAL  (235)
#define KEY_XX_ADC_WRAP (5)
/*#define KEY_K1_ADC_VAL  (736)
#define KEY_K2_ADC_VAL  (808)
#define KEY_K3_ADC_VAL  (676)
#define KEY_K4_ADC_VAL  (940)
#define KEY_XX_ADC_WRAP (20)*/

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
void keysmp_init(void);
void keysmp_task(void);
U08ET getkey_curst(U08ET keys);
U08ET getkey_stabe(U08ET keys);

#endif /* _KEYSMP_H */
