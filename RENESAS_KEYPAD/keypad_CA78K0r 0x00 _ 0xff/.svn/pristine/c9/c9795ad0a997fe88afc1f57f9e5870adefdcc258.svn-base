#include "LowPowerConsum.h"
#include "regdef.h"




void LowPowInit(void){	
	PORT_PD2  &= ~(1<<0);
    	PORT_PM2  &= ~(1<<0);
	
	PORT_PD2  |=  (1<<2);
	PORT_PM2  &= ~(1<<2);
}


void CutPower(void){
	PORT_PD2 |=  (1 << 0);
}


