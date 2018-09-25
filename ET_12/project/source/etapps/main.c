
#include "main.h"
#include "subs.h"
#if (DBG_LV_CURRENT & DBG_LV_PROCESS)
#include <stdio.h>
#endif


/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static SYS_CTRL sys_ctrl;

const FUNC func[SYS_ST_INVALID] = 
{
    sys_power_on,
    sys_read_eep,
    sys_limit_st,
    sys_limit_dn,
    sys_limit_gt,
    sys_train_dn,
    sys_train_up,
    sys_stay_stop,
    sys_excute_up,
    sys_excute_dn,
    sys_excute_bk,
    sys_system_st,
    sys_remote_st,
    sys_power_off,
    sys_bad_error,
};
/*0~2 down   3~5 up */
const U08ET bk_force[FORCE_LVL_MAX][] = {
    {  60,  60,  30,  60,  50,  30 },
    {  75,  75,  35,  80,  70,  35 },
    {  85,  90,  40,  100,  90,  40 },
    {  100,  100,  45,  120,  110,  45 },
    {  115,  110,  55,  135,  125,  55 },
    {  130,  120,  65,  150,  140,  65 },
    {  145,  130,  75,  165,  155,  75 },
    {  180,  160,  85,  180,  170,  85 },
	
	#if 0
    {  60,  60,  40,  60,  50,  40 },
    {  75,  75,  45,  80,  70,  45 },
    {  85,  90,  50,  100,  90,  55 },
    {  100,  100,  60,  120,  110,  65 },
    {  115,  110,  70,  135,  125,  70 },
    {  130,  120,  80,  150,  140,  80 },
    {  145,  130,  90,  165,  155,  90 },
    {  180,  160,  100,  180,  170,  100 },
    
  #endif
};

/*-----------------------------------------------------------------*/
/* FUNC : MAIN function of the project                             */
/* PARA :                                                          */
/*   NONE                                                          */   
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET main(VODET)
{
    sys_pre_init();
    while(1) {
        if (ET_FALSE == tskskm_flag())
        	continue;           
        abnormal_detect();
        wdtdog_feed();
        ledseg_task();
        keysmp_task();
        adcsmp_task();
        acpowr_task();
        func[sys_ctrl.sys_sts]();
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System initialize                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_pre_init(VODET)
{
    sysclk_init();
    motors_init();
    remote_init();
    ledseg_init();
    adcsmp_init();
    keysmp_init();
    timwdt_init();
    
    powers_init();
#if DBG_LV_CURRENT & DBG_LV_PROCESS
    uartct_init();
#endif
    ET_EI();

    sys_ctrl.sys_sts = SYS_ST_POWER_ON;
    sys_ctrl.sub_sts = 0;
    sys_ctrl.sys_lts = SYS_ST_INVALID;
    sys_ctrl.sub_lts = 0;
    sys_ctrl.lst_dir = MOT_CTS_DOWN;
    sys_ctrl.opn_pos = 0;
    sys_ctrl.bit_flg.bit0 = ET_FALSE;
    sys_ctrl.bit_flg.bit1 = ET_FALSE;
    sys_ctrl.bit_flg.bit2 = ET_FALSE;
    sys_ctrl.bit_flg.bit3 = ET_FALSE;
    sys_ctrl.bit_flg.bit4 = ET_FALSE;
    sys_ctrl.bit_flg.bit5 = ET_FALSE;
    sys_ctrl.bit_flg.bit6 = ET_FALSE;
    sys_ctrl.bit_flg.bit7 = ET_FALSE;

#if DBG_LV_CURRENT & DBG_LV_PROCESS
    printf("System initialize OK\r\n");
#endif
}

/*-----------------------------------------------------------------*/
/* FUNC : System power on                                          */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_power_on(VODET)
{
    /* display system version */
    if(0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_power_on>\r\n\r\n"); 
				#endif
        sys_ctrl.delay0 = 125;
				sys_ctrl.delay1 = PRG_MAIN_VER*2;
        sys_ctrl.sub_sts = 1;
    }
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sys_ctrl.delay0 = 125;						
            if(0 == sys_ctrl.delay1) {
                sys_ctrl.sys_sts = SYS_ST_READ_EEP;
                sys_ctrl.sub_sts = 0;
            }
            else if(0 == (sys_ctrl.delay1%2)) {
                ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 0);
            }
            else {
                ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            }
            sys_ctrl.delay1--;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Read eeprom data                                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_read_eep(VODET)
{
    U08ET *pbuf;
    U16ET size,index;
    #if DBG_LV_CURRENT & DBG_LV_PROCESS
		U16ET	 temp;
		#endif

    /* read system control */
		#if DBG_LV_CURRENT & DBG_LV_PROCESS
		printf("jump into <sys_read_eep>\r\n\r\n"); 
		#endif
    pbuf = (U08ET *)(&glb_ctrl.save_flg);
    size = (U08ET *)(&glb_ctrl.save_end) - (U08ET *)(&glb_ctrl.save_flg);
    for (index = 0; index < size; index++) {
        pbuf[index] = eepbyt_read(index);
    }
    wdtdog_feed();
    #if DBG_LV_CURRENT & DBG_LV_PROCESS
    
		temp = glb_ctrl.save_flg;
		printf("save flag == 0x%x \r\n",temp);
		#endif
    


    /* read system open force */
    pbuf = glb_ctrl.upForce;
    for (index = 0; index < UP_FORCE_SAMP; index++) {
        pbuf[index] = eepbyt_read(index+256);
    }
    wdtdog_feed();

    /* read system close force */
    pbuf = glb_ctrl.dnForce;
    for (index = 0; index < DN_FORCE_SAMP; index++) {
        pbuf[index] = eepbyt_read(index + 256 + UP_FORCE_SAMP);
    }
    wdtdog_feed();

    /* check saved flag */
    if (EEP_SAVE_FLG != glb_ctrl.save_flg) {
      #if DBG_LV_CURRENT & DBG_LV_PROCESS
    	printf("eep flag_1 \r\n");
			#endif
        glb_ctrl.save_flg = EEP_SAVE_FLG;
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        glb_ctrl.hall_cur = 0;
        glb_ctrl.force_lv = 2;
        glb_ctrl.auto_clo = 0;
        glb_ctrl.LorR_Set = getkey_stabe(KEY_ID_LR);

        sys_ctrl.bit_flg.bit0 = ET_TRUE;
        sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
        sys_ctrl.sub_sts = 0;
        return ;
    }

    /* check if route parameter is normal */
    if ((glb_ctrl.hall_top >= glb_ctrl.hall_bot) ||\
    	  (glb_ctrl.hall_bot > HAL_CNT_MAX)) {
    	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
    	  printf("eep flag_2 \r\n");
			  #endif
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        glb_ctrl.hall_cur = 0;
    }

    /* set current motor status */
    halsig_scnt(glb_ctrl.hall_cur);
    #if DBG_LV_CURRENT & DBG_LV_PROCESS
    temp = glb_ctrl.hall_cur;
    printf("glb_ctrl.hall_cur : %d \r\n", glb_ctrl.hall_cur);
		#endif
    if (glb_ctrl.hall_cur <= glb_ctrl.hall_top) {    
    	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
    	  printf("eep flag_3 \r\n");
			  #endif	
        sys_ctrl.lst_dir = MOT_CTS_UPUP;
    }

    /* check force level */
    if (glb_ctrl.force_lv > FORCE_LVL_MAX-1) {
        glb_ctrl.force_lv = 2;
    }
    /* check auto close minutes */
    glb_ctrl.auto_clo = 1;
    if (glb_ctrl.auto_clo > 9) {
        glb_ctrl.auto_clo = 0;
    }

    sys_ctrl.bit_flg.bit0 = ET_TRUE;
    sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
    sys_ctrl.sub_sts = 0;

    return ;
}

/*-----------------------------------------------------------------*/
/* FUNC : system limit set                                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_limit_st(VODET)
{
    /* initialize parameter */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_limit_st>\r\n\r\n"); 
				#endif
				
        ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 250);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 500);
        halsig_scnt(HAL_BOT_DFT);
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        glb_ctrl.hall_cur = 0;
        getkey_stabe(KEY_ID_LMT0);
        getkey_stabe(KEY_ID_LMT1);
        sub_save_sysctl(0);
        remote_hits();
        sys_ctrl.lst_dir = MOT_CTS_DOWN;
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay1 = 10000;
    }
    /* reponse to remote control */
    else if (1 == sys_ctrl.sub_sts) {
        if (ET_TRUE == remote_hits()) {
        		#if DBG_LV_CURRENT & DBG_LV_PROCESS
						printf("<sys_limit_st>:into sub_sts->1 \r\n"); 
						#endif
            if (MOT_CTS_DOWN == sys_ctrl.lst_dir) {
                motors_ctrl(MOT_CTS_UPUP);
                halsig_sdir(MOT_CTS_UPUP);
                sys_ctrl.lst_dir = MOT_CTS_UPUP;
                ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 100);
            }
            else {
                motors_ctrl(MOT_CTS_DOWN);
                halsig_sdir(MOT_CTS_DOWN);
                sys_ctrl.lst_dir = MOT_CTS_DOWN;
                ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 50);
            }

            if (KEY_ST_HIGH == getkey_curst(KEY_ID_LMT0)) {
								
                motors_ctrl(MOT_CTS_UPUP);
                halsig_sdir(MOT_CTS_UPUP);
                sys_ctrl.lst_dir = MOT_CTS_UPUP;
                ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 100);
            }
            
            if (KEY_ST_HIGH == getkey_curst(KEY_ID_LMT1)) {
                motors_ctrl(MOT_CTS_DOWN);
                halsig_sdir(MOT_CTS_DOWN);
                sys_ctrl.lst_dir = MOT_CTS_DOWN;
                ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 50);
            }

            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0  = 10;
            sys_ctrl.delay1  = 10000;
        //    ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 50);
        }

        if (KEY_ST_HIGH == getkey_stabe(KEY_ID_SET)) {
            sys_ctrl.sys_sts = SYS_ST_LIMIT_DN;
            sys_ctrl.sub_sts = 0;
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            return ;
        }

