#include "buzled.h"

static ST_BUZ st_buz;
void buz_init(void){
    TRISA &= ~(1<<1);
    RA1 = 1;
    
    st_buz.times = 0;
    st_buz.period = 0;
    st_buz.times =0;
}


void buz_task(void){
    if (st_buz.times) {
        st_buz.counter++;
        if (st_buz.counter == st_buz.period) {
            if (1 == st_buz.cur_sts) {
                RA1 = 1;
                st_buz.times--;
                st_buz.cur_sts = 0;
            } else {
                RA1 = 0;
                st_buz.cur_sts = 1;
            }
            st_buz.counter = 0;
        }
    } else {
        RA1 = 1;
    }
}



void buz_ctrl(u16 period, u8 times){
    st_buz.period = period;
    st_buz.times = times;
    st_buz.counter = 0;
    st_buz.cur_sts = 1;
    RA1 = 0;
}