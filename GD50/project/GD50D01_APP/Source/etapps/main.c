#include "main.h"
#include "subs.h"
#include <stdio.h>

/**
  ******************************************************************************
  * @file    main.c
  * @author  ETdoor team
  * @version V0.0.7
  * @date    2018/4/11
  * @brief   This file provides all application of GD50 garage door opener
  *          functionalities of the inital and listen for wifi adapter:
  *           + Interface seting, the current Interface is usart_1
  *           + error process
  *
 ===============================================================================

  ******************************************************************************
  * @attention
  *
  * &copy; COPYRIGHT 2018 ETdoor
  *
  *
  ******************************************************************************
 ===============================================================================

  ******************************************************************************
  * @verlog
  *
  * V0.0.0: original version;
  * V0.0.1: interface similar to GD125;
  * V0.0.2: slow stop change to 35 r/s;
  * V0.0.3: max currnet set as 6A;
  * V0.1.0: ET indoor keypad add;
  * V0.1.1: blink when touch ref hall;
  * V0.1.2: WiFi adapter add;
  * V0.1.3: reconstruction;
  *
  *
  ******************************************************************************
  */

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static SYS_CTRL sys_ctrl;

/**
  * @brief  application functions
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
const FUNC func[SYS_ST_INVALID] = {
    sys_power_on,     sys_read_eep,    sys_limittop_set, sys_limitbot_set,
    sys_train_open,   sys_train_close, sys_standby,      sys_excute_open,
    sys_excute_close, sys_excute_back, sys_remote_set,   sys_power_off,
    sys_err_process,
};

/**
  * @brief  force level define
  * @param  NONE
  * @retval NONE
  *
  */
const U08ET bk_force[8][6] = {
    {60, 60, 40, 60, 50, 40},     {75, 75, 45, 80, 70, 45},
    {90, 70, 30, 100, 70, 30},    {100, 100, 60, 120, 110, 65},
    {115, 110, 70, 135, 125, 70}, {130, 120, 80, 150, 140, 80},
    {145, 130, 90, 165, 155, 90}, {180, 160, 100, 180, 170, 100},
};

/**
  * @brief  c_entry
  * @param  NONE
  * @retval NONE
  *
  */
int main(VODET) {
  sys_pre_init();

  while (1) {
    if (ET_FALSE == tskskm_flag())
      continue;

    wdtdog_feed();
    LedBuzLampTask();
    keysmp_task();
    adcsmp_task();
    remote_sys_task();
    wifi_sys_task();
    WiFi_process();
    abnor_detectTask();
    func[sys_ctrl.sys_sts]();
  }
}

/**
  * @brief  System prepare initalize
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET sys_pre_init(VODET) {
  sysclk_init();
  uartct0_init();
  uartct1_init();
  motors_init();
  remote_init();
  LedBuzLampInital();
  adcsmp_init();
  keysmp_init();
  timwdt_init();
  wifi_init();
  __enable_irq();

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

  sys_ctrl.WiFi_connected_cnt = WIFI_OVERTIME;
  sys_ctrl.WiFi_connected = ET_FALSE;
  sys_ctrl.WiFi_PreLock = WIFI_UNLOCK;
  sys_ctrl.WiFi_DoorCommand = 0xff;
  sys_ctrl.WiFi_LampCommand = 0xff;
  sys_ctrl.WiFi_ProgramCommand = 0xff;
  Lamp_ctrl(0, 0, 0, 1000);

#ifdef START_STEP_DEBUG
  printf("system inital success\r\n");
#endif
}

/**
  * @brief  System power on
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET sys_power_on(VODET) {
  if (0 == sys_ctrl.sub_sts) {
#ifdef POWER_ON_DEBUG
    printf("power on\r\n");
#endif
    sys_ctrl.delay0 = 100;
    sys_ctrl.sub_sts = 1;
  }

  if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      if (PRG_MAIN_VER & (0x1 << 0))
        LedBuzTaskCtrl(LED_ID_CODE, 1, 2, 200);
      if (PRG_MAIN_VER & (0x1 << 1))
        LedBuzTaskCtrl(LED_ID_SET, 1, 2, 200);
      if (PRG_MAIN_VER & (0x1 << 2))
        LedBuzTaskCtrl(LED_ID_DN, 1, 2, 200);
      if (PRG_MAIN_VER & (0x1 << 3))
        LedBuzTaskCtrl(LED_ID_UP, 1, 2, 200);
      sys_ctrl.delay0 = 50;
      sys_ctrl.sub_sts = 2;
    }
  }

  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_READ_EEP;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }
}

/**
  * @brief  System read eep
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET sys_read_eep(VODET) {
  U32ET *pbuf;
  U32ET size, pflash;
#ifdef START_READEEP_DEBUG
  U32ET index;
#endif

/* read system control */
#ifdef START_READEEP_DEBUG
  printf("func <sys_read_eep>\r\n\r\n");
#endif
  size = (OFFSETOF(GLB_CTRL, save_end) + 1) / 4;
  pbuf = (U32ET *)(&glb_ctrl);
  pflash = DFMC_CTRLS_OFST;
  dflash_read_page(pbuf, pflash, size);
  wdtdog_feed();
#ifdef START_READEEP_DEBUG
  printf("save flag : 0x%x\r\n", (U16ET)glb_ctrl.save_flg);
#endif

  size = UP_FORCE_SAMP / 4;
  dflash_read_page((U32ET *)(&glb_ctrl.upForce[0])
                   , DFMC_UPFORCE_OFST
                   , size);
  wdtdog_feed();

  size = DN_FORCE_SAMP / 4;
  dflash_read_page((U32ET *)(&glb_ctrl.dnForce[0])
                   , DFMC_DNFORCE_OFST
                   , size);
  wdtdog_feed();

#ifdef START_READEEP_DEBUG
  printf("pre to read remote key\r\n");
#endif
  remote_keys_init();
#ifdef START_READEEP_DEBUG
  printf("read keys success\r\n");
#endif

  /* check saved flag */
  if (DFLASH_SAVE_FLG != glb_ctrl.save_flg) {
#ifdef START_READEEP_DEBUG
    printf("dataflash flag_1\r\n");
#endif
    glb_ctrl.save_flg = DFLASH_SAVE_FLG;
    glb_ctrl.hall_top = 0;
    glb_ctrl.hall_bot = 0;
    glb_ctrl.hall_cur = 0;
    glb_ctrl.hall_ratio = 69;
    glb_ctrl.force_lv = 4;
    glb_ctrl.vacation = ET_FALSE;
    glb_ctrl.lamp_secs = 3;
    glb_ctrl.WiFi_preset_position = 18;
    halsig_mode(HAL_MOD_NONE);
    sys_ctrl.bit_flg.bit0 = ET_TRUE;
    sys_ctrl.sys_sts = SYS_ST_STANDBY;
    sys_ctrl.sub_sts = 0;
    return;
  }

  /* check if route parameter is normal */
  if ((glb_ctrl.hall_top >= glb_ctrl.hall_bot) ||
      (glb_ctrl.hall_bot > HAL_CNT_MAX)) {
    glb_ctrl.hall_top = 0;
    glb_ctrl.hall_bot = 0;
    glb_ctrl.hall_cur = 0;
    glb_ctrl.force_lv = 4;
    halsig_mode(HAL_MOD_NONE);
  } else
    halsig_mode(HAL_MOD_NOR);
  /* set current motor status */
  halsig_scnt(glb_ctrl.hall_cur);
  halsig_ratio(glb_ctrl.hall_ratio);

#ifdef START_READEEP_DEBUG
  printf("UP MAX:%d\r\n", glb_ctrl.ufor_max);
  for (index = 0; index < UP_FORCE_SAMP; index++) {
    printf("%d\r\n", glb_ctrl.upForce[index]);
    wdtdog_feed();
  }
  printf("DN MAX:%d\r\n", glb_ctrl.dfor_max);
  for (index = 0; index < DN_FORCE_SAMP; index++) {
    printf("%d\r\n", glb_ctrl.dnForce[index]);
    wdtdog_feed();
  }
#endif
  if (glb_ctrl.hall_cur <= glb_ctrl.hall_top)
    sys_ctrl.lst_dir = MOT_CTS_UPUP;

  /* check force level */
  if (glb_ctrl.force_lv > FORCE_LVL_MAX - 1) {
    glb_ctrl.force_lv = 4;
  }

  if (0 == glb_ctrl.hall_ratio)
    glb_ctrl.hall_ratio = 69;

  glb_ctrl.force_lv = 4;
  sys_ctrl.bit_flg.bit0 = ET_TRUE; // for flashrom save judge
  sys_ctrl.sys_sts = SYS_ST_STANDBY;
  sys_ctrl.sub_sts = 0;
  return;
}

/**
  * @brief  System limit top set
  * @param  NONE
  * @retval NONE
  *
  */
