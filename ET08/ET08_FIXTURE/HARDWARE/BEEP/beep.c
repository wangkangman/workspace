#include "beep.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//蜂鸣器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#if 0
//初始化PF8为输出口		    
//BEEP IO初始化
void BEEP_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
  GPIO_ResetBits(GPIOG,GPIO_Pin_15);//GPIOG15设置低，灯灭
}
#endif

void BEEP_Init(void)
{    	 
    TIM2_PWM_Init(400 - 1,84 - 1);
    TIM_SetCompare3(TIM2,200);
    TIM_Cmd(TIM2,DISABLE);
}

void BEEP_OPEN(void){
    TIM_Cmd(TIM2,ENABLE);
}
void BEEP_CLOSE(void){
    TIM_Cmd(TIM2,DISABLE);
}




