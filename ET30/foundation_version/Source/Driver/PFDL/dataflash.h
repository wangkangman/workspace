#ifndef _FOUNDATION_H
#define _FOUNDATION_H
#include "global.h"



U08ET write_data_to_dataflash(U08ET * pbuf,U16ET index,U16ET count);
U08ET read_data_from_dataflash(U08ET * pbuf,U16ET index,U16ET count);



#endif