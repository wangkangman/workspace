#include "KeyTask.h"
#include "BuzTask.h"
#include "regdef.h"

static ETST_KEYS et_keys[KEY_ID_MAX];



void key_init(void){
    u8 index = 0;
    PORT_ADPC  =  (0x1U);
    PORT_PMC1  =  0x0; /* digital port */
    PORT_PM1   =  0x1f; /* input mode */
    PORT_PMC4  =  0xf9;
    PORT_PM1   =  0xff;
    
    
    for (index = 0; index < KEY_ID_MAX; index++)
    {
        et_keys[index].lstst = KEY_ST_MAX;
        et_keys[index].curst = KEY_ST_MAX;
        et_keys[index].stabe = KEY_ST_MAX;
	et_keys[index].actlv = KEY_ST_HIGH;
        et_keys[index].cycle = 10;
        et_keys[index].count = 0;
    }
}

void key_task(void)
{
    u8 index;
    et_keys[KEY_ID_1].curst  = PORT_PD1 &(1 << 0)? 1 : 0;
    et_keys[KEY_ID_2].curst  = PORT_PD1 &(1 << 1)? 1 : 0;
    et_keys[KEY_ID_3].curst  = PORT_PD1 &(1 << 2)? 1 : 0;
    et_keys[KEY_ID_4].curst  = PORT_PD1 &(1 << 3)? 1 : 0;
    et_keys[KEY_ID_5].curst  = PORT_PD1 &(1 << 4)? 1 : 0;
    et_keys[KEY_ID_6].curst  = PORT_PD6 &(1 << 1)? 1 : 0;
    et_keys[KEY_ID_7].curst  = PORT_PD6 &(1 << 0)? 1 : 0;
    et_keys[KEY_ID_8].curst  = PORT_PD12&(1 << 1)? 1 : 0;
    et_keys[KEY_ID_9].curst  = PORT_PD12&(1 << 2)? 1 : 0;
    et_keys[KEY_ID_X].curst  = PORT_PD13&(1 << 7)? 1 : 0;
    et_keys[KEY_ID_0].curst  = PORT_PD4 &(1 << 1)? 1 : 0;
    et_keys[KEY_ID_ENTER].curst  = PORT_PD4 &(1 << 2)? 1 : 0;    

    for (index = 0; index < KEY_ID_MAX; ++index) {
        if (et_keys[index].curst != et_keys[index].lstst) {
            et_keys[index].count  = 0;
            et_keys[index].stabe  = KEY_ST_MAX;
        }
        else {
            if (et_keys[index].count < et_keys[index].cycle) {
                ++et_keys[index].count;
            }
            else if (et_keys[index].count == et_keys[index].cycle) {
                et_keys[index].stabe = et_keys[index].curst;
                ++et_keys[index].count;
            }
        }
        et_keys[index].lstst = et_keys[index].curst;
    }
} 

u8 getkey(void)
{
    u8 index;
    for (index = 0; index < KEY_ID_MAX; ++index) {
    	if (et_keys[index].stabe == et_keys[index].actlv) {
		BUZ_ctrl(50, 1);
		et_keys[index].stabe = KEY_ST_MAX;
		return index;
    	}
    }
    return KEY_ID_MAX;
}