VODET sys_limittop_set(VODET) {
  if (0 == sys_ctrl.sub_sts) {
#ifdef LIMITSET_DEBUG
    printf("func <top set>\r\n");
#endif
    glb_ctrl.hall_top = 0;
    glb_ctrl.hall_bot = 0;
    /*set hall cnt as mode<none regulate> */
    halsig_mode(HAL_MOD_NONE);
    halsig_scnt(10000);
    sys_ctrl.delay0 = 1000;
    sys_ctrl.sub_sts = 1;
    return;
  } else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 250);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_OFF, 0, 0);
      Lamp_ctrl(0, 0, 0, 0);
      sys_ctrl.delay1 = 20000;
      sys_ctrl.sub_sts = 2;
      return;
    }

    if (ET_FALSE == getkey_stabe(KEY_A_UP)) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
      return;
    }
  } else if (2 == sys_ctrl.sub_sts) {
    if (ET_TRUE == getkey_stabe(KEY_A_UP)) {
      motors_ctrl(MOT_CTS_UPUP);
      halsig_sdir(MOT_CTS_UPUP);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 10;
      sys_ctrl.cur_dir = MOT_CTS_UPUP;
      return;
    }
    if (ET_TRUE == getkey_stabe(KEY_A_DOWN)) {
      motors_ctrl(MOT_CTS_DOWN);
      halsig_sdir(MOT_CTS_DOWN);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 10;
      sys_ctrl.cur_dir = MOT_CTS_DOWN;
      return;
    }
    /* set current position as top side */
    if (ET_TRUE == getkey_stabe(KEY_A_SET)) {
      halsig_scnt(HAL_TOP_DFT);
      glb_ctrl.hall_top = HAL_TOP_DFT;
      sys_ctrl.sub_sts = 6;
      sys_ctrl.delay1 = 20000;
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 0);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
#ifdef LIMITSET_DEBUG
      printf("top_set top : %d\r\n", (U16ET)glb_ctrl.hall_top);
      printf("top_set bot : %d\r\n", (U16ET)glb_ctrl.hall_bot);
      printf("top_set cur : %d\r\n", (U16ET)halsig_gcnt());
#endif
      return;
    }
    /* out of time ----- clear all limit data and return */
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
      glb_ctrl.hall_top = 0;
      glb_ctrl.hall_bot = 0;
      sys_ctrl.err_num = ERR_ROUTE_OUTTIME;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.sub_sts = 0;
      return;
    }
  } else if (3 == sys_ctrl.sub_sts) {
    /* move begin */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 4;
    }
  }
  /***start motor */
  else if (4 == sys_ctrl.sub_sts) {
    /* turn up pwm ratio */
    sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
    motors_pwms(sys_ctrl.cur_pwm);
    if ((MOT_CTS_UPUP == sys_ctrl.cur_dir)
        && (ET_FALSE == getkey_stabe(KEY_A_UP))) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 5;
      sys_ctrl.delay0 = 100;
    }

    if ((MOT_CTS_DOWN == sys_ctrl.cur_dir)
        && (ET_FALSE == getkey_stabe(KEY_A_DOWN))) {
      /* detect button relase, cut off pwm, jump to relay cut*/
      motors_pwms(0);
      sys_ctrl.sub_sts = 5;
      sys_ctrl.delay0 = 100;
    }
  }

  /* delay to stop motor */
  else if (5 == sys_ctrl.sub_sts) {
    /* delay smoe time, then turn off relay */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay1 = 20000;
    }
  }

  /* jump to limit bottom set */
  else if (6 == sys_ctrl.sub_sts) {
    if (ET_TRUE == getkey_stabe(KEY_A_DOWN)) {
      /* jump to function<bottom side set> */
      sys_ctrl.sys_sts = SYS_ST_LIMITBOT_SET;
      sys_ctrl.sub_sts = 0;
    }
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
      glb_ctrl.hall_top = 0;
      glb_ctrl.hall_bot = 0;
      sys_ctrl.err_num = ERR_ROUTE_OUTTIME;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.sub_sts = 0;
    }
  }
}

/**
  * @brief  System limit bot set
  * @param  NONE
  * @retval NONE
  *
  */
VODET sys_limitbot_set(VODET) {
  U16ET hallCnt;

  hallCnt = halsig_gcnt();
  /* iniltal parameter for dn set */
  if (0 == sys_ctrl.sub_sts) {
#ifdef LIMITSET_DEBUG
    printf("func <bot set>\r\n");
#endif
    sys_ctrl.delay0 = 1000;
    sys_ctrl.sub_sts = 1;
    return;
  }
  /* wait for long press */
  else if (1 == sys_ctrl.sub_sts) {
    /* press keep 2s, or return function<top set, sub 6> */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.delay1 = 20000;
      sys_ctrl.sub_sts = 2;
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 0);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 250);
      return;
    }

    /* set over time, return func<sys_standby> */
    if (ET_FALSE == getkey_stabe(KEY_A_DOWN)) {
      sys_ctrl.sys_sts = SYS_ST_LIMITTOP_SET;
      sys_ctrl.sub_sts = 6;
      sys_ctrl.delay1 = 20000;
      return;
    }
  }
  /* wait for control */
  else if (2 == sys_ctrl.sub_sts) {
    if ((ET_TRUE == getkey_stabe(KEY_A_UP))
        && (hallCnt > glb_ctrl.hall_top)) {
      motors_ctrl(MOT_CTS_UPUP);
      halsig_sdir(MOT_CTS_UPUP);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 10;
      sys_ctrl.cur_dir = MOT_CTS_UPUP;
      return;
    }

    if (ET_TRUE == getkey_stabe(KEY_A_DOWN)) {
      motors_ctrl(MOT_CTS_DOWN);
      halsig_sdir(MOT_CTS_DOWN);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 10;
      sys_ctrl.cur_dir = MOT_CTS_DOWN;
      return;
    }

    if (ET_TRUE == getkey_stabe(KEY_A_SET)) {
      glb_ctrl.hall_bot = hallCnt;
      if ((glb_ctrl.hall_bot < glb_ctrl.hall_top)
          || (glb_ctrl.hall_bot - glb_ctrl.hall_top < 33)) {
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        sys_ctrl.err_num = ERR_ROUTE_INVALID;
        sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
        sys_ctrl.sub_sts = 0;
      } else {
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 0);
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 0);
        glb_ctrl.hall_bot = hallCnt;
        sys_ctrl.sub_sts = 6;
        sys_ctrl.delay1 = 20000;
      }
#ifdef LIMITSET_DEBUG
      printf("bot_set top : %d\r\n", (U16ET)glb_ctrl.hall_top);
      printf("bot_set bot : %d\r\n", (U16ET)glb_ctrl.hall_bot);
      printf("bot_set cur : %d\r\n", (U16ET)hallCnt);
#endif
      return;
    }

    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
      glb_ctrl.hall_top = 0;
      glb_ctrl.hall_bot = 0;
      sys_ctrl.err_num = ERR_ROUTE_OUTTIME;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.sub_sts = 0;
    }
  }
  /*  delay to start */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 4;
    }
  }
  /***start motor */
  else if (4 == sys_ctrl.sub_sts) {
    sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
    motors_pwms(sys_ctrl.cur_pwm);

    if ((MOT_CTS_UPUP == sys_ctrl.cur_dir)
       && (ET_FALSE == getkey_stabe(KEY_A_UP) || hallCnt <= glb_ctrl.hall_top)) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 5;  
      sys_ctrl.delay0 = 100;
    }

    if ((MOT_CTS_DOWN == sys_ctrl.cur_dir)
        && (ET_FALSE == getkey_stabe(KEY_A_DOWN))) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 5;
      sys_ctrl.delay0 = 100;
    }
  }

  /* delay to stop motor */
  else if (5 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay1 = 20000;
#ifdef LIMITSET_DEBUG
      printf("debug hallcur : %d\r\n", hallCnt);
#endif
    }
  }

  /* jump to train open */
  else if (6 == sys_ctrl.sub_sts) {
    if (ET_TRUE == getkey_stabe(KEY_A_UP)) {
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
      Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
      sys_ctrl.delay0 = 350;
      sys_ctrl.sub_sts = 7;
#ifdef LIMITSET_DEBUG
      printf("bot_set top : %d\r\n", (U16ET)glb_ctrl.hall_top);
      printf("bot_set bot : %d\r\n", (U16ET)glb_ctrl.hall_bot);
      printf("bot_set cur : %d\r\n", (U16ET)hallCnt);
#endif
    }
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
      glb_ctrl.hall_top = 0;
      glb_ctrl.hall_bot = 0;
      sys_ctrl.err_num = ERR_ROUTE_OUTTIME;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.sub_sts = 0;
    }
  } else if (7 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_TRAIN_OPEN;
      sys_ctrl.sub_sts = 0;
    }
  }
}

/**
  * @brief  System train open
  * @param  NONE
  * @retval NONE
  *
  */
