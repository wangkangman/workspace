#include<pic.h>
#include"type.h"
#include"keeloq.h"
#include"remote.h"
#include "usart.h"
#include "timer.h"
#include "clkset.h"
#include "main.h"
#include "SPI.h"

static SYS_ST sys_ctrl;
static VODET main_process();
static VODET sys_init();
void data_construct(u8* buf);
// #define LED RA5
/* for picc complie
__CONFIG(FOSC_INTOSC&WDTE_ON&PWRTE_ON&MCLRE_OFF&CP_ON&CPD_OFF &BOREN_ON&CLKOUTEN_OFF&IESO_ON&FCMEN_ON);
__CONFIG(PLLEN_OFF&LVP_OFF) ;
*/

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


int main(int argc, char** argv){
  sys_init();
  while(1u){
    if (ET_FALSE == sys_task()) continue;
    CLRWDT();
    main_process();
  }
}

static VODET sys_init(){
  init_fosc();
  init_timer0();
  Init_Uart();
  remote_init();
  SPI_init();
  GIE=1;
  sys_ctrl.delay0 = 0;
  sys_ctrl.status = 0;
  sys_ctrl.delay1 = 250;
}

static VODET main_process(){

    U08ET buf[8];

    
    if (ET_TRUE == remote_hits(buf)) {
        data_construct(buf);
        SPI_send_string((U08ET*)buf, 8);
        Uart1SendString((U08ET*)buf, 8);
    }
}


void data_construct(u8* buf){
    u8 temp_buf[8], index;
    for (index = 0; index < 8; index++)temp_buf[index] = buf[index];
    
    buf[0] = temp_buf[4];
    buf[1] = temp_buf[5];
    buf[2] = temp_buf[6];
    buf[3] = temp_buf[7];
    buf[3] &= 0x0f;
    
    buf[4] = temp_buf[2];
    buf[5] = temp_buf[3];
    buf[6] = temp_buf[0];
    buf[7] = temp_buf[1];
}
