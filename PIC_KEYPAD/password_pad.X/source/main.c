/*
 * File:   main.c
 * Author: Gilgamesh
 *
 * Created on May 21, 2018, 9:27 AM
 */


#include <xc.h>
#include "type.h"
#include "pic.h"
#include "main.h"
#include "includes.h"

#pragma config FOSC = INTOSC
#pragma config WDTE = ON
#pragma config PWRTE = ON
#pragma config MCLRE = OFF
#pragma config CP = ON
//#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config IESO = OFF
#pragma config FCMEN = ON
#pragma config PLLEN = OFF
#pragma config LVP = OFF

static SYS_CTRL sys_ctrl;

__eeprom ST_PASSWORD password  = {
     0x01, 0x02, 0x03, 0x04,
     0x01, 0x02, 0x03, 0x04,
     0x01, 0x02, 0x03, 0x04,
     0x01, 0x02, 0x03, 0x04,
     0x0007,
};

const char flash_sn[4] __at(0xff8) = {0x08, 0x09, 0x0a, 0x0b};

static void sys_read(void);
static void sys_standby(void);
static void sys_pw_change(void);
static void sys_transmit(void);
static void sys_power_off(void);

static void get_source_data(u8 *data);


FUNC sys_func[]={
    sys_read,
    sys_standby,
    sys_pw_change,
    sys_transmit,
    sys_power_off
};

void interrupt ISR(void){
    if (1 == TMR1IF){
        TMR1 = 0xff38;
        TMR1IF = 0;        
        if (sys_ctrl.cnt < 5) 
            sys_ctrl.cnt++;        
        transmit_irq_task();
    }
}

u8 sys_ready(void){
    if (5 == sys_ctrl.cnt){
        sys_ctrl.cnt = 0;
        return 1;
    }
    return 0;
}


void sys_init(void){
    clk_init();
    wdt_init();
    power_init();
    transmit_init();
    timer_init();
    buz_init();
    key_init();
    led_init();
    
    sys_ctrl.sys_sts = SYS_ST_READ;
    sys_ctrl.sys_lts = SYS_ST_INVALID;
    sys_ctrl.sub_sts = 0;
    sys_ctrl.sub_lts = 0;
}


void main(void) {
    sys_init();

    while(1u){
        if (0 == sys_ready())
            continue;
        
        CLRWDT();
        sys_func[(u8)sys_ctrl.sys_sts]();
        
        
        key_task();
        buz_task();
    }
    return;
}

static void sys_read(void){
#if 0
    u16 temp;
    temp = FLASH_READ(0xff8);
    sys_ctrl.sn[0] = temp & 0xff;
    sys_ctrl.sn[1] = temp >> 8;
    temp = FLASH_READ(0xff9);
    sys_ctrl.sn[2] = temp & 0xff;
    sys_ctrl.sn[3] = temp >> 8;
#endif
#if 1
    sys_ctrl.sn[0] = FLASH_READ(0xff8);
    sys_ctrl.sn[1] = FLASH_READ(0xff9);
    sys_ctrl.sn[2] = FLASH_READ(0xffa);
    sys_ctrl.sn[3] = FLASH_READ(0xffb);
#endif
    sys_ctrl.sys_sts = SYS_ST_STANDBY;
    sys_ctrl.sub_sts = 0;
    sys_ctrl.sub_lts = 0;
}
static void sys_standby(void){
    u8 button, index;
    
    button = getkey();
    if (0 == sys_ctrl.sub_sts) {
        for (index = 0; index < 4; index++) {
            sys_ctrl.sign.first[index]  = 20;
            sys_ctrl.sign.second[index] = 20;
        }
        sys_ctrl.sign.step    = 0;
        sys_ctrl.sign.channel = 5;
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay1  = AUTO_CLOSE_CNT;
    } else if (1 == sys_ctrl.sub_sts) {//channel sign in
        if (button < 5 && button > 0){
            sys_ctrl.sign.channel = button;
            sys_ctrl.sign.step = 0;
            sys_ctrl.sub_sts = 2;
        }
    } else if (2 == sys_ctrl.sub_sts) { //get password
        if (button < 10) {
            if (sys_ctrl.sign.step < 4) {
                sys_ctrl.sign.first[sys_ctrl.sign.step] = button;
            }
            if (sys_ctrl.sign.step < 8) sys_ctrl.sign.step++;
        }
    }
    
    /* auto close */
    if (KEY_ID_MAX != button) sys_ctrl.delay1 = AUTO_CLOSE_CNT;
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
        sys_ctrl.sys_sts = SYS_ST_POWEROFF;
        sys_ctrl.sub_sts = 0;
    }
    
    /* judge if the password is right */
    if (KEY_ID_ENTER == button || KEY_ID_ASTERISK == button) {
        if ((password.keys[sys_ctrl.sign.channel - 1][0] == sys_ctrl.sign.first[0])
            && (password.keys[sys_ctrl.sign.channel - 1][1] == sys_ctrl.sign.first[1])
            && (password.keys[sys_ctrl.sign.channel - 1][2] == sys_ctrl.sign.first[2])
            && (password.keys[sys_ctrl.sign.channel - 1][3] == sys_ctrl.sign.first[3])
            && (4 == sys_ctrl.sign.step)) {
            buz_ctrl(400,1);
            sys_ctrl.sys_sts = (KEY_ID_ENTER == button) ? SYS_ST_TRANSMIT : SYS_ST_PW_CHANGE;
            sys_ctrl.sub_sts = 0;
        } else {
            buz_ctrl(50,5);
            sys_ctrl.sub_sts = 0;
        }
    }
}

