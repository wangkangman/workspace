#ifndef _FOUNDATION_H
#define _FOUNDATION_H
#include "global.h"



u8 write_data_to_dataflash(u8 * pbuf,u16 index,u16 count);
u8 read_data_from_dataflash(u8 * pbuf,u16 index,u16 count);



#endif