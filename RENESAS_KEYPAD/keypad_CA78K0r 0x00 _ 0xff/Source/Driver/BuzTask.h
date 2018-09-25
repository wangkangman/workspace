#ifndef _BUZ_H
#define _BUZ_H
#include "global.h"


typedef struct BUZ_CTR {
	u8  times;
	u16 count;
	u16 period;
}BUZ_CTR;

void BuzInit(void);
void BUZ_task(void);
void BUZ_ctrl(u16 period, u8 times);

#endif