VODET sys_train_open(VODET) {
  U08ET             adcValue;
  ETST_HALL_LEARN   hal_learn;
  U16ET             hallCnt, hallSmp, index;
  
  hallCnt  = halsig_gcnt();
  adcValue = getadc_data(ADC_CH_MOT);
  hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1)
             / UP_FORCE_SAMP;

  hallCnt = halsig_gcnt();
  /* iniltal parameter for open set */
  if (0 == sys_ctrl.sub_sts) {
#ifdef TRAIN_DEBUG
    printf("func <train open>\r\n");
#endif
    /* clean all data */
    for (index = 0; index < UP_FORCE_SAMP; index++) 
      glb_ctrl.upForce[index] = 0;

    halsig_mode(HAL_MOD_LEARN);
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    glb_ctrl.ufor_max = 0;
    halsig_sdir(MOT_CTS_UPUP);
    motors_ctrl(MOT_CTS_UPUP);
    LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 250);
    LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 0);
    Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
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

  /** run until top side */
  else if (2 == sys_ctrl.sub_sts) {
    if (hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP)
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
    else
      sub_sped_stable(&sys_ctrl.cur_pwm 
                      , PWM_VAL_MAX_RUN
                      , PWM_VAL_MIN_ALL
                      , HAL_DUTY_SLOW);

    motors_pwms(sys_ctrl.cur_pwm);

    /* beyond top side, stop motor */
    if (hallCnt <= glb_ctrl.hall_top) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;
      return;
    }
    /* learn force of the door close duration */
    if ((hallCnt >= glb_ctrl.hall_top)
       && (hallCnt <= glb_ctrl.hall_bot)) {
       	
      if (hallCnt < glb_ctrl.hall_top)
        hallCnt = glb_ctrl.hall_top;
      else if (hallCnt > glb_ctrl.hall_bot)
        hallCnt = glb_ctrl.hall_bot;

      index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;
      glb_ctrl.upForce[index] = (adcValue > glb_ctrl.upForce[index])
                                    ? adcValue
                                    : glb_ctrl.upForce[index];

      if (adcValue > glb_ctrl.ufor_max)
        glb_ctrl.ufor_max = adcValue;

      /* fill the slow stop area */
      if ((hallCnt <= glb_ctrl.hall_top + HAL_CNT_SLOW_STOP)
         && (hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP - 60)){
        if ( glb_ctrl.upForce[index] <
                 glb_ctrl.upForce[((glb_ctrl.hall_bot - glb_ctrl.hall_top
                                  - HAL_CNT_SLOW_STOP)
                                  / hallSmp)
                                  - 2]) {
          glb_ctrl.upForce[index] =
              glb_ctrl.upForce[((glb_ctrl.hall_bot - glb_ctrl.hall_top
                               - HAL_CNT_SLOW_STOP)
                               / hallSmp)
                               - 2];
        }
      }
    }
  }
  /* stop motor power */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      /* fill the tail */
      index = UP_FORCE_SAMP - 1;
      while (0 == glb_ctrl.upForce[index]) 
        index--;
      for (; index < UP_FORCE_SAMP; index++) 
        glb_ctrl.upForce[index] = glb_ctrl.upForce[index - 1];

      motors_ctrl(MOT_CTS_STOP);
      sys_ctrl.delay0 = 250;
      sys_ctrl.sub_sts = 4;
    }
  }
  /* motor have a break */
  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
/* printf data before learn */
#ifdef TRAIN_DEBUG
      printf("ratio             : %d\r\n", (U16ET)glb_ctrl.hall_ratio);
      printf("hallCnt           : %d\r\n", (U16ET)hallCnt);
      printf("glb_ctrl.hall_top : %d\r\n", (U16ET)glb_ctrl.hall_top);
      printf("glb_ctrl.hall_bot : %d\r\n", (U16ET)glb_ctrl.hall_bot);
#endif
      /* learn hall parameter*/
      hal_learn = hall_learn();
      if ((0 == hal_learn.ratio)
         || (0 == hal_learn.skew)) {
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        sys_ctrl.err_num = ERR_HAL_LEARN_FAIL;
        sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
        sys_ctrl.sub_sts = 0;
        return;
      }
      /* get ratio */
      glb_ctrl.hall_ratio = hal_learn.ratio;
      /* regulate skew */
      hallCnt -= hal_learn.skew;
      glb_ctrl.hall_top -= hal_learn.skew;
      glb_ctrl.hall_bot -= hal_learn.skew;
      /* set hall parameter */
      halsig_ratio(glb_ctrl.hall_ratio);
      halsig_scnt(hallCnt);
      halsig_mode(HAL_MOD_NOR);
/* printf data after learn */
#ifdef TRAIN_DEBUG
      printf("ratio             : %d\r\n", (U16ET)glb_ctrl.hall_ratio);
      printf("skew              : %d\r\n", (U16ET)hal_learn.skew);
      printf("hallCnt           : %d\r\n", (U16ET)hallCnt);
      printf("glb_ctrl.hall_top : %d\r\n", (U16ET)glb_ctrl.hall_top);
      printf("glb_ctrl.hall_bot : %d\r\n", (U16ET)glb_ctrl.hall_bot);
#endif
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 0);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 0);
      sys_ctrl.sub_sts = 5;
      sys_ctrl.delay1 = 20000;
    }
  }
  /**wait for long press of key_DN*/
  else if (5 == sys_ctrl.sub_sts) {
    if (ET_TRUE == getkey_stabe(KEY_A_DOWN)) {
      sys_ctrl.sys_sts = SYS_ST_TRAIN_CLOSE;
      sys_ctrl.sub_sts = 0;
    }
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
      glb_ctrl.hall_top = 0;
      glb_ctrl.hall_bot = 0;
#ifdef TRAIN_DEBUG
      printf("UP MAX:%d\r\n", (U16ET)glb_ctrl.ufor_max);
      for (index = 0; index < UP_FORCE_SAMP; index++) {
        printf("%d\r\n", (U16ET)glb_ctrl.upForce[index]);
        wdtdog_feed();
      }
#endif
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
    }
  }
}

/**
  * @brief  System train close
  * @param  NONE
  * @retval NONE
  *
  */
void sys_train_close(void) {
  U16ET hallCnt, hallSmp, index;
  U08ET adcValue;

  hallCnt  = halsig_gcnt();
  adcValue = getadc_data(ADC_CH_MOT);
  hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + DN_FORCE_SAMP - 1)
             / DN_FORCE_SAMP;

  /* iniltal parameter for open set */
  if (0 == sys_ctrl.sub_sts) {
#ifdef TRAIN_DEBUG
    printf("func <train close>\r\n");
#endif
    for (index = 0; index < DN_FORCE_SAMP; index++) 
      glb_ctrl.dnForce[index] = 0;

    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    glb_ctrl.dfor_max = 0;
    halsig_sdir(MOT_CTS_DOWN);
    motors_ctrl(MOT_CTS_DOWN);
    LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 0);
    LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 250);
    Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
  } else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
    }
  } else if (2 == sys_ctrl.sub_sts) {
    /* blink when touch ref hal */
    if ((hallCnt == (((glb_ctrl.hall_bot + glb_ctrl.hall_top) / 2) -
                     ((glb_ctrl.hall_bot + glb_ctrl.hall_top) / 2) %
                         glb_ctrl.hall_ratio))
       || (hallCnt == (glb_ctrl.hall_bot -
                      (glb_ctrl.hall_bot % glb_ctrl.hall_ratio)
                       - glb_ctrl.hall_ratio)))
      LedBuzTaskCtrl(LED_ID_SET, OUT_ST_ON, 1, 200);
  	
  	
    if (hallCnt < glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP)
      sub_sped_raises(&sys_ctrl.cur_pwm
                      , PWM_VAL_MAX_RUN
                      , 2);
    else
      sub_sped_stable(&sys_ctrl.cur_pwm
                      , PWM_VAL_MAX_RUN
                      , PWM_VAL_MIN_ALL
                      , HAL_DUTY_SLOW);

    motors_pwms(sys_ctrl.cur_pwm);
    /* hit bot side, stop motor */
    if (hallCnt >= glb_ctrl.hall_bot) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;
      return;
    }
    /* learn force */
    if ((hallCnt >= glb_ctrl.hall_top)
       && (hallCnt <= glb_ctrl.hall_bot)) {
       	
      if (hallCnt < glb_ctrl.hall_top)
        hallCnt = glb_ctrl.hall_top;
      else if (hallCnt > glb_ctrl.hall_bot)
        hallCnt = glb_ctrl.hall_bot;
      
      index = (hallCnt - glb_ctrl.hall_top) / hallSmp;
      glb_ctrl.dnForce[index] = (adcValue > glb_ctrl.dnForce[index])
                                    ? adcValue
                                    : glb_ctrl.dnForce[index];
      
      if (adcValue > glb_ctrl.dfor_max)
        glb_ctrl.dfor_max = adcValue;
      
      if ((hallCnt >= (glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP))
         && (hallCnt < (glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP + 60))) {
        if (glb_ctrl.dnForce[index] <
               glb_ctrl.dnForce[((glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP
                                - glb_ctrl.hall_top) 
                                / hallSmp)
                                - 2]){
          glb_ctrl.dnForce[index] =
             glb_ctrl.dnForce[((glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP
                              - glb_ctrl.hall_top) 
                              / hallSmp)
                              - 2];
        }                             
      }
    }
  }
  /* stop motor power */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      index = DN_FORCE_SAMP - 1;
      while (glb_ctrl.dnForce[index] == 0) 
        index--;
      for (; index < DN_FORCE_SAMP; index++) 
        glb_ctrl.dnForce[index] = glb_ctrl.dnForce[index - 1];
      
      motors_ctrl(MOT_CTS_STOP);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      sys_ctrl.delay0 = 250;
      sys_ctrl.sub_sts = 4;
    }
  }
  /* motor have a break */
  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
      sys_ctrl.lst_dir = MOT_CTS_DOWN;

