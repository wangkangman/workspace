#include "key.h"
#include "buzled.h"


static ST_KEYS st_keys[KEY_ID_MAX];

void key_init(void){
    u8 index;
    
    
    ANSELA = 0x0; 
    ANSELB = 0x0; 
    ANSELC = 0x0;    
    TRISA |= (0x1 << 2);
    TRISC |= (0x1 << 0);
    TRISC |= (0x1 << 1);
    TRISC |= (0x1 << 7);
    TRISB |= (0x1 << 4);
    TRISB |= (0x1 << 5);
    TRISB |= (0x1 << 6);
    TRISC |= (0x1 << 6);
    TRISC |= (0x1 << 3);
    TRISC |= (0x1 << 4);
    TRISA |= (0x1 << 4);
    TRISA |= (0x1 << 5);
    
    for (index = 0; index < KEY_ID_MAX; index++){
        st_keys[index].lstst  = KEY_ST_MAX;
        st_keys[index].curst  = KEY_ST_MAX;
        st_keys[index].stable = KEY_ST_MAX;
        st_keys[index].active = KEY_ST_HIGH;
        st_keys[index].cycle  = 10;
        st_keys[index].counter = 0;
    }
}

void key_task(void){
    u8 index;    
    st_keys[KEY_ID_0].curst = (u8)RA4;
    st_keys[KEY_ID_1].curst = (u8)RA2;
    st_keys[KEY_ID_2].curst = (u8)RC0;
    st_keys[KEY_ID_3].curst = (u8)RC1;
    st_keys[KEY_ID_4].curst = (u8)RC7;
    st_keys[KEY_ID_5].curst = (u8)RB4;
    st_keys[KEY_ID_6].curst = (u8)RB5;
    st_keys[KEY_ID_7].curst = (u8)RB6;
    st_keys[KEY_ID_8].curst = (u8)RC6;
    st_keys[KEY_ID_9].curst = (u8)RC3;
    st_keys[KEY_ID_ASTERISK].curst = (u8)RC4;
    st_keys[KEY_ID_ENTER].curst = (u8)RA5;
            
    for (index = 0; index < KEY_ID_MAX; index++){
        if (st_keys[index].curst != st_keys[index].lstst) {
            st_keys[index].counter = 0;
            st_keys[index].stable = KEY_ST_MAX;
        } else {
            if (st_keys[index].counter < st_keys[index].cycle)
                st_keys[index].counter++;
            else if (st_keys[index].counter == st_keys[index].cycle) {
                st_keys[index].stable = st_keys[index].curst;
                st_keys[index].counter++;
            }        
        }
        st_keys[index].lstst = st_keys[index].curst;
    }
}

u8 getkey(void){
    u8 index;
    for (index = 0; index < KEY_ID_MAX; index++){
        if (st_keys[index].stable == st_keys[index].active) {
            buz_ctrl(50, 1);
            st_keys[index].stable = KEY_ST_MAX;
            return index;
        }
    }
    return KEY_ID_MAX;
}

