#ifndef __BUZLAMP_H
#define __BUZLAMP_H
#include "global.h"

#define BUZ_MOV (300)

#define BUZ_SET_SUCCESS_TIMES  (1)
#define BUZ_SET_SUCCESS_PERIOD (300)

#define BUZ_SET_OVERTIME_TIMES  (3)
#define BUZ_SET_OVERTIME_PERIOD (50)

#define LAMP_OPEN_KEEP_TIME (90000) //500*60*MIN<3>

#define EXCUTE_AB_TIMES  (3)
#define EXCUTE_AB_PERIOD (200)

typedef struct BUZ_CTR {
	U16ET  times;
	U16ET count;
	U16ET period;
}BUZ_CTR;

typedef struct Lamp_CTR {
	U32ET  KeepTimes;
}Lamp_CTR;

void BuzLamp_init(void) ;
void BuzLamp_task(void);
void BUZ_ctrl(U16ET period, U16ET times);



void Lamp_ctrl(U32ET time);
//void Lamp_close(void);

#endif