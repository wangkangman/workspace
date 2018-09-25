#ifndef __SLEVE_COMMAND_H
#define __SLEVE_COMMAND_H
#include "common.h"

#define MOT_CTS_UPUP (1)
#define MOT_CTS_DOWN (2)
#define MOT_CTS_STOP (3)



typedef enum{
   INFRA = 0,
   WALL,
   DOOR,
   POWER,
   MAIN_HALL,
   REF_HALL
}SIGNAL_TYPE;



typedef struct {
  u8 (*button)(u32 *);
  u8 (*current)(float *);
  u8 (*signal)(SIGNAL_TYPE, u32 *);
  u8 (*seg)(u32, u32);
  u8 (*motor)(u32);
  u8 (*pwm)(u32);
  u8 (*rf)(u32*);
}SLAVE_COMMAND;

extern SLAVE_COMMAND slave;



#endif

/*
				(void *)usmart_getcurrent,
				"u32 usmart_getcurrent(void)",
				(void *)usmart_getkeyad,
        "u32 usmart_getkeyad(void)",
        (void *)usmart_segctrl,
        "void usmart_segctrl(u32 digit, u32 piont)",
        (void *)usmart_motorctrl,
        "void usmart_motorctrl(u32 dir)",
        (void *)usmart_pwmsctrl,
        "void usmart_pwmsctrl(u32 pwmval)",
        (void *)usmart_remotekey,
        "void usmart_remotekey(void)",
        (void *)usmart_infra_sta,
        "u32 usmart_infra_sta(void)",
        (void *)usmart_wall_sta,
        "u32 usmart_wall_sta(void)",
        (void *)usmart_door_sta,
        "u32 usmart_door_sta(void)",
        (void *)usmart_power_sta,
        "u32 usmart_power_sta(void)",
};*/