//        sys_ctrl.delay1--;
//        if (0 == sys_ctrl.delay1) {
//            glb_ctrl.hall_top = 0;
//            glb_ctrl.hall_bot = 0;
//            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
//            sys_ctrl.sub_sts = 0;
//        }
    }
    /* delay to start motor */
    else if (2 == sys_ctrl.sub_sts) 
    {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) 
        {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 3;
        }
    }
    /* run motor up or down */
    else if (3 == sys_ctrl.sub_sts) 
    {
        sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
        motors_pwms(sys_ctrl.cur_pwm);

        /* cut off motor power */
/* 2017/7/14 星期五 下午 4:17:03
        if (ET_TRUE == remote_hits() || \
            KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT0) ||\
            KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT1)) 
        {
            motors_pwms(0);
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 100;
        }
        2017/7/14 星期五 下午 4:17:03 */
        if(ET_TRUE == remote_hits()){
        		motors_pwms(0);
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 100;
        	}
        else if(KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT0) && sys_ctrl.lst_dir == MOT_CTS_DOWN){
	       		motors_pwms(0);
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 100;
        }
        else if(KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT1) && sys_ctrl.lst_dir == MOT_CTS_UPUP){
        		motors_pwms(0);
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 100;
        }
    }

    /* delay to stop motor */
    else if (4 == sys_ctrl.sub_sts) 
    {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) 
        {
            motors_ctrl(MOT_CTS_STOP);
            ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 250);
            sys_ctrl.sub_sts = 1;
            sys_ctrl.delay1 = 10000;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : motor go to down limit                                   */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_limit_dn(VODET)
{
    /* determine if need to run down */
    if (0 == sys_ctrl.sub_sts) 
    {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_limit_dn>\r\n\r\n"); 
				#endif
        if (KEY_ST_HIGH == getkey_curst(KEY_ID_LMT0)) 
        {
            sys_ctrl.sys_sts = SYS_ST_LIMIT_GT;
            sys_ctrl.sub_sts = 0;
        }
        else 
        {
            sys_ctrl.sub_sts = 1;
            sys_ctrl.delay0 = 10;
            sys_ctrl.delay2 = 0;
            sys_ctrl.train_jump = 0;
            halsig_sdir(MOT_CTS_DOWN);
            motors_ctrl(MOT_CTS_DOWN);
            halsig_scnt(HAL_BOT_DFT);
            ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 50);
        }
    }
    /* start motor power */
    else if (1 == sys_ctrl.sub_sts) 
    {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) 
        {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until bottom side */
    else if(2 == sys_ctrl.sub_sts)
    {
        sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit bottom side, stop motor */
        if (KEY_ST_HIGH==getkey_stabe(KEY_ID_LMT0))
        {
            motors_pwms(0);
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 100;
        }
        
        /**************auto stop **************************/  
				++sys_ctrl.delay2;
		    if (sys_ctrl.delay2 == AUTO_STOP) {
     	  	motors_pwms(0);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 100;
          sys_ctrl.train_jump = 1;
		      return;
		    }
    }
    /* stop motor power */
    else if (3 == sys_ctrl.sub_sts){
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 50);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if (4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
        	if (0 == sys_ctrl.train_jump) {
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            sys_ctrl.sys_sts = SYS_ST_LIMIT_GT;
            sys_ctrl.sub_sts = 0;
          } else {
						glb_ctrl.hall_top = 0;
        		glb_ctrl.hall_bot = 0;
        		glb_ctrl.hall_cur = 0;
        		ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
						sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;          	
          }
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : motor go to down limit                                   */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_limit_gt(VODET)
{
    U16ET hallCnt;

    hallCnt = halsig_gcnt();
    
    /* motor direction up */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_limit_gt>\r\n\r\n"); 
				#endif
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        sys_ctrl.delay2 = 0;  
				sys_ctrl.train_jump = 0;
        halsig_sdir(MOT_CTS_UPUP);
        motors_ctrl(MOT_CTS_UPUP);
        halsig_scnt(HAL_BOT_DFT);
        ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 50);
    }
    /* start motor power */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until top side */
    else if (2 == sys_ctrl.sub_sts) {
        sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit bottom side, stop motor */
        if (KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT1)) {
            motors_pwms(0);
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 100;
        }
        
        /**************auto stop **************************/  
				++sys_ctrl.delay2;
		    if (sys_ctrl.delay2 == AUTO_STOP) {
     	  	motors_pwms(0);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 100;
          sys_ctrl.train_jump = 1;
		      return;
		    }
    }
    /* stop motor power */
    else if (3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if (4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
        	if (0 == sys_ctrl.train_jump){
            glb_ctrl.hall_bot = HAL_BOT_DFT;
            glb_ctrl.hall_top = hallCnt;
            glb_ctrl.hall_cur = hallCnt;
            halsig_scnt(glb_ctrl.hall_cur);
						#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("Hall Top:%d\r\n", glb_ctrl.hall_top);
            printf("Hall Bot:%d\r\n", glb_ctrl.hall_bot);
            printf("Hall Cur:%d\r\n", hallCnt);
						#endif
            sys_ctrl.sys_sts = SYS_ST_TRAIN_DN;
            sys_ctrl.sub_sts = 0;
          } else {
						glb_ctrl.hall_top = 0;
        		glb_ctrl.hall_bot = 0;
        		glb_ctrl.hall_cur = 0;
        		ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
						sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
          }
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Learn open door force                                    */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_train_dn(void)
{
    U16ET hallCnt, hallSmp, index;
    U08ET adcValue;

    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + DN_FORCE_SAMP - 1)/DN_FORCE_SAMP;

    /* motor direction down */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_train_dn>\r\n\r\n"); 
				#endif				
        for (index = 0; index < DN_FORCE_SAMP; index ++){
          glb_ctrl.dnForce[index] = 0;
        }			
        	
				sys_ctrl.train_er_flg = 0;
				sys_ctrl.train_jump = 0;
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay2 = 0;
        sys_ctrl.delay0 = 10;
        glb_ctrl.dfor_max = 0;
        halsig_sdir(MOT_CTS_DOWN);
        motors_ctrl(MOT_CTS_DOWN);
        ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 50);
    }
    /* start motor power */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until bottom side */
    else if (2 == sys_ctrl.sub_sts)
    {
        if (hallCnt < glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP)
           sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
        else
        	 sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);
        
        motors_pwms(sys_ctrl.cur_pwm);
      
        /* hit bottom side, stop motor */
        if (KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT0)) {
        	 index = (hallCnt - glb_ctrl.hall_top) / hallSmp;
//        	 if (index < ((glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp)) {
//        	 		for (; index < ((glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp); ++index ){
//        	 			glb_ctrl.dnForce[index] = adcValue;
//        	 		}
//        	 }
     	  	 motors_pwms(0);
           sys_ctrl.sub_sts = 3;
           sys_ctrl.delay0 = 100;
           return;
        }        
        
        
        /* learn force of the door close duration */
        if ((hallCnt >= glb_ctrl.hall_top) && (hallCnt <= glb_ctrl.hall_bot)) {
            if (hallCnt < glb_ctrl.hall_top) hallCnt = glb_ctrl.hall_top;
            else if (hallCnt > glb_ctrl.hall_bot) hallCnt = glb_ctrl.hall_bot; 
            index = (hallCnt - glb_ctrl.hall_top) / hallSmp;
            glb_ctrl.dnForce[index] = (adcValue > glb_ctrl.dnForce[index]) ? adcValue : glb_ctrl.dnForce[index];
            if (adcValue > glb_ctrl.dfor_max) 
            	  glb_ctrl.dfor_max = adcValue;
            if ((hallCnt >= glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP) && (hallCnt < glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP + 60))
            	glb_ctrl.dnForce[index] = glb_ctrl.dnForce[((glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP - glb_ctrl.hall_top) / hallSmp) - 2];            
        }
        
        /**************auto stop **************************/  
				++sys_ctrl.delay2;
		    if (sys_ctrl.delay2 == AUTO_STOP) {
     	  	motors_pwms(0);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 100;
          sys_ctrl.train_jump = 1;
		      return;
		    }
    }
    
    /* stop motor power */
    else if (3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            index = DN_FORCE_SAMP - 1;
            while(glb_ctrl.dnForce[index] == 0){
              	index--;
            }
            for (; index < DN_FORCE_SAMP; index ++){
                glb_ctrl.dnForce[index] = glb_ctrl.dnForce[index - 1];
            }
            
            motors_ctrl(MOT_CTS_STOP);
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 4;
            if ((hallCnt > glb_ctrl.hall_bot && hallCnt - glb_ctrl.hall_bot > 500) || \
           		  (hallCnt < glb_ctrl.hall_bot && glb_ctrl.hall_bot - hallCnt > 500)) {
       				sys_ctrl.train_er_flg = 1;
        	  }            
        }
    }
    
    /* motor have a break */
    else if (4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
        	if (0 == sys_ctrl.train_jump){
						sys_ctrl.sys_sts = SYS_ST_TRAIN_UP;
            sys_ctrl.sub_sts = 0;
						#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("MAX:%d\r\n",(U16ET)glb_ctrl.dfor_max);
            for(index=0; index<DN_FORCE_SAMP; index++) {
                printf("%d\r\n", (U16ET)glb_ctrl.dnForce[index]);
                wdtdog_feed();
            }
						#endif
					} else {
						glb_ctrl.hall_top = 0;
        		glb_ctrl.hall_bot = 0;
        		glb_ctrl.hall_cur = 0;
        		ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
						sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
					}
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System stay sleep mode                                   */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_train_up(void)
{
    U16ET hallCnt, hallSmp, index;
    U08ET adcValue;

    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1)/UP_FORCE_SAMP;

    /* motor direction down */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_train_up>\r\n\r\n"); 
				#endif
        for (index = 0; index < UP_FORCE_SAMP; index ++){
          glb_ctrl.upForce[index] = 0;
        }			
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        sys_ctrl.train_jump = 0;
        glb_ctrl.ufor_max = 0;
        halsig_sdir(MOT_CTS_UPUP);
        motors_ctrl(MOT_CTS_UPUP);
        ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 100);
    }
    /* start motor power */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until top side */
    else if (2 == sys_ctrl.sub_sts) {
        if (hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP)
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
        else
            sub_sped_stable(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, HAL_DUTY_SLOW);
            
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit bottom side, stop motor */
        if (KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT1)) {
        	 index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;
//        	 if (index < ((glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp)) {
//        	 		for (; index < ((glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp); ++index ){
//        	 			glb_ctrl.upForce[index] = adcValue;
//        	 		}
//        	 }
     	  	 motors_pwms(0);
           sys_ctrl.sub_sts = 3;
           sys_ctrl.delay0 = 100;
           return;
        }
        
        /* learn force of the door open duration */
        if ((hallCnt >= glb_ctrl.hall_top) && \
        	  (hallCnt <= glb_ctrl.hall_bot)) {
            if (hallCnt < glb_ctrl.hall_top)       hallCnt = glb_ctrl.hall_top;
            else if (hallCnt > glb_ctrl.hall_bot)  hallCnt = glb_ctrl.hall_bot;
            index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;
            glb_ctrl.upForce[index] = (adcValue > glb_ctrl.upForce[index]) ? adcValue : glb_ctrl.upForce[index];
            if (adcValue > glb_ctrl.ufor_max) {
                glb_ctrl.ufor_max = adcValue;
            }
            if ((hallCnt <= glb_ctrl.hall_top + HAL_CNT_SLOW_STOP) && (hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP - 60))
            	  glb_ctrl.upForce[index] = glb_ctrl.upForce[((glb_ctrl.hall_bot - glb_ctrl.hall_top - HAL_CNT_SLOW_STOP) / hallSmp) - 2];            
            
        }
        
        /*****auto stop***********/
        ++sys_ctrl.delay2;
		    if (sys_ctrl.delay2 == AUTO_STOP) {
     	  	motors_pwms(0);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 100;
          sys_ctrl.train_jump = 1;
		      return;
		    }
    }
    /* stop motor power */
    else if (3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
        	
            index = UP_FORCE_SAMP - 1;
            while(glb_ctrl.upForce[index] == 0){
              	index--;
            }
            for (; index < UP_FORCE_SAMP; index ++){
                glb_ctrl.upForce[index] = glb_ctrl.upForce[index - 1];
            }
            
            motors_ctrl(MOT_CTS_STOP);
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 4;
            if ((hallCnt > glb_ctrl.hall_top && hallCnt - glb_ctrl.hall_top > 500) || \
             		(hallCnt < glb_ctrl.hall_top && glb_ctrl.hall_top - hallCnt > 500)) {
       				sys_ctrl.train_er_flg = 1;
        	  }  
        }
    }


    /* motor have a break */
    else if (4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
        	if (0 == sys_ctrl.train_er_flg && sys_ctrl.train_jump == 0) {
        		  ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
	            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
	            sys_ctrl.sub_sts = 0;
	            sys_ctrl.bit_flg.bit0 = ET_TRUE;
	            sys_ctrl.lst_dir = MOT_CTS_UPUP;
	            sub_save_sysctl(1);
							#if DBG_LV_CURRENT & DBG_LV_PROCESS
	            printf("MAX:%d\r\n",(U16ET)glb_ctrl.ufor_max);
	            for(index=0; index<UP_FORCE_SAMP; index++) {
	                printf("%d\r\n", (U16ET)glb_ctrl.upForce[index]);
	                wdtdog_feed();
	            }
							#endif
					} else {
						glb_ctrl.hall_top = 0;
        		glb_ctrl.hall_bot = 0;
        		glb_ctrl.hall_cur = 0;
        		ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
						sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
	          sys_ctrl.sub_sts = 0;
	          sys_ctrl.bit_flg.bit0 = ET_TRUE;
	          sys_ctrl.lst_dir = MOT_CTS_UPUP;
					}
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System stay stop mode                                    */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_stay_stop(void)
{
    U16ET hallCnt;
    #if DBG_LV_CURRENT & DBG_LV_PROCESS
    U16ET temp;
    #endif

    hallCnt = halsig_gcnt();

    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_stay_stop>\r\n\r\n"); 			
				#endif
        remote_hits();
        getkey_stabe(KEY_ID_WALL);
        ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
        ledtsk_ctrl(LED_ID_CODE, LED_ST_OFF, 0);
        sys_ctrl.clo_sec = 60;
        sys_ctrl.clo_cnt = 500;
        sys_ctrl.lmp_sec = 180;
        sys_ctrl.lmp_cnt = 500;
        sys_ctrl.sub_sts = 1;
        return ;
    }

		else if (1 == sys_ctrl.sub_sts) {
			 /* door auto close */
			if(KEY_ST_HIGH == getkey_curst(KEY_ID_AUTO_CLOSE) && \
				 KEY_ST_HIGH != getkey_stabe(KEY_ID_IRAD) && \
				 KEY_ST_HIGH == getkey_curst(KEY_ID_LMT1)){
		    if (sys_ctrl.clo_cnt > 0){
		      	sys_ctrl.clo_cnt--;
		    } else {
		      if (0 == sys_ctrl.clo_sec){
		        if (0 != glb_ctrl.hall_top && 0 != glb_ctrl.hall_bot) {	
		          sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
		          sys_ctrl.sub_sts = 0;
		          return ;
		        }
		      } else{
		        sys_ctrl.clo_sec--;
		        sys_ctrl.clo_cnt = 500;
		      }
		    }
		  } else {
        sys_ctrl.clo_sec = 60;
        sys_ctrl.clo_cnt = 500;
		  }
		  
	    
	    /* lamp auto off */
	    if (sys_ctrl.lmp_cnt > 0)
	        sys_ctrl.lmp_cnt--;
	    else {
	        if (0 == sys_ctrl.lmp_sec) {
	            ledtsk_ctrl(LED_ID_LAMP, LED_ST_OFF, 0);
	        }
	        else {
	            sys_ctrl.lmp_sec--;
	            sys_ctrl.lmp_cnt = 500;
	        }
	    }
	    
	    /* SET press detected, enter system set mode */
	    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_SET)) {
	        sys_ctrl.sys_sts = SYS_ST_SYSTEM_ST;
	        sys_ctrl.sub_sts = 0;
	    }
	    /* CODE press detected, enter remote set mode */
	    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_CODE)) {
	        sys_ctrl.sys_sts = SYS_ST_REMOTE_ST;
	        sys_ctrl.sub_sts = 0;
	    }
	    
	    /**get force lv */
	    glb_ctrl.force_lv=getdriver_level();
    
    	/* detect LorR set */
	    if (glb_ctrl.LorR_Set != getkey_stabe(KEY_ID_LR) && \
	    	  KEY_ST_MAX != getkey_stabe(KEY_ID_LR)){
	    	  glb_ctrl.LorR_Set = getkey_stabe(KEY_ID_LR);
	    	  glb_ctrl.hall_top = 0;
	        glb_ctrl.hall_bot = 0;
	        glb_ctrl.hall_cur = 0;
	        sub_save_sysctl(0);
	    }
	    
	    if (ET_TRUE == remote_hits() || KEY_ST_HIGH == getkey_stabe(KEY_ID_WALL)){
	    	  if (0 != glb_ctrl.hall_top && 0 != glb_ctrl.hall_bot) {
		        if (MOT_CTS_DOWN == sys_ctrl.lst_dir) { //UP   control
		        		#if DBG_LV_CURRENT & DBG_LV_PROCESS
		        		printf("flag_2");
		        		#endif
		            if ( KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT1)) { //top sensor
		            		#if DBG_LV_CURRENT & DBG_LV_PROCESS
		            		printf("flag_3");
		            		#endif
		                sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
		                sys_ctrl.sub_sts = 0;
		            } else {
		                sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
		                sys_ctrl.sub_sts = 0;
		            }
		        } else {	//down control
		            if (KEY_ST_HIGH == getkey_stabe(KEY_ID_IRAD)) {
		                if (KEY_ST_LOW == getkey_stabe(KEY_ID_LMT1)) {
		                    sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
		                    sys_ctrl.sub_sts = 0;
		                }
		            } else {
		            	  if (KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT0)) { //bot sensor
			                sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
			                sys_ctrl.sub_sts = 0;
		           			}
										else {
			                sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
			                sys_ctrl.sub_sts = 0;
		              	}
		            }
		        }
	      	} else { // no limit data
		        ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 150);
		        ledtsk_ctrl(LED_ID_CODE, LED_ST_ON, 150);
	      		sys_ctrl.sub_sts = 2;
	      		sys_ctrl.delay0 = 900;
	      		return;
	      	}
	    }
    } 
    else if (2 == sys_ctrl.sub_sts) {
    	--sys_ctrl.delay0;
    	if (0 == sys_ctrl.delay0){
    		sys_ctrl.sub_sts = 1;
    		ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
		    ledtsk_ctrl(LED_ID_CODE, LED_ST_OFF, 0);
    		return;
    	}
    }
    
}
/******************************************************************************
  * FUNC : Sub func of excute_up                                  
  * PARA :                                                          
  *   NONE                                                          
  * RETN :                                                          
  *   NONE                                                          
  *****************************************************************************
  **/
