#include "regdef.h"
#include "AdcFunc.h"
#include "Key_AandD.h"
#include "regdef.h"
/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_KEYS et_keys[KEY_ID_MAX-1];

/*-----------------------------------------------------------------*/
/* FUNC : KEY initialize                                           */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void keysmp_init(void)
{
    U08ET index;

    /* initialize key control structure */
    for(index=0; index<KEY_ID_MAX-1; index++) {
        et_keys[index].lstst = KEY_ST_MAX;
        et_keys[index].curst = KEY_ST_MAX;
        et_keys[index].stabe = KEY_ST_MAX;
        et_keys[index].cycle = KEY_SMP_PERIOD;
        et_keys[index].count = 0;
    }
    et_keys[KEY_ID_K1-1].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_K2-1].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_K3-1].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_K4-1].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_K5-1].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_INFRA-1].actlv   = KEY_ST_HIGH;
}

/*--------------------------------------------------------------*/
/* FUNC : KEY task dispatch function                            */
/* PARA :                                                       */
/*   NONE                                                       */
/* RETN :                                                       */
/*   NONE                                                       */
/*--------------------------------------------------------------*/
void keysmp_task(void)
{
    U08ET adcKey,index;

    adcKey = (U08ET)(getadc_data(ADC_CH_KEY)>>2);
    et_keys[KEY_ID_K1-1].curst = 1;
    et_keys[KEY_ID_K2-1].curst = 1;
    et_keys[KEY_ID_K3-1].curst = 1;
    et_keys[KEY_ID_K4-1].curst = 1;
    et_keys[KEY_ID_K5-1].curst = PORT_PD12&(0x1 << 5)? 1 : 0; 
    et_keys[KEY_ID_INFRA-1].curst = PORT_PD13&(0x1 << 7)? 1 : 0; 
    if( (adcKey<=KEY_K1_ADC_VAL+KEY_XX_ADC_WRAP) && (adcKey>=KEY_K1_ADC_VAL-KEY_XX_ADC_WRAP) ) {
        et_keys[KEY_ID_K1-1].curst = 0;
    }
    else if( (adcKey<=KEY_K2_ADC_VAL+KEY_XX_ADC_WRAP) && (adcKey>=KEY_K2_ADC_VAL-KEY_XX_ADC_WRAP) ) {
        et_keys[KEY_ID_K2-1].curst = 0;
    }
    else if( (adcKey<=KEY_K3_ADC_VAL+KEY_XX_ADC_WRAP) && (adcKey>=KEY_K3_ADC_VAL-KEY_XX_ADC_WRAP) ) {
        et_keys[KEY_ID_K3-1].curst = 0;
    }
    else if( (adcKey<=KEY_K4_ADC_VAL+KEY_XX_ADC_WRAP) && (adcKey>=KEY_K4_ADC_VAL-KEY_XX_ADC_WRAP) ) {
        et_keys[KEY_ID_K4-1].curst = 0;
    }

    for(index=0; index<KEY_ID_MAX-1; index++) {
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

/*--------------------------------------------------------------*/
/* FUNC : GET current key status                                */
/* PARA :                                                       */
/*   keys : key id                                              */
/* RETN :                                                       */
/*   if equal active level, return KEY_ST_HIGH                  */
/*   if opposite active level, return KEY_ST_LOW                */
/*--------------------------------------------------------------*/
U08ET getkey_curst(U08ET keys)
{
	
    if (keys == KEY_ID_INFRA && 1 != glb_ctrl.infra)
    	return KEY_ST_LOW;
	
	
    if( et_keys[keys-1].curst == et_keys[keys-1].actlv ) {
        return KEY_ST_HIGH;
    }
    else {
        return KEY_ST_LOW;
    }
}

/*--------------------------------------------------------------*/
/* FUNC : GET stable key status                                 */
/* PARA :                                                       */
/*   keys : key id                                              */
/* RETN :                                                       */
/*   if equal active level, return KEY_ST_HIGH                  */
/*   if opposite active level, return KEY_ST_LOW                */
/*   if no stable, return KEY_ST_MAX                            */
/*--------------------------------------------------------------*/
U08ET getkey_stabe(U08ET keys)
{
    if (keys == KEY_ID_INFRA && 1 != glb_ctrl.infra)
    	return KEY_ST_LOW;
	
    if( et_keys[keys-1].stabe == et_keys[keys-1].actlv ) {
	if(keys != KEY_ID_INFRA)
        	et_keys[keys-1].stabe = KEY_ST_MAX;		
        return KEY_ST_HIGH;
    }
    else if ( et_keys[keys-1].stabe == KEY_ST_MAX ) {
        return KEY_ST_MAX;
    }
    else {
        et_keys[keys-1].stabe = KEY_ST_MAX;
        return KEY_ST_LOW;
    }
}
