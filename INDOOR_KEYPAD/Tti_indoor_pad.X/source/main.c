#include<pic.h>
#include <xc.h>
#include"type.h"
#include"keeloq.h"
#include"remote.h"
#include "usart.h"
#include "timer.h"
#include "clkset.h"
#include "ledTask.h"
#include "main.h"

// #define LED RA5
//__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_ON&MCLRE_OFF&CP_ON&CPD_OFF &BOREN_ON&CLKOUTEN_OFF&IESO_ON&FCMEN_ON);
//__CONFIG(PLLEN_OFF&LVP_OFF) ;

#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN) 
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register) 
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT disabled) 
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is MCLR) 
#pragma config CP = ON          // Code Protection bit (Program memory code protection is disabled) 
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled) 
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled) 
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled) 
#pragma config FCMEN = ON  
#pragma config PLLEN = OFF  
#pragma config LVP = OFF  


static SYS_ST sys_ctrl;
VODET sys_power_on(VODET);
VODET standby(VODET);
VODET door_transmit(VODET);
VODET wifi_transmit(VODET);
VODET match_code(VODET);
VODET sys_init(VODET);
VODET equipmet_uart_send(U08ET data);
VODET sys_power_on(VODET);

FUN sys_fun[] = {
   sys_power_on,
   standby,
   door_transmit,
   wifi_transmit,
   match_code
};


int main(int argc, char** argv){
  sys_init();
  while(1u){
    if (ET_FALSE == sys_task()) continue;
    CLRWDT();
    sys_fun[sys_ctrl.sys_st]();
    led_task();
  }
}

 VODET sys_init(){
  init_fosc();
  init_timer0();
  remote_init();
  Init_Uart();
  led_init();
  /* button inital*/
  ANSELA = 0x00;
  TRISAbits.TRISA2 = 1;
    GIE=1;
  sys_ctrl.delay0 = 0;
  sys_ctrl.sub_st = 0;
  sys_ctrl.sys_st = SYS_POWER_ON;
  sys_ctrl.delay1 = 250;
}
 
 VODET sys_power_on(VODET){
    if (0 == sys_ctrl.sub_st){
        sys_ctrl.delay0 = 0;
        sys_ctrl.delay1 = 4;
        sys_ctrl.sub_st = 1;
    } else if (1 == sys_ctrl.sub_st) {
        if (0 == sys_ctrl.delay0) {
            if (0 == sys_ctrl.delay1) {
                sys_ctrl.sub_st = 0;
                sys_ctrl.sys_st = SYS_STANDBY;
                return;
            }
            sys_ctrl.delay1--;
            led_ctrl(VERSION & (0x1<<sys_ctrl.delay1) ? 400 : 30, 1);
            sys_ctrl.delay0 = 500;
        }
        sys_ctrl.delay0--;
    }
 }
 

 VODET standby(VODET){
    U08ET key;
    if (0 == sys_ctrl.sub_st){
        sys_ctrl.delay1 = 1000;
        sys_ctrl.delay0 = 0;
        sys_ctrl.sub_st = 1;
    }
    
    else if (1 == sys_ctrl.sub_st){
        sys_ctrl.delay1--;
        if ((0 == sys_ctrl.delay1 % 250) && (EQ_NONE != equipment_type())) {
            equipmet_uart_send(0x7f);
            if (0 == sys_ctrl.delay1) {
                if (0xff == get_ack_data())
                    led_ctrl(500, 1);
                sys_ctrl.delay1 = 1000;
            }
        }
        
        if ((sys_ctrl.delay1 % 250) > 6 && (sys_ctrl.delay1 % 250) <247) 
            key = remote_hits(); 
        else key = KEY_NONE;

        if (KEY_DOOR == key){
            sys_ctrl.sys_st = SYS_DOOR_TRANSMIT;
            sys_ctrl.sub_st = 0;
            return;
        } else if ((KEY_6 == key) || (KEY_7 == key) || (KEY_LOCK == key)){
            if (GD200 == equipment_type() && ((KEY_6 == key) || (KEY_7 == key))){
                equipmet_uart_send(key);
                clear_ack();
                led_ctrl(75, 1);
                sys_ctrl.repeat_key = key;
                sys_ctrl.delay0 = 310;            
                sys_ctrl.sub_st = 3;
                return;
            }
            sys_ctrl.sys_st = SYS_WIFI_TRANSMIT;
            sys_ctrl.repeat_key = key;
            sys_ctrl.sub_st = 0;
            return;
        } 
        else if (KEY_NONE != key) {
            equipmet_uart_send(key);
            clear_ack();
            sys_ctrl.repeat_key = key;
            sys_ctrl.delay0 = 310;
            led_ctrl(75, 1);
            sys_ctrl.sub_st = 3;
        }

        if (1 == RA2){
            sys_ctrl.sub_st = 2;
            sys_ctrl.delay0 = 1500;
        }
    }
  
    else if (2 == sys_ctrl.sub_st){
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0){
           sys_ctrl.sys_st = SYS_MATCH_CODE;
           sys_ctrl.sub_st = 0;
        }

        if (0 == RA2) {
           sys_ctrl.sub_st = 0;
        }
    }

    else if (3 == sys_ctrl.sub_st){
        sys_ctrl.delay0--;      
        if (150 == sys_ctrl.delay0) {
            if (get_ack()) {
                led_ctrl(75, 1);
            }
        }
        if (0 == sys_ctrl.delay0){
            sys_ctrl.sub_st = 0;
        }
    }
}
 
