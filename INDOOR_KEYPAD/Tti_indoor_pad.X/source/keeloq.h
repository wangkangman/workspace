#ifndef _KEELOQ_H
#define _KEELOQ_H
#include "type.h"


#define EZMODE   0
#define NORMODE  1



u8* ENcrypt(u8 *source, u8* _key);
u8* DEcrypt(u8 *source, u8* _key);
u8  getBit( u8 *source, u16 n);
u8* RRC(u8 *source, u8 c, u8 n);
u8* RLC(u8 *source, u8 c, u8 n);
u8* LRN_MKEY(u8 *source, u8 *new_key);

#endif