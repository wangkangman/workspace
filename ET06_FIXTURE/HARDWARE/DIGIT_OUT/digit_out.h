#ifndef __DIGIT_OUT_H
#define __DIGIT_OUT_H

#include "sys.h"


typedef struct {
  void (*init)(void);
  void (*wall)(u8);
  void (*infra)(u8);
  void (*door)(u8);
  void (*hall_main)(u8);
  void (*hall_ref)(u8);
  void (*refvol)(u8);
  void (*pwm)(u8);
  void (*rf)(u8);
}DIGIT_API;
extern DIGIT_API digit;


#define HIGH_POWER (0)
#define LOW_POWER  (1)

#define POW_OFF (0)
#define POW_ON  (1)

#define CH_MAIN (0)
#define CH_BAT  (1)

#define CH_P  (0)
#define CH_N  (1)

#define LEVEL_HIGH  (0)
#define LEVEL_LOW   (1)


typedef enum {
  SUP_POW = 0,
  SUP_BAT
} BATn_type;

typedef enum {
  N = 0,
  P
} ACn_type;

typedef  struct{
  void(*init)(void);
  void(*ctrl)(u8);
  void(*bat_main)(u8);
  void(*ac_channel)(u8);
  void(*level)(u8);
}POWER_API;

extern POWER_API power;


#endif

