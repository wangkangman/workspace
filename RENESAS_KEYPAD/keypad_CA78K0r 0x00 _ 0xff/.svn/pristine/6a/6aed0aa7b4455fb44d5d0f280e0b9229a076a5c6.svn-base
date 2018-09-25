#include "BuzTask.h"
#include "regdef.h"

static BUZ_CTR et_buz;

void BuzInit(void) {
	PORT_PD2 |=   (1<<3);
	PORT_PM2 &=  ~(1<<3);
	
	et_buz.times = 0;
	et_buz.count = 0;
	et_buz.period = 0;
}


void BUZ_task(void) {
	
	if (et_buz.count > 0) {
		if ( 0 == et_buz.times%2)
			PORT_PD2 &= ~(1<<3);
		else
			PORT_PD2 |=  (1<<3);
		--et_buz.count;
	} else if (et_buz.count == 0) {
		if (et_buz.times > 0){
			--et_buz.times;
			et_buz.count = et_buz.period;
		} else PORT_PD2 |=  (1<<3);
	}
}




void BUZ_ctrl(u16 period, u8 times) {
	et_buz.period = period;
	et_buz.count = period;
	if(times > 0)
		et_buz.times = times*2 - 2;
}