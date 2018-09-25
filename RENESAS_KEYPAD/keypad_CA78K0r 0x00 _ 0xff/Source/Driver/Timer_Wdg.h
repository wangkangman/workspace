#ifndef _TIMER_WDG_H
#define _TIMER_WDG_H
#include "global.h"



#define TSK_SKM_CNT  (16)

/*-----------------------------------------------------------------*/
/* Define task skeme control structure                             */
/*-----------------------------------------------------------------*/
typedef struct
{
    u8 skm_cnt;
}ETST_TASKSKM;




typedef struct AM_SEND_CTRL {
	u16 step;
	u8 times;
}AM_SEND_CTRL;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void timwdt_init(void);
void wdtdog_feed(void);
void Enable_TM02(void);
void Disable_TM02(void);
u8 Send_Detcet(void);
u8 tskskm_rdy(void);


#endif