VODET UP_FORCE_STOP()
{ 
	motors_pwms(0);
  sys_ctrl.sub_sts = 3;
	sys_ctrl.delay0 = 200;
}


/*-----------------------------------------------------------------*/
/* FUNC : System excute open door                                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_excute_up(void)
{
    U16ET hallCnt, hallSmp, index;
    U08ET adcValue;
    U16ET force_res, calcForce;
    
    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1)/UP_FORCE_SAMP;

    if (ET_TRUE     == remote_hits() || \
    	  KEY_ST_HIGH == getkey_stabe(KEY_ID_WALL)) {
    	  sys_ctrl.bit_flg.bit6 = ET_FALSE;
        UP_FORCE_STOP();
        return;
    }

    /* motor direction up */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_excute_up>\r\n\r\n"); 
				#endif
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        sys_ctrl.delay2 = 0;
    	  sys_ctrl.bit_flg.bit7 = ET_FALSE;
        halsig_sdir(HAL_DIR_UPUP);
        motors_ctrl(MOT_CTS_UPUP);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
        ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 100);
    }
    /* start motor power */
    else if (1 == sys_ctrl.sub_sts){
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0){
            sub_clrs_status();
            sys_ctrl.delay0 = 1500;
            sys_ctrl.delay1 = 300;
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until top side */
    else if (2 == sys_ctrl.sub_sts){
        /* calculate force sampling position */
        if (hallCnt > glb_ctrl.hall_bot) {
            index = 0;
        }
        else if (hallCnt < glb_ctrl.hall_top) {
            index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
        }
        else {
            index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;
        }
				
				/* normal route overforce judge */
        if (hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP) {
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);

            if (sys_ctrl.delay0 > 0) {
                sys_ctrl.delay0--;
                /* Resistance rebound in starting duration */
                calcForce = glb_ctrl.ufor_max;
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][3]) > AD_MAX ? AD_MAX : (glb_ctrl.ufor_max + bk_force[glb_ctrl.force_lv][3]);
                if (adcValue > force_res) {
//                if (adcValue > (glb_ctrl.ufor_max+bk_force[glb_ctrl.force_lv][3])) {
                	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
										printf("starting duration -> stop\r\n"); 
										#endif
										sys_ctrl.bit_flg.bit6 = ET_FALSE;
                    UP_FORCE_STOP();
                    return;
                }
            }
            else {
                /* Resistance rebound in normal duration */
                calcForce = sub_maxi_values(glb_ctrl.upForce[index-1], glb_ctrl.upForce[index], glb_ctrl.upForce[index+1]);
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][4]) > AD_MAX ? AD_MAX : (calcForce + bk_force[glb_ctrl.force_lv][4]);
								if (adcValue > force_res) {
//                if (adcValue > (calcForce+bk_force[glb_ctrl.force_lv][4])) {
                		#if DBG_LV_CURRENT & DBG_LV_PROCESS
										printf("normal duration -> stop\r\n"); 
										#endif
					    	  	sys_ctrl.bit_flg.bit6 = ET_FALSE;
                    UP_FORCE_STOP();
                    return;
                }
            }
        }
        
        /* stop route overforce judge */
        else {
            //sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);
            sub_sped_stable(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, HAL_DUTY_SLOW);
            /* Resistance rebound in slow stop duration */
            if ((sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0)) {
                if (sys_ctrl.delay0 > 0) {
                    sys_ctrl.delay0--;
                }
                if (sys_ctrl.delay1 > 0) {
                    sys_ctrl.delay1--;
                }
                calcForce = glb_ctrl.ufor_max;
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][3]) > AD_MAX ? AD_MAX : (glb_ctrl.ufor_max + bk_force[glb_ctrl.force_lv][3]);
                if (adcValue > force_res) {
//                if (adcValue > (glb_ctrl.ufor_max+bk_force[glb_ctrl.force_lv][3])) {
                	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
										printf(" delaytime slow stop duration -> stop\r\n"); 
										#endif	
										sys_ctrl.bit_flg.bit6 = ET_FALSE;
                    UP_FORCE_STOP();
                    return;
                }
            }
            else {
                calcForce = sub_maxi_values(glb_ctrl.upForce[index-1], \
                  glb_ctrl.upForce[index], glb_ctrl.upForce[index+1]);
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][5]) > AD_MAX ? AD_MAX : (calcForce + bk_force[glb_ctrl.force_lv][5]);
								if (adcValue > force_res) {
//                if (adcValue > (calcForce+bk_force[glb_ctrl.force_lv][5])){
                	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
										printf("no delaytime slow stop duration -> stop\r\n"); 
										#endif	
                	  sys_ctrl.bit_flg.bit6 = ET_FALSE;
                    UP_FORCE_STOP();
                    return;
                }
            }
        }
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit top side, stop motor */

