
#include "main.h"
#include "regdef.h"

static SYS_CTRL sys_ctrl;

const FUNC func[SYS_ST_INVALID] = {
    sys_power_on,  sys_read_eep,  sys_limit_st,  sys_train_dn,
    sys_train_up,  sys_stay_stop, sys_excute_up, sys_excute_dn,
    sys_excute_bk, sys_remote_st, sys_system_st, sys_force_st,
    sys_lr_st,     sys_atclo_st,  sys_infra_st,  sys_error,
};

const U08ET bk_force[FORCE_LVL_MAX][6] = {
    /*{  40,  30,  24,  40,  30,  24 },
    {  49,  40,  32,  49,  40,  32 },
    {  58,  48,  38,  58,  48,  38 },
    {  67,  57,  45,  67,  57,  45 },
    {  76,  66,  53,  76,  66,  53 },
    {  85,  75,  60,  85,  75,  60 },
    {  94,  84,  67,  94,  84,  67 },
    {  103, 93,  74,  103, 93,  74 },
    {  110, 100, 80,  110, 100, 80 }*/

    /*   {  50,  40,  32,  50,  40,  32 },
       {  57,  47,  38,  57,  47,  38 },
       {  64,  54,  43,  64,  54,  43 },
       {  70,  60,  48,  70,  60,  48 },
       {  77,  67,  54,  77,  67,  54 },
       {  83,  73,  58,  83,  73,  58 },
       {  90,  80,  64,  90,  80,  64 },
       {  97,  87,  70,  97,  87,  70 },
       {  110, 100,  80,  110, 100,  80 } 12-20*/

    {50, 40, 20, 50, 40, 32},    {57, 47, 26, 57, 47, 38},
    {64, 54, 31, 64, 54, 43},    {70, 60, 36, 70, 60, 48},
    {77, 67, 42, 77, 67, 54},    {83, 73, 46, 83, 73, 58},
    {90, 80, 52, 90, 80, 64},    {97, 87, 58, 97, 87, 70},
    {110, 100, 68, 110, 100, 80}

};

/*-----------------------------------------------------------------*/
/* FUNC : MAIN function of the project                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void main(void) {
  sys_pre_init();
  while (1) {
    if (ET_FALSE == tskskm_rdy()) {
      continue;
    }

    func[sys_ctrl.sys_sts]();

    if (SYS_ST_ERROR != sys_ctrl.sys_sts)
      AbnormalDetective();

    /*if (0 == sys_ctrl.sub_sts)
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);*/

    wdtdog_feed();
#ifndef UART_DEBUG
    SegCtrl_task();
#endif
    BuzLamp_task();
    wdtdog_feed();
    adcsmp_task();
    keysmp_task();
  }
}

/*-----------------------------------------------------------------*/
/* FUNC : System initialize after power on                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void sys_pre_init(void) {
  DI();
  clkrst_init();
  motors_init();
  adcsmp_init();
  keysmp_init();
  timwdt_init();
  BuzLamp_init();
  remote_init();
#ifdef UART_DEBUG
  uartct_init();
#else
  SegCtrl_init();
#endif
  sys_ctrl.sys_sts = SYS_ST_POWER_ON;
  sys_ctrl.sub_sts = 0;
  sys_ctrl.sys_lts = SYS_ST_INVALID;
  sys_ctrl.sub_lts = 0;
  sys_ctrl.cur_dir = MOT_CT_STOP;
  sys_ctrl.bit_flg.bit0 = ET_FALSE;
  sys_ctrl.bit_flg.bit1 = ET_FALSE;
  sys_ctrl.bit_flg.bit2 = ET_FALSE;
  sys_ctrl.bit_flg.bit3 = ET_FALSE;
  sys_ctrl.bit_flg.bit4 = ET_FALSE;
  sys_ctrl.bit_flg.bit5 = ET_FALSE;
  sys_ctrl.bit_flg.bit6 = ET_FALSE;
  sys_ctrl.bit_flg.bit7 = ET_FALSE;
#ifdef UART_DEBUG
  PutString("Sys init OK\r\n");
#endif
  EI();
}

/**
 * @brief  Power on sequence.
 * @param  None
 * @retval None
 *
 */
void sys_power_on(void) {

  /* delay for Seg inital */
  if (0 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0 = 10;
    sys_ctrl.sub_sts = 1;
    Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
  }
  /* display system version */
  if (1 == sys_ctrl.sub_sts) {
    --sys_ctrl.delay0;
    if (0 == sys_ctrl.delay0) {
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
      sys_ctrl.delay0 = 300;
      sys_ctrl.sub_sts = 2;
#ifdef UART_DEBUG
      PutString("pown:ver-");
      putint(PRG_MAIN_VER);
      PutString("\r\n");
#else
      segtsk_ctrl(ET_TRUE, PRG_MAIN_VER, 0, 0);
#endif
    }
  }
  /* display 9~0 count*/
  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.delay0 = 75;
      sys_ctrl.delay1 = 9;
      sys_ctrl.sub_sts = 3;
#ifdef UART_DEBUG
      PutString("pown : ");
      putint(9);
      PutString("\r\n");
#else
      segtsk_ctrl(ET_TRUE, 9, 0, 0);
#endif
    }
  } else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      if (sys_ctrl.delay1 > 0) {
        sys_ctrl.delay0 = 75;
        --sys_ctrl.delay1;
#ifdef UART_DEBUG
        PutString("pown : ");
        putint(sys_ctrl.delay1);
        PutString("\r\n");
#else
        segtsk_ctrl(ET_TRUE, (U08ET)sys_ctrl.delay1, 0, 0);
#endif
      } else {
        Lamp_ctrl(0);
        sys_ctrl.sys_sts = SYS_ST_READ_EEP;
        sys_ctrl.sub_sts = 0;
      }
    }
  }
}

/**
 * @brief  Read data form Eeprom.
 * @param  None
 * @retval None
 *
 */
void sys_read_eep(void) {

  U16ET index;
  read_data_from_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
  RemoteKey_Read();
  wdtdog_feed();
#ifdef UART_DEBUG
  PutString("lv : ");
  puthex(glb_ctrl.force_lv);
  PutString("\r\n");
#endif

  if (EEP_SAVE_FLG != glb_ctrl.save_flg) {
#ifdef UART_DEBUG
    PutString("save er\r\n ");
#endif
    wdtdog_feed();
    glb_ctrl.save_flg = EEP_SAVE_FLG;
    glb_ctrl.posval_top = POS_ERR;
    glb_ctrl.posval_bot = POS_ERR;
    glb_ctrl.force_lv = 3;
    glb_ctrl.auto_clo = 0;
    glb_ctrl.LR_set = 0;
    glb_ctrl.infra = 0;
    glb_ctrl.inertia_top = 0;
    glb_ctrl.inertia_bot = 0;
    sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
    sys_ctrl.sub_sts = 0;
    write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
#ifdef UART_DEBUG

#endif
    return;
  }

  /* check if route parameter is normal */
  wdtdog_feed();
  if ((glb_ctrl.posval_top >=
       glb_ctrl.posval_bot) /*|| (glb_ctrl.posval_bot > POS_VAL_MAX)*/) {
#ifdef UART_DEBUG
    PutString("--pos error !!\r\n ");
    PutString("postop : ");
    putint(glb_ctrl.posval_top);
    PutString("\r\n");
    PutString("posbot : ");
    putint(glb_ctrl.posval_bot);
    PutString("\r\n");
#endif
    glb_ctrl.posval_top = POS_ERR;
    glb_ctrl.posval_bot = POS_ERR;
  }

  /* set current motor status */
  sys_ctrl.lst_dir = MOT_CT_DOWN;

  /* check force level */
  if (glb_ctrl.force_lv > FORCE_LVL_MAX) {
    glb_ctrl.force_lv = 3;
  }
  /* check auto close minutes */
  if (glb_ctrl.auto_clo > 9) {
    glb_ctrl.auto_clo = 0;
  }

  sys_ctrl.bit_flg.bit0 = ET_TRUE;
  sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
  sys_ctrl.sub_sts = 0;

  return;
}

