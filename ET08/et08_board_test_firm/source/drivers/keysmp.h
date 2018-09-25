#ifndef _KEYSMP_H
#define _KEYSMP_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define KEY status                                               */
/*-----------------------------------------------------------------*/
#define KEY_ST_LOW (0)
#define KEY_ST_HIGH (1)
#define KEY_ST_MAX (2)

/*-----------------------------------------------------------------*/
/* Define KEY ID                                                   */
/*-----------------------------------------------------------------*/
#define KEY_ID_IRAD (0)
#define KEY_ID_WALL (1)
#define KEY_ID_DOOR (2)
#define KEY_ID_POW (3)
#define KEY_ID_K1 (4)
#define KEY_ID_K2 (5)
#define KEY_ID_K3 (6)
#define KEY_ID_K4 (7)
#define KEY_ID_MAX (8)

/*-----------------------------------------------------------------*/
/* Define KEY simulation                                           */
/*-----------------------------------------------------------------*/
#define KEY_ID_SIM (99)
#define KEY_TM_SIM (2500)

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
    u8 lstst : 2; /* last key status KEY_ST_HIGH/KEY_ST_LOW */
    u8 curst : 2; /* current key status KEY_ST_HIGH/KEY_ST_LOW */
    u8 stabe : 2; /* stable status KEY_ST_HIGH/KEY_ST_LOW */
    u8 actlv : 2; /* active level KEY_ST_HIGH/KEY_ST_LOW */
    u8 cycle : 8; /* control cycle, unit of task dispatch 2ms */
    u8 count : 8; /* cycle count */
} ETST_KEYS;

typedef struct
{
    u8 sims[8];
    u8 keys[8];
    u16 tims;
} ETST_SIMS;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void keysmp_init(void);
void keysmp_task(void);
u8 getkey_curst(u8 keys);
u8 getkey_stabe(u8 keys);
void simkey_clear(void);

#endif /* _KEYSMP_H */