/* 2017/6/17 星期六 上午 8:16:03 -sensor skew counter-------------*/

        if (KEY_ST_HIGH == getkey_stabe(KEY_ID_LMT1)) {

          sys_ctrl.bit_flg.bit7 = ET_TRUE;
          UP_FORCE_STOP();
          return;
        }
/* 2017/6/17 星期六 上午 8:16:03 -sensor skew counter-------------*/
    /**************auto stop*****************/
			++sys_ctrl.delay2;
	    if (sys_ctrl.delay2 == AUTO_STOP) {
	    	sys_ctrl.bit_flg.bit7 = ET_FALSE;
	    	UP_FORCE_STOP();
	      return;
	    }
    }
    /* delay sometime to stop motor */
    else if (3 == sys_ctrl.sub_sts)
    {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0)
        {
            motors_ctrl(MOT_CTS_STOP);
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            sys_ctrl.delay0 = 60;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if (4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
            sys_ctrl.opn_pos = 0;
            sys_ctrl.lst_dir = MOT_CTS_UPUP;
            if (sys_ctrl.bit_flg.bit7 == ET_TRUE && sys_ctrl.bit_flg.bit6 == ET_TRUE){
            	  if( (glb_ctrl.hall_top > hallCnt && glb_ctrl.hall_top - hallCnt < 80) || \
            	  	  (glb_ctrl.hall_top < hallCnt && hallCnt - glb_ctrl.hall_top < 80) )
            	 glb_ctrl.hall_top = hallCnt;
               sys_ctrl.bit_flg.bit7 == ET_FALSE;
               sys_ctrl.bit_flg.bit6 == ET_FALSE;
            }
            
						#if LIMIT_SEN_ERR     	
						if (hallCnt < glb_ctrl.hall_top - LIMIT_SEN_HALL_TOLERANCE){   
	        	  limit_sen_cnt_plus();        	 
	        	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
	        	  printf("up limit sensor out of range\r\n");
	        	  #endif
	       	  }
	        	else
	            limit_sen_cnt_min();
						#endif
						
#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("DOOR UP OK:\r\n");
            printf("Top hall: %d\r\n", glb_ctrl.hall_top);
            printf("Bot hall: %d\r\n", glb_ctrl.hall_bot);
            printf("glb Cur hall: %d\r\n", halsig_gcnt());
            printf("Cur hall: %d\r\n", hallCnt);
#endif
        }
    }
    return ;

}

