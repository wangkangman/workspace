/*===================================================================================================================
 * This file selects c file that demos Timer 0 and 1 in different modes. Please select in the configuration wizard. 
 *===================================================================================================================

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//
// <o0.0..1> Timer 0 & 1 Mode Demo Select
//     <1=> Timer 0 & 1 Mode 0 & 1 (activate "Timer01_M0_M1.c")
//     <2=> Timer 0 & 1 Mode 2 (activate "Timer01_M2.c")
//     <3=> Timer 0 Mode 3 (activate "Timer0_M3.c")
//
//-------- <<< end of configuration section >>> ------------------------------
*/

/****please define DEMO by configuration wizard before using this demo code****/
//#define DEMO    1       


#ifndef __MAIN_H__
#define __MAIN_H__

#define INFRA_SENSITIVITY   (2) 
#define INFRA_OVERFLOW      (INFRA_SENSITIVITY * 30) //TIMER 200us, pulse cycle 6ms






typedef struct{
  UINT16 cnt;
  UINT16 pulse_cnt;
	UINT8  flag;
	UINT8  cur_sta;
	UINT8  lst_sta;
	
}INFRARED;








#endif