#ifdef TRAIN_DEBUG
      printf("train save before\r\n");

      printf("be UP MAX:%d\r\n", glb_ctrl.ufor_max);
      for (index = 0; index < UP_FORCE_SAMP; index++) {
        printf("be %d\r\n", (U16ET)glb_ctrl.upForce[index]);
        wdtdog_feed();
      }

      printf("be DN MAX:%d\r\n", (U16ET)glb_ctrl.dfor_max);
      for (index = 0; index < DN_FORCE_SAMP; index++) {
        printf("be %d\r\n", glb_ctrl.dnForce[index]);
        wdtdog_feed();
      }
#endif
      sub_save_sysctl(1);
    }
  }
}

/**
  * @brief  System standby
  * @param  NONE
  * @retval NONE
  *
  */
void sys_standby(void) {
  U16ET hallCnt;
  U08ET infra_status;
  WIRELESS_COMMAND command;
  ENUM_DIRECTION direction;

  hallCnt      = halsig_gcnt();
  infra_status = getkey_stabe(KEY_P_INFRA);
  command      = get_wireless_command();
  direction    = DIR_NONE;

  if (0 == sys_ctrl.sub_sts) {
#ifdef STANDBY_DEBUG
    printf("func <standby>\r\n");
#endif
    get_wireless_command();
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay1 = 1500;
    sys_ctrl.delay3_vacation = 0;
    return;
  } else if (1 == sys_ctrl.sub_sts) {
    /* delay 3sec, then close all output except Lamp */
    if (sys_ctrl.delay1 > 0)
      sys_ctrl.delay1--;
    
    if (1 == sys_ctrl.delay1) {
      LedBuzTaskCtrl(LED_ID_SET, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_OFF, 0, 0);
    }
    /* ===============================================================================
       -------------------------------vacation mode
       blink-----------------------------
       ===============================================================================*/
    if (ET_TRUE == glb_ctrl.vacation) {
      if (0 == sys_ctrl.delay3_vacation) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 1, 250);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 1, 250);
      }
      
      if (sys_ctrl.delay3_vacation < 500)
        sys_ctrl.delay3_vacation++;
      else
        sys_ctrl.delay3_vacation = 0;
    } else {
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
    }
    /* ===============================================================================
       -------------------------limit set & force train
       ------------------------------
       ===============================================================================*/
    /* limit set from button */
    if (ET_TRUE == getkey_stabe(KEY_A_UP)) {
      sys_ctrl.sys_sts = SYS_ST_LIMITTOP_SET;
      sys_ctrl.sub_sts = 0;
      return;
    }
    /* ===============================================================================
       -------------------------------remote command
       ---------------------------------
       ===============================================================================*/
    /* remote set from button */
    if (ET_TRUE == getkey_stabe(KEY_A_CODE)) {
      sys_ctrl.sys_sts = SYS_ST_REMOTE_ST;
      sys_ctrl.sub_sts = 0;
      return;
    }

    /* remote set from wifi */
    if (COMMAND_WIFI_PROGRAM_ON == command) {
      sys_ctrl.sys_sts = SYS_ST_REMOTE_ST;
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 20000;
      return;
    }
    /* ===============================================================================
       -----------------------------lamp light&silent
       --------------------------------
       ===============================================================================*/
    /* infra trig lamp light */
    if (ET_TRUE == infra_status)
      Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);

    /* lamp set from keypad */
    if (COMMAND_KEYPAD_LAMP == command) {
      if (get_out_state(LAMP_ID))
        Lamp_ctrl(0, 0, 0, 0);
      else
        Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
      return;
    }

    /* lamp set from wifi  */
    if (COMMAND_WIFI_LAMP_ON == command) {
      Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
      return;
    }
    
    if (COMMAND_WIFI_LAMP_OFF == command) {
      Lamp_ctrl(0, 0, 0, 0);
      return;
    }
    /* ===============================================================================
       -------------------------------lock command -----------------------------------
       ===============================================================================*/
    /* lock set from keypad  */
    if (COMMAND_KEYPAD_LOCK == command) {
      glb_ctrl.vacation = ET_TRUE ? ET_FALSE : ET_TRUE;
      return;
    }
    /* ===============================================================================
       -------------------------------door command------------------------------------
       ===============================================================================*/
    /* hold --- disable infra  */
    if (COMMAND_KEYPAD_HOLD_EN == command)
      infra_status = ET_FALSE;

    /*********************************move mode judge**********************************/
    /*--------- troggle  command ----------*/
    if (((COMMAND_OUTDOOR_DOOR == command) && (ET_TRUE != glb_ctrl.vacation)) 
       || (COMMAND_KEYPAD_DOOR == command) 
       || ((COMMAND_WIFI_DOOR_TOGGLE == command) && (WIFI_LOCKED != sys_ctrl.WiFi_PreLock) && (ET_TRUE != glb_ctrl.vacation)) 
       || (COMMAND_KEYPAD_HOLD_EN == command)) {
      if (MOT_CTS_DOWN == sys_ctrl.lst_dir) {
        if (hallCnt <= glb_ctrl.hall_top) {
          if (ET_FALSE == infra_status) {
            direction = DIR_CLOSE;
            sys_ctrl.hall_dest = glb_ctrl.hall_bot;
          } else {
            direction = DIR_STADY;
          }
        } else {
          direction = DIR_OPEN;
          sys_ctrl.hall_dest = glb_ctrl.hall_top;
        }
      } else {
        if (ET_TRUE == infra_status) {
          if (hallCnt > glb_ctrl.hall_top) {
            direction = DIR_OPEN;
            sys_ctrl.hall_dest = glb_ctrl.hall_top;
          }
        } else {
          if (hallCnt >= glb_ctrl.hall_bot) {
            direction = DIR_OPEN;
            sys_ctrl.hall_dest = glb_ctrl.hall_top;
          } else {
            direction = DIR_CLOSE;
            sys_ctrl.hall_dest = glb_ctrl.hall_bot;
          }
        }
      }
    }
    /* WiFi close command */
    else if ((COMMAND_WIFI_DOOR_CLOSE == command)
            && (WIFI_LOCKED != sys_ctrl.WiFi_PreLock)) {
      if (ET_TRUE == infra_status) {
        direction = DIR_STADY;
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 1, 1500);
      } else {
        if (hallCnt >= glb_ctrl.hall_bot) {
          direction = DIR_STADY;
        } else {
          direction = DIR_CLOSE;
          sys_ctrl.hall_dest = glb_ctrl.hall_bot;
        }
      }
    }
    /* WiFi open command */
    else if ((COMMAND_WIFI_DOOR_OPEN == command)
            && (WIFI_LOCKED != sys_ctrl.WiFi_PreLock)) {
      if (hallCnt <= glb_ctrl.hall_top) {
        direction = DIR_STADY;
      } else {
        direction = DIR_OPEN;
        sys_ctrl.hall_dest = glb_ctrl.hall_top;
      }
    }
    /* WiFi preset command */
    else if ((COMMAND_WIFI_DOOR_PRESET == command)
            && (WIFI_LOCKED != sys_ctrl.WiFi_PreLock)) {
      sys_ctrl.hall_dest = glb_ctrl.hall_bot
                           - ((U16ET)glb_ctrl.WiFi_preset_position * 254 / 180);
      
      if (sys_ctrl.hall_dest < glb_ctrl.hall_top)
        sys_ctrl.hall_dest = glb_ctrl.hall_top;

      if (sys_ctrl.hall_dest < hallCnt - 1) {
        direction = DIR_OPEN;
      } else if (sys_ctrl.hall_dest > hallCnt + 1) {
        if (ET_FALSE == infra_status)
          direction = DIR_CLOSE;
        else {
          direction = DIR_STADY;
          LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 1, 1500);
        }
      } else {
        direction = DIR_STADY;
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 1, 1500);
      }
    }

    /*   move    */
    if (DIR_NONE != direction) {
      /* move without route */
      if ((0 == glb_ctrl.hall_top) 
         || (0 == glb_ctrl.hall_bot)) {
        sys_ctrl.err_num = ERR_EXCUTE_WITHOUT_ROUTE;
        sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
        sys_ctrl.sub_sts = 0;
        return;
      }

      if (DIR_OPEN == direction) {
        sys_ctrl.sys_sts = SYS_ST_EXCUTE_OPEN;
        sys_ctrl.sub_sts = 0;
      } else if (DIR_CLOSE == direction) {
        sys_ctrl.sys_sts = SYS_ST_EXCUTE_CLOSE;
        sys_ctrl.sub_sts = 0;
      }
      
      sys_ctrl.door_command_type = command;
    }
  }
}