/**
 * @brief  Set the Top&Botton Position.
 * @param  None
 * @retval None
 *
 */
void sys_limit_st(void) {
  U16ET CurPosition;
  CurPosition = pos_get();
  /* initialize parameter */
  if (0 == sys_ctrl.sub_sts) {
#ifdef UART_DEBUG
    PutString("------limit_st\r\n");
#endif
    glb_ctrl.posval_top = POS_ERR;
    glb_ctrl.posval_bot = POS_ERR;
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay1 = 10000;
    sys_ctrl.sub_lts = 1; // for top bot set count
  }
  /* reponse to remote control */
  else if (1 == sys_ctrl.sub_sts) {
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K2)) {
      motors_ctrl(MOT_CT_UP);
      sys_ctrl.cur_dir = MOT_CT_UP;
      sys_ctrl.start_count = 0;
#ifdef UART_DEBUG
      PutString("move--up\r\n");
#else
      segtsk_ctrl(0, SEG_UPUP, 0, UP_SEG_FLICKER_PERIOD);
#endif
      BUZ_ctrl(BUZ_MOV, 0xffff);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 10;
      sys_ctrl.delay1 = 10000;
    } else if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K4)) {
      motors_ctrl(MOT_CT_DOWN);
      sys_ctrl.cur_dir = MOT_CT_DOWN;
      sys_ctrl.start_count = 0;
#ifdef UART_DEBUG
      PutString("move--down\r\n");
#else
      segtsk_ctrl(0, SEG_DOWN, 0, DOWN_SEG_FLICKER_PERIOD);
#endif
      BUZ_ctrl(BUZ_MOV, 0xffff);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 10;
      sys_ctrl.delay1 = 10000;
    }
    /*********set top and bot position*************/
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) {
      if (1 == sys_ctrl.sub_lts) {
        glb_ctrl.posval_top = CurPosition;
        ++sys_ctrl.sub_lts;
// BUZ_ctrl(300, 1);
#ifdef UART_DEBUG
        PutString("set--top\r\n");
#else
        segtsk_ctrl(1, 2, 0, 0);
#endif
        sys_ctrl.delay1 = 10000;
      } else if (2 == sys_ctrl.sub_lts) {
        glb_ctrl.posval_bot = CurPosition;
        sys_ctrl.sub_sts = 5;
        return;
      }
    }
    --sys_ctrl.delay1;
    if (0 == sys_ctrl.delay1) {
      sys_ctrl.sub_sts = 6;
      sys_ctrl.delay0 = 500;
      BUZ_ctrl(BUZ_SET_OVERTIME_PERIOD, BUZ_SET_OVERTIME_TIMES);
      glb_ctrl.posval_top = POS_ERR;
      glb_ctrl.posval_bot = POS_ERR;
      write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      return;
    }
  } else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
      Motor_PWM_ctrl_init();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 3;
    }
  }
  /* run motor up or down */
  else if (3 == sys_ctrl.sub_sts) {
    Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
    motors_pwms(sys_ctrl.cur_pwm);
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K2) ||
        KEY_ST_LOW == getkey_stabe(KEY_ID_K4)) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 4;
      sys_ctrl.delay0 = 200;
      BUZ_ctrl(0, 0);
#ifdef UART_DEBUG
      PutString("pos:");
      putint(CurPosition);
      PutString("\r\n");
#endif
    }
  } else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CT_STOP);
      sys_ctrl.cur_dir = MOT_CT_STOP;
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
#ifdef UART_DEBUG
      PutString("move--up\r\n");
#else
      segtsk_ctrl(1, sys_ctrl.sub_lts, 0, 0);
#endif
      sys_ctrl.sub_sts = 1;
      sys_ctrl.delay1 = 10000;
    }
  }
  /*************top bot judge****************/
  else if (5 == sys_ctrl.sub_sts) {
    if ((glb_ctrl.posval_top > glb_ctrl.posval_bot) ||
        (glb_ctrl.posval_bot - glb_ctrl.posval_top < 80)) {
      sys_ctrl.sub_sts = 6;
      sys_ctrl.delay0 = 500;
#ifndef UART_DEBUG
      segtsk_ctrl(0, SEG_LINE, 0, 0);
#endif
      glb_ctrl.posval_top = POS_ERR;
      glb_ctrl.posval_bot = POS_ERR;
      write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
    } else {
      if (glb_ctrl.posval_top < 6)
        glb_ctrl.posval_top = 6;
      if (glb_ctrl.posval_bot > 1018)
        glb_ctrl.posval_bot = 1018;
      // BUZ_ctrl(1000, 1);
      sys_ctrl.sub_sts = 7;
      sys_ctrl.delay0 = 300;
#ifdef UART_DEBUG
      PutString("--Position--\r\n");
      PutString("top:");
      putint(glb_ctrl.posval_top);
      PutString("\r\nbot:");
      putint(glb_ctrl.posval_bot);
      PutString("\r\n");
#else
      segtsk_ctrl(0, SEG_UPUP, 0, 0);
#endif
    }
  }
  /*******Position val invalid jump to standby****************/
  else if (6 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }
  /*******Position val valid  jump to train up****************/
  else if (7 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_TRAIN_UP;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }
}
/**
 * @brief  Learn the force of down route.
 * @param  None
 * @retval None
 *
 */
