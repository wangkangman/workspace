#ifndef __MAIN_H
#define __MIAN_H
#include "global.h"

typedef enum
{
    SYS_ST_POWER_ON       = 0,
    SYS_ST_READ,
    SYS_ST_STAY_STOP,
    SYS_ST_PW_CHANGE,
    SYS_ST_RF_SEND,
    SYS_ST_POWER_OFF,
    SYS_ST_INVALID
}SYS_STATUS, SUB_STATUS;


typedef struct
{
    SYS_STATUS sys_sts;
    u8 sub_sts;
    SYS_STATUS sys_lts;
    u8 sub_lts;

    u16 delay0;
    u16 delay1;
}SYS_CTRL;

void SysPreInit(void);
void sys_power_on(void);
void sys_read(void);
void sys_stay_stop(void);
void sys_pw_change(void);
void sys_rf_send(void);
void sys_power_off(void);

u8 string_compare(u8 *a, u8 *b, u8 length);

#endif