/**
  * @brief  System excute open
  * @param  NONE
  * @retval NONE
  *
  */
void sys_excute_open(void) {
  U16ET hallCnt, hallSmp, index;
  U08ET adcValue;
  U16ET force_res, calcForce;
  WIRELESS_COMMAND command;

  command  = get_wireless_command();
  hallCnt  = halsig_gcnt();
  adcValue = getadc_data(ADC_CH_MOT);
  hallSmp  = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1) 
            / UP_FORCE_SAMP;

  if (((COMMAND_OUTDOOR_DOOR == command) && (ET_TRUE != glb_ctrl.vacation))
     || (COMMAND_KEYPAD_DOOR == command)
     || (COMMAND_KEYPAD_HOLD_DIS == command)) {
#ifdef EXCUTE_DEBUG
    printf("st_open get stop signal\r\n\r\n");
#endif
    motors_pwms(0);
    sys_ctrl.sub_sts = 3;
    sys_ctrl.delay0 = 200;
    return;
  }

  /* lock set from keypad  */
  if (COMMAND_KEYPAD_LOCK == command)
    glb_ctrl.vacation = ET_TRUE ? ET_FALSE : ET_TRUE;

  /* motor direction up */
  if (0 == sys_ctrl.sub_sts) {
#ifdef EXCUTE_DEBUG
    printf("func <excute up>\r\n\r\n");
#endif
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    sys_ctrl.delay2 = 0;
    halsig_sdir(HAL_DIR_UPUP);
    motors_ctrl(MOT_CTS_UPUP);
    LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
    LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 0);
    Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
    return;
  }
  /* start motor power */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
#ifdef EXCUTE_DEBUG
      printf("sub1 -> sub2\r\n\r\n");
#endif
      sub_clrs_status();
      sys_ctrl.delay0 = 1500;
      sys_ctrl.delay1 = 300;
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
      return;
    }
  }
  /* run until top side */
  else if (2 == sys_ctrl.sub_sts) {
    /* calculate force sampling position */
    if (hallCnt > glb_ctrl.hall_bot)
      index = 0;
    else if (hallCnt < glb_ctrl.hall_top)
      index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
    else
      index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;

    /* normal route overforce judge */
    if (hallCnt > sys_ctrl.hall_dest + HAL_CNT_SLOW_STOP) {
      sub_sped_raises(&sys_ctrl.cur_pwm
                      , PWM_VAL_MAX_RUN
                      , 2);
                      
      if (sys_ctrl.delay0 > 0) {
        sys_ctrl.delay0--;
        
        /* Resistance rebound in starting duration */
        calcForce = glb_ctrl.ufor_max;
        force_res = ((calcForce + bk_force[glb_ctrl.force_lv][3]) > AD_MAX)
                        ? AD_MAX
                        : (glb_ctrl.ufor_max + bk_force[glb_ctrl.force_lv][3]);
        
        if (adcValue > force_res) {
#ifdef EXCUTE_DEBUG
      printf("open stop 1\r\n");
#endif
          motors_pwms(0);
          sys_ctrl.sub_sts = 5;
          sys_ctrl.delay0 = 200;
          return;
        }
      } else {
        /* Resistance rebound in normal duration */
        calcForce = sub_maxi_values(glb_ctrl.upForce[index - 1]
                                    , glb_ctrl.upForce[index]
                                    , glb_ctrl.upForce[index + 1]);
        force_res = ((calcForce + bk_force[glb_ctrl.force_lv][4]) > AD_MAX)
                        ? AD_MAX
                        : (calcForce + bk_force[glb_ctrl.force_lv][4]);
        if (adcValue > force_res) {
#ifdef EXCUTE_DEBUG
      printf("open stop 2\r\n");
#endif
          motors_pwms(0);
          sys_ctrl.sub_sts = 5;
          sys_ctrl.delay0 = 200;
          return;
        }
      }
    }
    /* stop route overforce judge */
    else {
      sub_sped_stable(&sys_ctrl.cur_pwm 
                      , PWM_VAL_MAX_RUN 
                      , PWM_VAL_MIN_ALL
                      , HAL_DUTY_SLOW);
      /* Resistance rebound in slow stop duration */
      if ((sys_ctrl.delay0 > 0) 
         || (sys_ctrl.delay1 > 0)) {
         	
        if (sys_ctrl.delay0 > 0)
          sys_ctrl.delay0--;
        
        if (sys_ctrl.delay1 > 0)
          sys_ctrl.delay1--;

        calcForce = glb_ctrl.ufor_max;
        force_res = (calcForce + bk_force[glb_ctrl.force_lv][3]) > AD_MAX
                        ? AD_MAX
                        : (glb_ctrl.ufor_max + bk_force[glb_ctrl.force_lv][3]);

        if (adcValue > force_res) {
#ifdef EXCUTE_DEBUG
      printf("open stop 3\r\n");
#endif
          motors_pwms(0);
          sys_ctrl.sub_sts = 5;
          sys_ctrl.delay0 = 200;
          return;
        }
      } else {
        calcForce = sub_maxi_values(glb_ctrl.upForce[index - 1]
                                    , glb_ctrl.upForce[index]
                                    , glb_ctrl.upForce[index + 1]);
        force_res = (calcForce + bk_force[glb_ctrl.force_lv][5]) > AD_MAX
                        ? AD_MAX
                        : (calcForce + bk_force[glb_ctrl.force_lv][5]);
        if (adcValue > force_res) {
#ifdef EXCUTE_DEBUG
      printf("open stop 4\r\n");
#endif
          motors_pwms(0);
          sys_ctrl.sub_sts = 5;
          sys_ctrl.delay0 = 200;
          return;
        }
      }
    }
    motors_pwms(sys_ctrl.cur_pwm);

    if (hallCnt <= sys_ctrl.hall_dest) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 200;
      return;
    }
  }
  
/*------------ normal stop : delay sometime to stop motor -----------*/
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP);
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      sys_ctrl.delay0 = 60;
      sys_ctrl.sub_sts = 4;
    }
  }
  
  /* motor have a break */
  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
      sys_ctrl.lst_dir = MOT_CTS_UPUP;
#ifdef EXCUTE_DEBUG
      printf("hall cur: %d\r\n", (U16ET)hallCnt);
      printf("hall des: %d\r\n", (U16ET)sys_ctrl.hall_dest);
      printf("hall top: %d\r\n", (U16ET)glb_ctrl.hall_top);
      printf("hall bot: %d\r\n", (U16ET)glb_ctrl.hall_bot);
#endif
    }
  }
  
/*------------ abnormal stop : delay sometime to stop motor ----------*/
  else if (5 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP);
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      sys_ctrl.delay0 = 60;
      sys_ctrl.sub_sts = 6;
    }
  }
  
  /* motor have a break */
  else if (6 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.err_num = ERR_EXOPEN_OVERFORCE;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.lst_dir = MOT_CTS_UPUP;
      sys_ctrl.sub_sts = 0;
#ifdef EXCUTE_DEBUG
      printf("OverForce\r\n");
      printf("hall cur: %d\r\n", (U16ET)hallCnt);
#endif
    }
  }
  
  return;
}