void sys_train_dn(void) {
  U16ET CurPosition, PosSmp, index;
  U08ET adcValue;

  CurPosition = pos_get();
  adcValue = (U08ET)(getadc_data(ADC_CH_MOTOR) >> 2);
  PosSmp = (glb_ctrl.posval_bot - glb_ctrl.posval_top + DN_FORCE_SAMP - 1) /
           DN_FORCE_SAMP;

  /* motor direction down */
  if (0 == sys_ctrl.sub_sts) {
#ifdef UART_DEBUG
    PutString("--trdn\r\n");
#endif
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    glb_ctrl.dforce_max = 0;
    motors_ctrl(MOT_CT_DOWN);
    sys_ctrl.cur_dir = MOT_CT_DOWN;
    sys_ctrl.start_count = 0;
#ifdef UART_DEBUG
    PutString("mv-dn\r\n");
#else
    segtsk_ctrl(0, SEG_DOWN, 0, DOWN_SEG_FLICKER_PERIOD);
#endif
    BUZ_ctrl(BUZ_MOV, 0xffff);
  }
  /* start motor power */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
      Motor_PWM_ctrl_init();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
    }
  }
  /* run until top side */
  else if (2 == sys_ctrl.sub_sts) {
    if (CurPosition < glb_ctrl.posval_bot - POS_SLOW_STOP)
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
    else
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);

    motors_pwms(sys_ctrl.cur_pwm);
    /* hit top side, stop motor */
    if (CurPosition >= glb_ctrl.posval_bot) {
      BUZ_ctrl(0, 0);
      glb_ctrl.dnForce[(glb_ctrl.posval_bot - glb_ctrl.posval_top) / PosSmp] =
          adcValue;
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;
      return;
    }
    /* learn force of the door open duration */
    if ((CurPosition >= glb_ctrl.posval_top) &&
        (CurPosition <= glb_ctrl.posval_bot)) {
      index = ((CurPosition - glb_ctrl.posval_top) / PosSmp);
      glb_ctrl.dnForce[index] = adcValue;
      if (adcValue > glb_ctrl.dforce_max) {
        glb_ctrl.dforce_max = adcValue;
      }
    }
  }
  /* stop motor power */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
      motors_ctrl(MOT_CT_STOP);
      sys_ctrl.cur_dir = MOT_CT_STOP;
      sys_ctrl.delay0 = 250;
      sys_ctrl.sub_sts = 4;
    }
  }
  /* motor have a break */
  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      if (CurPosition > glb_ctrl.posval_bot) {
        glb_ctrl.inertia_bot = (U08ET)(CurPosition - glb_ctrl.posval_bot);
      } else
        glb_ctrl.inertia_bot = 0;
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
      sys_ctrl.lst_dir = MOT_CT_DOWN;
      write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
#ifdef UART_DEBUG
      PutString("MAX:");
      puthex(glb_ctrl.dforce_max);
      PutString("\r\n");
      PutString("FORCE:\r\n");
      for (index = 0; index < DN_FORCE_SAMP; index++) {
        PutString("index(");
        putint(index);
        PutString("):");
        puthex(glb_ctrl.dnForce[index]);
        PutString("\r\n");
        wdtdog_feed();
      }
#endif
    }
  }
}
/**
 * @brief  Learn the force of up route.
 * @param  None
 * @retval None
 *
 */
void sys_train_up(void) {
  U16ET CurPosition, PosSmp, index;
  U08ET adcValue;

  CurPosition = pos_get();
  adcValue = (U08ET)(getadc_data(ADC_CH_MOTOR) >> 2);
  PosSmp = (glb_ctrl.posval_bot - glb_ctrl.posval_top + UP_FORCE_SAMP - 1) /
           UP_FORCE_SAMP;

  /* motor direction down */
  if (0 == sys_ctrl.sub_sts) {
#ifdef UART_DEBUG
    PutString("--tr_up\r\n");
#endif
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    glb_ctrl.uforce_max = 0;
    motors_ctrl(MOT_CT_UP);
    sys_ctrl.cur_dir = MOT_CT_UP;
    sys_ctrl.start_count = 0;
#ifdef UART_DEBUG
    PutString("mv-up\r\n");
#else
    segtsk_ctrl(0, SEG_UPUP, 0, UP_SEG_FLICKER_PERIOD);
#endif
    BUZ_ctrl(BUZ_MOV, 0xffff);
  }
  /* start motor power */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
      Motor_PWM_ctrl_init();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
    }
  }
  /* run until top side */
  else if (2 == sys_ctrl.sub_sts) {
    if (CurPosition > glb_ctrl.posval_top + POS_SLOW_STOP)
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
    else
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);

    motors_pwms(sys_ctrl.cur_pwm);
    /* hit top side, stop motor */
    if (CurPosition <= glb_ctrl.posval_top) {
      glb_ctrl.upForce[(glb_ctrl.posval_bot - glb_ctrl.posval_top) / PosSmp] =
          adcValue;
      BUZ_ctrl(0, 0);
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;
      return;
    }
    /* learn force of the door open duration */
    if ((CurPosition >= glb_ctrl.posval_top) &&
        (CurPosition <= glb_ctrl.posval_bot)) {
      index = (glb_ctrl.posval_bot - CurPosition) / PosSmp;
      glb_ctrl.upForce[index] = adcValue;
      if (adcValue > glb_ctrl.uforce_max) {
        glb_ctrl.uforce_max = adcValue;
      }
    }
  }
  /* stop motor power */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CT_STOP);
      sys_ctrl.cur_dir = MOT_CT_STOP;
      sys_ctrl.delay0 = 250;
      sys_ctrl.sub_sts = 4;
    }
  }
  /* motor have a break */
  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      if (glb_ctrl.posval_top > CurPosition)
        glb_ctrl.inertia_top = (U08ET)(glb_ctrl.posval_top - CurPosition);
      else
        glb_ctrl.inertia_top = 0;
      sys_ctrl.sys_sts = SYS_ST_TRAIN_DN;
      sys_ctrl.sub_sts = 0;
      sys_ctrl.lst_dir = MOT_CT_UP;
#ifdef UART_DEBUG
      PutString("MAX:");
      puthex(glb_ctrl.uforce_max);
      PutString("\r\n");
      PutString("FORCE:\r\n");
      for (index = 0; index < UP_FORCE_SAMP; index++) {
        PutString("index(");
        putint(index);
        PutString("):");
        puthex(glb_ctrl.upForce[index]);
        PutString("\r\n");
        wdtdog_feed();
      }
#endif
    }
  }
}
/**
 * @brief  Standby mode.
 * @param  None
 * @retval None
 *
 */
