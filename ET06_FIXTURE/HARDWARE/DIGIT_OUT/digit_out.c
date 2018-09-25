#include "digit_out.h"
#include "iic.h"
#include "delay.h"

static u16 current_register;

static void digit_output_init(void);
static void digit_wall_drive(u8 state);
static void digit_infra_drive(u8 state);
static void digit_door_drive(u8 state);
static void digit_hall_main_drive(u8 state);
static void digit_hall_ref_drive(u8 state);
static void digit_ref_vol_drive(u8 state);
static void digit_pwm_drive(u8 state);
static void digit_rf_drive(u8 state);

static void pca9555_write_r2(u8 index, u8 state);

DIGIT_API digit = {
    digit_output_init,   digit_wall_drive,      digit_infra_drive,
    digit_door_drive,    digit_hall_main_drive, digit_hall_ref_drive,
    digit_ref_vol_drive, digit_pwm_drive,       digit_rf_drive};

static void power_init(void);
static void pwr_control(u8 mode);
static void main_or_bat_set(u8 mode);
static void ac_chanel_set(u8 mode);
static void lowpower_set(u8 mode);

POWER_API power = {
  power_init,
  pwr_control,
  main_or_bat_set,
  ac_chanel_set,
  lowpower_set
};



void pca9555_write_r2(u8 index, u8 state){  
  if (0 == state)
    current_register &= ~(1 << index);
  else if (1 == state)
    current_register |= 1 << index;

  IIC_Start();
  IIC_Send_Byte(0x42);
  IIC_Wait_Ack();
  IIC_Send_Byte(0x02);
  IIC_Wait_Ack();
  IIC_Send_Byte((current_register & 0xff));
  IIC_Wait_Ack();
  IIC_Send_Byte((current_register >> 8));
  IIC_Wait_Ack();
  IIC_Stop();
}

static void digit_output_init(void) {
  IIC_Init();

  current_register = 0x0;
  IIC_Start();
  IIC_Send_Byte(0x42);
  IIC_Wait_Ack();
  IIC_Send_Byte(0x02);
  IIC_Wait_Ack();
  IIC_Send_Byte((current_register & 0xff));
  IIC_Wait_Ack();
  IIC_Send_Byte((current_register >> 8));
  IIC_Wait_Ack();
  IIC_Stop();

  delay_ms(10);
  IIC_Start();
  IIC_Send_Byte(0x42);
  IIC_Wait_Ack();
  IIC_Send_Byte(0x06);
  IIC_Wait_Ack();
  IIC_Send_Byte(0x00);
  IIC_Wait_Ack();
  IIC_Send_Byte(0x00);
  IIC_Wait_Ack();
  IIC_Stop();
}


static void digit_wall_drive(u8 state){
  pca9555_write_r2(3, state);
}

static void digit_infra_drive(u8 state){
  pca9555_write_r2(2, state); //4
}


static void digit_door_drive(u8 state){
  pca9555_write_r2(5, state);
}

#if 1
static void digit_hall_main_drive(u8 state){
  pca9555_write_r2(7, state);
}


static void digit_hall_ref_drive(u8 state){
  pca9555_write_r2(6, state);
}
#endif




void power_init(void){

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

  //GPIOG6初始化设置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化

  GPIO_ResetBits(GPIOG, GPIO_Pin_6);
}


void pwr_control(u8 mode){
  pca9555_write_r2(13, mode);
}

void main_or_bat_set(u8 mode){
  pca9555_write_r2(14, mode);
}

void ac_chanel_set(u8 mode){
  pca9555_write_r2(15, mode);
}

void lowpower_set(u8 mode){
  if (mode)  GPIO_SetBits(GPIOG, GPIO_Pin_6);
  else GPIO_ResetBits(GPIOG, GPIO_Pin_6);
}


static void digit_ref_vol_drive(u8 state){
  pca9555_write_r2(0, state);
}


static void digit_pwm_drive(u8 state){  
  pca9555_write_r2(12, state);
}



static void digit_rf_drive(u8 state){
  pca9555_write_r2(1, state);
}