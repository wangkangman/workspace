#ifndef _EEPROM_H
#define _EEPROM_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define EEPROM parameter                                         */
/*-----------------------------------------------------------------*/
#define EEP_SAVE_FLG (0xA5A5)
#define EEP_START_ADDR (0x4000)
#define EEP_TOTAL_SIZE (1024)
#define EEP_BLK_SIZE (128)
#define EEP_RASS_KEY1 (0x56)
#define EEP_RASS_KEY2 (0xAE)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
void eepbyt_writ(u16 addr, u8 bdat);
u8 eepbyt_read(u16 addr);
void eepblk_writ(u8 blks, u8 *pbuf, u8 size);

#endif /* _EEPROM_H */