/******************************************************************************
  * FUNC : Sub func of excute_dn                                  
  * PARA :                                                          
  *   NONE                                                          
  * RETN :                                                          
  *   NONE                                                          
  *****************************************************************************
  **/
VODET DN_FORCE_STOP()
{
	 motors_pwms(0);
  #if DBG_LV_CURRENT & DBG_LV_PROCESS
   printf("DN FORCE STOP:\r\n");
	#endif
   sys_ctrl.sub_sts = 3;
   sys_ctrl.delay0 = 200;
}

/******************************************************************************
  * FUNC : Sub func of excute_dn                                  
  * PARA :                                                          
  *   NONE                                                          
  * RETN :                                                          
  *   NONE                                                          
  *****************************************************************************
  **/
VODET DN_FORCE_BACK()
{
	 if (ET_FALSE == sys_ctrl.bit_flg.bit1) {
   sys_ctrl.bit_flg.bit2 = ET_TRUE;
   }
   sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
   sys_ctrl.sub_sts = 0;
}
/*-----------------------------------------------------------------*/
/* FUNC : System excute close door                                 */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_excute_dn(void)
{
    U16ET hallCnt, hallSmp, index, current_force;
    U08ET adcValue;
    U16ET calcForce, force_res;

    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + DN_FORCE_SAMP - 1)/DN_FORCE_SAMP;

    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_IRAD)) {
        sys_ctrl.bit_flg.bit1 = ET_TRUE;
			  if (ET_FALSE == sys_ctrl.bit_flg.bit1) {
		    sys_ctrl.bit_flg.bit2 = ET_TRUE;
		    }
		    sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
		    sys_ctrl.sub_sts = 0;
        return;
    }

    if (ET_TRUE == remote_hits() || KEY_ST_HIGH == getkey_stabe(KEY_ID_WALL)) {
        DN_FORCE_STOP();
        return;
    }

    /* motor direction down */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_excute_dn>\r\n\r\n"); 
				#endif
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        sys_ctrl.delay2 = 0;
        sys_ctrl.bit_flg.bit1 = ET_FALSE;
        sys_ctrl.bit_flg.bit2 = ET_FALSE;
        sys_ctrl.bit_flg.bit7 = ET_FALSE;
        sys_ctrl.bit_flg.bit6 = ET_FALSE;        
        halsig_sdir(HAL_DIR_DOWN);
        motors_ctrl(MOT_CTS_DOWN);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
        ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 50);
    }
    /*   start motor power  */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.delay0 = 1000;
            sys_ctrl.delay1 = 300;
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until bottom side */
    else if (2 == sys_ctrl.sub_sts) {
        /* calculate force sampling position */
        if (hallCnt < glb_ctrl.hall_top) {
            index = 0;
        }
        else if (hallCnt > glb_ctrl.hall_bot) {
            index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
        } else {
            index = (hallCnt - glb_ctrl.hall_top) / hallSmp;
        }

        if (hallCnt < glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP) {
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
            
            if (sys_ctrl.delay0 > 0) {
                sys_ctrl.delay0--;
                /* Resistance rebound in starting duration */
                calcForce = glb_ctrl.dfor_max;
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][0]) > AD_MAX ? AD_MAX : (calcForce + bk_force[glb_ctrl.force_lv][0]);
                if (adcValue > force_res) {
//                if (adcValue > (glb_ctrl.dfor_max + bk_force[glb_ctrl.force_lv][0])) {
                	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
    								printf("resis tence rebound start :\r\n");
										#endif
									  if (ET_FALSE == sys_ctrl.bit_flg.bit1) {
								    sys_ctrl.bit_flg.bit2 = ET_TRUE;
								    }
								    sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
								    sys_ctrl.sub_sts = 0;
      						  return;
                }
            }
            else {
                /* Resistance rebound in normal duration */
                calcForce = sub_maxi_values(glb_ctrl.dnForce[index-1], glb_ctrl.dnForce[index], glb_ctrl.dnForce[index+1]);
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][1]) > AD_MAX ? AD_MAX : (calcForce + bk_force[glb_ctrl.force_lv][1]);
								if (adcValue > force_res) {
//                if (adcValue > (calcForce + bk_force[glb_ctrl.force_lv][1])) {
                	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
    								printf("resis tence rebound normal :\r\n");
										#endif
									  if (ET_FALSE == sys_ctrl.bit_flg.bit1) {
								    sys_ctrl.bit_flg.bit2 = ET_TRUE;
								    }
								    sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
								    sys_ctrl.sub_sts = 0;
      						  return;
                }
            }
        }
        else {
            /* 2017/8/4 星期五 上午 7:56:27
            sub_sped_stable(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, PWM_VAL_MIN_ALL, HAL_DUTY_SLOW);
            2017/8/4 星期五 上午 7:56:27 */
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);
            //sub_sped_stable(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, HAL_DUTY_SLOW);
            /* Resistance rebound in slow stop duration */
            if ((sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0)) {
                if (sys_ctrl.delay0 > 0) {
                    sys_ctrl.delay0--;
                }
                if (sys_ctrl.delay1 > 0) {
                    sys_ctrl.delay1--;
                }
                calcForce = glb_ctrl.dfor_max;
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][0]) > AD_MAX ? AD_MAX : (calcForce + bk_force[glb_ctrl.force_lv][0]);
                if (adcValue > force_res) {
//                if (adcValue > (glb_ctrl.dfor_max+bk_force[glb_ctrl.force_lv][0])) {
                	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
    								printf("resis tence rebound slow stop 1:\r\n");
										#endif
									  if (ET_FALSE == sys_ctrl.bit_flg.bit1) {
								    sys_ctrl.bit_flg.bit2 = ET_TRUE;
								    }
								    sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
								    sys_ctrl.sub_sts = 0;
                    return;
                }
            }
            else {
                calcForce = sub_maxi_values(glb_ctrl.dnForce[index-1], glb_ctrl.dnForce[index], glb_ctrl.dnForce[index+1]);
                force_res = (calcForce + bk_force[glb_ctrl.force_lv][2]) > AD_MAX ? AD_MAX : (calcForce + bk_force[glb_ctrl.force_lv][2]);
								if (adcValue > force_res) {
//                if (adcValue > (calcForce+bk_force[glb_ctrl.force_lv][2])) {
                	  #if DBG_LV_CURRENT & DBG_LV_PROCESS
    								printf("resis tence rebound slow stop 2:\r\n");
										#endif
									  if (ET_FALSE == sys_ctrl.bit_flg.bit1) {
								    sys_ctrl.bit_flg.bit2 = ET_TRUE;
								    }
								    sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
								    sys_ctrl.sub_sts = 0;
                    return;
                }
            }
        }
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit bottom side, stop motor */
/* 2017/6/17 星期六 上午 9:07:23-limit sensor skew detect------------*/
    	if (KEY_ST_HIGH==getkey_stabe(KEY_ID_LMT0)) {
						sys_ctrl.bit_flg.bit7 = ET_TRUE;
            DN_FORCE_STOP();
            return;
        }
    /**************auto stop **************************/  
			++sys_ctrl.delay2;
	    if (sys_ctrl.delay2 == AUTO_STOP) {
	    	sys_ctrl.bit_flg.bit7 = ET_FALSE;
	    	DN_FORCE_STOP();
	      return;
	    }
    }
    /* delay sometime to stop motor */
    else if (3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
            sys_ctrl.delay0 = 60;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if (4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
            sys_ctrl.lst_dir = MOT_CTS_DOWN;
            if(sys_ctrl.bit_flg.bit7 == ET_TRUE){
               halsig_scnt(glb_ctrl.hall_bot);
               sys_ctrl.bit_flg.bit6 = ET_TRUE;
            }
            
            #if LIMIT_SEN_ERR     	
						if (hallCnt > glb_ctrl.hall_bot + LIMIT_SEN_HALL_TOLERANCE) {
							#if DBG_LV_CURRENT & DBG_LV_PROCESS
    					printf("HALL out of RAGE:\r\n");
							#endif
						 	limit_sen_cnt_plus();
						}
						else 	 
							limit_sen_cnt_min();
						#endif  
            
            
#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("DOOR DN OK:\r\n");
            printf("Top hall: %d\r\n", glb_ctrl.hall_top);
            printf("Bot hall: %d\r\n", glb_ctrl.hall_bot);
            printf("glb Cur hall: %d\r\n", halsig_gcnt());
            printf("Cur hall: %d\r\n", hallCnt);            
#endif
        }
    }
    return ;
}

