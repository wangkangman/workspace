#include "power.h"

#if 0
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

void power_init(void){

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化

  GPIO_ResetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
  GPIO_ResetBits(GPIOG, GPIO_Pin_6);
}


void pwr_control(u8 mode){
  if (mode)  GPIO_SetBits(GPIOB, GPIO_Pin_12);
  else GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void main_or_bat_set(u8 mode){
  if (mode)  GPIO_SetBits(GPIOB, GPIO_Pin_10);
  else GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

void ac_chanel_set(u8 mode){
  if (mode)  GPIO_SetBits(GPIOB, GPIO_Pin_11);
  else GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}

void lowpower_set(u8 mode){
  if (mode)  GPIO_SetBits(GPIOG, GPIO_Pin_6);
  else GPIO_ResetBits(GPIOG, GPIO_Pin_6);
}
#endif
