#ifndef _DFLASH_H
#define _DFLASH_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define data flash parameter                                     */
/*-----------------------------------------------------------------*/
#define FLASH_TYPE_CFGS  (1)
#define FLASH_TYPE_APROM (2)
#define FLASH_TYPE_LDROM (3)
#define FLASH_TYPE_DATA  (4)
#define FLASH_PAGE_SIZE  (0x200)

#define FLASH_CFGSS_BASE (0x00300000)
#define FLASH_LDROM_BASE (0x00100000)
#define FLASH_APROM_BASE (0x00000000)
#define FLASH_DATAS_BASE (0x0001F000)
#define FLASH_CFGSS_SIZE (0x00000004)
#define FLASH_LDROM_SIZE (0x00001000)
#define FLASH_APROM_SIZE (0x00010000)
#define FLASH_DATAS_SIZE (0x00001000)

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET flashs_read(U08ET type, U32ET addr, U32ET *pbuf, U32ET size);
VODET flashs_write(U08ET type, U32ET addr, U32ET *pbuf, U32ET size);
VODET vector_remap(U32ET addr);

#endif /* _DFLASH_H */
