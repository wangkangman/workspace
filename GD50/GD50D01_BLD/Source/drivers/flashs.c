#include "regdef.h"
#include "flashs.h"

/*-----------------------------------------------------------------*/
/* FUNC : Read data from                                           */
/* PARA :                                                          */
/*   type : target flash type                                      */
/*   addr : flash address(must be page align)                      */
/*   pbuf : data buff                                              */
/*   size : data size(must be 4 byte align, maximum one page)      */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET flashs_read(U08ET type, U32ET addr, U32ET *pbuf, U32ET size)
{
    U32ET index;

    /* check flash address page align */
    if( 0 != (addr%FLASH_PAGE_SIZE) ) {
        return ;
    }

    /* check data size format */
    if( (size > FLASH_PAGE_SIZE) || (0 != (size%4)) ) {
        return ;
    }

    /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;

    /* enable ISP function */
    FMC_ISPCON |= (1<<0);

    /* enable target flash type */
    switch(type) {
        case FLASH_TYPE_CFGS:
            FMC_ISPCON |= (1<<4);
            break;
        case FLASH_TYPE_APROM:
            FMC_ISPCON |= (1<<3);
            break;
        case FLASH_TYPE_LDROM:
            FMC_ISPCON |= (1<<5);
            break;
        case FLASH_TYPE_DATA:
            break;
        default:
            break;
    }

    /* flash data write */
    for(index=0; index<size/4; index++) {
        FMC_ISPCMD = 0x00;
        FMC_ISPADR = addr + index*4;
        FMC_ISPTRG = 0x01;
        __isb(0xF);    /* make sure ISP/CPU be synchronized */
        while(FMC_ISPTRG);
        pbuf[index] = FMC_ISPDAT;
    }

    /* disable target flash type */
    switch(type) {
        case FLASH_TYPE_CFGS:
            FMC_ISPCON &= ~(1<<4);
            break;
        case FLASH_TYPE_APROM:
            FMC_ISPCON &= ~(1<<3);
            break;
        case FLASH_TYPE_LDROM:
            FMC_ISPCON &= ~(1<<5);
            break;
        case FLASH_TYPE_DATA:
            break;
        default:
            break;
    }

    /* disable ISP function */
    FMC_ISPCON &= ~(1<<0);

    /* lock register */
    GCR_WPRPROT = 0x00;
}

/*-----------------------------------------------------------------*/
/* FUNC : Save data to flash                                       */
/* PARA :                                                          */
/*   type : target flash type                                      */
/*   addr : flash address(must be page align)                      */
/*   pbuf : data buff                                              */
/*   size : data size(must be 4 byte align, maximum one page)      */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET flashs_write(U08ET type, U32ET addr, U32ET *pbuf, U32ET size)
{
    U32ET index;

    /* check flash address page align */
    if( 0 != (addr%FLASH_PAGE_SIZE) ) {
        return ;
    }

    /* check data size format */
    if( (size > FLASH_PAGE_SIZE) || (0 != (size%4)) ) {
        return ;
    }

    /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;

    /* enable ISP function */
    FMC_ISPCON |= (1<<0);

    /* enable target flash type */
    switch(type) {
        case FLASH_TYPE_CFGS:
            FMC_ISPCON |= (1<<4);
            break;
        case FLASH_TYPE_APROM:
            FMC_ISPCON |= (1<<3);
            break;
        case FLASH_TYPE_LDROM:
            FMC_ISPCON |= (1<<5);
            break;
        case FLASH_TYPE_DATA:
            break;
        default:
            break;
    }

    /* flash page erase */
    FMC_ISPCMD = 0x22;
    FMC_ISPADR = addr;
    FMC_ISPTRG = 0x1;
    __isb(0xF);    /* make sure ISP/CPU be synchronized */
    while(FMC_ISPTRG);

    /* flash data write */
    for(index=0; index<size/4; index++) {
        FMC_ISPCMD = 0x21;
        FMC_ISPADR = addr + index*4;
        FMC_ISPDAT = pbuf[index];
        FMC_ISPTRG = 0x01;
        __isb(0xF);    /* make sure ISP/CPU be synchronized */
        while(FMC_ISPTRG);
    }

    /* disable target flash type */
    switch(type) {
        case FLASH_TYPE_CFGS:
            FMC_ISPCON &= ~(1<<4);
            break;
        case FLASH_TYPE_APROM:
            FMC_ISPCON &= ~(1<<3);
            break;
        case FLASH_TYPE_LDROM:
            FMC_ISPCON &= ~(1<<5);
            break;
        case FLASH_TYPE_DATA:
            break;
        default:
            break;
    }

    /* disable ISP function */
    FMC_ISPCON &= ~(1<<0);

    /* lock register */
    GCR_WPRPROT = 0x00;
}

/*-----------------------------------------------------------------*/
/* FUNC : Remap vector address                                     */
/* PARA :                                                          */
/*   addr : flash address                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET vector_remap(U32ET addr)
{
    /* unlock register */
    GCR_WPRPROT = 0x59;
    GCR_WPRPROT = 0x16;
    GCR_WPRPROT = 0x88;

    /* enable ISP function */
    FMC_ISPCON |= (1<<0);

    /* remap vector address */
    FMC_ISPCMD = 0x2E;
    FMC_ISPADR = addr;
    FMC_ISPTRG = 0x1;
    __isb(0xF);    /* make sure ISP/CPU be synchronized */
    while(FMC_ISPTRG);

    /* disable ISP function */
    FMC_ISPCON &= ~(1<<0);

    /* lock register */
    GCR_WPRPROT = 0x00;
}
