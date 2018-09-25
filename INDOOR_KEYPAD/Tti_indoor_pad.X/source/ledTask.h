/* 
 * File:   ledTask.h
 * Author: Administrator
 *
 * Created on November 15, 2017, 3:12 PM
 */

#ifndef LEDTASK_H
#define	LEDTASK_H
#include "type.h"

#define LED_OFF (0)
#define LED_ON  (1)


typedef struct {
    U08ET stats;      /* current status */
    U16ET period;     /* led control period */
    U16ET count;      /* led period counter */
    U08ET times;      /* led times counter */
    U08ET times_cnt;      /* led times counter */
}ST_LED;



void led_init(void);
void led_task(void);
void led_ctrl(U16ET period, U08ET times);
#endif	/* LEDTASK_H */