/**``
  * @brief  System excute close
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
void sys_excute_close(void) {
  WIRELESS_COMMAND command;
  U08ET            adcValue, infra_status;
  U16ET            hallCnt, hallSmp, index, force_res, calcForce;

  infra_status = getkey_stabe(KEY_P_INFRA);
  command      = get_wireless_command();
  hallCnt      = halsig_gcnt();
  adcValue     = getadc_data(ADC_CH_MOT);
  hallSmp      = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1) 
                 / UP_FORCE_SAMP;

  /* lock set from keypad  */
  if (COMMAND_KEYPAD_LOCK == command)
    glb_ctrl.vacation = ET_TRUE ? ET_FALSE : ET_TRUE;

  /* HOLD command, ignor infra signal */
  if (COMMAND_KEYPAD_HOLD_EN == sys_ctrl.door_command_type)
    infra_status = ET_FALSE;

  if (((COMMAND_OUTDOOR_DOOR == command) && (ET_TRUE != glb_ctrl.vacation))
     || (COMMAND_KEYPAD_DOOR == command)
     || (COMMAND_KEYPAD_HOLD_DIS == command)) {
#ifdef EXCUTE_DEBUG
    printf("st_close get stop signal\r\n\r\n");
#endif
    motors_pwms(0);
    sys_ctrl.sub_sts = 4;
    sys_ctrl.delay0 = 200;
    return;
  }
  /* motor direction up */
  if (0 == sys_ctrl.sub_sts) {
#ifdef EXCUTE_DEBUG
    printf("excute close\r\n");
    printf("command:%d\r\n", (U16ET)sys_ctrl.door_command_type);
#endif
    if ((COMMAND_WIFI_DOOR_PRESET != sys_ctrl.door_command_type)
       && (COMMAND_WIFI_DOOR_TOGGLE != sys_ctrl.door_command_type)
       && (COMMAND_WIFI_DOOR_CLOSE != sys_ctrl.door_command_type)) {
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 10;
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 0);
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      halsig_sdir(HAL_DIR_DOWN);
      motors_ctrl(MOT_CTS_DOWN);
      if (ET_TRUE == sys_ctrl.WiFi_connected) {
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 1, 2500);
        Lamp_ctrl(LAMP_BLINK_TIMES 
                  , LAMP_BLINK_PERIOD 
                  , LAMP_BLINK_RATIO
                  , glb_ctrl.lamp_secs * 30000);
      } else {
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_OFF, 0, 0);
        Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
      }
    } else {
      if (WIFI_LOCKED == sys_ctrl.WiFi_PreLock) {
        sys_ctrl.sys_sts = SYS_ST_STANDBY;
        sys_ctrl.sub_sts = 0;
        return;
      }
      
      sys_ctrl.sub_sts = 1;
      sys_ctrl.delay0 = 2500;
      LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 1, 2500);
      Lamp_ctrl(LAMP_BLINK_TIMES
                , LAMP_BLINK_PERIOD
                , LAMP_BLINK_RATIO
                , glb_ctrl.lamp_secs * 30000);
    }
    return;
  }
  /* command from WiFi, delay 5sec to move door */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 10;
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 0);
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      halsig_sdir(HAL_DIR_DOWN);
      motors_ctrl(MOT_CTS_DOWN);
      return;
    }
  }
  /* start motor power */
  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay2--;
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.delay0 = 1500;
      sys_ctrl.delay1 = 300;
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 3;
      return;
    }
  }
  /* run until top side */
  else if (3 == sys_ctrl.sub_sts) {
    /* calculate force sampling position */
    if (hallCnt < glb_ctrl.hall_top)
      index = 0;
    else if (hallCnt > glb_ctrl.hall_bot)
      index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
    else
      index = (hallCnt - glb_ctrl.hall_top) / hallSmp;

    /* blink when touch ref hal */
    if ((hallCnt == (((glb_ctrl.hall_bot + glb_ctrl.hall_top) / 2) -
                     ((glb_ctrl.hall_bot + glb_ctrl.hall_top) / 2) %
                         glb_ctrl.hall_ratio))
       || (hallCnt == (glb_ctrl.hall_bot -
                      (glb_ctrl.hall_bot % glb_ctrl.hall_ratio)
                       - glb_ctrl.hall_ratio)))
      LedBuzTaskCtrl(LED_ID_SET, OUT_ST_ON, 1, 200);

    /* normal route overforce judge */
    if (hallCnt < sys_ctrl.hall_dest - HAL_CNT_SLOW_STOP) {
      sub_sped_raises(&sys_ctrl.cur_pwm
                      , PWM_VAL_MAX_RUN
                      , 2);
      if (sys_ctrl.delay0 > 0) {
        sys_ctrl.delay0--;
        /* Resistance rebound in starting duration */
        calcForce = glb_ctrl.dfor_max;
        force_res = (calcForce + bk_force[glb_ctrl.force_lv][0]) > AD_MAX
                        ? AD_MAX
                        : (glb_ctrl.dfor_max + bk_force[glb_ctrl.force_lv][0]);
        if (adcValue > force_res) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
          sys_ctrl.sub_sts = 0;
          return;
        }
      } else {
        /* Resistance rebound in normal duration */
        calcForce = sub_maxi_values(glb_ctrl.dnForce[index - 1]
                                    , glb_ctrl.dnForce[index]
                                    , glb_ctrl.dnForce[index + 1]);
        force_res = (calcForce + bk_force[glb_ctrl.force_lv][1]) > AD_MAX
                        ? AD_MAX
                        : (calcForce + bk_force[glb_ctrl.force_lv][1]);
        if (adcValue > force_res) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
          sys_ctrl.sub_sts = 0;
          return;
        }
      }
    }
    /* stop route overforce judge */
    else {
      sub_sped_stable(&sys_ctrl.cur_pwm
                      , PWM_VAL_MAX_RUN
                      , PWM_VAL_MIN_ALL
                      , HAL_DUTY_SLOW);
      /* Resistance rebound in sOUT_ST_OFF stop duration */
      if ((sys_ctrl.delay0 > 0)
         || (sys_ctrl.delay1 > 0)) {
         	
        if (sys_ctrl.delay0 > 0)
          sys_ctrl.delay0--;
        
        if (sys_ctrl.delay1 > 0)
          sys_ctrl.delay1--;
        
        calcForce = glb_ctrl.dfor_max;
        force_res = (calcForce + bk_force[glb_ctrl.force_lv][0]) > AD_MAX
                        ? AD_MAX
                        : (glb_ctrl.dfor_max + bk_force[glb_ctrl.force_lv][0]);
        
        if (adcValue > force_res) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
          sys_ctrl.sub_sts = 0;
          return;
        }
      } else {
        calcForce = sub_maxi_values(glb_ctrl.dnForce[index - 1]
                                    , glb_ctrl.dnForce[index]
                                    , glb_ctrl.dnForce[index + 1]);
        force_res = (calcForce + bk_force[glb_ctrl.force_lv][2]) > AD_MAX
                        ? AD_MAX
                        : (calcForce + bk_force[glb_ctrl.force_lv][2]);
        
        if (adcValue > force_res) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
          sys_ctrl.sub_sts = 0;
          return;
        }
      }
    }
    motors_pwms(sys_ctrl.cur_pwm);

    if (ET_TRUE == infra_status) {
      sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
      sys_ctrl.sub_sts = 0;
      return;
    }

    if (hallCnt >= sys_ctrl.hall_dest) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 4;
      sys_ctrl.delay0 = 200;
      return;
    }
  }
  /* delay sometime to stop motor */
  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP);
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
      Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
      sys_ctrl.delay0 = 60;
      sys_ctrl.sub_sts = 5;
    }
  }
  /* motor have a break */
  else if (5 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
      sys_ctrl.lst_dir = MOT_CTS_DOWN;
      sys_ctrl.WiFi_PreLock = WIFI_UNLOCK;
#ifdef EXCUTE_DEBUG
      printf("hall cur: %d\r\n", (U16ET)hallCnt);
      printf("hall des: %d\r\n", (U16ET)sys_ctrl.hall_dest);
      printf("hall top: %d\r\n", (U16ET)glb_ctrl.hall_top);
      printf("hall bot: %d\r\n", (U16ET)glb_ctrl.hall_bot);
#endif
    }
  }
  return;
}

/**
  * @brief  System excute back
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
void sys_excute_back(void) {
  /* stop motor power */
  if (0 == sys_ctrl.sub_sts)
  {
#ifdef EXCUTE_DEBUG
    printf("func <back >\r\n");
#endif
    Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
    LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
    motors_pwms(0);
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 250;
    if ((COMMAND_WIFI_DOOR_CLOSE == sys_ctrl.door_command_type)
       || (COMMAND_WIFI_DOOR_PRESET == sys_ctrl.door_command_type) 
       || (COMMAND_WIFI_DOOR_TOGGLE == sys_ctrl.door_command_type))
      sys_ctrl.WiFi_PreLock = (sys_ctrl.WiFi_PreLock < WIFI_LOCKED)
                                  ? (sys_ctrl.WiFi_PreLock + 1)
                                  : sys_ctrl.WiFi_PreLock;
  }
  
  /* stop motor action  */
  else if (1 == sys_ctrl.sub_sts)
  {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 60;
    }
  }
  
  /* motor direction up */
  else if (2 == sys_ctrl.sub_sts)
  {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0)
    {
      motors_ctrl(MOT_CTS_UPUP);
      halsig_sdir(HAL_DIR_UPUP);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 10;
    }
  }
  
  /* wait sometime to start power motor */
  else if (3 == sys_ctrl.sub_sts)
  {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0)
    {
    	#ifdef EXCUTE_DEBUG
        printf("back to open\r\n\r\n");
      #endif
      sub_clrs_status();
      sys_ctrl.delay0 = 700;
      sys_ctrl.delay1 = 300;
      sys_ctrl.hall_dest = glb_ctrl.hall_top;
      sys_ctrl.cur_pwm = PWM_VAL_DFT_REV;
      sys_ctrl.sys_sts = SYS_ST_EXCUTE_OPEN;
      sys_ctrl.sub_sts = 2;
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 250);
    }
  }
}

