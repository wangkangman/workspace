#include <stdio.h>
#include "main.h"
#include "subs.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static SYS_CTRL sys_ctrl;

const FUNC func[SYS_ST_INVALID] = 
{
    sys_power_on,
    sys_read_eep,
    sys_route_up,
    sys_route_dn,
    sys_train_up,
    sys_train_dn,
    sys_stay_stop,
    sys_excute_up,
    sys_excute_dn,
    sys_excute_bk,
    sys_system_st,
    sys_remote_st,
    sys_power_off,
    sys_bad_error,
    sys_simu_func,
};

const U16ET ct_speed[3][] = 
{
    { 285, 410 },  /*  4NM 60R 42R */
    { 255, 380 },  /*  8NM 60R 42R */
};

const U08ET bk_force_4NM[9][] = 
{
    {  75,  40,  20,  100,   80,  40 },
    {  75,  45,  22,  100,   80,  40 },
    {  75,  50,  24,  100,   80,  40 },
    {  75,  65,  35,  100,   80,  40 },
    {  90,  90,  45,  100,   84,  50 },
    { 115, 115,  55,  100,  108,  60 },
    { 140, 140,  65,  132,  132,  70 },
    { 165, 165,  75,  156,  156,  75 },
    { 200, 200,  80,  180,  180,  80 },
};

const U08ET bk_force_8NM[9][] = 
{
    {  75,  30,  18,  100,   80,  40 },
    {  75,  35,  20,  100,   80,  40 },
    {  75,  40,  22,  100,   80,  40 },
    {  75,  60,  30,  100,   80,  40 },
    {  85,  85,  40,  100,   84,  50 },
    { 115, 115,  50,  100,  108,  60 },
    { 140, 140,  60,  132,  132,  70 },
    { 170, 170,  70,  156,  156,  75 },
    { 200, 200,  80,  180,  180,  80 },
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
        if( ET_FALSE == tskskm_flag() ) {
            continue;
        }

        /* power off process */
        if( (SYS_ST_POWER_OFF != sys_ctrl.sys_sts) && (KEY_ST_HIGH == getkey_stabe(KEY_ID_POW)) ) {
            sys_ctrl.sys_sts = SYS_ST_POWER_OFF;
            sys_ctrl.sub_sts = 0;
        }

        /* system error maintenance */
        if( SYS_ST_BAD_ERROR != sys_ctrl.sys_sts ) {
            /* main hall lost error */
            if( (sys_ctrl.sys_sts>=SYS_ST_ROUTE_UP) && (sys_ctrl.sys_sts<=SYS_ST_EXCUTE_BK) ) {
                switch( halsig_gerr() ) {
                    case ERR_HAL_MAIN:
                        sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
                        sys_ctrl.err_num = 0xE1;
                        sys_ctrl.sub_sts = 0;
                        break;
                    case ERR_HAL_SUBS:
                        sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
                        sys_ctrl.err_num = 0xE2;
                        sys_ctrl.sub_sts = 0;
                        break;
                    case ERR_HAL_MUTX:
                        sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
                        sys_ctrl.err_num = 0xE3;
                        sys_ctrl.sub_sts = 0;
                        break;
                    default:
                        break;
                }
            }

            /* In door error process */
            if( sys_ctrl.sys_sts > SYS_ST_READ_EEP ) {
                if( KEY_ST_HIGH == getkey_stabe(KEY_ID_DOOR) ) {
                    sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
                    sys_ctrl.err_num = 0xE4;
                    sys_ctrl.sub_sts = 0;
                }
            }
        }

        wdtdog_feed();
        ledseg_task();
        keysmp_task();
        adcsmp_task();
        Receiver_sys_task();

#if DBG_LV_CURRENT & DBG_LV_PROCESS
        if( (sys_ctrl.sys_sts != sys_ctrl.sys_lts) || (sys_ctrl.sub_sts != sys_ctrl.sub_lts) ) {
            printf("SYSTEM STATUS : %d %d\r\n", (U16ET)sys_ctrl.sys_sts, (U16ET)sys_ctrl.sub_sts);
            sys_ctrl.sys_lts = sys_ctrl.sys_sts;
            sys_ctrl.sub_lts = sys_ctrl.sub_sts;
        }
#endif

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
//    remote_init();
    Si4355_Configuration();
    ledseg_init();
    adcsmp_init();
    keysmp_init();
    timwdt_init();
    uartct_init();
    glbcfg_init();
    ET_EI();

    sys_ctrl.sys_sts = SYS_ST_POWER_ON;
    sys_ctrl.sub_sts = 0;
    sys_ctrl.sys_lts = SYS_ST_INVALID;
    sys_ctrl.sub_lts = 0;
    sys_ctrl.lst_dir = MOT_CTS_DOWN;
    sys_ctrl.opn_pos = 0;
    sys_ctrl.slp_cnt = 0;
    sys_ctrl.dyn_ups = ET_FALSE;
    sys_ctrl.dyn_dns = ET_FALSE;
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
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
        segtsk_ctrl(ET_TRUE, glb_cfgs.prg_vers/10, ET_TRUE, 0);
        sys_ctrl.delay0 = 250;
        sys_ctrl.sub_sts = 1;
    }
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 2;
        }
    }
    else if(2 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_TRUE, glb_cfgs.prg_vers%10, ET_FALSE, 0);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 3;
        }
    }
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
            sys_ctrl.delay0 = 500;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* display number [9-0] */
    else if( (sys_ctrl.sub_sts>=4) && (sys_ctrl.sub_sts<=13) ) {
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_TRUE, 13-sys_ctrl.sub_sts, ET_FALSE, 0);
            sys_ctrl.delay0 = 125;
            sys_ctrl.sub_sts++;
        }
        else {
            sys_ctrl.delay0--;
        }
    }
    else if(14 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            ledtsk_ctrl(LED_ID_LAMP, LED_ST_OFF, 0);
            sys_ctrl.sys_sts = SYS_ST_READ_EEP;
            sys_ctrl.sub_sts = 0;
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

    /* read system control */
    pbuf = (U08ET *)(&glb_ctrl.save_flg);
    size = (U08ET *)(&glb_ctrl.save_end) - (U08ET *)(&glb_ctrl.save_flg);
    for(index=0; index<size; index++) {
        pbuf[index] = eepbyt_read(index);
    }

    /* read system open force */
    pbuf = glb_ctrl.upForce;
    for(index=0; index<UP_FORCE_SAMP; index++) {
        pbuf[index] = eepbyt_read(index+256);
    }

    /* read system close force */
    pbuf = glb_ctrl.dnForce;
    for(index=0; index<DN_FORCE_SAMP; index++) {
        pbuf[index] = eepbyt_read(index+256+UP_FORCE_SAMP);
    }

    /* check saved flag */
    if( EEP_SAVE_FLG != glb_ctrl.save_flg ) {
        glb_ctrl.save_flg = EEP_SAVE_FLG;
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        glb_ctrl.force_lv = glb_cfgs.dft_levl - 1;
        glb_ctrl.irad_ena = ET_FALSE;
        glb_ctrl.auto_clo = 0;

        sys_ctrl.bit_flg.bit0 = ET_TRUE;
        sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
        sys_ctrl.sub_sts = 1;
        sub_save_sysctl(0, 0);
        halsub_clrs();
        return ;
    }

    /* check if route parameter is normal */
    if( (0 == glb_ctrl.hall_cur) || (glb_ctrl.hall_top>=glb_ctrl.hall_bot) || (glb_ctrl.hall_bot>HAL_CNT_MAX) ) {
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
    }

    /* set current motor status */
    halsig_scnt(glb_ctrl.hall_cur);
    if( glb_ctrl.hall_cur <= glb_ctrl.hall_top ) {
        sys_ctrl.lst_dir = MOT_CTS_UPUP;
    }

    sys_ctrl.bit_flg.bit0 = ET_TRUE;
    sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
    sys_ctrl.sub_sts = 1;
    sub_save_sysctl(0, 0);
    halsub_clrs();

    return ;
}

