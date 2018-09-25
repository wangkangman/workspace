#include "pic.h"
#include "clkset.h"


VODET init_fosc(VODET){
    OSCCON= 0x78;
    
    /* WDT SET TO 256ms*/
    WDTCON = 0x10;
}