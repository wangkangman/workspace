#ifndef _EEPROM_H
#define _EEPROM_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define EEPROM parameter                                         */
/*-----------------------------------------------------------------*/
#define EEP_SAVE_FLG     (0xA5A5)
#define EEP_START_ADDR   (0x4000)
#define EEP_TOTAL_SIZE   (1024)
#define EEP_BLK_SIZE     (128)
#define EEP_RASS_KEY1    (0x56)
#define EEP_RASS_KEY2    (0xAE)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET eepbyt_writ(U16ET addr, U08ET bdat);
U08ET eepbyt_read(U16ET addr);
VODET eepblk_writ(U08ET blks, U08ET *pbuf, U08ET size);

#endif /* _EEPROM_H */