/**
  * @brief  System set remote code
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET sys_remote_set(VODET) {
  U08ET rem_key[RBUF_SIZE];
#ifdef REMOTE_DEBUG
  U08ET index;
#endif

  if (0 == sys_ctrl.sub_sts) {
#ifdef REMOTE_DEBUG
    printf("func <remote set>\r\n");
#endif
    sys_ctrl.delay0 = 1000;
    sys_ctrl.sub_sts = 1;
    return;
  }

  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.delay0 = 2500;
      sys_ctrl.sub_sts = 2;
      LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
      LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 3, 200);
      Lamp_ctrl(0, 0, 0, 0);
      return;
    }

    if (ET_FALSE == getkey_stabe(KEY_A_CODE)) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }

  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      remote_clr(CLEAR_ALL);
      LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 10, 200);
      sys_ctrl.delay0 = 3800;
      sys_ctrl.sub_sts = 4;
    }

    if (ET_FALSE == getkey_stabe(KEY_A_CODE)) {
      get_TTI_spi(rem_key);
      get_ET_keypad(rem_key);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 20000;
    }
  }

  else if (3 == sys_ctrl.sub_sts) {
    if (ET_TRUE == get_TTI_spi(rem_key)) {
#ifdef REMOTE_DEBUG
      printf("rem_key:");
      for (index = 0; index < KEY_LENGTH; index++)
        printf("0x%x  ", rem_key[index]);
      printf("\r\n");
#endif
      if (ET_TRUE == remote_save(rem_key
      	, (LOCAL_KEY_TYPE)KEY_TTI_SPI)) {
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 3, 200);
        sys_ctrl.sub_sts = 4;
        sys_ctrl.delay0 = 1000;
      }
      return;
    }

    if (ET_TRUE == get_ET_keypad(rem_key)) {
      if (ET_TRUE == remote_save(rem_key
      	, (LOCAL_KEY_TYPE)KEY_ET_KEYPAD)) {
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 3, 200);
        sys_ctrl.sub_sts = 4;
        sys_ctrl.delay0 = 1000;
        return;
      }
    }

    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.err_num = ERR_PROGRAM_OUTTIME;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }

  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
    }
  }
}

/**
  * @brief  System power off process
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET sys_power_off(VODET) {

  /* disable peripheral unit to reduce power consumption */
  if (0 == sys_ctrl.sub_sts) {
#ifdef POWER_OFF_DEBUG
    printf("FUNC <power off>\r\n\r\n");
    printf("cur I :%d\r\n\r\n", getadc_data(ADC_CH_MOT));
#endif
    motors_pwms(0);
    LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
    LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
    LedBuzTaskCtrl(LED_ID_SET, OUT_ST_OFF, 0, 0);
    LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_OFF, 0, 0);
    Lamp_ctrl(0, 0, 0, 0);
    LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_OFF, 0, 0);
    sys_ctrl.delay0 = 4;
    sys_ctrl.sub_sts = 1;
  }

  /* safe to stop motor */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP);
      sys_ctrl.delay0 = 15;
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
        remote_Write_FlashMemory();
#ifdef POWER_OFF_DEBUG
        printf("save suc\r\n\r\n");
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
#ifdef POWER_OFF_DEBUG
      printf("save success!!!!!!!\r\n\r\n");
#endif
      while (1)
        ;
    }
  }
}

/**
  * @brief  System error process
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET sys_err_process(VODET) {
  if (0 == sys_ctrl.sub_sts) {
#ifdef POWER_OFF_DEBUG
    printf("FUNC <sys err >\r\n\r\n");
#endif
    motors_pwms(0);
    LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
    LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
    LedBuzTaskCtrl(LED_ID_SET, OUT_ST_OFF, 0, 0);
    LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_OFF, 0, 0);
    Lamp_ctrl(0, 0, 0, 0);
    sys_ctrl.delay0 = 5;
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
      if (ERR_HAL_MAIN == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 0, 200);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 0, 200);
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        sys_ctrl.sub_sts = 3;
      } else if (ERR_HAL_RFER == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 200);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 0, 200);
        sys_ctrl.sub_sts = 3;
      } else if (ERR_INFRA_DURING_TRAINING == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 11, 200);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 11, 200);
        LedBuzTaskCtrl(LED_ID_SET, OUT_ST_ON, 11, 200);
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 11, 200);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 2, 200);
        sys_ctrl.delay0 = 5000;
        sys_ctrl.sub_sts = 5;
      } else if (ERR_EXCUTE_WITHOUT_ROUTE == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 5, 200);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 5, 200);
        LedBuzTaskCtrl(LED_ID_SET, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 5, 200);
        sys_ctrl.delay0 = 2200;
        sys_ctrl.sub_sts = 5;
      } else if (ERR_ROUTE_OUTTIME == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(LED_ID_SET, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 7, 200);
        sys_ctrl.delay0 = 3000;
        sys_ctrl.sub_sts = 5;
      } else if (ERR_ROUTE_INVALID == sys_ctrl.err_num ||
                 ERR_HAL_LEARN_FAIL == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 200);
        LedBuzTaskCtrl(LED_ID_SET, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 0, 200);
        sys_ctrl.sub_sts = 3;
      } else if (ERR_EXOPEN_OVERFORCE == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 0, 0);
        LedBuzTaskCtrl(LED_ID_SET, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 5, 250);
        Lamp_ctrl(5, 500, 50, glb_ctrl.lamp_secs * 30000);
        sys_ctrl.sub_sts = 5;
        sys_ctrl.delay0 = 2500;
      } else if (ERR_PROGRAM_OUTTIME == sys_ctrl.err_num) {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(LED_ID_SET, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 7, 200);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 7, 200);
        Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
        sys_ctrl.sub_sts = 5;
        sys_ctrl.delay0 = 3000;
      } 
      else {
        LedBuzTaskCtrl(LED_ID_DN, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(LED_ID_UP, OUT_ST_OFF, 0, 0);
        LedBuzTaskCtrl(LED_ID_SET, OUT_ST_ON, 0, 200);
        LedBuzTaskCtrl(LED_ID_CODE, OUT_ST_ON, 0, 200);
        LedBuzTaskCtrl(BUZ_ID_1, OUT_ST_ON, 0, 0);
        sys_ctrl.sub_sts = 3;
      }
    }
  }
  /* infinity loop */
  else if (3 == sys_ctrl.sub_sts) {
  } else if (4 == sys_ctrl.sub_sts) {
    if (ET_FALSE == getkey_stabe(KEY_P_INFRA)) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
    }
  } else if (5 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
      if (ERR_ROUTE_OUTTIME == sys_ctrl.err_num)
        Lamp_ctrl(0, 0, 0, glb_ctrl.lamp_secs * 30000);
    }
  }
}

/**
  * @brief  System task error detect
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET abnor_detectTask(VODET) {
  U08ET hall_err;
  hall_err = halsig_gerr();
  /* power off */
  if ((SYS_ST_POWER_OFF != sys_ctrl.sys_sts)
     && (ET_TRUE == getkey_stabe(KEY_D_POWDN))) {
     	
    if ((sys_ctrl.sys_sts >= SYS_ST_LIMITTOP_SET)
    	 && (sys_ctrl.sys_sts <= SYS_ST_TRAIN_CLOSE)) {
      glb_ctrl.hall_top = 0;
      glb_ctrl.hall_bot = 0;
    }
    
    sys_ctrl.sys_sts = SYS_ST_POWER_OFF;
    sys_ctrl.sub_sts = 0;
    return;
  }
  /* state : power off or error process, ignore all */
  if ((SYS_ST_POWER_OFF == sys_ctrl.sys_sts)
     || (SYS_ST_ERR_PROCESS == sys_ctrl.sys_sts))
    return;

  /* hall err */
  if (ERR_HAL_MAIN == hall_err) {
    if ((sys_ctrl.sys_sts >= SYS_ST_LIMITTOP_SET)
       && (sys_ctrl.sys_sts <= SYS_ST_EXCUTE_CLOSE)){
      sys_ctrl.err_num = ERR_HAL_MAIN;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.sub_sts = 0;
      return;
    }
  } else if (ERR_HAL_RFER == hall_err) {
    if ((sys_ctrl.sys_sts >= SYS_ST_LIMITTOP_SET)
       && (sys_ctrl.sys_sts <= SYS_ST_EXCUTE_CLOSE)) {
      sys_ctrl.err_num = ERR_HAL_RFER;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }

  /* infra err when set rotue and force training */
  if (ET_TRUE == getkey_stabe(KEY_P_INFRA)) {
    if (((sys_ctrl.sys_sts > SYS_ST_LIMITTOP_SET) && (sys_ctrl.sys_sts <= SYS_ST_TRAIN_CLOSE))
       || ((SYS_ST_LIMITTOP_SET == sys_ctrl.sys_sts) && (sys_ctrl.sub_sts >= 2))) {
      sys_ctrl.err_num = ERR_INFRA_DURING_TRAINING;
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.sub_sts = 0;
    }
  }
}