void sys_stay_stop(void) {
  U16ET CurPosition;
  CurPosition = pos_get();

  if (0 == sys_ctrl.sub_sts) {
#ifdef UART_DEBUG
    PutString("--Stop\r\n");
#else
    if (glb_ctrl.posval_top == POS_ERR || glb_ctrl.posval_bot == POS_ERR)
      segtsk_ctrl(0, SEG_LINE, 0, 0);
    else
      segtsk_ctrl(0, SEG_STOP, 0, 0);
#endif
    remote_hits();
    getkey_stabe(KEY_ID_K5);
    sys_ctrl.clo_cnt = ((U32ET)glb_ctrl.auto_clo) * 30000;
    sys_ctrl.lmp_sec = 180;
    sys_ctrl.lmp_cnt = 500;
    sys_ctrl.sub_sts = 1;
    return;
  }
  /************auto close***********/

  if ((0 != glb_ctrl.auto_clo) &&
      (CurPosition < glb_ctrl.posval_top + glb_ctrl.inertia_top) &&
      (glb_ctrl.posval_top != POS_ERR && glb_ctrl.posval_bot != POS_ERR) &&
      (KEY_ST_HIGH != getkey_stabe(KEY_ID_INFRA))) {
    --sys_ctrl.clo_cnt;
    if (0 == sys_ctrl.clo_cnt) {
      sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }
  /* SET press detected, enter system set mode */
  if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) {
    sys_ctrl.sys_sts = SYS_ST_SYSTEM_ST;
    sys_ctrl.sub_sts = 0;
  }
  /* CODE press detected, enter remote set mode */
  if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K3)) {
    sys_ctrl.sys_sts = SYS_ST_REMOTE_ST;
    sys_ctrl.sub_sts = 0;
  }
  /* Autoclose press detected, enter AutoClose set mode */
  if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K2)) {
    sys_ctrl.sys_sts = SYS_ST_ATCLO_ST;
    sys_ctrl.sub_sts = 0;
  }

  if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K4)) {
    sys_ctrl.sys_sts = SYS_ST_INFRA_ST;
    sys_ctrl.sub_sts = 0;
  }

  if ((ET_TRUE == remote_hits() || KEY_ST_HIGH == getkey_stabe(KEY_ID_K5)) &&
      (glb_ctrl.posval_top != POS_ERR && glb_ctrl.posval_bot != POS_ERR)) {
    if (MOT_CT_DOWN == sys_ctrl.lst_dir) { // UP   control
      if (CurPosition <
          glb_ctrl.posval_top + glb_ctrl.inertia_top) { // pos == top
#ifdef UART_DEBUG
        PutString("fl-1\r\n");
#endif
        sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
        sys_ctrl.sub_sts = 0;
      } else {
#ifdef UART_DEBUG
        PutString("fl-2\r\n");
#endif
        sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
        sys_ctrl.sub_sts = 0;
      }
    } else { // down control
      if (KEY_ST_HIGH == getkey_stabe(KEY_ID_INFRA)) {
#ifdef UART_DEBUG
        PutString("fl-3\r\n");
#endif
        if (CurPosition > glb_ctrl.posval_top + glb_ctrl.inertia_top) {
#ifdef UART_DEBUG
          PutString("fl-4\r\n");
#endif
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
          sys_ctrl.sub_sts = 0;
        }
      } else {
        if (CurPosition >
            glb_ctrl.posval_bot - glb_ctrl.inertia_bot) { // bot sensor
#ifdef UART_DEBUG
          PutString("fl-5\r\n");
#endif
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
          sys_ctrl.sub_sts = 0;
        } else {
#ifdef UART_DEBUG
          PutString("fl-6\r\n");
#endif
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_DN;
          sys_ctrl.sub_sts = 0;
        }
      }
    }
  }
}
/**
 * @brief  Excute up.
 * @param  None
 * @retval None
 *
 */
void sys_excute_up(void) {

  U16ET Position, PosSmp, index;
  U08ET Current, calcForce;

  Position = pos_get();
  Current = (U08ET)(getadc_data(ADC_CH_MOTOR) >> 2);
  PosSmp = (glb_ctrl.posval_bot - glb_ctrl.posval_top + UP_FORCE_SAMP - 1) /
           UP_FORCE_SAMP;

  if (ET_TRUE == remote_hits() || KEY_ST_HIGH == getkey_stabe(KEY_ID_K5)) {
    motors_pwms(0);
    sys_ctrl.sub_sts = 3;
    sys_ctrl.delay0 = 100;
    return;
  }

  if (0 == sys_ctrl.sub_sts) {
    motors_ctrl(MOT_CT_UP);
    sys_ctrl.cur_dir = MOT_CT_UP;
    sys_ctrl.start_count = 0;
#ifdef UART_DEBUG
    PutString("excute--up\r\n");
#else
    segtsk_ctrl(0, SEG_UPUP, 0, UP_SEG_FLICKER_PERIOD);
#endif
    Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
  } else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Motor_PWM_ctrl_init();
      sys_ctrl.delay0 = 1500;
      sys_ctrl.delay1 = 300;
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
    }
  } else if (2 == sys_ctrl.sub_sts) {
    /* calculate force sampling position */
    if (Position > glb_ctrl.posval_bot)
      index = 0;
    else if (Position < glb_ctrl.posval_top)
      index = (glb_ctrl.posval_bot - glb_ctrl.posval_top) / PosSmp;
    else
      index = (glb_ctrl.posval_bot - Position) / PosSmp;

    if (index == ((glb_ctrl.posval_bot - glb_ctrl.posval_top) / PosSmp))
      --index;

    /* normal route overforce judge */
    if (Position > glb_ctrl.posval_top + HAL_CNT_SLOW_STOP) {
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);

      if (sys_ctrl.delay0 > 0) {
        sys_ctrl.delay0--;
        /* Resistance rebound in starting duration */
        if (Current >
            (glb_ctrl.uforce_max + bk_force[glb_ctrl.force_lv - 1][3])) {
          //			sys_ctrl.OvCurCount++;
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          motors_pwms(0);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 200;
          return;
        } /*else if (Current < (calcForce+(bk_force[glb_ctrl.force_lv -
         1][3]/2))){ if (sys_ctrl.OvCurCount > 0) sys_ctrl.OvCurCount--;
         }*/
      } else {
        /* Resistance rebound in normal duration */
        calcForce = max(glb_ctrl.upForce[index - 1], glb_ctrl.upForce[index],
                        glb_ctrl.upForce[index + 1]);
        if (Current > (calcForce + bk_force[glb_ctrl.force_lv - 1][4])) {
          //			sys_ctrl.OvCurCount++;
#ifdef UART_DEBUG
          PutString("OV FORCR 1 :  cur:");
          puthex(Current);
          PutString("    store:");
          puthex(calcForce);
          PutString("    level:");
          puthex(bk_force[glb_ctrl.force_lv - 1][4]);
          PutString("\r\n");
#endif
          motors_pwms(0);
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 200;
          return;
        } /*else if (Current < (calcForce+(bk_force[glb_ctrl.force_lv -
         1][4]/2))){ if (sys_ctrl.OvCurCount > 0) sys_ctrl.OvCurCount--;
         }*/
      }
    }
    /* stop route overforce judge */
    else {
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);

      /* Resistance rebound in slow stop duration */
      if ((sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0)) {
        if (sys_ctrl.delay0 > 0) {
          sys_ctrl.delay0--;
        }
        if (sys_ctrl.delay1 > 0) {
          sys_ctrl.delay1--;
        }
        if (Current >
            (glb_ctrl.uforce_max + bk_force[glb_ctrl.force_lv - 1][3])) {
          //		    sys_ctrl.OvCurCount++;
          motors_pwms(0);
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 200;
          return;
        } /*else if (Current < (calcForce+(bk_force[glb_ctrl.force_lv -
         1][3]/2))){ if (sys_ctrl.OvCurCount > 0) sys_ctrl.OvCurCount--;
         }*/
      } else {
        calcForce = max(glb_ctrl.upForce[index - 1], glb_ctrl.upForce[index],
                        glb_ctrl.upForce[index + 1]);
        if (Current > (calcForce + bk_force[glb_ctrl.force_lv - 1][5])) {
#ifdef UART_DEBUG
          PutString("OV FORCR 2 :  cur:");
          puthex(Current);
          PutString("    store:");
          puthex(calcForce);
          PutString("    level:");
          puthex(bk_force[glb_ctrl.force_lv - 1][5]);
          PutString("\r\n");
#endif
          motors_pwms(0);
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          sys_ctrl.sub_sts = 3;
          sys_ctrl.delay0 = 200;
          return;
          //		    sys_ctrl.OvCurCount++;
        } /*else if (Current < (calcForce+(bk_force[glb_ctrl.force_lv -
         1][5]/2))){ if (sys_ctrl.OvCurCount > 0) sys_ctrl.OvCurCount--;
         }*/
      }
    }
    motors_pwms(sys_ctrl.cur_pwm);

    /* hit top side, stop motor */
    if (Position <= glb_ctrl.posval_top + glb_ctrl.inertia_top) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;
      return;
    }
  }
  /* delay sometime to stop motor */
  else if (3 == sys_ctrl.sub_sts) {
    --sys_ctrl.delay0;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CT_STOP);
      sys_ctrl.cur_dir = MOT_CT_STOP;
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
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
      sys_ctrl.lst_dir = MOT_CT_UP;
