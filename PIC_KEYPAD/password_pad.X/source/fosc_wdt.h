/* 
 * File:   fosc_wdt.h
 * Author: Gilgamesh
 *
 * Created on May 21, 2018, 10:40 AM
 */

#ifndef FOSC_WDT_H
#define	FOSC_WDT_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "pic.h"
    
#define clk_init() do{\
                    OSCCON = 0X78;\
                    }while(0)

#define wdt_init() do{\
                    WDTCON = 0x14;\
                    }while(0)



#ifdef	__cplusplus
}
#endif

#endif	/* FOSC_WDT_H */

