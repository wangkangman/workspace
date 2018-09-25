#include "pic.h"
#include "clkset.h"


VODET init_fosc(VODET){
    /* clock set to 16Mhz*/
    OSCCON= 0x78;
    
    /* WDT SET TO 256ms*/
    WDTCON = 0x14;
}
