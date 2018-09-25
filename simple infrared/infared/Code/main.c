/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 Timer0/1 Mode0 demo code
//***********************************************************************************************************
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
/* app typedef */
#include "main.h"

//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
#if 0
//#define		TIMER1_MODE0_ENABLE		TMOD&=0x0F
//#define		TIMER1_MODE1_ENABLE		TMOD&=0x0F;TMOD|=0x10
//#define		TIMER1_MODE2_ENABLE		TMOD&=0x0F;TMOD|=0x20
//#define		TIMER1_MODE3_ENABLE		TMOD&=0x0F;TMOD|=0x3F

//#define		TIMER0_MODE0_ENABLE		TMOD&=0xF0
//#define		TIMER0_MODE1_ENABLE		TMOD&=0xF0;TMOD|=0x01
//#define		TIMER0_MODE2_ENABLE		TMOD&=0xF0;TMOD|=0x02
//#define		TIMER0_MODE3_ENABLE		TMOD&=0xF0;TMOD|=0xF3
#endif
#if 0
#define TH0_INIT        0xFC //5.0ms@XTAL=12MHz, Period = (10.85/2) ms@XTAL=22.1184MHz
#define TL0_INIT        0x0F // 64527
#endif


#define TH0_INIT        0xF3 //5.0ms@XTAL=12MHz, Period = (10.85/2) ms@XTAL=22.1184MHz
#define TL0_INIT        0x37

#define TH1_INIT        0xE0 //2.5ms@XTAL=12MHz, Period = (5.425/2) ms@XTAL=22.1184MHz
#define TL1_INIT        0x00


static INFRARED infra;

/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/
void Timer0_ISR (void) interrupt 1          //interrupt address is 0x000B
{
    TH0 = TH0_INIT;
    TL0 = TL0_INIT;
	
	  infra.lst_sta = infra.cur_sta;
	  infra.cur_sta = P17;
	  
	
	  if (infra.lst_sta == infra.cur_sta) {
      if (infra.cnt < INFRA_OVERFLOW){
        infra.cnt++;
      }
			else {
				infra.flag = 1;
				infra.pulse_cnt = 0;
      }
    } 
		/* get pulse */ 
		else {
      infra.cnt = 0;
			
			/* filter */
			if (infra.pulse_cnt < 4) infra.pulse_cnt++;
			
			if (infra.pulse_cnt == 4) 
			  infra.flag = 0;
    }// GPIO toggle when interrupt
}
/************************************************************************************************************
*    TIMER 1 interrupt subroutine
************************************************************************************************************/
void Timer1_ISR (void) interrupt 3          //interrupt address is 0x001B
{
    TH1 = TH1_INIT;
    TL1 = TL1_INIT;   
    P03 = ~P03;															// GPIO toggle when interrupt
}

void main (void)
{
	TMOD = 0XFF;
	Set_All_GPIO_Quasi_Mode;
	P17_Input_Mode;
	P12_PushPull_Mode;
	P05_PushPull_Mode;
	
	/* infra init */
	infra.cnt     = 0;
	infra.flag    = 0;
	infra.cur_sta = 1;
	infra.lst_sta = 1;
	infra.pulse_cnt = 0;
	
	/* 200us timer init */
	TIMER0_MODE1_ENABLE;                        //Timer 0 and Timer 1 mode configuration	
	set_T0M;
	TH0 = TH0_INIT;
	TL0 = TL0_INIT;    
	set_ET0;                                    //enable Timer0 interrupt
	set_EA;                                     //enable interrupts	
	set_TR0;                                    //Timer0 run

//	P05 = 0;
  while(1)
	{
    if (infra.flag == 1) {
      P12 = 1; //relay
      P05 = 1; //led
    }		
		else {
      P12 = 0;
      P05 = 0;			
    }		
  }
}
/************************************************************************************************************
*    Main function 
************************************************************************************************************/
#if 0
void main (void)
{
	TMOD = 0XFF;
	Set_All_GPIO_Quasi_Mode;
	TIMER0_MODE0_ENABLE;                        //Timer 0 and Timer 1 mode configuration
	TIMER1_MODE0_ENABLE;
    
	clr_T0M;
	clr_T1M;
    
	TH0 = TH0_INIT;
	TL0 = TL0_INIT;
	TH1 = TH1_INIT;
	TL1 = TL1_INIT;
    
//	set_ET0;                                    //enable Timer0 interrupt
	set_ET1;                                    //enable Timer1 interrupt
	set_EA;                                     //enable interrupts
	
	set_TR0;                                    //Timer0 run
//	set_TR1;                                    //Timer1 run

  while(1)
	{
		TH0 = TH0_INIT;
		TL0 = TL0_INIT;
		set_TR0; 
		while(!TF0);
		clr_TR0;
		P12 = ~P12;	
		TF0 = 0 ;
}
}
#endif