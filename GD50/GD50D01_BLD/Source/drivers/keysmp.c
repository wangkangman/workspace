#include "regdef.h"
#include "adcsmp.h"
#include "keysmp.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_KEYS et_keys[KEY_ID_MAX];

/*-----------------------------------------------------------------*/
/* FUNC : KEY initialize                                           */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET keysmp_init(VODET)
{
    U08ET index;

    /* initialize key control structure */
    for(index=0; index<KEY_ID_MAX; index++) {
        et_keys[index].lstst = KEY_ST_MAX;
        et_keys[index].curst = KEY_ST_MAX;
        et_keys[index].stabe = KEY_ST_MAX;
        et_keys[index].actlv = KEY_ST_LOW;
        et_keys[index].cycle = 20;
        et_keys[index].count = 0;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : KEY task dispatch function                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET keysmp_task(VODET)
{
    U08ET adcKey, index;

    /* current status detect */
    et_keys[KEY_ID_K1].curst = 1;
    et_keys[KEY_ID_K2].curst = 1;
    et_keys[KEY_ID_K3].curst = 1;
    et_keys[KEY_ID_K4].curst = 1;

    adcKey = (U08ET)(getadc_data() >> 4);
    if( (adcKey<=KEY_K1_ADC_VAL+KEY_X_ADC_WRAP) && (adcKey>=KEY_K1_ADC_VAL-KEY_X_ADC_WRAP) ) {
        et_keys[KEY_ID_K1].curst = 0;
    }
    else if( (adcKey<=KEY_K2_ADC_VAL+KEY_X_ADC_WRAP) && (adcKey>=KEY_K2_ADC_VAL-KEY_X_ADC_WRAP) ) {
        et_keys[KEY_ID_K2].curst = 0;
    }
    else if( (adcKey<=KEY_K3_ADC_VAL+KEY_X_ADC_WRAP) && (adcKey>=KEY_K3_ADC_VAL-KEY_X_ADC_WRAP) ) {
        et_keys[KEY_ID_K3].curst = 0;
    }
    else if( (adcKey<=KEY_K4_ADC_VAL+KEY_X_ADC_WRAP) && (adcKey>=KEY_K4_ADC_VAL-KEY_X_ADC_WRAP) ) {
        et_keys[KEY_ID_K4].curst = 0;
    }

    /* stable status detect */
    for(index=0; index<KEY_ID_MAX; index++) {
        /* stable key detect */
        if( et_keys[index].curst != et_keys[index].lstst ) {
            et_keys[index].count = 0;
            et_keys[index].stabe = KEY_ST_MAX;
        }
        else {
            if( et_keys[index].count < et_keys[index].cycle ) {
                et_keys[index].count++;
            }
            else if( et_keys[index].count == et_keys[index].cycle ) {
                et_keys[index].stabe = et_keys[index].curst;
                et_keys[index].count++;
            }
        }
        et_keys[index].lstst = et_keys[index].curst;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : GET current key status                                   */
/* PARA :                                                          */
/*   keys : key id                                                 */
/* RETN :                                                          */
/*   if equal active level, return KEY_ST_HIGH                     */
/*   if opposite active level, return KEY_ST_LOW                   */
/*-----------------------------------------------------------------*/
U08ET getkey_curst(U08ET keys)
{
    if( et_keys[keys].curst == et_keys[keys].actlv ) {
        return KEY_ST_HIGH;
    }
    else {
        return KEY_ST_LOW;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : GET stable key status                                    */
/* PARA :                                                          */
/*   keys : key id                                                 */
/* RETN :                                                          */
/*   if equal active level, return KEY_ST_HIGH                     */
/*   if opposite active level, return KEY_ST_LOW                   */
/*   if no stable, return KEY_ST_MAX                               */
/*-----------------------------------------------------------------*/
U08ET getkey_stabe(U08ET keys)
{
    if( et_keys[keys].stabe == et_keys[keys].actlv ) {
        et_keys[keys].stabe = KEY_ST_MAX;
        return KEY_ST_HIGH;
    }
    else if ( et_keys[keys].stabe == KEY_ST_MAX ) {
        return KEY_ST_MAX;
    }
    else {
        et_keys[keys].stabe = KEY_ST_MAX;
        return KEY_ST_LOW;
    }
}