#ifdef UART_DEBUG
      PutString("DOOR UP OK:");
      PutString("Top pos: ");
      putint(glb_ctrl.posval_top);
      PutString("\r\n");
      PutString("Bot pos: ");
      putint(glb_ctrl.posval_bot);
      PutString("\r\n");
      PutString("Position: ");
      putint(Position);
      PutString("\r\n");
#endif
    }
  }
  return;
}
/**
 * @brief  Excute down.
 * @param  None
 * @retval None
 *
 */
void sys_excute_dn(void) {
  U16ET Position, PosSmp, index;
  U08ET Current, calcForce;

  Position = pos_get();
  Current = (U08ET)(getadc_data(ADC_CH_MOTOR) >> 2);
  PosSmp = (glb_ctrl.posval_bot - glb_ctrl.posval_top + DN_FORCE_SAMP - 1) /
           DN_FORCE_SAMP;

  if (ET_TRUE == remote_hits() || KEY_ST_HIGH == getkey_stabe(KEY_ID_K5)) {
    motors_pwms(0);
    Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
    sys_ctrl.sub_sts = 3;
    sys_ctrl.delay0 = 100;
    return;
  }

  if (KEY_ST_HIGH == getkey_stabe(KEY_ID_INFRA)) {
    sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
    sys_ctrl.sub_sts = 0;
    return;
  }

  if (0 == sys_ctrl.sub_sts) {
    motors_ctrl(MOT_CT_DOWN);
    sys_ctrl.cur_dir = MOT_CT_DOWN;
    sys_ctrl.start_count = 0;
    Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
#ifdef UART_DEBUG
    PutString("excute--dn\r\n");
#else
    segtsk_ctrl(0, SEG_DOWN, 0, DOWN_SEG_FLICKER_PERIOD);
#endif
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
  } else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Motor_PWM_ctrl_init();
      sys_ctrl.delay0 = 1000;
      sys_ctrl.delay1 = 300;
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
    }
  } else if (2 == sys_ctrl.sub_sts) {

    if (Position < glb_ctrl.posval_top)
      index = 0;
    else if (Position >= glb_ctrl.posval_bot)
      index = ((glb_ctrl.posval_bot - glb_ctrl.posval_top) / PosSmp);
    else
      index = (Position - glb_ctrl.posval_top) / PosSmp;

    if (index == ((glb_ctrl.posval_bot - glb_ctrl.posval_top) / PosSmp))
      --index;

    if (Position < glb_ctrl.posval_bot - HAL_CNT_SLOW_STOP) {
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);

      if (sys_ctrl.delay0 > 0) {
        sys_ctrl.delay0--;
        if (Current >
            (glb_ctrl.dforce_max + bk_force[glb_ctrl.force_lv - 1][0])) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          sys_ctrl.sub_sts = 0;
          return;
        }
      } else {
        calcForce = max(glb_ctrl.dnForce[index - 1], glb_ctrl.dnForce[index],
                        glb_ctrl.dnForce[index + 1]);
        if (Current > (calcForce + bk_force[glb_ctrl.force_lv - 1][1])) {
#ifdef UART_DEBUG
          PutString("OV FORCR 1 :  cur:");
          puthex(Current);
          PutString("    store:");
          puthex(calcForce);
          PutString("    level:");
          puthex(bk_force[glb_ctrl.force_lv - 1][1]);
          PutString("\r\n");
#endif
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          sys_ctrl.sub_sts = 0;
          return;
        }
      }
    }

    else {
      Motor_PWM_Raise(&sys_ctrl.cur_pwm, PWM_VAL_MAX_STP, 2);

      if ((sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0)) {
        if (sys_ctrl.delay0 > 0) {
          sys_ctrl.delay0--;
        }
        if (sys_ctrl.delay1 > 0) {
          sys_ctrl.delay1--;
        }
        if (Current >
            (glb_ctrl.dforce_max + bk_force[glb_ctrl.force_lv - 1][0])) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          sys_ctrl.sub_sts = 0;
          return;
        }
      } else {
        calcForce = max(glb_ctrl.dnForce[index - 1], glb_ctrl.dnForce[index],
                        glb_ctrl.dnForce[index + 1]);
        if (Current > (calcForce + bk_force[glb_ctrl.force_lv - 1][2])) {
#ifdef UART_DEBUG
          PutString("OV FORCR 2 :  cur:");
          puthex(Current);
          PutString("    store:");
          puthex(calcForce);
          PutString("    level:");
          puthex(bk_force[glb_ctrl.force_lv - 1][2]);
          PutString("\r\n");
#endif

          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BK;
          BUZ_ctrl(EXCUTE_AB_PERIOD, EXCUTE_AB_TIMES);
          sys_ctrl.sub_sts = 0;
          return;
        }
      }
    }
    motors_pwms(sys_ctrl.cur_pwm);

    if (Position >= (glb_ctrl.posval_bot - glb_ctrl.inertia_bot)) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 200;
      return;
    }
  }

  else if (3 == sys_ctrl.sub_sts) {
    --sys_ctrl.delay0;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CT_STOP);
      sys_ctrl.cur_dir = MOT_CT_STOP;
      Lamp_ctrl(LAMP_OPEN_KEEP_TIME);
      sys_ctrl.delay0 = 60;
      sys_ctrl.sub_sts = 4;
    }
  }

  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
      sys_ctrl.lst_dir = MOT_CT_DOWN;

#ifdef UART_DEBUG
      PutString("DOOR DOWN OK:\r\n");
      PutString("Top pos: ");
      putint(glb_ctrl.posval_top);
      PutString("\r\n");
      PutString("Bot pos: ");
      putint(glb_ctrl.posval_bot);
      PutString("\r\n");
      PutString("Position: ");
      putint(Position);
      PutString("\r\n");
#endif
    }
  }
  return;
}
/**
 * @brief  Excute down out of force, prepare for Excute up.
 * @param  None
 * @retval None
 *
 */