static void sys_pw_change(void){
    u8 button = getkey();
    
    if (0 == sys_ctrl.sub_sts) {
        sys_ctrl.sub_sts = 1;
        sys_ctrl.sign.step = 0;
        sys_ctrl.delay1 = AUTO_CLOSE_CNT;
    } else if (1 == sys_ctrl.sub_sts) {
        if (button < 10) {
            if (sys_ctrl.sign.step < 4) {
                sys_ctrl.sign.first[sys_ctrl.sign.step] = button;
            }
            if (sys_ctrl.sign.step < 8) sys_ctrl.sign.step++;
        }
        else if (KEY_ID_ENTER == button && (sys_ctrl.sign.step >= 4)) {
            buz_ctrl(400, 1);
            sys_ctrl.sub_sts = 2;
            sys_ctrl.sign.step = 0;
        }
    } else if (2 == sys_ctrl.sub_sts) {
        if (button < 10) {
            if (sys_ctrl.sign.step < 4) {
                sys_ctrl.sign.second[sys_ctrl.sign.step] = button;
            }
            if (sys_ctrl.sign.step < 8) sys_ctrl.sign.step++;
        } else if (KEY_ID_ENTER == button) {
            if ((sys_ctrl.sign.first[0] == sys_ctrl.sign.second[0])
               && (sys_ctrl.sign.first[1] == sys_ctrl.sign.second[1])
               && (sys_ctrl.sign.first[2] == sys_ctrl.sign.second[2])
               && (sys_ctrl.sign.first[3] == sys_ctrl.sign.second[3])) {
                buz_ctrl(400, 1);
                password.keys[sys_ctrl.sign.channel - 1][0] = sys_ctrl.sign.first[0];
                password.keys[sys_ctrl.sign.channel - 1][1] = sys_ctrl.sign.first[1];
                password.keys[sys_ctrl.sign.channel - 1][2] = sys_ctrl.sign.first[2];
                password.keys[sys_ctrl.sign.channel - 1][3] = sys_ctrl.sign.first[3];
            } else {
                buz_ctrl(50, 5);
            }
            sys_ctrl.sys_sts = SYS_ST_STANDBY;
            sys_ctrl.sub_sts = 0;
        }
    }
    
    /* auto close */
    if (KEY_ID_MAX != button) sys_ctrl.delay1 = AUTO_CLOSE_CNT;
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
        sys_ctrl.sys_sts = SYS_ST_POWEROFF;
        sys_ctrl.sub_sts = 0;
    }
}


static void sys_transmit(void){
    u8 index, button = getkey();
    if (0 == sys_ctrl.sub_sts) {
        get_source_data(sys_ctrl.transmit_data);
#if defined NORMAL_MODE
        _keeloq_encrypt(sys_ctrl.transmit_data, NORMODE);
#elif defined EASY_MODE
        _keeloq_encrypt(sys_ctrl.transmit_data, EZMODE);
#endif
        transmit(sys_ctrl.transmit_data, 6);
        password.counter++;
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 500;
        sys_ctrl.delay1 = AUTO_CLOSE_CNT;
    } else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay1 = AUTO_CLOSE_CNT;
        }
    } else if (2 == sys_ctrl.sub_sts) {
        if (KEY_ID_ENTER == button) {
            buz_ctrl(400, 1);
            get_source_data(sys_ctrl.transmit_data);
#if defined NORMAL_MODE
            _keeloq_encrypt(sys_ctrl.transmit_data, NORMODE);
#elif defined EASY_MODE
            _keeloq_encrypt(sys_ctrl.transmit_data, EZMODE);
#endif
            transmit(sys_ctrl.transmit_data, 6);
            password.counter++;
            sys_ctrl.delay0 = 500;
            sys_ctrl.sub_sts = 3;
        } else if (button < 5 && button >0){
            for (index = 0; index < 4; index++) {
                sys_ctrl.sign.first[index]  = 20;
                sys_ctrl.sign.second[index] = 20;
            }
            sys_ctrl.sign.step    = 0;
            sys_ctrl.sign.channel = button;
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay1  = AUTO_CLOSE_CNT;
            sys_ctrl.sys_sts = SYS_ST_STANDBY;
        } else if (button < KEY_ID_MAX) {
            sys_ctrl.sys_sts = SYS_ST_STANDBY;
            sys_ctrl.sub_sts = 0;
        }
    } else if (3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STANDBY;
            sys_ctrl.sub_sts = 0;
        }
    }

    /* auto close */
    if (KEY_ID_MAX != button) sys_ctrl.delay1 = AUTO_CLOSE_CNT;
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
        sys_ctrl.sys_sts = SYS_ST_POWEROFF;
        sys_ctrl.sub_sts = 0;
    }
}
static void sys_power_off(void){
    power_cut();
}

static void get_source_data(u8 *data){
    data[0] = password.counter & 0xff;
    data[1] = (password.counter >> 8) & 0xff;
    data[2] = sys_ctrl.sn[0];
    data[3] = sys_ctrl.sn[1];
    data[3] &= 0x0f;
    data[4] = sys_ctrl.sn[0];
    data[5] = sys_ctrl.sn[1];
    data[6] = sys_ctrl.sn[2];
    data[7] = sys_ctrl.sn[3];
    data[7] &= 0x0f;
    switch (sys_ctrl.sign.channel) {
        case 1:
            data[3] |= 0x20;
            data[7] |= 0x20;
            break;
        case 2:
            data[3] |= 0x40;
            data[7] |= 0x40;
            break;
        case 3:
            data[3] |= 0x80;
            data[7] |= 0x80;
            break;
        case 4:
            data[3] |= 0x10;
            data[7] |= 0x10;
            break;
        default:
            break;
    }
}