/*-----------------------------------------------------------------*/
/* FUNC : System force back                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_excute_bk(void)
{
    U16ET hallCnt;

    hallCnt  = halsig_gcnt();

    /* stop motor power */
    if (0 == sys_ctrl.sub_sts) {
        motors_pwms(0);
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 250;
        if (ET_TRUE == sys_ctrl.bit_flg.bit2) {
            sys_ctrl.delay0 = 5;
        }
    }
    /* stop motor action  */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 60;
            if (ET_TRUE == sys_ctrl.bit_flg.bit2) {
                sys_ctrl.delay0 = 5;
            }
        }
    }
    /* motor direction up */
    else if(2 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_UPUP);
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 10;
            if( ET_TRUE == sys_ctrl.bit_flg.bit2 ) {
                sys_ctrl.delay0 = 5;
            }
        }
    }
    /* wait sometime to start power motor */
    else if (3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sub_clrs_status();
            halsig_sdir(HAL_DIR_UPUP);
            sys_ctrl.delay0 = 700;
            sys_ctrl.delay1 = 300;
            if (hallCnt - glb_ctrl.hall_top > 600) {
                sys_ctrl.opn_pos = hallCnt - 600;
            }
            else {
                sys_ctrl.opn_pos = 0;
            }
            sys_ctrl.cur_pwm = PWM_VAL_DFT_REV;
            sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
            sys_ctrl.sub_sts = 2;
            ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 50);
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System set mode                                          */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_system_st(VODET)
{
    ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);

    /* initialize parameter */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_system_st>\r\n\r\n"); 
				#endif
        sys_ctrl.delay0 = 1000;
        sys_ctrl.sub_sts = 1;
    }
    /* wait set long press */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (KEY_ST_LOW == getkey_stabe(KEY_ID_SET)) {
            goto BACK_STOP;
        }
        if (0 == sys_ctrl.delay0) {
            ledtsk_ctrl(LED_ID_SET, LED_ST_ON, 0);
            sys_ctrl.sub_sts = 2;
        }
    }
    /* wait set release */
    else if (2 == sys_ctrl.sub_sts) {
        if (KEY_ST_LOW == getkey_stabe(KEY_ID_SET)) {
            sys_ctrl.sub_sts = 3;
        }
    }
    /* wait set press again */
    else if (3 == sys_ctrl.sub_sts) {
        if (KEY_ST_HIGH == getkey_stabe(KEY_ID_SET)) {
            sys_ctrl.sys_sts = SYS_ST_LIMIT_ST;
            sys_ctrl.sub_sts = 0;
        }
    }
    return ;

