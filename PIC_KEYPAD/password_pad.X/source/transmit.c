#include "pic.h"
#include "transmit.h"


void transmit_standby(void);
void transmit_tp(void);
void transmit_th(void);
void transmit_data(void);
void transmit_tg(void);


ST_TRANSMIT tcb_transmit;

FUNC transmit_func[]={
    transmit_standby,
    transmit_tp,
    transmit_th,
    transmit_data,
    transmit_tg
};

void transmit_init(void){
    TRISC &= ~(1<<2);
    RC2 = 0;
    tcb_transmit.cur_st      = STANDBY;
    tcb_transmit.bit_index   = 0;
    tcb_transmit.pulse_index = 0;
    tcb_transmit.repeat      = 0;
    tcb_transmit.times       = 0;
    return;
}

void transmit(u8* data, u8 times){
    u8 index;
    if (STANDBY != tcb_transmit.cur_st)
        return;
    
    for (index = 0; index < 8; index++){
        tcb_transmit.data[index] = data[index];    
    }
    
    for (index = 0; index < 64; index++){
        tcb_transmit.data_bits[index] = (tcb_transmit.data[index / 8] & (1 << (index % 8)))
                  ? 0 : 1;
    }
    
    tcb_transmit.times       = times;    
    tcb_transmit.bit_index   = 0;
    tcb_transmit.pulse_index = 0;    
    tcb_transmit.repeat      = 0;
    tcb_transmit.cur_st      = TP;
    return;
}

void transmit_irq_task(void){
    transmit_func[tcb_transmit.cur_st]();
    return;
}

void transmit_standby(void){
    RC2 = 0;
    return;
}

void transmit_tp(void){
    if (tcb_transmit.bit_index < 23) {
        if (0 == (tcb_transmit.bit_index % 2)) RC2 = 1;
        else RC2 = 0;
    } 
    tcb_transmit.bit_index++;
    
    if (tcb_transmit.bit_index >= 23) {
        tcb_transmit.cur_st      = TH;
        tcb_transmit.bit_index   = 0;
        tcb_transmit.pulse_index = 0;
    }
    return;
}


void transmit_th(void){
    if (tcb_transmit.bit_index < 10) {
        RC2 = 0;
    }
    
    tcb_transmit.bit_index++;
    if (tcb_transmit.bit_index >= 10) {
        tcb_transmit.cur_st      = DATA;
        tcb_transmit.bit_index   = 0;
        tcb_transmit.pulse_index = 0;
    }
    return;
}

void transmit_data(void){
    
    if (0 == tcb_transmit.pulse_index)
        RC2 = 1; 
    else if (1 == tcb_transmit.pulse_index){
        if (tcb_transmit.bit_index < 64)
            RC2 = tcb_transmit.data_bits[tcb_transmit.bit_index];
            /*RC2 = (tcb_transmit.data[tcb_transmit.bit_index / 8] & (1 << (tcb_transmit.bit_index % 8)))
                  ? 0 : 1;*/

        else if (64 == tcb_transmit.bit_index){
            RC2 = 1;
        }
        else if (65 == tcb_transmit.bit_index) {
            RC2 = tcb_transmit.repeat ? 0 : 1;
        } else 
            RC2 = 0;
    }    
    else if (2 == tcb_transmit.pulse_index) {
        RC2 = 0;
    }
    
    tcb_transmit.pulse_index++;
    if (3 == tcb_transmit.pulse_index) {
        tcb_transmit.pulse_index = 0;
        tcb_transmit.bit_index++;
    }
    
    if (tcb_transmit.bit_index >= 66){
        tcb_transmit.cur_st      = TG;
        tcb_transmit.bit_index   = 0;
        tcb_transmit.pulse_index = 0;
    }   
}


void transmit_tg(void){
    if (tcb_transmit.bit_index < 39) {
        RC2 = 0;
        tcb_transmit.bit_index++;
    } else {
        tcb_transmit.times--;
        if (tcb_transmit.times > 0) {
            tcb_transmit.cur_st      = TP;
            tcb_transmit.bit_index   = 0;
            tcb_transmit.pulse_index = 0;
            tcb_transmit.repeat      = 1;
        }
        else {
            tcb_transmit.cur_st      = STANDBY;
            tcb_transmit.bit_index   = 0;
            tcb_transmit.pulse_index = 0;
            tcb_transmit.repeat      = 0;
        }
    }
    return;
}
