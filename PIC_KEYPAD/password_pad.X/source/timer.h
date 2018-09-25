/* 
 * File:   timer3.h
 * Author: Gilgamesh
 *
 * Created on May 21, 2018, 10:48 AM
 */

#ifndef TIMER3_H
#define	TIMER3_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "pic.h"
    
/* timer 1 2us */
#define timer_init() do{\
        T1CON = 0x30;\
        TMR1 = 0xff38;\
        \
        TMR1IF = 0;\
        TMR1IE = 1;\
        GIE = 1;\
        PEIE = 1;\
        \
        T1CONbits.TMR1ON  = 1;\
       }while(0)
    


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER3_H */