void sys_excute_bk(void) {
  U16ET Position;

  Position = pos_get();

  if (0 == sys_ctrl.sub_sts) {
    motors_pwms(0);
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 100;
  }

  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CT_STOP);
      sys_ctrl.cur_dir = MOT_CT_STOP;
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 60;
    }
  }

  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CT_UP);
      sys_ctrl.cur_dir = MOT_CT_UP;
      sys_ctrl.start_count = 0;
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 10;
    }
  }

  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      Motor_PWM_ctrl_init();
      sys_ctrl.delay0 = 700;
      sys_ctrl.delay1 = 300;
      sys_ctrl.cur_pwm = PWM_VAL_DFT_REV;
      //	    sys_ctrl.OvCurCount = 0;
      sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
      sys_ctrl.sub_sts = 2;
// BUZ_ctrl(BUZ_MOV, 0xffff);
#ifdef UART_DEBUG
      PutString("bk--up\r\n");
#else
      segtsk_ctrl(0, SEG_UPUP, 0, UP_SEG_FLICKER_PERIOD);
#endif
    }
  }
}

/**
 * @brief  Set Romte Key.
 * @param  None
 * @retval None
 *
 */
void sys_remote_st(void) {
  U08ET rem_key[REID_SIZE];
  U08ET index;
  /* operation cancel */
  if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) {
    sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
    sys_ctrl.sub_sts = 0;
    return;
  }
  /* initialize parameter */
  if (0 == sys_ctrl.sub_sts) {
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 1000;
  }
  /* wait code key long press */
  else if (1 == sys_ctrl.sub_sts) {
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K3)) {
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
      return;
    }
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("-Re_st\r\n");
#else
      segtsk_ctrl(0, SEG_NONE, 1, 300);
#endif
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 4500;
    }
  }
  /* wait code key release */
  else if (2 == sys_ctrl.sub_sts) {
    if (KEY_ST_LOW == getkey_curst(KEY_ID_K3)) {
      sys_ctrl.sub_sts = 3;
#ifndef UART_DEBUG
      segtsk_ctrl(0, SEG_NONE, 1, 300);
#endif
      sys_ctrl.delay0 = 10000;
      remote_hits();
    }
    sys_ctrl.delay0--;
    /* clear the saved remoter code */
    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("------Remote_stay\r\n");
#else
      segtsk_ctrl(0, SEG_CLRS, 0, 0);
#endif
      BUZ_ctrl(BUZ_SET_SUCCESS_PERIOD, BUZ_SET_SUCCESS_TIMES);
      remote_clrs();
      sys_ctrl.delay0 = 500;
      sys_ctrl.sub_sts = 5;
      return;
    }
  }
  /* wait remoter key */
  else if (3 == sys_ctrl.sub_sts) {
    if (ET_TRUE == remote_gkey(sys_ctrl.rem_key)) {
      sys_ctrl.sub_sts = 4;
      sys_ctrl.delay0 = 5000;
#ifdef UART_DEBUG
      PutString("1st\r\n");
#else
      segtsk_ctrl(0, SEG_NONE, 0, 0);
#endif
    }
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      BUZ_ctrl(BUZ_SET_OVERTIME_PERIOD, BUZ_SET_OVERTIME_TIMES);
      sys_ctrl.delay0 = 500;
      sys_ctrl.sub_sts = 5;
    }
  }
  /* wait another remoter key */
  else if (4 == sys_ctrl.sub_sts) {
    if (ET_TRUE == remote_gkey(rem_key)) {
      if (rem_key[0] == sys_ctrl.rem_key[0] &&
          rem_key[1] == sys_ctrl.rem_key[1] &&
          rem_key[2] == sys_ctrl.rem_key[2] &&
          rem_key[3] == sys_ctrl.rem_key[3]) {
        sys_ctrl.delay0 = 500;
        sys_ctrl.sub_sts = 5;
#ifdef UART_DEBUG
        PutString("2nd\r\n");
#else
        segtsk_ctrl(0, SEG_NONE, 1, 50);
#endif
        BUZ_ctrl(BUZ_SET_SUCCESS_PERIOD, BUZ_SET_SUCCESS_TIMES);
        remote_insert_key(rem_key); // save key to flashrom
      }
    }
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      BUZ_ctrl(BUZ_SET_OVERTIME_PERIOD, BUZ_SET_OVERTIME_TIMES);
      sys_ctrl.delay0 = 500;
      sys_ctrl.sub_sts = 5;
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
/**
 * @brief  Judge which Function the sys will jump to.
 * @param  None
 * @retval None
 *
 */
void sys_system_st(void) {
  if (0 == sys_ctrl.sub_sts) {
#ifdef UART_DEBUG
    PutString("--sys_st\r\n");
#endif
    sys_ctrl.delay0 = 1000;
    sys_ctrl.sub_sts = 1;
  }
  /* wait set long press */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K1)) {
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
      return;
    }
    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("Seg-1\r\n");
#else
      segtsk_ctrl(ET_TRUE, 1, 0, 0);
#endif
      sys_ctrl.delay0 = 1000;
      sys_ctrl.sub_sts = 2;
    }
  }
  /* wait set release */
  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K1)) {
      sys_ctrl.sys_sts = SYS_ST_LIMIT_ST;
      sys_ctrl.sub_sts = 0;
      return;
    }

    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("Seg-3\r\n");
#else
      segtsk_ctrl(ET_TRUE, 3, 0, 0);
#endif
      sys_ctrl.delay0 = 1000;
      sys_ctrl.sub_sts = 3;
    }
  }
  /* wait set press again */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K1)) {
      sys_ctrl.sys_sts = SYS_ST_FORCE_ST;
      sys_ctrl.sub_sts = 0;
      return;
    }
    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("Seg-5\r\n");
#else
      segtsk_ctrl(ET_TRUE, 5, 0, 0);
#endif
      sys_ctrl.delay0 = 1000;
      sys_ctrl.sub_sts = 4;
    }
  } else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K1)) {
      sys_ctrl.sys_sts = SYS_ST_LR_ST;
      sys_ctrl.sub_sts = 0;
      return;
    }
    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("Seg-1\r\n");
#else
      segtsk_ctrl(ET_TRUE, 1, 0, 0);
#endif
      sys_ctrl.delay0 = 1000;
      sys_ctrl.sub_sts = 2;
    }
  }
  return;
}
/**
 * @brief  Set the force Lv. of Excute.
 * @param  None
 * @retval None
 *
 */