/*-----------------------------------------------------------------*/
/* FUNC : System route train up                                    */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_route_up(VODET)
{
    /* initialize parameter */
    if(0 == sys_ctrl.sub_sts) {
        segtsk_ctrl(ET_TRUE, 1, ET_FALSE, 0);
        halsig_scnt(HAL_BOT_DFT);
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        glb_ctrl.hsub_oft = 0;
        glb_ctrl.hsub_dif = 0;
        sub_save_sysctl(0, 0);
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay1 = 10000;
        sys_ctrl.bit_flg.bit0 = ET_FALSE;
    }
    /* reponse to key press */
    else if(1 == sys_ctrl.sub_sts) {
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K2) ) {
            motors_ctrl(MOT_CTS_UPUP);
            halsig_sdir(MOT_CTS_UPUP);
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 10;
            sys_ctrl.delay1 = 10000;
            segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 50);
        }
        else if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K4) ) {
            motors_ctrl(MOT_CTS_DOWN);
            halsig_sdir(MOT_CTS_DOWN);
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 10;
            sys_ctrl.delay1 = 10000;
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 50);
        }
        else if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K1) ) {
            sys_ctrl.sub_sts = 5;
            sys_ctrl.delay1 = 10000;
        }

        sys_ctrl.delay1--;
        if(0 == sys_ctrl.delay1) {
            glb_ctrl.hall_top = 0;
            glb_ctrl.hall_bot = 0;
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
    /* delay to start motor */
    else if(2 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 3;
        }
    }
    /* run motor up or down */
    else if(3 == sys_ctrl.sub_sts) {
        sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
        motors_pwms(sys_ctrl.cur_pwm);

        /* cut off motor power */
        if( (KEY_ST_LOW == getkey_stabe(KEY_ID_K2)) || (KEY_ST_LOW == getkey_stabe(KEY_ID_K4)) ) {
            motors_pwms(0);
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 100;
        }
    }
    /* delay to stop motor */
    else if(4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            segtsk_ctrl(ET_TRUE, 1, ET_FALSE, 0);
            sys_ctrl.sub_sts = 1;
            sys_ctrl.delay1 = 10000;
        }
    }
    /* wait K1 release */
    else if(5 == sys_ctrl.sub_sts) {
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K1) ) {
            sys_ctrl.sys_sts = SYS_ST_ROUTE_DN;
            sys_ctrl.sub_sts = 0;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System route train down                                  */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_route_dn(VODET)
{
    U16ET hallCnt;

    hallCnt = halsig_gcnt();

    /* initialize parameter */
    if(0 == sys_ctrl.sub_sts) {
        segtsk_ctrl(ET_TRUE, 2, ET_FALSE, 0);
        halsig_scnt(HAL_BOT_DFT);
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay1 = 10000;
    }
    /* reponse to key press */
    else if(1 == sys_ctrl.sub_sts) {
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K2) ) {
            motors_ctrl(MOT_CTS_UPUP);
            halsig_sdir(MOT_CTS_UPUP);
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 10;
            sys_ctrl.delay1 = 10000;
            segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 50);
        }
        else if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K4) ) {
            motors_ctrl(MOT_CTS_DOWN);
            halsig_sdir(MOT_CTS_DOWN);
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 10;
            sys_ctrl.delay1 = 10000;
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 50);
        }
        else if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K1) ) {
            sys_ctrl.sub_sts = 5;
            sys_ctrl.delay1 = 10000;
        }

        sys_ctrl.delay1--;
        if(0 == sys_ctrl.delay1) {
            glb_ctrl.hall_top = 0;
            glb_ctrl.hall_bot = 0;
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
    /* delay to start motor */
    else if(2 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 3;
        }
    }
    /* run motor up or down */
    else if(3 == sys_ctrl.sub_sts) {
        sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
        motors_pwms(sys_ctrl.cur_pwm);

        /* cut off motor power */
        if( (KEY_ST_LOW == getkey_stabe(KEY_ID_K2)) || (KEY_ST_LOW == getkey_stabe(KEY_ID_K4)) ) {
            motors_pwms(0);
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 100;
        }
    }
    /* delay to stop motor */
    else if(4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            segtsk_ctrl(ET_TRUE, 2, ET_FALSE, 0);
            sys_ctrl.sub_sts = 1;
            sys_ctrl.delay1 = 10000;
        }
    }
    /* wait K1 release */
    else if(5 == sys_ctrl.sub_sts) {
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K1) ) {
            glb_ctrl.hall_bot = HAL_BOT_DFT;
            glb_ctrl.hall_top = HAL_BOT_DFT + HAL_BOT_DFT - hallCnt;
            glb_ctrl.hall_cur = HAL_BOT_DFT;
            halsig_scnt(glb_ctrl.hall_cur);
#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("Hall Top:%d\r\n", glb_ctrl.hall_top);
            printf("Hall Bot:%d\r\n", glb_ctrl.hall_bot);
#endif
            if( ((glb_ctrl.hall_bot - glb_ctrl.hall_top) > 5650) || (glb_ctrl.hall_top >= glb_ctrl.hall_bot) ) {
                glb_ctrl.hall_top = 0;
                glb_ctrl.hall_bot = 0;
                sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
                sys_ctrl.sub_sts = 0;
            }
            else {
                sys_ctrl.sys_sts = SYS_ST_TRAIN_UP;
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
void sys_train_up(void)
{
    U16ET hallCnt, hallSmp, ofst, index;
    U08ET adcValue;

    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1)/UP_FORCE_SAMP;

    if( adcValue > glb_ctrl.ufor_max ) {
        glb_ctrl.ufor_max = adcValue;
    }

    if( ET_TRUE == halsub_gets() ) {
        glb_ctrl.hsub_lst = hallCnt;
        if( 0 == glb_ctrl.hsub_oft ) {
            glb_ctrl.hsub_oft = 1;
        }
        else if( 1 == glb_ctrl.hsub_oft ) {
            glb_ctrl.hsub_oft = hallCnt;
        }
        else if( 0 == glb_ctrl.hsub_dif ) {
            glb_ctrl.hsub_dif = glb_ctrl.hsub_oft - hallCnt;
        }
    }

    /* motor direction down */
    if(0 == sys_ctrl.sub_sts) {
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        glb_ctrl.ufor_max = 0;
        glb_ctrl.dfor_max = 0;
        for(index=0; index<UP_FORCE_SAMP; index++) {
            glb_ctrl.upForce[index] = 0;
        }
        for(index=0; index<DN_FORCE_SAMP; index++) {
            glb_ctrl.dnForce[index] = 0;
        }
        halsig_sdir(MOT_CTS_UPUP);
        motors_ctrl(MOT_CTS_UPUP);
        segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 50);
    }
    /* start motor power */
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until top side */
    else if(2 == sys_ctrl.sub_sts) {
        if(hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP) {
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
        }
        else {
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);
        }
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit top side, stop motor */
        if( hallCnt <= glb_ctrl.hall_top ) {
            motors_pwms(0);
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 100;
        }

        /* learn force of the door close duration */
        if( (hallCnt >= glb_ctrl.hall_top) && (hallCnt <= glb_ctrl.hall_bot) ) {
            index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;
            if( adcValue > glb_ctrl.upForce[index] ) {
                glb_ctrl.upForce[index] = adcValue;
            }
        }
    }
    /* stop motor power */
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 0);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if(4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            ofst = (glb_ctrl.hall_bot - glb_ctrl.hall_top - HAL_CNT_SLOW_STOP)/hallSmp;
            for(index=0; index<150/hallSmp; index++) {
                glb_ctrl.upForce[ofst+index] = glb_ctrl.upForce[ofst-1];
            }
            sys_ctrl.sys_sts = SYS_ST_TRAIN_DN;
            sys_ctrl.sub_sts = 0;
#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("Hall Sub:%dx+%d\r\n", glb_ctrl.hsub_dif, glb_ctrl.hsub_oft);
            printf("MAX:%d\r\n",(U16ET)glb_ctrl.ufor_max);
            for(index=0; index<UP_FORCE_SAMP; index++) {
                printf("%d\r\n", (U16ET)glb_ctrl.upForce[index]);
                wdtdog_feed();
            }
#endif
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
void sys_train_dn(void)
{
    U16ET hallCnt, hallSmp, ofst, index;
    U08ET adcValue;

    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + DN_FORCE_SAMP - 1)/DN_FORCE_SAMP;

    if( adcValue > glb_ctrl.dfor_max ) {
        glb_ctrl.dfor_max = adcValue;
    }

    if( ET_FALSE == sub_hall_verify(hallCnt) ) {
        sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
        sys_ctrl.err_num = 0xE5;
        sys_ctrl.sub_sts = 0;
    }

    /* motor direction down */
    if(0 == sys_ctrl.sub_sts) {
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        halsig_sdir(MOT_CTS_DOWN);
        motors_ctrl(MOT_CTS_DOWN);
        if( glb_ctrl.hsub_dif < 145 ) {
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 50);
        }
        else {
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 100);
        }
    }
    /* start motor power */
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.delay0 = 200;
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until bottom side */
    else if(2 == sys_ctrl.sub_sts) {
        if(hallCnt < glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP) {
            if( glb_ctrl.hsub_dif < 145 ) {
                sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[0][0]);
            }
            else {
                sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[1][0]);
            }
        }
        else {
            if( sys_ctrl.delay0 > 0 ) {
                sys_ctrl.delay0--;
                sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);
            }
            else {
                if( glb_ctrl.hsub_dif < 145 ) {
                    sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[0][1]);
                }
                else {
                    sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[1][1]);
                }
            }
        }
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit bottom side, stop motor */
        if( hallCnt >= glb_ctrl.hall_bot ) {
            motors_pwms(0);
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 100;
        }

        /* learn force of the door close duration */
        if( (hallCnt >= glb_ctrl.hall_top) && (hallCnt <= glb_ctrl.hall_bot) ) {
            index = (hallCnt - glb_ctrl.hall_top) / hallSmp;
            if( adcValue > glb_ctrl.dnForce[index] ) {
                glb_ctrl.dnForce[index] = adcValue;
            }
        }
    }
    /* stop motor power */
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 0);
            sys_ctrl.delay0 = 250;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if(4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
            for(index=0; index<320/hallSmp; index++) {
                if( glb_ctrl.dnForce[index] < 235 ) {
                    glb_ctrl.dnForce[index] += 20;
                }
                else {
                    glb_ctrl.dnForce[index] = 255;
                }
            }
            ofst = (glb_ctrl.hall_bot - glb_ctrl.hall_top - HAL_CNT_SLOW_STOP)/hallSmp;
            for(index=0; index<120/hallSmp; index++) {
                if( glb_ctrl.dnForce[ofst+index] < 235 ) {
                    glb_ctrl.dnForce[ofst+index] += 20;
                }
                else {
                    glb_ctrl.dnForce[ofst+index] = 255;
                }
            }
            ofst = (glb_ctrl.hall_bot - glb_ctrl.hall_top - HAL_CNT_REAL_STOP)/hallSmp;
            for(index=0; index<HAL_CNT_REAL_STOP/hallSmp; index++) {
                if( glb_ctrl.dnForce[ofst+index] < 235 ) {
                    glb_ctrl.dnForce[ofst+index] += 20;
                }
                else {
                    glb_ctrl.dnForce[ofst+index] = 255;
                }
            }
            sub_save_sysctl(1, 0);
            sys_ctrl.slp_cnt = 0;
            sys_ctrl.bit_flg.bit0 = ET_TRUE;
            sys_ctrl.dyn_ups = ET_FALSE;
            sys_ctrl.dyn_dns = ET_FALSE;
            sys_ctrl.lst_dir = MOT_CTS_DOWN;
#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("MAX:%d\r\n",(U16ET)glb_ctrl.dfor_max);
            for(index=0; index<DN_FORCE_SAMP; index++) {
                printf("%d\r\n", (U16ET)glb_ctrl.dnForce[index]);
                wdtdog_feed();
            }
#endif
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
    U08ET rmtkey;

    hallCnt = halsig_gcnt();

    if( 0 == sys_ctrl.sub_sts ) {
        remote_hits();
        getkey_oshot(KEY_ID_WALL);
        sys_ctrl.clo_cnt = 500;
        sys_ctrl.clo_cnt *= glb_ctrl.auto_clo * glb_cfgs.clo_auto;
        sys_ctrl.lmp_cnt = 90000;
        sys_ctrl.dym_cnt = 150000;
        sys_ctrl.sub_sts = 2;
        return ;
    }
    else if( 1 == sys_ctrl.sub_sts ) {
        remote_hits();
        getkey_oshot(KEY_ID_WALL);
        sys_ctrl.clo_cnt = 500;
        sys_ctrl.clo_cnt *= glb_ctrl.auto_clo * glb_cfgs.clo_auto;
        sys_ctrl.lmp_cnt = 0;
        sys_ctrl.dym_cnt = 150000;
        sys_ctrl.sub_sts = 2;
        return ;
    }

    if( (ET_TRUE == glb_ctrl.irad_ena) && (KEY_ST_HIGH==getkey_curst(KEY_ID_IRAD)) ) {
        sys_ctrl.clo_cnt = 500;
        sys_ctrl.clo_cnt *= glb_ctrl.auto_clo * glb_cfgs.clo_auto;
    }

    /* door auto close */
    if( sys_ctrl.clo_cnt > 0 ) {
        sys_ctrl.clo_cnt--;
    }
    else {
        if( glb_ctrl.hall_top != glb_ctrl.hall_bot ) {
            if( ( 0 != glb_ctrl.auto_clo ) && (hallCnt <= glb_ctrl.hall_top) ) {
                if( (ET_FALSE==glb_ctrl.irad_ena) || ((ET_TRUE == glb_ctrl.irad_ena)&&(KEY_ST_LOW==getkey_curst(KEY_ID_IRAD))) ) {
                    sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
                    sys_ctrl.sub_sts = 0;
                    return ;
                }
            }
        }
    }

    /* lamp auto off */
    if( sys_ctrl.lmp_cnt > 0 ) {
        sys_ctrl.lmp_cnt--;
    }
    else {
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_OFF, 0);
    }

    /* system sleep time count */
    if( sys_ctrl.slp_cnt < 600000 ) {
        sys_ctrl.slp_cnt++;
    }
    else {
        sys_ctrl.bit_flg.bit5 = ET_TRUE;
    }

    /* dynamic force prepare */
    if( sys_ctrl.dym_cnt > 0 ) {
        sys_ctrl.dym_cnt--;
    }
    else {
        sys_ctrl.dyn_ups = ET_TRUE;
        sys_ctrl.dyn_dns = ET_TRUE;
    }

    /* K1 press detected, enter system set mode */
    if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K1) ) {
        sys_ctrl.sys_sts = SYS_ST_SYSTEM_ST;
        sys_ctrl.sub_sts = 0;
    }
    /* K2 press detected, enter system set mode */
    if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K2) ) {
        sys_ctrl.sys_sts = SYS_ST_SYSTEM_ST;
        sys_ctrl.sub_sts = 10;
        sys_ctrl.delay0 = 1000;
    }
    /* K3 press detected, enter remote set mode */
    if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K3) ) {
        sys_ctrl.sys_sts = SYS_ST_REMOTE_ST;
        sys_ctrl.sub_sts = 0;
    }
    /* K4 press detected, enter system set mode */
    if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K4) ) {
        sys_ctrl.sys_sts = SYS_ST_SYSTEM_ST;
        sys_ctrl.sub_sts = 20;
        sys_ctrl.delay0 = 1000;
    }
    /* SIM press detected, enter system simulation mode */
    if( KEY_ST_HIGH == getkey_stabe(KEY_ID_SIM) ) {
        sys_ctrl.sys_sts = SYS_ST_SIMU_FUNC;
        sys_ctrl.sub_sts = 0;
    }

    /* display segment in stop duration */
    if( glb_ctrl.hall_top >= glb_ctrl.hall_bot ) {
        segtsk_ctrl(ET_FALSE, SEG_LINE, ET_FALSE, 0);
        return ;
    }
    else {
        segtsk_ctrl(ET_FALSE, SEG_STOP, ET_FALSE, 0);
    }

    /* get remoter key or get wall key */
    if( RMT_KEY_CTRL == remote_hits() || KEY_ST_HIGH == getkey_oshot(KEY_ID_WALL) ) {
        if( MOT_CTS_DOWN == sys_ctrl.lst_dir ) {
            if( hallCnt <= glb_ctrl.hall_top ) {
                sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
                sys_ctrl.sub_sts = 0;
            }
            else {
                sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
                sys_ctrl.sub_sts = 0;
            }
        }
        else {
            if( (ET_TRUE == glb_ctrl.irad_ena) &&  (KEY_ST_HIGH == getkey_curst(KEY_ID_IRAD)) ) {
                if( hallCnt > glb_ctrl.hall_top ) {
                    sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
                    sys_ctrl.sub_sts = 0;
                }
            }
            else {
                sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
                sys_ctrl.sub_sts = 0;
            }
        }
    }
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
    U16ET hallCnt, hallSmp, ofst, index;
    U16ET adcValue, calcForce;

    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1)/UP_FORCE_SAMP;

    if( RMT_KEY_CTRL == remote_hits() || KEY_ST_HIGH == getkey_oshot(KEY_ID_WALL) ) {
        goto UP_FORCE_STOP;
    }

    if( ET_FALSE == sub_hall_verify(hallCnt) ) {
        sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
        sys_ctrl.err_num = 0xE5;
        sys_ctrl.sub_sts = 0;
    }

    if( adcValue > sys_ctrl.uformax ) {
        sys_ctrl.uformax = adcValue;
    }

    if( sys_ctrl.slp_cnt > 20 ) {
        sys_ctrl.slp_cnt -= 20;
    }
    else {
        sys_ctrl.bit_flg.bit5 = ET_FALSE;
    }

    /* motor direction down */
    if(0 == sys_ctrl.sub_sts) {
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        sys_ctrl.uformax = 0;
        for(index=0; index<UP_FORCE_SAMP; index++) {
            sys_ctrl.upForce[index] = 0;
        }
        if( hallCnt >= glb_ctrl.hall_bot ) {
            sys_ctrl.bit_flg.bit3 = ET_TRUE;
        }
        else {
            sys_ctrl.bit_flg.bit3 = ET_FALSE;
        }
        halsig_sdir(HAL_DIR_UPUP);
        motors_ctrl(MOT_CTS_UPUP);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
        if( glb_ctrl.hsub_dif < 145 ) {
            segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 50);
        }
        else {
            segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 100);
        }
    }
    /* start motor power */
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.delay0 = 1500;
            sys_ctrl.delay1 = 300;
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until top side */
    else if(2 == sys_ctrl.sub_sts) {
        /* calculate force sampling position */
        if( hallCnt > glb_ctrl.hall_bot ) {
            index = 0;
        }
        else if( hallCnt < glb_ctrl.hall_top ) {
            index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
        }
        else {
            index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;
        }

        if(hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP) {
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);

            if(sys_ctrl.delay0 > 0) {
                sys_ctrl.delay0--;
                /* Resistance rebound in starting duration */
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce = glb_ctrl.ufor_max+bk_force_4NM[glb_ctrl.force_lv][3];
                }
                else {
                    calcForce = glb_ctrl.ufor_max+bk_force_8NM[glb_ctrl.force_lv][3];
                }
                if( (adcValue > calcForce) && (ET_TRUE != sys_ctrl.bit_flg.bit2) ) {
                    goto UP_FORCE_STOP;
                }
            }
            else {
                /* Resistance rebound in normal duration */
                calcForce = sub_maxi_values(glb_ctrl.upForce[index-1], glb_ctrl.upForce[index], glb_ctrl.upForce[index+1]);
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce += bk_force_4NM[glb_ctrl.force_lv][4];
                }
                else {
                    calcForce += bk_force_8NM[glb_ctrl.force_lv][4];
                }
                if( adcValue > calcForce ) {
                    goto UP_FORCE_STOP;
                }
            }
        }
        else {
            sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);

            /* Resistance rebound in slow stop duration */
            if( (sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0) ) {
                if(sys_ctrl.delay0 > 0) {
                    sys_ctrl.delay0--;
                }
                if(sys_ctrl.delay1 > 0) {
                    sys_ctrl.delay1--;
                }
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce = glb_ctrl.ufor_max+bk_force_4NM[glb_ctrl.force_lv][3];
                }
                else {
                    calcForce = glb_ctrl.ufor_max+bk_force_8NM[glb_ctrl.force_lv][3];
                }
                if( (adcValue > calcForce) && (ET_TRUE != sys_ctrl.bit_flg.bit2) ) {
                    goto UP_FORCE_STOP;
                }
            }
            else {
                calcForce = sub_maxi_values(glb_ctrl.upForce[index-1], glb_ctrl.upForce[index], glb_ctrl.upForce[index+1]);
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce += bk_force_4NM[glb_ctrl.force_lv][5];
                }
                else {
                    calcForce += bk_force_8NM[glb_ctrl.force_lv][5];
                }
                if( adcValue > calcForce ) {
                    goto UP_FORCE_STOP;
                }
            }
        }
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit top side, stop motor */
        if( 0 == sys_ctrl.opn_pos ) {
            if( hallCnt <= glb_ctrl.hall_top ) {
                goto UP_FORCE_STOP;
            }
        }
        else {
            if( hallCnt <= sys_ctrl.opn_pos ) {
                goto UP_FORCE_STOP;
            }
        }

        /* learn force of the door open duration */
        if( adcValue > sys_ctrl.upForce[index] ) {
            sys_ctrl.upForce[index] = adcValue;
        }
    }
    /* delay sometime to stop motor */
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 0);
            sys_ctrl.delay0 = 60;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if(4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            if( (hallCnt<=glb_ctrl.hall_top) && (ET_TRUE==sys_ctrl.dyn_ups) && (ET_TRUE==sys_ctrl.bit_flg.bit3) ) {
                ofst = (glb_ctrl.hall_bot - glb_ctrl.hall_top - HAL_CNT_SLOW_STOP)/hallSmp;
                for(index=0; index<150/hallSmp; index++) {
                    sys_ctrl.upForce[ofst+index] = sys_ctrl.upForce[ofst-1];
                }

                for(index=0; index<UP_FORCE_SAMP; index++) {
                    glb_ctrl.upForce[index] = (3*glb_ctrl.upForce[index] + sys_ctrl.upForce[index])/4;
                }
                glb_ctrl.ufor_max = (3*glb_ctrl.ufor_max + sys_ctrl.uformax)/4;

                sys_ctrl.dyn_ups = ET_FALSE;
                sub_save_sysctl(1, 0);
            }
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
            sys_ctrl.opn_pos = 0;
            sys_ctrl.lst_dir = MOT_CTS_UPUP;
#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("DOOR UP OK:\r\n");
            printf("Top hall: %d\r\n", glb_ctrl.hall_top);
            printf("Bot hall: %d\r\n", glb_ctrl.hall_bot);
            printf("Cur hall: %d\r\n", hallCnt);
#endif
        }
    }

    return ;

