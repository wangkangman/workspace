/* 
 * File:   timer.h
 * Author: Kimi
 *
 * Created on November 10, 2017, 8:16 AM
 */

#ifndef TIMER_H
#define	TIMER_H
#include"type.h"

void interrupt ISR(void);
void init_timer0(void);
U08ET sys_task();
U08ET get_ack(void);
U08ET clear_ack(void);
U08ET get_ack_data(void);

#endif	/* TIMER_H */

