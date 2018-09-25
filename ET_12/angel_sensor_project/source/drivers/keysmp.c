#include "regdef.h"
#include "keysmp.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_KEYS et_keys[KEY_ID_MAX];

/* 2017/6/17 星期六 上午 8:16:03 -sensor skew counter-------------*/
#if LIMIT_SEN_ERR
static U08ET limit_sensor_skew_counter;        /* detect if the limit sensor is broken */
#endif
/* 2017/6/17 星期六 上午 8:16:03 -sensor skew counter-------------*/

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

    /* KEY_SET  <--> PC3 */
    GPIO_PC_DDR &= ~(1<<3);  /* direction input  */
    GPIO_PC_CR1 &= ~(1<<3);  /* float input      */
    GPIO_PC_CR2 &= ~(1<<3);  /* max speed 2MHz   */

    /* KEY_CODE  <--> PC4 */
    GPIO_PC_DDR &= ~(1<<4);  /* direction input  */
    GPIO_PC_CR1 &= ~(1<<4);  /* float input      */
    GPIO_PC_CR2 &= ~(1<<4);  /* max speed 2MHz   */

    /* KEY_IRAD  <--> PA2 */
    GPIO_PA_DDR &= ~(1<<2);  /* direction input  */
    GPIO_PA_CR1 &= ~(1<<2);  /* float input      */
    GPIO_PA_CR2 &= ~(1<<2);  /* max speed 2MHz   */

    /* KEY_WALL  <--> PE5 */
    GPIO_PE_DDR &= ~(1<<5);  /* direction input  */
    GPIO_PE_CR1 &= ~(1<<5);  /* float input      */
    GPIO_PE_CR2 &= ~(1<<5);  /* max speed 2MHz   */

    /* LIMIT0  <--> PD0 */
    GPIO_PD_DDR &= ~(1<<0);  /* direction input  */
    GPIO_PD_CR1 &= ~(1<<0);  /* float input      */
    GPIO_PD_CR2 &= ~(1<<0);  /* max speed 2MHz   */

    /* LIMIT1  <--> PC7 */
    GPIO_PC_DDR &= ~(1<<7);  /* direction input  */
    GPIO_PC_CR1 &= ~(1<<7);  /* float input      */
    GPIO_PC_CR2 &= ~(1<<7);  /* max speed 2MHz   */

    /* LR  <--> PC5 */
    GPIO_PC_DDR &= ~(1<<5);  /* direction input  */
    GPIO_PC_CR1 &= ~(1<<5);  /* float input      */
    GPIO_PC_CR2 &= ~(1<<5);  /* max speed 2MHz   */
    
    /* 2017/6/16 星期五 上午 8:47:43  for driver level------------*/
    /* DRIVER1 <--> PB4  */
    GPIO_PB_DDR &= ~(1<<4);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<4);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<4);  /* max speed 2MHz   */    
    
    /* DRIVER2 <--> PB3  */
    GPIO_PB_DDR &= ~(1<<3);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<3);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<3);  /* max speed 2MHz   */    
        
    /* DRIVER3 <--> PB2  */
    GPIO_PB_DDR &= ~(1<<2);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<2);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<2);  /* max speed 2MHz   */    
        
    /* DRIVER4 _ FOR IRAD SWITCH <--> PF4  */    
    GPIO_PF_DDR &= ~(1<<4);  /* direction input  */
    GPIO_PF_CR1 &= ~(1<<4);  /* float input      */
    GPIO_PF_CR2 &= ~(1<<4);  /* max speed 2MHz   */
    /*2017/6/16 星期五 上午 8:47:43 for driver level-------------*/
    
    /* 2017/6/16 星期五 上午 11:56:55 OC_detect <--> PD6---------*/ 
    GPIO_PD_DDR &= ~(1<<6);  /* direction input  */
    GPIO_PD_CR1 &= ~(1<<6);  /* float input      */
    GPIO_PD_CR2 &= ~(1<<6);  /* max speed 2MHz   */
    /* 2017/6/16 星期五 上午 11:56:55 OC_detect <--> PD6---------*/
    
/* 2017/7/15 星期六 上午 11:02:49 AUTO_close <-->PC1-------------*/
    GPIO_PC_DDR &= ~(1<<1);  /* direction input  */
    GPIO_PC_CR1 &= ~(1<<1);  /* float input      */
    GPIO_PC_CR2 &= ~(1<<1);  /* max speed 2MHz   */
