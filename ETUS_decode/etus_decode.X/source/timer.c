#include"timer.h"
#include "type.h"
#include "pic.h"
#include "remote.h"

static U08ET sys_cnt = 0; 

void init_timer0(void){
  OPTION_REG=0x81; //????256
  TMR0IF=0;
  TMR0= 208;
  T0IF=0;
  T0IE=1; 
}

void interrupt ISR(void) {
  if(T0IF==1) {
    TMR0=208;
    TMR0IF=0;
    T0IF=0;
    remote_timer_task();
    if (sys_cnt < 50) sys_cnt++;
  }
}

U08ET sys_task(){
  if (sys_cnt < 40) return ET_FALSE;
  sys_cnt = 0;
  return ET_TRUE;
}