void sys_force_st(void) {
  /* initialize parameter */
  if (0 == sys_ctrl.sub_sts) {
    sys_ctrl.sub_sts = 1;
#ifdef UART_DEBUG
    PutString("fc lv:");
    puthex(glb_ctrl.force_lv);
    PutString("\r\n");
#else
    segtsk_ctrl(1, glb_ctrl.force_lv, 0, 0);
#endif
    sys_ctrl.delay1 = 10000;
  }
  /* wait code key long press */
  else if (1 == sys_ctrl.sub_sts) {
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K2) && glb_ctrl.force_lv < 9) {
      ++glb_ctrl.force_lv;
#ifdef UART_DEBUG
      PutString("fc lv:");
      puthex(glb_ctrl.force_lv);
      PutString("\r\n");
#else
      segtsk_ctrl(1, glb_ctrl.force_lv, 0, 0);
#endif
      sys_ctrl.delay1 = 10000;
      // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K4) && glb_ctrl.force_lv > 1) {
      --glb_ctrl.force_lv;
#ifdef UART_DEBUG
      PutString("fc lv:");
      puthex(glb_ctrl.force_lv);
      PutString("\r\n");
#else
      segtsk_ctrl(1, glb_ctrl.force_lv, 0, 0);
#endif
      sys_ctrl.delay1 = 10000;
      // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) {
#ifdef UART_DEBUG
      PutString("fc set:");
      puthex(glb_ctrl.force_lv);
      PutString("\r\n");
      //		#else
      //		segtsk_ctrl(0, SEG_STOP, 0, 0);
#endif
      BUZ_ctrl(BUZ_SET_SUCCESS_PERIOD, BUZ_SET_SUCCESS_TIMES);
      sys_ctrl.delay0 = 500;
      sys_ctrl.sub_sts = 2;
      // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      return;
    }
    --sys_ctrl.delay1;
    if (0 == sys_ctrl.delay1) {
      sys_ctrl.delay0 = 500;
      BUZ_ctrl(BUZ_SET_OVERTIME_PERIOD, BUZ_SET_OVERTIME_TIMES);
      // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      sys_ctrl.sub_sts = 2;
      return;
    }
  }
  /* wait code key release */
  else if (2 == sys_ctrl.sub_sts) {
    --sys_ctrl.delay0;
    if (0 == sys_ctrl.delay0) {
      write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
    }
  }
}
/**
 * @brief  Set the mode left or right side.
 * @param  None
 * @retval None
 *
 */
void sys_lr_st(void) {
  if (0 == sys_ctrl.sub_sts) {
    sys_ctrl.sub_sts = 1;
    sys_ctrl.bit_flg.bit0 = glb_ctrl.LR_set;
#ifdef UART_DEBUG
    PutString("LR_set:");
    puthex(glb_ctrl.LR_set);
    PutString("\r\n");
#else
    segtsk_ctrl(1, glb_ctrl.LR_set, 0, 0);
#endif
    sys_ctrl.delay1 = 10000;
  } else if (1 == sys_ctrl.sub_sts) {
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K2)) {
      if (0 == glb_ctrl.LR_set) {
        glb_ctrl.LR_set = 1;
#ifdef UART_DEBUG
        PutString("LR_set:");
        puthex(glb_ctrl.LR_set);
        PutString("\r\n");
#else
        segtsk_ctrl(1, glb_ctrl.LR_set, 0, 0);
#endif
      }
      sys_ctrl.delay1 = 10000;
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K4)) {
      if (1 == glb_ctrl.LR_set) {
        glb_ctrl.LR_set = 0;
#ifdef UART_DEBUG
        PutString("LR_set:");
        puthex(glb_ctrl.LR_set);
        PutString("\r\n");
#else
        segtsk_ctrl(1, glb_ctrl.LR_set, 0, 0);
#endif
      }
      sys_ctrl.delay1 = 10000;
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) {
#ifdef UART_DEBUG
      PutString("LR_set:");
      puthex(glb_ctrl.LR_set);
      PutString("\r\n");
#else
// segtsk_ctrl(0, SEG_STOP, 0, 0);
#endif
      BUZ_ctrl(BUZ_SET_SUCCESS_PERIOD, BUZ_SET_SUCCESS_TIMES);
      sys_ctrl.delay0 = 500;
      sys_ctrl.sub_sts = 2;
      if (glb_ctrl.LR_set != sys_ctrl.bit_flg.bit0) {
        glb_ctrl.posval_top = POS_ERR;
        glb_ctrl.posval_bot = POS_ERR;
        // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      }
      return;
    }
    --sys_ctrl.delay1;
    if (0 == sys_ctrl.delay1) {
      sys_ctrl.delay0 = 500;
      BUZ_ctrl(BUZ_SET_OVERTIME_PERIOD, BUZ_SET_OVERTIME_TIMES);
      if (glb_ctrl.LR_set != sys_ctrl.bit_flg.bit0) {
        glb_ctrl.posval_top = POS_ERR;
        glb_ctrl.posval_bot = POS_ERR;
        // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      }
      sys_ctrl.sub_sts = 2;
      return;
    }
  } else if (2 == sys_ctrl.sub_sts) {
    --sys_ctrl.delay0;
    if (0 == sys_ctrl.delay0) {
      write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
    }
  }
}
/**
 * @brief  Set Auto close time.
 * @param  None
 * @retval None
 *
 */
void sys_atclo_st(void) {
  /* initialize parameter */
  if (0 == sys_ctrl.sub_sts) {
#ifdef UART_DEBUG
    PutString("--sys_atclo\r\n");
#endif
    sys_ctrl.delay0 = 1000;
    sys_ctrl.sub_sts = 1;
  } else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K2)) {
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
      return;
    }
    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("Seg-line\r\n");
#else
      segtsk_ctrl(0, SEG_LINE, 0, 0);
#endif
      sys_ctrl.sub_sts = 2;
    }
  } else if (2 == sys_ctrl.sub_sts) {
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K2)) {
#ifdef UART_DEBUG
      PutString("atlv:");
      puthex(glb_ctrl.auto_clo);
      PutString("\r\n");
#else
      segtsk_ctrl(1, glb_ctrl.auto_clo, 0, 0);
#endif
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay1 = 10000;
      return;
    }
  } else if (3 == sys_ctrl.sub_sts) {
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K2)) {
      if (glb_ctrl.auto_clo < 9) {
        ++glb_ctrl.auto_clo;
#ifdef UART_DEBUG
        PutString("fc lv:");
        puthex(glb_ctrl.auto_clo);
        PutString("\r\n");
#else
        segtsk_ctrl(1, glb_ctrl.auto_clo, 0, 0);
#endif
        // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      }
      sys_ctrl.delay1 = 10000;
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K4)) {
      if (glb_ctrl.auto_clo > 0) {
        --glb_ctrl.auto_clo;
#ifdef UART_DEBUG
        PutString("fc lv:");
        puthex(glb_ctrl.auto_clo);
        PutString("\r\n");
#else
        segtsk_ctrl(1, glb_ctrl.auto_clo, 0, 0);
#endif
        // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      }
      sys_ctrl.delay1 = 10000;
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) {
#ifdef UART_DEBUG
      PutString("fc set:");
      puthex(glb_ctrl.auto_clo);
      PutString("\r\n");
      //		#else
      //		segtsk_ctrl(0, SEG_STOP, 0, 0);
#endif
      BUZ_ctrl(BUZ_SET_SUCCESS_PERIOD, BUZ_SET_SUCCESS_TIMES);
      sys_ctrl.delay0 = 500;
      sys_ctrl.sub_sts = 4;
      // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      return;
    }
    --sys_ctrl.delay1;
    if (0 == sys_ctrl.delay1) {
      sys_ctrl.delay0 = 500;
      BUZ_ctrl(BUZ_SET_OVERTIME_PERIOD, BUZ_SET_OVERTIME_TIMES);
      sys_ctrl.sub_sts = 4;
      return;
    }
  } else if (4 == sys_ctrl.sub_sts) {
    --sys_ctrl.delay0;
    if (0 == sys_ctrl.delay0) {
      write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
    }
  }
}
/**
 * @brief  Set if the infra Function enable.
 * @param  None
 * @retval None
 *
 */