/* 2017/7/15 星期六 上午 11:02:49 AUTO_close <-->PC1-------------*/

    /* initialize key control structure */
    for (index = 0; index < KEY_ID_MAX; index++)
    {
        et_keys[index].lstst = KEY_ST_MAX;
        et_keys[index].curst = KEY_ST_MAX;
        et_keys[index].stabe = KEY_ST_MAX;
        et_keys[index].cycle = 20;
        et_keys[index].count = 0;
    }
    et_keys[KEY_ID_SET].actlv  = KEY_ST_LOW;
    et_keys[KEY_ID_CODE].actlv = KEY_ST_LOW;
    et_keys[KEY_ID_IRAD].actlv = KEY_ST_LOW;
    et_keys[KEY_ID_WALL].actlv = KEY_ST_LOW;
    et_keys[KEY_ID_LMT0].actlv = KEY_ST_HIGH;
    et_keys[KEY_ID_LMT1].actlv = KEY_ST_HIGH;
    et_keys[KEY_ID_LR].actlv   = KEY_ST_LOW;
    et_keys[KEY_ID_OC].actlv   = KEY_ST_HIGH;
    et_keys[KEY_ID_IRAD_SWITCH].actlv = KEY_ST_LOW;
    et_keys[KEY_ID_AUTO_CLOSE].actlv = KEY_ST_LOW;
    et_keys[KEY_ID_LMT0].cycle = 5;
    et_keys[KEY_ID_LMT1].cycle = 5;
/* 2017/6/17 星期六 上午 8:48:58-sensor skew counter-----------*/
#if LIMIT_SEN_ERR
    limit_sensor_skew_counter = 0;        /* detect if the limit sensor is broken */
