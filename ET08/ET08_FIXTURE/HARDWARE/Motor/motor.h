#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h" 





typedef struct{
  void (*init)(void);
  u8 (*up)(void);
  u8 (*down)(void);
  void (*ref)(u8);
}MOTOR_API;

typedef struct{
  void (*init)(void);
  void (*start)(void);
  u32  (*get)(void);
  void (*ctrl)(u8);
}PWM_API;




extern MOTOR_API motor;
extern PWM_API pwm;

#endif

