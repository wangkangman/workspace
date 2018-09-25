/* 
 * File:   main.h
 * Author: Gilgamesh
 *
 * Created on May 21, 2018, 9:28 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "type.h"

//#define EASY_MODE
#define NORMAL_MODE
    
#define AUTO_CLOSE_SEC (9)
    
#define AUTO_CLOSE_CNT ((unsigned short)(AUTO_CLOSE_SEC * 500))
    
   
typedef enum{
    SYS_ST_READ  = 0,
    SYS_ST_STANDBY,
    SYS_ST_PW_CHANGE,
    SYS_ST_TRANSMIT,
    SYS_ST_POWEROFF,
    SYS_ST_INVALID,
}nSYS_STATUS;

typedef struct {
    u8 keys[4][4];
    u16 counter;    
}ST_PASSWORD;

typedef struct{
    u8 channel;
    u8 step;
    u8 first[4];
    u8 second[4];
}ST_SIGN;

typedef struct {
    u8 cnt;
    nSYS_STATUS sys_sts;
    nSYS_STATUS sys_lts;
    u8  sub_sts;
    u8  sub_lts;
    
    u16 delay0;
    u16 delay1;
        
    ST_SIGN sign;
    u8 sn[4];

    u8 transmit_data[8];


}SYS_CTRL;
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

