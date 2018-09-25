#include "ledTask.h"
#include "pic.h"


static ST_LED st_led;


void led_init(void){
  st_led.period = 0;
  st_led.count  = 0;
  st_led.times = 0;
  st_led.times_cnt = 0;
  /* led */
  TRISA &= ~(1<<4);
  RA4 = 1;
}

void led_task(void){
      /* segment control */
    if(0 != st_led.period) {
        st_led.count++;
        if(st_led.count == st_led.period) {
            if( LED_ON == st_led.stats ) {
                RA4 = 0;
                st_led.stats = LED_OFF;
            } else {
                if (0 != st_led.times){
                    st_led.times_cnt--;
                    if (0 == st_led.times_cnt) {
                        st_led.times  = 0;
                        st_led.times_cnt  = 0;
                        st_led.period = 0;
                        st_led.count  = 0;
                        st_led.stats  = LED_OFF;
                    }   
                }
                RA4 = 1;
                st_led.stats = LED_ON;
            }
            st_led.count = 0;
        }
    }  
}

void led_ctrl(U16ET period, U08ET times){
    if (0 != period) RA4 = 0;
    else RA4 = 1;;
    st_led.times  = times;
    st_led.times_cnt  = times;
    st_led.period = period;
    st_led.count  = 0;
    st_led.stats  = LED_OFF;
}