BACK_STOP:
    sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
    sys_ctrl.sub_sts = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Remote controller adjust                                 */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_remote_st(void)
{
    U08ET rem_key[REID_SIZE], ngflg;
    #if DBG_LV_CURRENT & DBG_LV_PROCESS
    U16ET rem_key_prin,i;
		U08ET *ptr;
    #endif
    U08ET index;
    ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
    /* operation cancel */
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_SET)) {
        sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
        sys_ctrl.sub_sts = 0;
    }
    /* initialize parameter */
    if (0 == sys_ctrl.sub_sts) {
			  sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 1000;
    }
    /* wait code key long press */
    else if (1 == sys_ctrl.sub_sts) {
        if (KEY_ST_LOW == getkey_curst(KEY_ID_CODE)) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
            return ;
        }
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
        	#if DBG_LV_CURRENT & DBG_LV_PROCESS
						printf("stay in <sys_remote_st>\r\n\r\n"); 
					#endif
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 2500;
            ledtsk_ctrl(LED_ID_CODE, LED_ST_ON, 250);
        }
    }
    /* wait code key release */
    else if (2 == sys_ctrl.sub_sts) {
        if (KEY_ST_LOW == getkey_curst(KEY_ID_CODE)) {
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 10000;
            //ledtsk_ctrl(LED_ID_CODE, LED_ST_ON, 0);
            remote_gkey(rem_key);
        }
        sys_ctrl.delay0--;
        /* clear the saved remoter code */
        if (0 == sys_ctrl.delay0) {
            remote_clrs();
            sys_ctrl.delay0  = 500;
            sys_ctrl.sub_sts = 5;
        }
    }
    /* wait remoter key */
    else if (3 == sys_ctrl.sub_sts) {
        if (ET_TRUE == remote_gkey(sys_ctrl.rem_key)) {
						ledtsk_ctrl(LED_ID_CODE, LED_ST_OFF, 0);
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 10000;
            return;
        }        
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
    /* wait another remoter key */
    else if (4 == sys_ctrl.sub_sts) {
        if (ET_TRUE == remote_gkey(rem_key)) {
            ngflg = 0;                 
            for (index = 0; index < REID_SIZE; index++) { 
            	  if (sys_ctrl.rem_key[index] != rem_key[index]) {
                    ngflg = 1;
                } 
                //sys_ctrl.rem_key[index] = rem_key[index];
            }            
                      
            if ((0 == ngflg) && (ET_TRUE==remote_save(rem_key))) {
                sys_ctrl.delay0 = 500;
                sys_ctrl.sub_sts = 5;
                ledtsk_ctrl(LED_ID_CODE, LED_ST_ON, 50);
            }
        }        
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
    /* keep led status sometime */
    else if (5 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System power off                                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_power_off(void)
{
    /* disable peripheral unit to reduce power consumption */
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_power_off>\r\n\r\n"); 
				#endif
        motors_pwms(0);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_OFF, 0);
        ledtsk_ctrl(LED_ID_CODE, LED_ST_OFF, 0);
        ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 0);
        sys_ctrl.delay0 = 20;
        sys_ctrl.sub_sts = 1;
    }
    /* safe to stop motor */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            sys_ctrl.delay0 = 40;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* save current position */
    else if (2 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            if (ET_TRUE == sys_ctrl.bit_flg.bit0) {
                glb_ctrl.hall_cur = halsig_gcnt();
                sub_save_sysctl(0);
                #if DBG_LV_CURRENT & DBG_LV_PROCESS
                printf("suc\r\n\r\n");
                #endif
            }
            sys_ctrl.delay0 = 500;
            sys_ctrl.sub_sts = 3;
        }
    }
    /* delay sometime to be certain power if off */
    else if (3 == sys_ctrl.sub_sts) {

        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
         		#if DBG_LV_CURRENT & DBG_LV_PROCESS
						printf("save success!!!\r\n\r\n"); 
						#endif
            while(1);
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System hall error exist                                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_bad_error(VODET)
{
    /* stop motor power */
    #if DBG_LV_CURRENT & DBG_LV_PROCESS
		U16ET error = sys_ctrl.err_num;
		#endif    
    if (0 == sys_ctrl.sub_sts) {
				#if DBG_LV_CURRENT & DBG_LV_PROCESS
				printf("jump into <sys_bad_error>\r\n"); 
				printf("reeor code : 0x%x\r\n",error);
				#endif
        motors_pwms(0);
        if (0xE1 == sys_ctrl.err_num) {
            glb_ctrl.hall_top = 0;
            glb_ctrl.hall_bot = 0;
            glb_ctrl.hall_cur = 0;
            sub_save_sysctl(0);
        }
        else if (0xE3 == sys_ctrl.err_num) {
            #if DBG_LV_CURRENT & DBG_LV_PROCESS
              printf("limit sesor error\r\n");
            #endif
				}
               
        sys_ctrl.delay0 = 100;
        sys_ctrl.sub_sts = 1;
    }
    /* stop motor excution */
    else if (1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if (0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            sys_ctrl.sub_sts = 2;
        }
    }
    /* No hall signal detect */
    else if(2 == sys_ctrl.sub_sts) {
    	if (0xE1 == sys_ctrl.err_num) {
        ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 250);
        ledtsk_ctrl(LED_ID_CODE, LED_ST_ON, 250);
      }
      else if (0xE2 == sys_ctrl.err_num)
      	ledtsk_ctrl(LED_ID_SET, LED_ST_OFF, 250);
      else if (0xE3 == sys_ctrl.err_num)
      	ledtsk_ctrl(LED_ID_CODE, LED_ST_ON, 250);      	
        sys_ctrl.sub_sts = 3;
    }
    else if(3 == sys_ctrl.sub_sts) {}
}

