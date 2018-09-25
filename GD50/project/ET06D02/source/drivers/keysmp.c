#include "regdef.h"
#include "adcsmp.h"
#include "keysmp.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_KEYS et_keys[KEY_ID_MAX];
static ETST_SIMS et_sims;

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

    /* KEY_IRAD  <--> PA2 */
    GPIO_PA_DDR &= ~(1<<2);  /* direction input  */
    GPIO_PA_CR1 &= ~(1<<2);  /* float input      */
    GPIO_PA_CR2 &= ~(1<<2);  /* max speed 2MHz   */

    /* KEY_WALL  <--> PB2 */
    GPIO_PB_DDR &= ~(1<<2);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<2);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<2);  /* max speed 2MHz   */

    /* KEY_DOOR  <--> PB3 */
    GPIO_PB_DDR &= ~(1<<3);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<3);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<3);  /* max speed 2MHz   */

    /* KEY_POWER  <--> PA1 */
    GPIO_PA_DDR &= ~(1<<1);  /* direction input  */
    GPIO_PA_CR1 &= ~(1<<1);  /* float input      */
    GPIO_PA_CR2 &= ~(1<<1);  /* max speed 2MHz   */

    /* initialize key control structure */
    for(index=0; index<KEY_ID_MAX; index++) {
        et_keys[index].lstst = KEY_ST_MAX;
        et_keys[index].curst = KEY_ST_MAX;
        et_keys[index].stabe = KEY_ST_MAX;
        et_keys[index].cycle = 20;
        et_keys[index].count = 0;

        et_keys[index].oshot = KEY_ST_MAX;
        et_keys[index].ocnts = 0;
    }
    et_keys[KEY_ID_IRAD].actlv = KEY_ST_HIGH;
    et_keys[KEY_ID_WALL].actlv = KEY_ST_LOW;
    et_keys[KEY_ID_DOOR].actlv = KEY_ST_HIGH;
    et_keys[KEY_ID_POW].actlv  = KEY_ST_HIGH;
    et_keys[KEY_ID_K1].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_K2].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_K3].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_K4].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_POW].cycle  = 5;

    /* initialize key simulation structure */
    et_sims.sims[0] = KEY_ID_K1;
    et_sims.sims[1] = KEY_ID_K2;
    et_sims.sims[2] = KEY_ID_K3;
    et_sims.sims[3] = KEY_ID_K4;
    et_sims.sims[4] = KEY_ID_K1;
    et_sims.sims[5] = KEY_ID_K4;
    et_sims.sims[6] = KEY_ID_K3;
    et_sims.sims[7] = KEY_ID_K2;
    et_sims.tims = KEY_TM_SIM;
    simkey_clear();
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
    U08ET adcKey, simidx, index;

    et_keys[KEY_ID_IRAD].curst = (GPIO_PA_IDR & 0x04) ? 1:0;
    et_keys[KEY_ID_WALL].curst = (GPIO_PB_IDR & 0x04) ? 1:0;
    et_keys[KEY_ID_DOOR].curst = (GPIO_PB_IDR & 0x08) ? 1:0;
    et_keys[KEY_ID_POW].curst  = (GPIO_PA_IDR & 0x02) ? 1:0;
    et_keys[KEY_ID_K1].curst   = 1;
    et_keys[KEY_ID_K2].curst   = 1;
    et_keys[KEY_ID_K3].curst   = 1;
    et_keys[KEY_ID_K4].curst   = 1;

    adcKey = getadc_data(ADC_CH_KEY);
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
        /* one shot key detect */
        if( KEY_ST_LOW==et_keys[index].oshot ) {
            if( et_keys[index].ocnts < 500 ) {
                et_keys[index].ocnts++;
            }
            else {
                et_keys[index].oshot = KEY_ST_MAX;
            }
        }

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
                /* simulation key detect */
                if( (et_keys[index].stabe == et_keys[index].actlv) ) {
                    if( (index>=KEY_ID_K1) && (index<=KEY_ID_K4) ) {
                        et_sims.tims = KEY_TM_SIM;
                        for(simidx=0; simidx<7; simidx++) {
                            et_sims.keys[simidx] = et_sims.keys[simidx+1];
                        }
                        et_sims.keys[7] = index;
                    }
                }
                /* one shot key detect */
                if( (et_keys[index].stabe == et_keys[index].actlv) ) {
                    et_keys[index].ocnts = 0;
                    et_keys[index].oshot = KEY_ST_LOW;
                }
                else {
                      if( (et_keys[index].ocnts<500) && (KEY_ST_LOW==et_keys[index].oshot) ) {
                        et_keys[index].oshot = KEY_ST_HIGH;
                      }
                }
            }
        }
        et_keys[index].lstst = et_keys[index].curst;
    }

    /* simulation key clear */
    if( 0 == et_sims.tims ) {
        simkey_clear();
    }
    else {
        et_sims.tims--;
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
    U08ET index;

    if( KEY_ID_SIM == keys ) {
        for(index=0; index<8; index++) {
            if( et_sims.keys[index] != et_sims.sims[index] ) {
                break;
            }
        }
        if( 8 == index ) {
            simkey_clear();
            return KEY_ST_HIGH;
        }
        else {
            return KEY_ST_LOW;
        }
    }

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

/*-----------------------------------------------------------------*/
/* FUNC : GET stable one shot status                               */
/* PARA :                                                          */
/*   keys : key id                                                 */
/* RETN :                                                          */
/*   return current key one shot status                            */
/*-----------------------------------------------------------------*/
U08ET getkey_oshot(U08ET keys)
{
    if( KEY_ST_HIGH == et_keys[keys].oshot ) {
        et_keys[keys].oshot = KEY_ST_MAX;
        return KEY_ST_HIGH;
    }

    return KEY_ST_MAX;
}

/*-----------------------------------------------------------------*/
/* FUNC : Clear simulation key buff                                */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET simkey_clear(VODET)
{
    U08ET index;

    for(index=0; index<8; index++) {
        et_sims.keys[index] = KEY_ID_SIM;
    }
}