VODET door_transmit(VODET){
  U08ET key;
  
  if (0 == sys_ctrl.sub_st){
    sys_ctrl.delay0 = 150;
    sys_ctrl.delay1 = 400;
    sys_ctrl.sub_st = 1;
  }
  else if (1 == sys_ctrl.sub_st){
    key = remote_hits();
   
    /* judge if button keep press*/
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0){        
        repeat_lock();
        clear_ack();
        equipmet_uart_send(KEY_DOOR);          
        sys_ctrl.repeat_key = KEY_DOOR;
        led_ctrl(75, 1);
        sys_ctrl.delay0 = 310;
        sys_ctrl.sub_st = 3;  
        sys_ctrl.sys_st = SYS_STANDBY;
        return;
    }
    
    /* delay to repeat mode*/
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1){
        sys_ctrl.sub_st = 2;
        led_ctrl(75, 125);
        sys_ctrl.delay1 = 50;
        sys_ctrl.delay0 = 400;
        equipmet_uart_send(0x0a);
      
    }
    
    if (KEY_DOOR == key){
        sys_ctrl.delay0 = 150;
    }
  }  
  else if (2 == sys_ctrl.sub_st) {
      key = remote_hits();
      sys_ctrl.delay1--;
      if (0 == sys_ctrl.delay1){
          equipmet_uart_send(0x0a);
          sys_ctrl.delay1 = 50;
      }
      
      sys_ctrl.delay0--;
      if (0 == sys_ctrl.delay0){
          led_ctrl(0, 0);
          sys_ctrl.sub_st = 0;
          repeat_lock();
          sys_ctrl.sys_st = SYS_STANDBY;
      }
      
      if (KEY_DOOR == key){
          sys_ctrl.delay0 = 400;
      }
  }
}

VODET wifi_transmit(VODET){
 
    U08ET key;
  
    if (0 == sys_ctrl.sub_st){
        sys_ctrl.delay0 = 150;
        sys_ctrl.delay1 =  (KEY_LOCK == sys_ctrl.repeat_key) ? 250 : 1500;
        sys_ctrl.sub_st = 1;
    } else if (1 == sys_ctrl.sub_st) {
        key = remote_hits();
   
        /* judge if button keep press*/
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0){
            sys_ctrl.sub_st = 0;
            sys_ctrl.sys_st = SYS_STANDBY;
            repeat_lock();
            return;
        } 
        /* delay to repeat mode*/
        sys_ctrl.delay1--;
        if (0 == sys_ctrl.delay1){
            led_ctrl(75, 1);
            equipmet_uart_send(sys_ctrl.repeat_key);
            clear_ack();
            repeat_lock();
            sys_ctrl.delay0 = 310;
            sys_ctrl.sub_st = 3;
            sys_ctrl.sys_st = SYS_STANDBY;
        } 
        
        if (sys_ctrl.repeat_key == key){
            sys_ctrl.delay0 = 150;
        }
    }
    

}


VODET match_code(VODET){
  if (0 == sys_ctrl.sub_st){
    led_ctrl(250, 0);
    sys_ctrl.delay0 = 200;
    sys_ctrl.delay1 = 2800;
    sys_ctrl.sub_st = 1;
  }
  else if (1 == sys_ctrl.sub_st){
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1){
        sys_ctrl.delay1 = 400;
        remote_clear();
        sys_ctrl.sub_st = 4;
        led_ctrl(50,3);
    }
    
    if (0 == RA2) {
      sys_ctrl.sub_st = 2;
      sys_ctrl.delay1 = 10051;
    }
  } else if (2 == sys_ctrl.sub_st){    
    if (ET_TRUE == remote_learn(sys_ctrl.match_buf, 1)){
        led_ctrl(0,0);
        sys_ctrl.sub_st = 3;
        sys_ctrl.delay1 = 10000;
    }
   
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1){
        sys_ctrl.sub_st = 0;
        sys_ctrl.sys_st = SYS_STANDBY;
        led_ctrl(0,0);
    }
  }
  else if (3 == sys_ctrl.sub_st){
    if (ET_TRUE == remote_learn(sys_ctrl.match_buf, 2)){
        sys_ctrl.sub_st = 0;
        sys_ctrl.sys_st = SYS_STANDBY;
        led_ctrl(50,5);
    }
    
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1){
        sys_ctrl.sub_st = 0;
        sys_ctrl.sys_st = SYS_STANDBY;
        led_ctrl(1000,1);
    }
  }
  else if (4 == sys_ctrl.sub_st){
      sys_ctrl.delay1--;
      if (0 == sys_ctrl.delay1){
        sys_ctrl.sub_st = 0;
        sys_ctrl.sys_st = SYS_STANDBY;    
        led_ctrl(0,0);
    }
  }
}

VODET equipmet_uart_send(U08ET data){
    if (GD200 == equipment_type())
        Uart1SendByte(data);
    else {
        if (KEY_1 == data)
            data = KEY_4;
        
        else if (KEY_4 == data)
            data = KEY_1;
        
        else if (KEY_2 == data)
            data = KEY_3;
        
        else if (KEY_3 == data)
            data = KEY_2;
        
        
        Uart1SendByte(0x01);
        Uart1SendByte(data);
        Uart1SendByte(data + 0x01);
    }
}