/******************************************************************************
  * FUNC : abnormal status detective                                  
  * PARA :                                                          
  *   NONE                                                          
  * RETN :                                                          
  *   NONE                                                          
  *****************************************************************************
  **/
VODET abnormal_detect(VODET)
{
  if ((SYS_ST_POWER_OFF != sys_ctrl.sys_sts) && (ET_TRUE == powsts_flag())) {
    sys_ctrl.sys_sts = SYS_ST_POWER_OFF;
    sys_ctrl.sub_sts = 0;
    return;
  }
  /* system error ma`````````````````intenance */
  if (SYS_ST_BAD_ERROR != sys_ctrl.sys_sts) {
    /* main hall lost error */
    if ((sys_ctrl.sys_sts >= SYS_ST_LIMIT_ST) && \
 	  (sys_ctrl.sys_sts <= SYS_ST_EXCUTE_BK)) {
       switch (halsig_gerr()) {
         case ERR_HAL_MAIN:
           sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
           sys_ctrl.err_num = 0xE1;
           sys_ctrl.sub_sts = 0;
           break;
         default:
           break;
       }
    }
    if ((sys_ctrl.sys_sts >= SYS_ST_LIMIT_ST) && \
       (sys_ctrl.sys_sts <= SYS_ST_EXCUTE_BK)) {
		     if (ET_TRUE == oc_detect()) {
		       sys_ctrl.sys_sts         = SYS_ST_BAD_ERROR;
		       sys_ctrl.err_num         = 0xE2;
		       sys_ctrl.sub_sts         = 0;
		   }
    }
#/*if LIMIT_SEN_ERR
    if (sys_ctrl.sys_sts == SYS_ST_STAY_STOP)
       {
         if (ET_TRUE == limit_sen_err_check())
         {
           sys_ctrl.sys_sts         = SYS_ST_BAD_ERROR;
           sys_ctrl.err_num         = 0xE3;
           sys_ctrl.sub_sts         = 0; 
         }
       }
#endif*/
  }
}


