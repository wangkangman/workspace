#include"slave_command.h"


static u8 interface(u8 *psend, u8 *pval);
static u8 slave_button_check(u32 *pval);
static u8 slave_current_check(float *pval);
static u8 slave_signal_check(SIGNAL_TYPE signal, u32 *pstate);
static u8 slave_seg_control(u32 num, u32 piont);
static u8 slave_motor_control(u32 relay_direction);
static u8 slave_pwm_control(u32 pwm_val);
static u8 slave_rf_get(u32 *prturn_res);

SLAVE_COMMAND slave = {
  slave_button_check,
  slave_current_check,
  slave_signal_check,
  slave_seg_control,
  slave_motor_control,
  slave_pwm_control,
  slave_rf_get
};
//uart1_get memin_malloc memin_free


static u8  slave_button_check(u32 *pval){
  u32 retry = 5;
  u8 pres[30];

  while (retry > 0){
    if (0 == interface((unsigned char*)"usmart_getkeyad()", pres) > 0)
      break;
    retry--;
  }
  if (0 == retry) return 9;


  *pval = atoi((char*)pres);
  return 0;
}
static u8 slave_current_check(float *pval){
  u32 retry = 5;
  u8 pres[30];
  float f_res;

  while (retry > 0){
    if (0 == interface((unsigned char*)"usmart_getcurrent()", pres) > 0)
      break;
    retry--;
  }

  if (0 == retry) return 9;

  f_res = atof((char*)pres);
  f_res *= 5;
  f_res /= 255;
//  f_res /= 0.05;
	f_res *= 20;
  f_res /= 16;  
  f_res *= 1000;

  *pval = f_res;
  return 0;
}
static u8 slave_signal_check(SIGNAL_TYPE signal, u32 *pstate){
  u32 retry = 5;
  u8 psend[30];
  u8 pres[30];

//  sprintf(psend, )
  switch(signal){
    case INFRA:
      sprintf((char*)psend, "usmart_infra_sta()");
      break;
    case WALL:
      sprintf((char*)psend, "usmart_wall_sta()");
      break;
    case DOOR:
      sprintf((char*)psend, "usmart_door_sta()");
      break;
    case POWER:
      sprintf((char*)psend, "usmart_power_sta()");
      break;
    case MAIN_HALL:
      sprintf((char*)psend, "usmart_main_hall_sta()");
      break;
    case REF_HALL:
      sprintf((char*)psend, "usmart_ref_hall_sta()");
      break;

    default:
      return 1;
  }

  while (retry > 0){
    if (0 == interface((unsigned char*)psend, pres) > 0)
      break;
    retry--;
  }
  if (0 == retry) return 9;

  *pstate = atoi((char*)pres);
  return 0;
}
static u8 slave_seg_control(u32 num, u32 piont){
  u32 retry = 5;
  u8 psend[30];
  u8 pres[30];

  sprintf((char*)psend, "usmart_segctrl(%d,%d)", num, piont);
  while (retry > 0){
    if (0 == interface((unsigned char*)psend, pres) > 0)
      break;
    retry--;
  }
  if (0 == retry) return 9;
  return 0;
}


static u8 slave_motor_control(u32 relay_direction){
  u32 retry = 5;
  u8 psend[30];
  u8 pres[30];

  sprintf((char*)psend, "usmart_motorctrl(%d)", relay_direction);
  while (retry > 0){
    if (0 == interface((unsigned char*)psend, pres) > 0)
      break;
    retry--;
  }
  if (0 == retry) return 9;
  return 0;
}


static u8 slave_pwm_control(u32 pwm_val){
  u32 retry = 5;
  u8 psend[30];
  u8 pres[30];

  sprintf((char*)psend, "usmart_pwmsctrl(%d)", pwm_val);
  while (retry > 0){
    if (0 == interface((unsigned char*)psend, pres) > 0)
      break;
    retry--;
  }
  if (0 == retry) return 9;
  return 0;
}



static u8  interface_rf(u8 *psend, u8 *pval){
  u32 retry;
  u8  *prec, *temp_psend, *temp_prec;
  prec = memin_malloc(100); 

  uart1_cleart();
  printf("%s\r\n", psend);
  retry = 40;
  while(retry > 0){
    if(uart1_get(prec)) break;
    retry--;
    delay_ms(1);
  }

  while (*prec != '\0') {
    *pval = *prec;
    pval++;
    prec++;
  }

 
  *pval = '\0';
  memin_free(prec);
  return 0;
}

static u8  interface(u8 *psend, u8 *pval){
  u32 retry;
  u8  *prec, *temp_psend, *temp_prec;
  prec = memin_malloc(100); 

  uart1_cleart();
  printf("%s\r\n", psend);
  retry = 40;
  while(retry > 0){
    if(uart1_get(prec)) break;
    retry--;
    delay_ms(1);
  }

  if (0 == retry) {
    memin_free(prec);
    return 1;
  }

  temp_psend = psend;
  temp_prec = prec;
  while('\0' != *temp_psend) {
    if (*temp_psend++ != *temp_prec++){
      memin_free(prec);
      return 2;
    }
  }

  temp_prec++;
  while(*temp_prec != ';' 
        && *temp_prec != '\0') {
    *pval++ = *temp_prec++;
  }

  *pval = '\0';
  memin_free(prec);
  return 0;
}

static u8 find_str_in_buf(u8* str, u8 *buf){
  u8* str_temp, buf_temp;

  while (*buf != '\0') {
    str_temp = str;
    while (*str_temp == *buf) {
      str_temp++; buf++;
      if (*(str_temp)== '\0')
        return 1;
    }
    buf++;
  }

  return 0;
}


static u8 slave_rf_get(u32 *prturn_res){
  u32 retry = 5;
  u8 psend[30];
  u8 pres[30];

  sprintf((char*)psend, "usmart_remotekey()");
  while (retry > 0){
    if (0 == interface_rf((unsigned char*)psend, pres) > 0)
      break;
    retry--;
  }
  if (0 == retry) return 9;

  if (find_str_in_buf("ture", pres)) *prturn_res = 1;
  else *prturn_res = 0;
	return 0;
}