UP_FORCE_STOP:
    motors_pwms(0);
    sys_ctrl.sub_sts = 3;
    sys_ctrl.delay0 = 100;
    return ;
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
    U16ET hallCnt, hallSmp, ofst, index;
    U16ET adcValue, calcForce;

    hallCnt  = halsig_gcnt();
    adcValue = getadc_data(ADC_CH_MOT);
    hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + DN_FORCE_SAMP - 1)/DN_FORCE_SAMP;

    if( adcValue > sys_ctrl.dformax ) {
        sys_ctrl.dformax = adcValue;
    }

    if( (ET_TRUE == glb_ctrl.irad_ena) &&  (KEY_ST_HIGH == getkey_curst(KEY_ID_IRAD)) ) {
        sys_ctrl.bit_flg.bit1 = ET_TRUE;
        goto DN_FORCE_BACK;
    }

    if( RMT_KEY_CTRL == remote_hits() || KEY_ST_HIGH == getkey_oshot(KEY_ID_WALL) ) {
        goto DN_FORCE_STOP;
    }

    if( ET_FALSE == sub_hall_verify(hallCnt) ) {
        sys_ctrl.sys_sts = SYS_ST_BAD_ERROR;
        sys_ctrl.err_num = 0xE5;
        sys_ctrl.sub_sts = 0;
    }

    if( sys_ctrl.slp_cnt > 20 ) {
        sys_ctrl.slp_cnt -= 20;
    }
    else {
        sys_ctrl.bit_flg.bit5 = ET_FALSE;
    }

    if( (ET_TRUE == sys_ctrl.bit_flg.bit5) && (glb_ctrl.force_lv < 3) ) {
        sys_ctrl.vir_lvl = 3;
    }
    else {
        sys_ctrl.vir_lvl = glb_ctrl.force_lv;
    }

    /* motor direction down */
    if(0 == sys_ctrl.sub_sts) {
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 10;
        sys_ctrl.dformax = 0;
        for(index=0; index<DN_FORCE_SAMP; index++) {
            sys_ctrl.dnForce[index] = 0;
        }
        if( hallCnt <= glb_ctrl.hall_top ) {
            sys_ctrl.bit_flg.bit4 = ET_TRUE;
        }
        else {
            sys_ctrl.bit_flg.bit4 = ET_FALSE;
        }
        sys_ctrl.bit_flg.bit1 = ET_FALSE;
        sys_ctrl.bit_flg.bit2 = ET_FALSE;
        halsig_sdir(HAL_DIR_DOWN);
        motors_ctrl(MOT_CTS_DOWN);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
        if( glb_ctrl.hsub_dif < 145 ) {
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 50);
        }
        else {
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 100);
        }
    }
    /* start motor power */
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            sys_ctrl.delay0 = 1000;
            sys_ctrl.delay1 = 300;
            sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
            sys_ctrl.sub_sts = 2;
        }
    }
    /* run until bottom side */
    else if(2 == sys_ctrl.sub_sts) {
        /* calculate force sampling position */
        if( hallCnt < glb_ctrl.hall_top ) {
            index = 0;
        }
        else if( hallCnt > glb_ctrl.hall_bot ) {
            index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
        }
        else {
            index = (hallCnt - glb_ctrl.hall_top) / hallSmp;
        }

        if(hallCnt < glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP) {
            if( glb_ctrl.hsub_dif < 145 ) {
                sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[0][0]);
            }
            else {
                sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[1][0]);
            }

            if(sys_ctrl.delay0 > 0) {
                sys_ctrl.delay0--;
                /* Resistance rebound in starting duration */
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce = glb_ctrl.dfor_max+bk_force_4NM[sys_ctrl.vir_lvl][0];
                }
                else {
                    calcForce = glb_ctrl.dfor_max+bk_force_8NM[sys_ctrl.vir_lvl][0];
                }
                if( adcValue > calcForce ) {
                    goto DN_FORCE_BACK;
                }
            }
            else {
                /* Resistance rebound in normal duration */
                calcForce = sub_maxi_values(glb_ctrl.dnForce[index-1], glb_ctrl.dnForce[index], glb_ctrl.dnForce[index+1]);
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce += bk_force_4NM[sys_ctrl.vir_lvl][1];
                }
                else {
                    calcForce += bk_force_8NM[sys_ctrl.vir_lvl][1];
                }
                if( adcValue > calcForce ) {
                    goto DN_FORCE_BACK;
                }
            }
        }
        else {
            if( sys_ctrl.delay1 > 100 ) {
                sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);
            }
            else {
                if( glb_ctrl.hsub_dif < 145 ) {
                    sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[0][1]);
                }
                else {
                    sub_sped_pidctl(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, PWM_VAL_MIN_ALL, ct_speed[1][1]);
                }
            }

            /* Resistance rebound in slow stop duration */
            if( (sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0) ) {
                if(sys_ctrl.delay0 > 0) {
                    sys_ctrl.delay0--;
                }
                if(sys_ctrl.delay1 > 0) {
                    sys_ctrl.delay1--;
                }
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce = glb_ctrl.dfor_max+bk_force_4NM[sys_ctrl.vir_lvl][0];
                }
                else {
                    calcForce = glb_ctrl.dfor_max+bk_force_8NM[sys_ctrl.vir_lvl][0];
                }
                if( adcValue > calcForce ) {
                    goto DN_FORCE_BACK;
                }
            }
            else {
                calcForce = sub_maxi_values(glb_ctrl.dnForce[index-1], glb_ctrl.dnForce[index], glb_ctrl.dnForce[index+1]);
                if( glb_ctrl.hsub_dif < 145 ) {
                    calcForce += bk_force_4NM[sys_ctrl.vir_lvl][2];
                }
                else {
                    calcForce += bk_force_8NM[sys_ctrl.vir_lvl][2];
                }
                if( adcValue > calcForce ) {
                    goto DN_FORCE_BACK;
                }
            }
        }
        motors_pwms(sys_ctrl.cur_pwm);

        /* hit bottom side, stop motor */
        if( hallCnt >= glb_ctrl.hall_bot ) {
            motors_pwms(0);
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 100;
        }

        /* learn force of the door close duration */
        if( adcValue > sys_ctrl.dnForce[index] ) {
            sys_ctrl.dnForce[index] = adcValue;
        }
    }
    /* delay sometime to stop motor */
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            segtsk_ctrl(ET_FALSE, SEG_DOWN, ET_FALSE, 0);
            sys_ctrl.delay0 = 60;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* motor have a break */
    else if(4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            if( (hallCnt>=glb_ctrl.hall_bot) && (ET_TRUE==sys_ctrl.dyn_dns) && (ET_TRUE==sys_ctrl.bit_flg.bit4) ) {
                for(index=0; index<320/hallSmp; index++) {
                    if( sys_ctrl.dnForce[index] < 235 ) {
                        sys_ctrl.dnForce[index] += 20;
                    }
                    else {
                        sys_ctrl.dnForce[index] = 255;
                    }
                }
                ofst = (glb_ctrl.hall_bot - glb_ctrl.hall_top - HAL_CNT_SLOW_STOP)/hallSmp;
                for(index=0; index<120/hallSmp; index++) {
                    if( sys_ctrl.dnForce[ofst+index] < 235 ) {
                        sys_ctrl.dnForce[ofst+index] += 20;
                    }
                    else {
                        sys_ctrl.dnForce[ofst+index] = 255;
                    }
                }
                ofst = (glb_ctrl.hall_bot - glb_ctrl.hall_top - HAL_CNT_REAL_STOP)/hallSmp;
                for(index=0; index<HAL_CNT_REAL_STOP/hallSmp; index++) {
                    if( sys_ctrl.dnForce[ofst+index] < 235 ) {
                        sys_ctrl.dnForce[ofst+index] += 20;
                    }
                    else {
                        sys_ctrl.dnForce[ofst+index] = 255;
                    }
                }

                for(index=0; index<DN_FORCE_SAMP; index++) {
                    glb_ctrl.dnForce[index] = (3*glb_ctrl.dnForce[index] + sys_ctrl.dnForce[index])/4;
                }
                glb_ctrl.dfor_max = (3*glb_ctrl.dfor_max + sys_ctrl.dformax)/4;

                sys_ctrl.dyn_dns = ET_FALSE;
                sub_save_sysctl(1, 0);
            }
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
            sys_ctrl.lst_dir = MOT_CTS_DOWN;
#if DBG_LV_CURRENT & DBG_LV_PROCESS
            printf("DOOR DN OK:\r\n");
            printf("Top hall: %d\r\n", glb_ctrl.hall_top);
            printf("Bot hall: %d\r\n", glb_ctrl.hall_bot);
            printf("Cur hall: %d\r\n", hallCnt);
#endif
        }
    }

    return ;