/**
  * @brief  WiFi module process, call by main cycle;
  * @param
  *   none;
  * @retval
  *   none;
  *
  */
VODET WiFi_process(VODET) {
  REC_UNION  rec_data;
  SEND_UNION send_data;
  U16ET      hallCnt;
  U08ET      infra_status, lamp_status;

  infra_status = getkey_stabe(KEY_P_INFRA);
  lamp_status  = get_out_state(LAMP_ID);
  hallCnt      = halsig_gcnt();
  /* ===============================================================================
     ------------------judge if GD50 connected with wifi adapter--------------------
     ===============================================================================*/
  sys_ctrl.WiFi_connected_cnt = (sys_ctrl.WiFi_connected_cnt < WIFI_OVERTIME)
                                    ? (sys_ctrl.WiFi_connected_cnt + 1)
                                    : sys_ctrl.WiFi_connected_cnt;
  sys_ctrl.WiFi_connected = (sys_ctrl.WiFi_connected_cnt < WIFI_OVERTIME)
                                ? ET_TRUE
                                : ET_FALSE;
  /* REC success */
  if (ET_FALSE == wifi_get_data(&rec_data))
    return;

  sys_ctrl.WiFi_connected_cnt = 0;
  /* ===============================================================================
     -----------------------------wifi adapter init---------------------------------
     ===============================================================================*/
  if (0xFE == rec_data.commandlist.module_id) {
    send_data.buf[0] = 0x0D;
    send_data.buf[1] = 0x05;
    send_data.buf[2] = 0x0C;
    send_data.buf[3] = 0x00;
    wifi_send_string(send_data.buf, 4);
  }
  /* ===============================================================================
     ------------------------------------info check---------------------------------
     ===============================================================================*/
  else if (0xFF == rec_data.commandlist.module_id) {
    send_data.infolist.module_id = 0x0D;
    send_data.infolist.snesor_flag = infra_status;
    send_data.infolist.lamp_state = lamp_status;
    send_data.infolist.lamp_time = glb_ctrl.lamp_secs;
    /* convert hall_cnt to inch */
    send_data.infolist.door_position =
        (U08ET)(((U32ET)(glb_ctrl.hall_bot - hallCnt)) * 180 / 254);
    
    /* door status check */
    if ((SYS_ST_TRAIN_OPEN == sys_ctrl.sys_sts)
       || (SYS_ST_EXCUTE_OPEN == sys_ctrl.sys_sts)
       || (SYS_ST_EXCUTE_BACK == sys_ctrl.sys_sts))
      send_data.infolist.door_state = DOOR_OPENING;
    else if ((SYS_ST_TRAIN_CLOSE == sys_ctrl.sys_sts)
            || (SYS_ST_EXCUTE_CLOSE == sys_ctrl.sys_sts))
      send_data.infolist.door_state = DOOR_CLOSING;
    else {
      if (hallCnt >= glb_ctrl.hall_bot)
        send_data.infolist.door_state = DOOR_CLOSED;
      else if (hallCnt <= glb_ctrl.hall_top)
        send_data.infolist.door_state = DOOR_OPEN;
      else
        send_data.infolist.door_state = DOOR_FAULT;
    }
    
    send_data.infolist.operation_mode = sys_ctrl.WiFi_PreLock;
    send_data.infolist.preset_position = glb_ctrl.WiFi_preset_position;
    send_data.infolist.door_max_position =
        (glb_ctrl.hall_bot - glb_ctrl.hall_top) * 180 / 254;
    send_data.infolist.vacation = glb_ctrl.vacation;

    if (SYS_ST_REMOTE_ST == sys_ctrl.sys_sts)
      send_data.infolist.program = REMOTE_PROGRAM;
    else
      send_data.infolist.program = NOT_REMOTE_PROGRAM;

    wifi_send_string(send_data.buf, 11);
  }
  /* ===============================================================================
     ------------------------get command from wifi adapter--------------------------
     ===============================================================================*/
  else {
    /*----------------------------equip id judge---------------------------------*/
    send_data.buf[0] = 0xff;
    wifi_send_string(send_data.buf, 1);
    /*-----------------------judge if module id is right-------------------------*/
    if (0x0d != rec_data.commandlist.module_id)
      return;
    /*--------------------------vacation command process-------------------------*/
    if ((rec_data.commandlist.vacation <= 0x01)
       && (sys_ctrl.sys_sts >= SYS_ST_STANDBY))
      glb_ctrl.vacation = rec_data.commandlist.vacation;
    /*---------------------------lamp timer change ------------------------------*/
    if ((rec_data.commandlist.lamp_time <= 0x0a)
       && (rec_data.commandlist.lamp_time > 0x0)
       && (sys_ctrl.sys_sts >= SYS_ST_STANDBY))
      glb_ctrl.lamp_secs = rec_data.commandlist.lamp_time;
    /*----------write command to sys_ctrl, then processed by main_func-----------*/
    sys_ctrl.WiFi_LampCommand = (rec_data.commandlist.lamp_state <= 0x01)
                                    ? rec_data.commandlist.lamp_state
                                    : 0xff;
    sys_ctrl.WiFi_DoorCommand = (rec_data.commandlist.door_command <= 0x03)
                                    ? rec_data.commandlist.door_command
                                    : 0xff;
    sys_ctrl.WiFi_ProgramCommand = (rec_data.commandlist.program <= 0x01)
                                       ? (rec_data.commandlist.program <= 0x01)
                                       : 0xff;
  }
}

/**
  * @brief  get wireless command;
  * @param
  *   none;
  * @retval
  *   WIRELESS_COMMAND: current command;
  *
  */
WIRELESS_COMMAND get_wireless_command(VODET) {
  WIRELESS_COMMAND result = COMMAND_NONE;
  LOCAL_COMMAND_LIST local_command;

  /* ===============================================================================
     ------------------------------local command -----------------------------------
     ===============================================================================*/
  local_command = remote_hits();
  if (LOCAL_COMMAND_NONE != local_command) {
    switch (local_command) {
    case TTI_REMOTE:
      result = COMMAND_OUTDOOR_DOOR;
      break;
    case TTI_KEYPAD:
      result = COMMAND_OUTDOOR_DOOR;
      break;
    case TTI_HOMELIK:
      result = COMMAND_OUTDOOR_DOOR;
      break;
    case ET_KEYPAD_DOOR:
      result = COMMAND_KEYPAD_DOOR;
      break;
    case ET_KEYPAD_LOCK:
      result = COMMAND_KEYPAD_LOCK;
      break;
    case ET_KEYPAD_LAMP:
      result = COMMAND_KEYPAD_LAMP;
      break;
    case ET_KEYPAD_DOOR_HOLD_ENABLE:
      result = COMMAND_KEYPAD_HOLD_EN;
      break;
    case ET_KEYPAD_DOOR_HOLD_DISABLE:
      result = COMMAND_KEYPAD_HOLD_DIS;
      break;
    default:
      break;
    }
    goto RESULT;
  }
  /* ===============================================================================
     -------------------------------wifi command
     -----------------------------------
     ===============================================================================*/
  /*-------------------------------door command-------------------------------*/
  if (sys_ctrl.WiFi_DoorCommand <= 0x3) {
    switch (sys_ctrl.WiFi_DoorCommand) {
    case 0x00:
      result = COMMAND_WIFI_DOOR_CLOSE;
      break;
    case 0x01:
      result = COMMAND_WIFI_DOOR_OPEN;
      break;
    case 0x02:
      result = COMMAND_WIFI_DOOR_PRESET;
      break;
    case 0x03:
      result = COMMAND_WIFI_DOOR_TOGGLE;
      break;
    default:
      break;
    }
    goto RESULT;
  }
  /*-------------------------------lamp command-------------------------------*/
  if (sys_ctrl.WiFi_LampCommand <= 0x01) {
    if (0x0 == sys_ctrl.WiFi_LampCommand)
      result = COMMAND_WIFI_LAMP_OFF;
    else if (0x1 == sys_ctrl.WiFi_LampCommand)
      result = COMMAND_WIFI_LAMP_ON;
    goto RESULT;
  }
  /*-------------------------------pogram command-----------------------------*/
  if (sys_ctrl.WiFi_ProgramCommand <= 0x1) {
    switch (sys_ctrl.WiFi_DoorCommand) {
    case 0x00:
      result = COMMAND_WIFI_PROGRAM_OFF;
      break;
    case 0x01:
      result = COMMAND_WIFI_PROGRAM_ON;
      break;
    default:
      break;
    }
    goto RESULT;
  }

RESULT:
  sys_ctrl.WiFi_LampCommand    = 0xff;
  sys_ctrl.WiFi_DoorCommand    = 0xff;
  sys_ctrl.WiFi_ProgramCommand = 0xff;
  return result;
}
