/* 
 * File:   main.h
 * Author: Administrator
 *
 * Created on November 10, 2017, 1:14 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define VERSION       (2)

#define SYS_POWER_ON  (0)
#define SYS_STANDBY   (1)
#define SYS_DOOR_TRANSMIT (2)
#define SYS_WIFI_TRANSMIT (3)
#define SYS_MATCH_CODE (4)
#define SYS_INVALID (5)
#define LOCK_ADRESS     ((U08ET)(40))

typedef struct{
    U16ET delay0;
    U16ET delay1;
    U08ET sys_st;
    U08ET sub_st;
//    U08ET lock;
    U08ET repeat_key;
    U08ET match_buf[4];
}SYS_ST;

void led_init(void);
void led_task(void);
void led_ctrl(U16ET period, U08ET times);

#endif	/* MAIN_H */

