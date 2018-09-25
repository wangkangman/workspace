#include "BuzLamp.h"
#include "regdef.h"

static BUZ_CTR et_buz;
static Lamp_CTR et_Lamp;
/**
  * @brief  Inital the val of Buz and Lamp task.
  * @param  None
  * @retval None
  *
  */
void BuzLamp_init(void) {
    /** Buz port inital ***/
    PORT_PMC1 &= ~(1<<0);
    PORT_PD1  |=  (1<<0);
    PORT_PM1  &= ~(1<<0);
    
    /** Lamp port inital **/
    PORT_ADPC  =  (0x2U);
    PORT_PD2  &= ~(1<<3);
    PORT_PM2  &= ~(1<<3);
    
    /** BUZ ctrl_st inital **/
    et_buz.times  = 0;
    et_buz.count  = 0;
    et_buz.period = 0;
    
    et_Lamp.KeepTimes = 0;
}


/**
  * @brief  sys_task of Buz and Lamp.
  * @param  None
  * @retval None
  *
  */
void BuzLamp_task(void) {
	if (et_buz.count > 0) {
		if ( 0 == et_buz.times%2)
			PORT_PD1 &= ~0x1U;
		else
			PORT_PD1 |=  0x1U;
		--et_buz.count;
	}
	else if (et_buz.count == 0) {
		if (et_buz.times > 0){
			--et_buz.times;
			et_buz.count = et_buz.period;
		}
		else
			PORT_PD1 |=  0x1U;
	}
	
	
	if (et_Lamp.KeepTimes > 0){		
		et_Lamp.KeepTimes--;
		PORT_PD2 |=  (0x1 << 3);
	} 
	else {
		PORT_PD2 &= ~(0x1 << 3);	
	}
}
/*********************************************************************
   FUNC : BUZ control
   PARA :
     period : mantain time per period 
     time   : times of ringing
   RETN : 
     NONE
**********************************************************************
**/
void BUZ_ctrl(U16ET period, U16ET times) {
	if (0 == period ){
		PORT_PD1 |= 0x1U;	
	} 
	et_buz.period = period;
	et_buz.count = period;
	if(times > 0)
		et_buz.times = times*2 - 2;
	else
		et_buz.times = 0;	
}


/**
  * @brief  Lamp task control.
  * @param  
  *	time : How long the Lamp will keep light (min*60*500).
  * @retval None
  *
  */
void Lamp_ctrl(U32ET time) {
     PORT_PD2 |=  (0x1 << 3);
     et_Lamp.KeepTimes = time;
}

/*void Lamp_close(void) {
	PORT_PD2 &= ~(0x1 << 3);
}*/