#endif
/* 2017/6/17 星期六 上午 8:48:58-sensor skew counter-----------*/
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

    et_keys[KEY_ID_SET].curst  = (GPIO_PC_IDR & 0x08) ? 1:0;
    et_keys[KEY_ID_CODE].curst = (GPIO_PC_IDR & 0x10) ? 1:0;
    et_keys[KEY_ID_IRAD].curst = (GPIO_PA_IDR & 0x04) ? 1:0;
    et_keys[KEY_ID_WALL].curst = (GPIO_PE_IDR & 0x20) ? 1:0;
    et_keys[KEY_ID_LMT0].curst = (GPIO_PD_IDR & 0x01) ? 1:0;
    et_keys[KEY_ID_LMT1].curst = (GPIO_PC_IDR & 0x80) ? 1:0;
    et_keys[KEY_ID_LR].curst   = (GPIO_PC_IDR & 0x20) ? 1:0;
    et_keys[KEY_ID_OC].curst   = (GPIO_PD_IDR & 0x40) ? 1:0;
    et_keys[KEY_ID_IRAD_SWITCH].curst   = (GPIO_PF_IDR & 0x10) ? 1:0;
		et_keys[KEY_ID_AUTO_CLOSE].curst    = (GPIO_PC_IDR & 0x02) ? 1:0;


    for (index=0; index<KEY_ID_MAX; index++)
    {
        if (et_keys[index].curst != et_keys[index].lstst) 
        {
          et_keys[index].count  = 0;
          et_keys[index].stabe  = KEY_ST_MAX;
        }
        else 
        {
            if (et_keys[index].count < et_keys[index].cycle) 
            {
                et_keys[index].count++;
            }
            else if (et_keys[index].count == et_keys[index].cycle) 
            {
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
   // if (et_keys[KEY_ID_LR].curst == et_keys[KEY_ID_LR].actlv) 
   if (KEY_ST_HIGH == glb_ctrl.LorR_Set) {
        switch (keys) {
            case KEY_ID_LMT0:
                keys = KEY_ID_LMT1;
                break;
            case KEY_ID_LMT1:
                keys = KEY_ID_LMT0;
                break;
            default:
                break;
        }
    }

    if (et_keys[keys].curst == et_keys[keys].actlv) {
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
 // if (et_keys[KEY_ID_LR].curst == et_keys[KEY_ID_LR].actlv)
    if (KEY_ST_HIGH == glb_ctrl.LorR_Set) {
        switch (keys) {
            case KEY_ID_LMT0:
                keys = KEY_ID_LMT1;
                break;
            case KEY_ID_LMT1:
                keys = KEY_ID_LMT0;
                break;
            default:
                break;
        }
    }    

    if (et_keys[keys].stabe == et_keys[keys].actlv) {
    		if (keys != KEY_ID_LR && \
    			  keys != KEY_ID_IRAD && \
    			  keys != KEY_ID_LMT0 && \
    			  keys != KEY_ID_LMT1 && \
    			  keys != KEY_ID_AUTO_CLOSE){
        	et_keys[keys].stabe = KEY_ST_MAX;
        }
        return KEY_ST_HIGH;
    }
    else if (et_keys[keys].stabe == KEY_ST_MAX) {
        return KEY_ST_MAX;
    }
    else {
    		if (keys != KEY_ID_LR && \
    			  keys != KEY_ID_IRAD && \
    			  keys != KEY_ID_LMT0 && \
    			  keys != KEY_ID_LMT1 && \
    			  keys != KEY_ID_AUTO_CLOSE){
        	et_keys[keys].stabe = KEY_ST_MAX;
        }    	
//        et_keys[keys].stabe = KEY_ST_MAX;
        return KEY_ST_LOW;
    }
}

/* -------2017/6/16 星期五 上午 8:48:12 GET driver level ----------*/
/*-----------------------------------------------------------------*/
/* FUNC : GET driver level                                         */
/* PARA :                                                          */
/*   RETN :  level of  torque                                      */
/*   4 switch : 0-->15                                             */
/*   MSB driver_1 --> driver_4 LSB                                 */
/*   low is logic_1                                                */
/*-----------------------------------------------------------------*/
U08ET getdriver_level(VODET)
{
    U08ET level = 0x0;

    level |= (GPIO_PB_IDR & 0x10) ? 0 : (1<<0);
    level |= (GPIO_PB_IDR & 0x08) ? 0 : (1<<1);
    level |= (GPIO_PB_IDR & 0x04) ? 0 : (1<<2);

    return level;
}
/* -------2017/6/16 星期五 上午 8:48:12 GET driver level ----------*/

/* 2017/6/16 星期五 下午 2:42:41   OC_detect-----------------------*/
/*-----------------------------------------------------------------*/
/* FUNC : over current detect                                      */
/* PARA :                                                          */
/*   RETN :  KEY_ST_HIGH or  KEY_ST_LOW                            */
/*   KEY_ST_HIGH is active                                         */
/*-----------------------------------------------------------------*/
U08ET oc_detect(VODET)
{
   return (et_keys[KEY_ID_OC].stabe) ? KEY_ST_HIGH:KEY_ST_LOW;
}
/* 2017/6/16 星期五 下午 2:42:41   OC_detect-----------------------*/



/* 2017/6/17 星期六 上午 8:42:28-limit sensor skew detect----------*/
#if LIMIT_SEN_ERR
/*-----------------------------------------------------------------*/
/* FUNC : limit_sensor_skew_counter += 2                           */
/* PARA :                                                          */
/* RETN :                                                          */
/*-----------------------------------------------------------------*/
VODET limit_sen_cnt_plus(VODET)
{
   limit_sensor_skew_counter += 2;
}
/*-----------------------------------------------------------------*/
/* FUNC : limit_sensor_skew_counter -= 1                           */
/* PARA :                                                          */
/* RETN :                                                          */
/*-----------------------------------------------------------------*/
VODET limit_sen_cnt_min(VODET)
{
  if (limit_sensor_skew_counter > 0)
     limit_sensor_skew_counter--;
}
/*-----------------------------------------------------------------*/
/* FUNC : limit sensor error detect                                */
/* PARA :                                                          */
/*   RETN : KEY_ST_HIGH or KEY_ST_LOW                              */
/*   KEY_ST_HIGH is active                                         */
/*-----------------------------------------------------------------*/
U08ET limit_sen_err_check(VODET)
{
  return (limit_sensor_skew_counter > LIMIT_SEN_ERR_NUM) ? KEY_ST_HIGH : KEY_ST_LOW;
}
#endif
/* 2017/6/17 星期六 上午 8:42:28-limit sensor skew detect----------*/