#ifndef __STRING_PROCESS_H
#define __STRING_PROCESS_H

#include "stm32f4xx.h"

u8 GetSectionVal(u8 *text, u8 *section, u8 *key, u8 *val);
u8 GetLimitVal(u8 *str, float* low, float* upper);


#endif

