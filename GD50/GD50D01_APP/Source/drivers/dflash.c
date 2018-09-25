#include "regdef.h"
#include "dflash.h"

/*-----------------------------------------------------------------*/
/* FUNC : Read one page from data flash                            */
/* PARA :                                                          */
/*   addr : address to be read                                     */
/*   pbuf : buff to store data                                     */
/*   size : size to be read(4 byte unit)                           */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET dflash_read_page(U32ET *pbuf, U32ET addr, U32ET size)
{
  U32ET index;
  __disable_irq();
  /* unlock register */
  GCR_WPRPROT = 0x59;
  GCR_WPRPROT = 0x16;
  GCR_WPRPROT = 0x88;
  /* enable ISP function */
  FMC_ISPCON |= (1 << 0);
  for (index = 0; index < size; index++)
  {
    FMC_ISPCMD = 0x00;
    FMC_ISPADR = DFMC_BASE_ADDR + addr + index * 4;
    FMC_ISPTRG = 0x01;
    __isb(0xF);
    while (FMC_ISPTRG)
      ; /* make sure ISP/CPU be synchronized */
    pbuf[index] = FMC_ISPDAT;
  }
  /* disable ISP function */
  FMC_ISPCON &= ~(1 << 0);
  /* lock register */
  GCR_WPRPROT = 0x00;
  __enable_irq();
}

/*-----------------------------------------------------------------*/
/* FUNC : write one page to data flash                             */
/* PARA :                                                          */
/*   addr : address to be read                                     */
/*   pbuf : buff to store data                                     */
/*   size : size to be write(4 byte unit)                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET dflash_writ_page(U32ET *pbuf, U32ET addr, U32ET size)
{
  U32ET index;
  __disable_irq();
  /* unlock register */
  GCR_WPRPROT = 0x59;
  GCR_WPRPROT = 0x16;
  GCR_WPRPROT = 0x88;
  /* enable ISP function */
  FMC_ISPCON |= (1 << 0);
  /* page erase */
  FMC_ISPCMD = 0x22;
  FMC_ISPADR = DFMC_BASE_ADDR + addr;
  FMC_ISPTRG = 0x1;
  __isb(0xF); /* make sure ISP/CPU be synchronized */
  while (FMC_ISPTRG)
    ;
  for (index = 0; index < size; index++)
  {
    FMC_ISPCMD = 0x21;
    FMC_ISPADR = DFMC_BASE_ADDR + addr + index * 4;
    FMC_ISPDAT = pbuf[index];
    FMC_ISPTRG = 0x01;
    __isb(0xF); /* make sure ISP/CPU be synchronized */
    while (FMC_ISPTRG)
      ;
  }
  /* disable ISP function */
  FMC_ISPCON &= ~(1 << 0);
  /* lock register */
  GCR_WPRPROT = 0x00;
  __enable_irq();
}
