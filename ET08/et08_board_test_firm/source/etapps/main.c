#include <stdio.h>
#include "main.h"
#include "subs.h"
#include "usmart.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static void sys_pre_init(void);
static u8 strcmp(u8 *str1, u8 *str2);

/*-----------------------------------------------------------------*/
/* FUNC : MAIN function of the project                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void main(void)
{
  sys_pre_init();
  while (1) {
    if (ET_FALSE == tskskm_flag()) {
      continue;
    }
    wdtdog_feed();
    ledseg_task();
    keysmp_task();
    adcsmp_task();
    usmart_dev.task();
  }
}


void sys_pre_init(void)
{
    sysclk_init();
    motors_init();
    remote_init();
    ledseg_init();
    adcsmp_init();
    keysmp_init();
    timwdt_init();
    usmart_dev.init();
    ET_EI();
    printf("System initialize OK\r\n");
}


u32 usmart_getcurrent(void){
		return getadc_data(ADC_CH_MOT);
}


/*u32 usmart_getkeyad(void){
	u8 adcKey;
	adcKey = getadc_data(ADC_CH_KEY);
  if ((adcKey <= KEY_K1_ADC_VAL + KEY_X_ADC_WRAP) && (adcKey >= KEY_K1_ADC_VAL - KEY_X_ADC_WRAP)) return 1;
  else if ((adcKey <= KEY_K2_ADC_VAL + KEY_X_ADC_WRAP) && (adcKey >= KEY_K2_ADC_VAL - KEY_X_ADC_WRAP)) return 2;
  else if ((adcKey <= KEY_K3_ADC_VAL + KEY_X_ADC_WRAP) && (adcKey >= KEY_K3_ADC_VAL - KEY_X_ADC_WRAP)) return 3;
  else if ((adcKey <= KEY_K4_ADC_VAL + KEY_X_ADC_WRAP) && (adcKey >= KEY_K4_ADC_VAL - KEY_X_ADC_WRAP)) return 4;
	else return 0;
}*/

u32 usmart_getkeyad(void){
  if (ET_TRUE == getkey_stabe(KEY_ID_K1)) return 1;
  if (ET_TRUE == getkey_stabe(KEY_ID_K2)) return 2;
  if (ET_TRUE == getkey_stabe(KEY_ID_K3)) return 3;
  if (ET_TRUE == getkey_stabe(KEY_ID_K4)) return 4;

  return 0;
}

u32 usmart_getkeycur(u32 channel){
	return getadc_data(ADC_CH_KEY);
}

void usmart_segctrl(u32 digit, u32 piont){
	segtsk_ctrl(1, digit, piont, 0);
}

void usmart_motorctrl(u32 dir){
	motors_ctrl(dir);
}

void usmart_pwmsctrl(u32 pwmval){
	pwmval = pwmval > 255? 255 : pwmval;
  motors_pwms(pwmval);
}

void usmart_remotekey(void){
	u8 buf[4], index;
	u16 temp;
	if (remote_gkey(buf)) {
		printf("ture=");
		for(index = 0; index < 4; index++){
			temp = buf[index];
		  printf("%02x", temp);
	  }
	}	else {
		printf("false");
	}
}

u32 usmart_infra_sta(void){
	return !getkey_curst(KEY_ID_IRAD);
}

u32 usmart_wall_sta(void){
  return getkey_curst(KEY_ID_WALL);
}

u32 usmart_door_sta(void){
  return !getkey_curst(KEY_ID_DOOR);
}

u32 usmart_power_sta(void){
  return getkey_curst(KEY_ID_POW);
}


u32 usmart_main_hall_sta(void){
  return main_hal_get();
}
u32 usmart_ref_hall_sta(void){
  return ref_hal_get();
}