DN_FORCE_STOP:
    motors_pwms(0);
    sys_ctrl.sub_sts = 3;
    sys_ctrl.delay0 = 100;
    return ;

DN_FORCE_BACK:
    if( ET_FALSE == sys_ctrl.bit_flg.bit1 ) {
        sys_ctrl.bit_flg.bit2 = ET_TRUE;
    }
    sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
    sys_ctrl.sub_sts = 0;
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
    if(0 == sys_ctrl.sub_sts) {
        motors_pwms(0);
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 100;
        if( ET_TRUE == sys_ctrl.bit_flg.bit2 ) {
            sys_ctrl.delay0 = 5;
        }
    }
    /* stop motor action  */
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 60;
            if( ET_TRUE == sys_ctrl.bit_flg.bit2 ) {
                sys_ctrl.delay0 = 40;
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
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sub_clrs_status();
            halsig_sdir(HAL_DIR_UPUP);
            sys_ctrl.delay0 = 700;
            sys_ctrl.delay1 = 300;
            if( 0 == glb_cfgs.siz_ropn ) {
                sys_ctrl.opn_pos = 0;
            }
            else {
                if( (hallCnt-glb_ctrl.hall_top > glb_cfgs.siz_ropn) && (hallCnt >= glb_ctrl.hall_top) ) {
                    sys_ctrl.opn_pos = hallCnt - glb_cfgs.siz_ropn;
                }
                else {
                    sys_ctrl.opn_pos = 0;
                }
            }
            if( ET_TRUE == sys_ctrl.bit_flg.bit1 ) {
                sys_ctrl.opn_pos = 0;
            }
            sys_ctrl.cur_pwm = PWM_VAL_DFT_REV;
            sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
            sys_ctrl.sub_sts = 2;
            if( glb_ctrl.hsub_dif < 145 ) {
                segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 50);
            }
            else {
                segtsk_ctrl(ET_FALSE, SEG_UPUP, ET_FALSE, 100);
            }
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
    if(0 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0 = 1000;
        sys_ctrl.sub_sts = 1;
    }
    /* wait K1 long press */
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K1) ) {
            goto BACK_STOP;
        }
        if( 0 == sys_ctrl.delay0 ) {
            sys_ctrl.delay0  = 1000;
            sys_ctrl.sub_sts = 2;
            sys_ctrl.sys_set = 0;
        }
    }
    /* item select */
    else if(2 == sys_ctrl.sub_sts) {
        switch(sys_ctrl.sys_set) {
            case 0:
                segtsk_ctrl(ET_TRUE, 1, ET_FALSE, 0);
                break;
            case 1:
                segtsk_ctrl(ET_TRUE, 3, ET_FALSE, 0);
                break;
            default:
                break;
        }
        sys_ctrl.delay0--;
        if( (0 == sys_ctrl.delay0) && (sys_ctrl.sys_set < 1) ) {
            sys_ctrl.delay0  = 1000;
            sys_ctrl.sys_set++;
        }
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K1) ) {
            sys_ctrl.sub_sts = 3 + sys_ctrl.sys_set;
            sys_ctrl.delay0 = 10000;
        }
    }
    /* travel route set */
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.sys_sts = SYS_ST_ROUTE_UP;
        sys_ctrl.sub_sts = 0;
    }
    /* force level set */
    else if(4 == sys_ctrl.sub_sts) {
        segtsk_ctrl(ET_TRUE, glb_ctrl.force_lv+1, ET_FALSE, 0);
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K2) ) {
            if( glb_ctrl.force_lv < glb_cfgs.max_levl - 1 ) {
                glb_ctrl.force_lv++;
            }
            sys_ctrl.delay0 = 10000;
        }
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K4) ) {
            if( glb_ctrl.force_lv > 0 ) {
                glb_ctrl.force_lv--;
            }
            sys_ctrl.delay0 = 10000;
        }
        sys_ctrl.delay0--;
        if( (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) || (0 == sys_ctrl.delay0) ) {
            sub_save_sysctl(0, 0);
            goto BACK_STOP;
        }
    }

    /* auto close set - 1 */
    else if(10 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K2) ) {
            goto BACK_STOP;
        }
        if( 0 == sys_ctrl.delay0 ) {
            sys_ctrl.sub_sts = 11;
            sys_ctrl.delay0 = 10000;
        }
    }
    /* auto close set - 2 */
    else if(11 == sys_ctrl.sub_sts) {
        segtsk_ctrl(ET_TRUE, glb_ctrl.auto_clo, ET_FALSE, 0);
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K2) ) {
            if( glb_ctrl.auto_clo < 9 ) {
                glb_ctrl.auto_clo++;
            }
            sys_ctrl.delay0 = 10000;
        }
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K4) ) {
            if( glb_ctrl.auto_clo > 0 ) {
                glb_ctrl.auto_clo--;
            }
            sys_ctrl.delay0 = 10000;
        }
        sys_ctrl.delay0--;
        if( (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) || (0 == sys_ctrl.delay0) ) {
            sub_save_sysctl(0, 0);
            goto BACK_STOP;
        }
    }

    /* IRAD enable set - 1 */
    else if(20 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K4) ) {
            goto BACK_STOP;
        }
        if( 0 == sys_ctrl.delay0 ) {
            segtsk_ctrl(ET_FALSE, SEG_DLIN, ET_FALSE, 0);
            sys_ctrl.sub_sts = 21;
        }
    }
    /* IRAD enable set - 2 */
    else if(21 == sys_ctrl.sub_sts) {
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K4) ) {
            if( ET_TRUE == glb_ctrl.irad_ena ) {
                segtsk_ctrl(ET_FALSE, SEG_LETH, ET_FALSE, 0);
            }
            else {
                segtsk_ctrl(ET_FALSE, SEG_DLIN, ET_FALSE, 0);
            }
            sys_ctrl.sub_sts = 22;
            sys_ctrl.delay0 = 10000;
        }
    }
    /* IRAD enable set - 3 */
    else if(22 == sys_ctrl.sub_sts) {
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K2) ) {
            segtsk_ctrl(ET_FALSE, SEG_LETH, ET_FALSE, 0);
            glb_ctrl.irad_ena = ET_TRUE;
            sys_ctrl.delay0 = 10000;
        }
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K4) ) {
            segtsk_ctrl(ET_FALSE, SEG_DLIN, ET_FALSE, 0);
            glb_ctrl.irad_ena = ET_FALSE;
            sys_ctrl.delay0 = 10000;
        }
        sys_ctrl.delay0--;
        if( (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) || (0 == sys_ctrl.delay0) ) {
            sub_save_sysctl(0, 0);
            goto BACK_STOP;
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
    U08ET index;

    ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);

    /* operation cancel */
    if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K1) ) {
        sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
        sys_ctrl.sub_sts = 0;
        return ;
    }

    /* initialize parameter */
    if(0 == sys_ctrl.sub_sts) {
        sys_ctrl.sub_sts = 1;
        sys_ctrl.delay0 = 1000;
    }
    /* wait code key long press */
    else if(1 == sys_ctrl.sub_sts) {
        if( KEY_ST_LOW == getkey_curst(KEY_ID_K3) ) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
            return ;
        }
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sys_ctrl.sub_sts = 2;
            sys_ctrl.delay0 = 2500;
            segtsk_ctrl(ET_FALSE, SEG_NONE, ET_TRUE, 250);
        }
    }
    /* wait code key release */
    else if(2 == sys_ctrl.sub_sts) {
        if( KEY_ST_LOW == getkey_curst(KEY_ID_K3) ) {
            sys_ctrl.sub_sts = 3;
            sys_ctrl.delay0 = 10000;
            remote_gkey(rem_key);
        }
        sys_ctrl.delay0--;
        /* clear the saved remoter code */
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_FALSE, SEG_CLRS, ET_FALSE, 0);
            remote_clrs();
            sys_ctrl.delay0  = 500;
            sys_ctrl.sub_sts = 5;
        }
    }
    /* wait remoter key */
    else if(3 == sys_ctrl.sub_sts) {
        if( ET_TRUE == remote_gkey(sys_ctrl.rem_key) ) {
            sys_ctrl.sub_sts = 4;
            sys_ctrl.delay0 = 10000;
            segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
        }
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
    /* wait another remoter key */
    else if(4 == sys_ctrl.sub_sts) {
        if( ET_TRUE == remote_gkey(rem_key) ) {
            ngflg = 0;
            for(index=0; index<REID_SIZE; index++) {
                if( sys_ctrl.rem_key[index] != rem_key[index] ) {
                    ngflg = 1;
                }
            }
            if( (0 == ngflg) && (ET_TRUE==remote_save(rem_key)) ) {
                sys_ctrl.delay0 = 500;
                sys_ctrl.sub_sts = 5;
                segtsk_ctrl(ET_FALSE, SEG_NONE, ET_TRUE, 50);
            }
        }
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
    /* keep led status sometime */
    else if(5 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
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
    if( 0 == sys_ctrl.sub_sts ) {
        motors_pwms(0);
        motors_ctrl(MOT_CTS_STOP);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_OFF, 0);
        segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
        if( ET_TRUE == sys_ctrl.bit_flg.bit0 ) {
            sub_save_sysctl(0, 1);
        }
        sys_ctrl.delay0 = 500;
        sys_ctrl.sub_sts = 1;
    }
    /* delay sometime to be certain power if off */
    else if( 1 == sys_ctrl.sub_sts ) {
        sys_ctrl.delay0--;
        if( 0 == sys_ctrl.delay0 ) {
            while(1);
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : system get serious error                                 */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_bad_error(VODET)
{
    /* In door error recovery */
    if( (0xE4 == sys_ctrl.err_num) && (KEY_ST_LOW == getkey_stabe(KEY_ID_DOOR)) ) {
        if( ET_FALSE == sys_ctrl.bit_flg.bit0 ) {
            glb_ctrl.hall_top = 0;
            glb_ctrl.hall_bot = 0;
        }
        sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
        sys_ctrl.sub_sts = 0;
        return ;
    }

    /* stop motor power */
    if(0 == sys_ctrl.sub_sts) {
        motors_pwms(0);
        ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);
        if( (0xE1==sys_ctrl.err_num) || (0xE2==sys_ctrl.err_num) ||
            (0xE3==sys_ctrl.err_num) || (0xE5==sys_ctrl.err_num) ) {
            glb_ctrl.hall_top = 0;
            glb_ctrl.hall_bot = 0;
            sub_save_sysctl(0, 0);
        }
        sys_ctrl.delay0 = 100;
        sys_ctrl.sub_sts = 1;
    }
    /* stop motor excution */
    else if(1 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            motors_ctrl(MOT_CTS_STOP);
            sys_ctrl.sub_sts = 2;
        }
    }
    /* display 'E' */
    else if(2 == sys_ctrl.sub_sts) {
        segtsk_ctrl(ET_FALSE, SEG_EROR, ET_FALSE, 0);
        sys_ctrl.delay0 = 125;
        sys_ctrl.sub_sts = 3;
    }
    /* display none */
    else if(3 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
            sys_ctrl.delay0 = 125;
            sys_ctrl.sub_sts = 4;
        }
    }
    /* display error code */
    else if(4 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_TRUE, sys_ctrl.err_num & 0x0F, ET_FALSE, 0);
            sys_ctrl.delay0 = 125;
            sys_ctrl.sub_sts = 5;
        }
    }
    /* display none */
    else if(5 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
            sys_ctrl.delay0 = 125;
            sys_ctrl.sub_sts = 6;
        }
    }
    /* time delay */
    else if(6 == sys_ctrl.sub_sts) {
        sys_ctrl.delay0--;
        if(0 == sys_ctrl.delay0) {
            sys_ctrl.sub_sts = 2;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : system simulation function                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_simu_func(VODET)
{
    volatile U08ET *pStart;
    U32ET index;
    U08ET segdis;

    ledtsk_ctrl(LED_ID_LAMP, LED_ST_ON, 0);

    if( 0 == sys_ctrl.sub_sts ) {
        segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
        sys_ctrl.val_sum = 0;
        pStart = (volatile U08ET *)(0x00008000);
        for(index=0; index<0x00008000; index++) {
            wdtdog_feed();
            sys_ctrl.val_sum += *pStart;
            pStart++;
        }
        sys_ctrl.sub_sts++;
        sys_ctrl.delay0 = 250;
    }
    else if( (sys_ctrl.sub_sts >= 1) && (sys_ctrl.sub_sts <= 8) ) {
        if( 250 == sys_ctrl.delay0 ) {
            segtsk_ctrl(ET_FALSE, SEG_NONE, ET_FALSE, 0);
            sys_ctrl.delay0--;
        }
        else if( 0 == sys_ctrl.delay0 ) {
            segdis = sys_ctrl.val_sum >> 28;
            sys_ctrl.val_sum <<= 4;
            if( segdis < 10 ) {
                segtsk_ctrl(ET_TRUE, segdis, ET_FALSE, 0);
            }
            else {
                segtsk_ctrl(ET_TRUE, segdis-10, ET_TRUE, 0);
            }
            sys_ctrl.sub_sts++;
            sys_ctrl.delay0 = 500;
        }
        else {
            sys_ctrl.delay0--;
        }
    }
    else if( 9 == sys_ctrl.sub_sts ) {
        sys_ctrl.delay0--;
        if( 0 == sys_ctrl.delay0 ) {
            sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
            sys_ctrl.sub_sts = 0;
        }
    }
}

