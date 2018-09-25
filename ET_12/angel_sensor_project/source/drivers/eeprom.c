#include "regdef.h"
#include "timwdt.h"
#include "eeprom.h"

/*-----------------------------------------------------------------*/
/* Local function declare                                          */
/*-----------------------------------------------------------------*/
static VODET eepblk_prog(U08ET blkidx, U08ET *pbuf);
int _fctcpy(char name);

/*-----------------------------------------------------------------*/
/* FUNC : Write one byte to eeprom                                 */
/* PARA :                                                          */
/*   addr : address to be writen                                   */
/*   bdat : data to be be writen                                   */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET eepbyt_writ(U16ET addr, U08ET bdat)
{
    /* select standard programming timing */
    FLASH_CR1 &= ~(1<<0);

    /* unlock eeprom */
    do {
        FLASH_DUKR = EEP_RASS_KEY2;
        FLASH_DUKR = EEP_RASS_KEY1;
    }while (0 == (FLASH_IAPSR & (1<<3)));

    *((volatile U08ET *)(EEP_START_ADDR+addr)) = bdat;

    /* wait finished */
    while (0 == (FLASH_IAPSR & (1<<2)))
    {
        wdtdog_feed();
    }

    /* lock eeprom */
    FLASH_IAPSR &= ~(1<<3);
}

/*-----------------------------------------------------------------*/
/* FUNC : Read one byte from eeprom                                */
/* PARA :                                                          */
/*   addr : address to be read                                     */
/* RETN :                                                          */
/*   Return read result                                            */
/*-----------------------------------------------------------------*/
U08ET eepbyt_read(U16ET addr)
{
    return *((volatile U08ET *)(EEP_START_ADDR+addr));
}

/*-----------------------------------------------------------------*/
/* FUNC : Write one block data to eeprom                           */
/* PARA :                                                          */
/*   blks : block address to be writen                             */
/*   pbuf : buff stored data to be writen                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET eepblk_writ(U08ET blks, U08ET *pbuf)
{
    /* select standard programming timing */
    FLASH_CR1 &= ~(1<<0);

    /* unlock eeprom */ 
    do {
        FLASH_DUKR = EEP_RASS_KEY2;
        FLASH_DUKR = EEP_RASS_KEY1;
    }while (0 == (FLASH_IAPSR & (1<<3)));

    /* copy source code to ram area */
    wdtdog_feed();
    _fctcpy('F');
    wdtdog_feed();

    /* call the function that excute in ram */
    eepblk_prog(blks, pbuf);

    /* wait finished */
    while (0 == (FLASH_IAPSR & (1<<6))) {
        wdtdog_feed();
    }

    /* lock eeprom */
    FLASH_IAPSR &= ~(1<<3);
}

/*-----------------------------------------------------------------*/
/* FUNC : Write one block data to eeprom (excute in ram)           */
/* PARA :                                                          */
/*   blkidx : block address to be writen                           */
/*   pbuf   : buff stored data to be writen                        */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
#pragma section (FLASH_CODE)
void eepblk_prog(U08ET blks, U08ET *pbuf)
{
    U16ET startAddr;
    U08ET index;

    startAddr = EEP_START_ADDR + (U16ET)EEP_BLK_SIZE*blks;

    /* standard programming mode */
    FLASH_CR2  |=  (1<<0);
    FLASH_NCR2 &= ~(1<<0);

    /* copy data bytes from RAM to FLASH memory */
    for (index = 0; index < EEP_BLK_SIZE; index++) {
        *((volatile U08ET *)(startAddr + index)) = pbuf[index];
    }
}
#pragma section ()