void sys_infra_st(void) {
  /* initialize parameter */
  if (0 == sys_ctrl.sub_sts) {
#ifdef UART_DEBUG
    PutString("--sys_infra\r\n");
#endif
    sys_ctrl.delay0 = 1000;
    sys_ctrl.sub_sts = 1;
  }
  /* wait set long press */
  else if (1 == sys_ctrl.sub_sts) {
    if (KEY_ST_LOW == getkey_stabe(KEY_ID_K4)) {
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
      return;
    }
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
#ifdef UART_DEBUG
      PutString("Seg-line\r\n");
#else
      {
        if (1 == glb_ctrl.infra) {
          segtsk_ctrl(0, SEG_INF1, 0, 0);
        } else {
          segtsk_ctrl(0, SEG_INF0, 0, 0);
        }
      }
#endif
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay1 = 10000;
    }
  }
  /* wait code key long press */
  else if (2 == sys_ctrl.sub_sts) {
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K2)) {
      if (0 == glb_ctrl.infra) {
        glb_ctrl.infra = 1;
#ifdef UART_DEBUG
        PutString("infra:");
        puthex(glb_ctrl.infra);
        PutString("\r\n");
#else
        segtsk_ctrl(0, SEG_INF1, 0, 0);
#endif
        // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      }
      sys_ctrl.delay1 = 10000;
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K4)) {
      if (1 == glb_ctrl.infra) {
        glb_ctrl.infra = 0;
#ifdef UART_DEBUG
        PutString("LR_set:");
        puthex(glb_ctrl.infra);
        PutString("\r\n");
#else
        segtsk_ctrl(0, SEG_INF0, 0, 0);
#endif
        // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      }
      sys_ctrl.delay1 = 10000;
    }
    if (KEY_ST_HIGH == getkey_stabe(KEY_ID_K1)) {
#ifdef UART_DEBUG
      PutString("infra:");
      puthex(glb_ctrl.infra);
      PutString("\r\n");
      //		#else
      //		segtsk_ctrl(0, SEG_STOP, 0, 0);
#endif
      BUZ_ctrl(BUZ_SET_SUCCESS_PERIOD, BUZ_SET_SUCCESS_TIMES);
      sys_ctrl.delay0 = 500;
      sys_ctrl.sub_sts = 4;
      // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      return;
    }
    --sys_ctrl.delay1;
    if (0 == sys_ctrl.delay1) {
      sys_ctrl.delay0 = 500;
      BUZ_ctrl(BUZ_SET_OVERTIME_PERIOD, BUZ_SET_OVERTIME_TIMES);
      // write_data_to_dataflash((U08ET*)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      sys_ctrl.sub_sts = 4;
      return;
    }
  }
  /* wait code key release */
  else if (4 == sys_ctrl.sub_sts) {
    --sys_ctrl.delay0;
    if (0 == sys_ctrl.delay0) {
      write_data_to_dataflash((U08ET *)&glb_ctrl, 0x0, sizeof(GLB_CTRL));
      sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
      sys_ctrl.sub_sts = 0;
    }
  }
}

/**
 * @brief  Detect the error of sys.
 * @param  None
 * @retval None
 *
 */
void AbnormalDetective(void) {
  U16ET Postition;

  if (SYS_ST_ERROR == sys_ctrl.sys_sts)
    return;

  Postition = pos_get();
  if (0 == sys_ctrl.start_count)
    sys_ctrl.ErrPrePosition = Postition;

  if (sys_ctrl.start_count < 500)
    ++sys_ctrl.start_count;

  if ((MOT_CT_DOWN == sys_ctrl.cur_dir) &&
      (SYS_ST_LIMIT_ST != sys_ctrl.sys_sts)) {
    if ((Postition > sys_ctrl.ErrPrePosition) &&
        ((Postition - sys_ctrl.ErrPrePosition) > 4)) {
      sys_ctrl.EncoderErrCnt = 0;
      sys_ctrl.ErrPrePosition = Postition;
    } else {
      if (sys_ctrl.start_count < 500)
        ++sys_ctrl.EncoderErrCnt;
      else
        sys_ctrl.EncoderErrCnt += 3;
    }
  } else if ((MOT_CT_UP == sys_ctrl.cur_dir) &&
             (SYS_ST_LIMIT_ST != sys_ctrl.sys_sts)) {
    if ((Postition < sys_ctrl.ErrPrePosition) &&
        ((sys_ctrl.ErrPrePosition - Postition) > 4)) {
      sys_ctrl.EncoderErrCnt = 0;
      sys_ctrl.ErrPrePosition = Postition;
    } else {
      if (sys_ctrl.start_count < 500)
        ++sys_ctrl.EncoderErrCnt;
      else
        sys_ctrl.EncoderErrCnt += 3;
    }
  } else {
    sys_ctrl.EncoderErrCnt = 0;
  }

  if (sys_ctrl.EncoderErrCnt > 2400) {
    sys_ctrl.sys_sts = SYS_ST_ERROR;
    sys_ctrl.sub_sts = 0;
    sys_ctrl.err_num = 0xE1;
    if ((U08ET)(getadc_data(ADC_CH_MOTOR) >> 2) > 0x04)
      BUZ_ctrl(300, 3);
    else
      BUZ_ctrl(300, 3);
  }
}
/**
 * @brief  Display the Error code.
 * @param  None
 * @retval None
 *
 */
void sys_error(void) {
  U16ET CurPosition;
  CurPosition = pos_get();
  if (0 == sys_ctrl.sub_sts) {
    motors_pwms(0);
    sys_ctrl.delay0 = 100;
    sys_ctrl.sub_sts = 1;
  }
  /* stop motor excution */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CT_STOP);
      sys_ctrl.cur_dir = MOT_CT_STOP;
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 0;
      sys_ctrl.delay1 = 1;
    }
  }

  else if (2 == sys_ctrl.sub_sts) {
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.delay0 = 300;
      if (1 == sys_ctrl.delay1) {
#ifndef UART_DEBUG
        segtsk_ctrl(0, SEG_EROR_3LINE, 0, 0);
#endif
        sys_ctrl.delay1 = 0;
      } else {
#ifndef UART_DEBUG
        segtsk_ctrl(0, SEG_EROR_3LINE, 0, 0);
#endif
        sys_ctrl.delay1 = 1;
      }
    }
    --sys_ctrl.delay0;
    if (0xE1 == sys_ctrl.err_num) {
      if ((ET_TRUE == remote_hits() ||
           KEY_ST_HIGH == getkey_stabe(KEY_ID_K5)) &&
          (glb_ctrl.posval_top != POS_ERR && glb_ctrl.posval_bot != POS_ERR)) {
        if (CurPosition > glb_ctrl.posval_top + TOP_TOLERANCE) { // pos == top
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_UP;
          sys_ctrl.sub_sts = 0;
        } else {
          sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
          sys_ctrl.sub_sts = 0;
        }
      }
    }
  }
}
