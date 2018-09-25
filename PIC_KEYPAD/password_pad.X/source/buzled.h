/* 
 * File:   buzled.h
 * Author: Gilgamesh
 *
 * Created on May 22, 2018, 2:15 PM
 */

#ifndef BUZLED_H
#define	BUZLED_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#include "pic.h"
#include "type.h"
    


    
#define led_init() do{\
        TRISA &= ~(1<<0);\
        RA0 = 1;\
        }while(0)     
    
    
    typedef struct {
        u8 cur_sts;
        u8 times;
        u16 counter;
        u16 period;
    }ST_BUZ;
    

    void buz_init(void);
    void buz_task(void);
    void buz_ctrl(u16 period, u8 times);

#ifdef	__cplusplus
}
#endif

#endif	/* BUZLED_H */

