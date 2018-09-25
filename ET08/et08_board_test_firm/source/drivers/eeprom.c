#include "regdef.h"
#include "timwdt.h"
#include "eeprom.h"

/*-----------------------------------------------------------------*/
/* Local function declare                                          */
/*-----------------------------------------------------------------*/
static void eepblk_prog(u8 blks, u8 *pbuf, u8 size);
int _fctcpy(char name);

/*-----------------------------------------------------------------*/
/* FUNC : Write one byte to eeprom                                 */
/* PARA :                                                          */
/*   addr : address to be writen                                   */
/*   bdat : data to be be writen                                   */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void eepbyt_writ(u16 addr, u8 bdat)
{
    /* select standard programming timing */
    FLASH_CR1 &= ~(1 << 0);

    /* unlock eeprom */
    do
    {
        FLASH_DUKR = EEP_RASS_KEY2;
        FLASH_DUKR = EEP_RASS_KEY1;
    } while (0 == (FLASH_IAPSR & (1 << 3)));

    *((volatile u8 *)(EEP_START_ADDR + addr)) = bdat;

    /* wait finished */
    while (0 == (FLASH_IAPSR & (1 << 2)))
    {
        wdtdog_feed();
    }

    /* lock eeprom */
    FLASH_IAPSR &= ~(1 << 3);
}

/*-----------------------------------------------------------------*/
/* FUNC : Read one byte from eeprom                                */
/* PARA :                                                          */
/*   addr : address to be read                                     */
/* RETN :                                                          */
/*   Return read result                                            */
/*-----------------------------------------------------------------*/
u8 eepbyt_read(u16 addr)
{
    return *((volatile u8 *)(EEP_START_ADDR + addr));
}

/*-----------------------------------------------------------------*/
/* FUNC : Write one block data to eeprom                           */
/* PARA :                                                          */
/*   blks : block address to be writen                             */
/*   pbuf : buff stored data to be writen                          */
/*   size : data size to be writen                                 */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void eepblk_writ(u8 blks, u8 *pbuf, u8 size)
{
    /* select standard programming timing */
    FLASH_CR1 &= ~(1 << 0);

    /* unlock eeprom */
    do
    {
        FLASH_DUKR = EEP_RASS_KEY2;
        FLASH_DUKR = EEP_RASS_KEY1;
    } while (0 == (FLASH_IAPSR & (1 << 3)));

    /* copy source code to ram area */
    wdtdog_feed();
    _fctcpy('F');
    wdtdog_feed();

    /* call the function that excute in ram */
    eepblk_prog(blks, pbuf, size);

    /* wait finished */
    while (0 == (FLASH_IAPSR & (1 << 6)))
    {
        wdtdog_feed();
    }

    /* lock eeprom */
    FLASH_IAPSR &= ~(1 << 3);
}

/*-----------------------------------------------------------------*/
/* FUNC : Write one block data to eeprom (excute in ram)           */
/* PARA :                                                          */
/*   blkidx : block address to be writen                           */
/*   pbuf   : buff stored data to be writen                        */
/*   size : data size to be writen                                 */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
#pragma section(FLASH_CODE)
static void eepblk_prog(u8 blks, u8 *pbuf, u8 size)
{
    u16 startAddr;
    u8 index;

    startAddr = EEP_START_ADDR + (u16)EEP_BLK_SIZE * blks;

    /* standard programming mode */
    FLASH_CR2 |= (1 << 0);
    FLASH_NCR2 &= ~(1 << 0);

    /* copy data bytes from RAM to FLASH memory */
    for (index = 0; index < size; index++)
    {
        *((volatile u8 *)(startAddr + index)) = pbuf[index];
    }
    for (index = size; index < EEP_BLK_SIZE; index++)
    {
        *((volatile u8 *)(startAddr + index)) = pbuf[size];
    }
}
#pragma section()
