#ifndef __APPET08_TEST_H
#define __APPET08_TEST_H
#include "common.h"


#define MOTOR_FLOW_THRESHOLD (50)
#define MOTOR_UPER_THRESHOLD (70)

typedef enum {
  TEST_INIT = 0,
  TEST_BIN_SET,
  TEST_BOARD_SET,
  TEST_STANDBY,
  TEST_POWER,
  TEST_PROGRAM,
  TEST_SIGNAL,
  TEST_SEG,
  TEST_BUTTON,
  TEST_RF,
  TEST_358_PWM,
  TEST_MOTOR,
  APP_PROGRAM,
  TEST_FINAL
} TEST_STATES;


typedef struct {
  float low;
  float upper;
}SUB_RATIO;

typedef struct {
  SUB_RATIO current;
  SUB_RATIO vot_main;
  SUB_RATIO vot_bat;
  SUB_RATIO vot_12v;
  SUB_RATIO vot_lamp;
  SUB_RATIO vot_5v;
}TEST_AD_RATIO;


typedef struct {
  TEST_AD_RATIO main_ch_p;
  TEST_AD_RATIO main_ch_n;
  TEST_AD_RATIO battery;
}TEST_RATIO;

typedef struct {
  ADCn_FLOAT main_ch_p;
  ADCn_FLOAT main_ch_n;
  ADCn_FLOAT battery;
  u8 AD_falg;

  u8 lowpower;
  u8 infra;
  u8 doorindoor;
  u8 wall_switch;
  u8 hal_main;
  u8 hal_ref;

  u8 seg;
  u8 button_1;
  u8 button_2;
  u8 button_3;
  u8 button_4;
 
  u8 rf_data[8];

  u8 motor_p;
  u8 motor_n;  
  u8 pwm;
  float current_motor;
}TEST_RESULT;

typedef enum {
  UNKNOW = 0,
  STANDARD,
  MUTIFUNC,
} PRODUCT_MODE;

typedef struct ST_BIN{
  struct ST_BIN *pre;
  u8* name;
  struct ST_BIN *next;  
} BIN_ST;

typedef struct {

  PRODUCT_MODE type;
  TEST_STATES cur_state;
  TEST_STATES lst_state;
  u8 sub_state;

//  TEST_STATES err_step;
  u8 err_code[100];
  
  TEST_RESULT result;
  TEST_RATIO  ratio;

  u8* bin_config_txt;
  u8* current_bin;
  BIN_ST* list_bin;
  u8* testbin;
  u8* appbin;

  u8* type_config_txt;
  u8* current_type;



  u8 object[50];
  u8 test_firm[50];

  u16 cnt;
} TEST_ST;





typedef void (*FUNC)(void);
void ET_08_test_init(void);
void ET_08_test_app(void);


#endif



