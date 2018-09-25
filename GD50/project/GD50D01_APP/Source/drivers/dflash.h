#ifndef _DFLASH_H
#define _DFLASH_H

#include "global.h"

/**
  * @brief  Define EEPROM parameter 
  *
  */
#define DFLASH_SAVE_FLG (0x9527)

/*-----------------------------------------------------------------*/
/* Define data flash parameter                                     */
/*-----------------------------------------------------------------*/
#define DFMC_BASE_ADDR (0x0001F000)
#define DFMC_AREA_SIZE (0x1000)
#define DFMC_PAGE_SIZE (0x0200)

#define DFMC_CTRLS_OFST (0x0000)
#define DFMC_RKEYS_OFST (0x0200)
#define DFMC_UPFORCE_OFST (0x0400)
#define DFMC_DNFORCE_OFST (0x0600)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET dflash_read_page(U32ET *pbuf, U32ET addr, U32ET size);
VODET dflash_writ_page(U32ET *pbuf, U32ET addr, U32ET size);

#endif /* _DFLASH_H */
