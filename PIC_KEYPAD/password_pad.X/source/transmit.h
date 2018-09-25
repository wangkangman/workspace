/* 
 * File:   transmit.h
 * Author: Gilgamesh
 *
 * Created on May 21, 2018, 6:25 PM
 */

#ifndef TRANSMIT_H
#define	TRANSMIT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "type.h"

#define SEND_TIMES (6)
    
typedef enum{
    STANDBY = 0,
    TP,
    TH,
    DATA,
    TG
}nTRANSMIT_STATUS;
    

typedef struct{    
    nTRANSMIT_STATUS cur_st;
    u8 data[8];
    u8 data_bits[64];
    u8 pulse_index;
    u8 bit_index;
    u8 repeat;
    u8 times;
}ST_TRANSMIT;




void transmit_irq_task(void);
void transmit_init(void);
void transmit(u8* data, u8 times);

#ifdef	__cplusplus
}
#endif

#endif	/* TRANSMIT_H */

