/* 
 * File:   power.h
 * Author: Gilgamesh
 *
 * Created on May 21, 2018, 4:59 PM
 */

#ifndef POWER_H
#define	POWER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define power_init() do{\
        TRISB &= ~(1<<7);\
        RB7 = 0;\
        }while(0)

#define power_cut() do{\
        RB7 = 1;\
        }while(0)

#ifdef	__cplusplus
}
#endif

#endif	/* POWER_H */

