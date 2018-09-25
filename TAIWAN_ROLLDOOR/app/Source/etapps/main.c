#include "main.h"
#include "subs.h"
#include <stdio.h>

/**
  ******************************************************************************
  * @file    main.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/8/15
  * @brief   This file provides all application of roll door opener 
  *          functionalities of the application:
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
  *
  *
  ******************************************************************************
  */

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static SYS_CTRL sys_ctrl;

static ENUM_MOTOR_INSTALL_nMODE install_lr_get(VODET);
static ENUM_MOTOR_IFLEARN_nMODE iflearn_get(VODET);
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
#if 1
const U08ET bk_force[8][6] = {
    {60, 60, 40, 60, 50, 40},     {75, 75, 45, 80, 70, 45},
    {90, 70, 30, 100, 70, 30},    {100, 100, 60, 120, 110, 65},
    {115, 110, 70, 135, 125, 70}, {130, 120, 80, 150, 140, 80},
    {145, 130, 90, 165, 155, 90}, {180, 160, 100, 180, 170, 100},
};
#else
const U08ET bk_force[8][6] = {
    {15, 15, 20, 20, 20, 20},     {75, 75, 45, 80, 70, 45},
    {90, 70, 30, 100, 70, 30},    {100, 100, 60, 120, 110, 65},
    {115, 110, 70, 135, 125, 70}, {130, 120, 80, 150, 140, 80},
    {145, 130, 90, 165, 155, 90}, {180, 160, 100, 180, 170, 100},
};
#endif

/**
 * @brief  get l_r install mode
 * @param  NONE
 * @retval 
 *   mode_left or mode_right
 *
 */
static ENUM_MOTOR_INSTALL_nMODE install_lr_get(VODET) {
  return glb_ctrl.install_lr_mode;
}

/**
 * @brief  get force level config
 * @param  NONE
 * @retval 
 *   force level: 0 ~ 7
 *
 */
static ENUM_MOTOR_IFLEARN_nMODE iflearn_get(VODET) {
  ENUM_MOTOR_IFLEARN_nMODE res;
  if ((SYS_ST_LIMITTOP_SET == sys_ctrl.sys_sts) || (SYS_ST_LIMITBOT_SET == sys_ctrl.sys_sts))
		res = MOTOR_LEARN;
  else res = MOTOR_NORMAL;
	
  return res;
}

/**
 * @brief  c_entry
 * @param  NONE
 * @retval NONE
 *
 */
int main(VODET) {
  sys_pre_init();

  while (1u) {
    if (ET_FALSE == tskskm_flag())
      continue;

    wdtdog_feed();
    LedTask();
    keysmp_task();
    adcsmp_task();
    configs_task();
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
  motors_init();
  remote_init();
  LedInital();
  adcsmp_init();
  keysmp_init();
  timwdt_init();  
  configs_init();
  __enable_irq();

  sys_ctrl.sys_sts = SYS_ST_POWER_ON;
  sys_ctrl.sub_sts = 0;
  sys_ctrl.sys_lts = SYS_ST_INVALID;
  sys_ctrl.sub_lts = 0;
  *((U08ET *)(&sys_ctrl.bit_flg)) = 0;

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
    LedTaskCtrl(LED_ID_REMOTE, 1, PRG_MAIN_VER, 150);
    sys_ctrl.delay0 = 200;
    sys_ctrl.sub_sts = 1;
  }

  if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 50;
    }
  }
  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_READ_EEP;
      sys_ctrl.sub_sts = 0;
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
  U32ET size;
#ifdef START_READEEP_DEBUG
  U16ET index;
#endif

/* read system control */
#ifdef START_READEEP_DEBUG
  printf("func <sys_read_eep>\r\n\r\n");
#endif
  size = (OFFSETOF(GLB_CTRL, save_end) + 1);
  size /=	 4;
  dflash_read_page((U32ET *)(&glb_ctrl), DFMC_CTRLS_OFST, size);
#ifdef START_READEEP_DEBUG
  printf("save flag : 0x%x\r\n", (U16ET)glb_ctrl.save_flg);
#endif

  size = UP_FORCE_SAMP / 4;
  dflash_read_page((U32ET *)(&glb_ctrl.upForce[0]), DFMC_UPFORCE_OFST, size);

  size = DN_FORCE_SAMP / 4;
  dflash_read_page((U32ET *)(&glb_ctrl.dnForce[0]), DFMC_DNFORCE_OFST, size);
  wdtdog_feed();
#ifdef START_READEEP_DEBUG
  printf("pre to read remote key\r\n");
#endif
  size = sizeof(ETST_REMOTE_KEYS);
  size /= 4;
  dflash_read_page(remote_get_keys_addr(), DFMC_RKEYS_OFST, size);
  remote_keys_init();

#ifdef START_READEEP_DEBUG
  printf("read keys success\r\n");
#endif
  wdtdog_feed();
  /* check saved flag */
  if (DFLASH_SAVE_FLG != glb_ctrl.save_flg) {
#ifdef START_READEEP_DEBUG
    printf("dataflash flag_1\r\n");
#endif
    glb_ctrl.save_flg = DFLASH_SAVE_FLG;
    glb_ctrl.hall_top = 0;
    glb_ctrl.hall_bot = 0;
    glb_ctrl.install_lr_mode = INSTALL_LEFT;
    sys_ctrl.bit_flg.bit0 = ET_TRUE;
    sys_ctrl.sys_sts = SYS_ST_STANDBY;
    sys_ctrl.sub_sts = 0;
    return;
  }

  /* check if route parameter is normal */
  if ((glb_ctrl.hall_top >= glb_ctrl.hall_bot) ||
      (glb_ctrl.hall_bot > HAL_MAX * 2)) {
    glb_ctrl.hall_top = 0;
    glb_ctrl.hall_bot = 0;
  }
  /* set current motor status */
  halset_bot(glb_ctrl.hall_bot);
  halset_top(glb_ctrl.hall_top);
  wdtdog_feed();
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
  sys_ctrl.bit_flg.bit0 = ET_TRUE;
  sys_ctrl.sys_sts = SYS_ST_STANDBY;
  sys_ctrl.sub_sts = 0;
  sys_ctrl.lst_dir = MOT_CTS_UPUP;
  return;
}

/**
 * @brief  System limit top set
 * @param
 *     NONE
 * @retval
 *     NONE
 *
 */
VODET sys_limittop_set(VODET) {
  U16ET hallCnt;
  LOCAL_COMMAND_LIST remote_command;
  remote_command = remote_hits(&iflearn_get);
  hallCnt = position_get(&install_lr_get, &iflearn_get);
  if (0 == sys_ctrl.sub_sts) {
#ifdef LIMITSET_DEBUG
    printf("func <top set>\r\n");
#endif
    glb_ctrl.hall_top = 0;
    glb_ctrl.hall_bot = 0;
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay1 = 10000;
    return;
  } else if (1 == sys_ctrl.sub_sts) {
    /* move up */
    if (ET_REMOTE_OPEN_BEGIN == remote_command) {
      motors_ctrl(MOT_CTS_UPUP, &install_lr_get);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 10;
      return;
    } else if (ET_REMOTE_CLOSE_BEGIN == remote_command) {
      motors_ctrl(MOT_CTS_DOWN, &install_lr_get);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 10;
      return;
    } else if (ET_REMOTE_SET == remote_command) {
      LedTaskCtrl(LED_ID_REMOTE, 1, 1, 1000);
      glb_ctrl.hall_top = hallCnt;
      sys_ctrl.sys_sts = SYS_ST_LIMITBOT_SET;
      sys_ctrl.sub_sts = 0;
#ifdef LIMITSET_DEBUG
      printf("top_set top : %d\r\n", (U16ET)glb_ctrl.hall_top);
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
  } else if (2 == sys_ctrl.sub_sts) {
    /* move begin */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 3;
    }
  } else if (3 == sys_ctrl.sub_sts) {
    /* turn up pwm ratio */
    sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
    motors_pwms(sys_ctrl.cur_pwm);
    if ((ET_REMOTE_OPEN_END == remote_command) ||
        (ET_REMOTE_CLOSE_END == remote_command)) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 4;
      sys_ctrl.delay0 = 100;
    }
  }
  /***stop motor */
  else if (4 == sys_ctrl.sub_sts) {
    /* delay smoe time, then turn off relay */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
      sys_ctrl.sub_sts = 1;
      sys_ctrl.delay1 = 10000;
#ifdef LIMITSET_DEBUG
      printf("top_set top : %d\r\n", hallCnt);
#endif
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
  LOCAL_COMMAND_LIST remote_command;
  remote_command = remote_hits(&iflearn_get);
  hallCnt = position_get(&install_lr_get, &iflearn_get);
  if (0 == sys_ctrl.sub_sts) {
#ifdef LIMITSET_DEBUG
    printf("func <bot set>\r\n");
#endif
    glb_ctrl.hall_bot = 0;
    sys_ctrl.sub_sts = 1;
    sys_ctrl.learn_up_distance = 0;
    sys_ctrl.learn_down_distance = 0;
    sys_ctrl.times1_distance = 0;
    sys_ctrl.delay1 = 10000;
    return;
  } else if (1 == sys_ctrl.sub_sts) {
    /* move up */
    if (ET_REMOTE_OPEN_BEGIN == remote_command) {
      motors_ctrl(MOT_CTS_UPUP, &install_lr_get);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 10;
      return;
    } else if (ET_REMOTE_CLOSE_BEGIN == remote_command) {
      motors_ctrl(MOT_CTS_DOWN, &install_lr_get);
      sys_ctrl.sub_sts = 5;
      sys_ctrl.delay0 = 10;
      return;
    } else if (ET_REMOTE_SET == remote_command) {
      sys_ctrl.sys_sts = SYS_ST_REMOTE_ST;
      sys_ctrl.sub_sts = 0;

      if ((sys_ctrl.learn_down_distance > sys_ctrl.learn_up_distance) &&
          (sys_ctrl.learn_down_distance - sys_ctrl.learn_up_distance > 300) &&
          (sys_ctrl.learn_down_distance - sys_ctrl.learn_up_distance < 3500)) {

        if (hallCnt < glb_ctrl.hall_top)
          glb_ctrl.hall_bot = hallCnt + PWM_FULL;
        else if ((PWM_FULL - hallCnt) > (PWM_FULL / 18)) {
          glb_ctrl.hall_bot = hallCnt + PWM_FULL;
          glb_ctrl.hall_top += PWM_FULL;
        } else {
          glb_ctrl.hall_bot = hallCnt;
        }

        halset_bot(glb_ctrl.hall_bot);
        halset_top(glb_ctrl.hall_top);
        LedTaskCtrl(LED_ID_REMOTE, 1, 1, 1000);
        sys_ctrl.sys_sts = SYS_ST_TRAIN_OPEN;
        sys_ctrl.sub_sts = 0;
      } else {
        glb_ctrl.hall_bot = 0;
        glb_ctrl.hall_top = 0;
        sys_ctrl.sys_sts = SYS_ST_STANDBY;
        sys_ctrl.sub_sts = 0;
      }
#ifdef LIMITSET_DEBUG
      printf("top : %d\r\n", glb_ctrl.hall_top);
      printf("bot : %d\r\n", glb_ctrl.hall_bot);
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
  }

  /* move up */
  else if (2 == sys_ctrl.sub_sts) {
    /* move begin */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 3;
      sys_ctrl.times1_distance = 0;
      sys_ctrl.start_position = hallCnt;
      sys_ctrl.distance_integral = hallCnt;
    }
  } else if (3 == sys_ctrl.sub_sts) {
    /* turn up pwm ratio */
    sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
    motors_pwms(sys_ctrl.cur_pwm);
    if ((sys_ctrl.distance_integral < LERAN_CRITICAL_RANGE) &&
        (hallCnt > PWM_FULL - LERAN_CRITICAL_RANGE)) {
      sys_ctrl.times1_distance +=
          sys_ctrl.distance_integral + (PWM_FULL - hallCnt);
      sys_ctrl.distance_integral = hallCnt;
    } else if ((sys_ctrl.distance_integral > hallCnt) &&
               (sys_ctrl.distance_integral - hallCnt > 3) &&
               (sys_ctrl.distance_integral - hallCnt < 100)) {
      sys_ctrl.times1_distance += sys_ctrl.distance_integral - hallCnt;
      sys_ctrl.distance_integral = hallCnt;
    }

    if ((ET_REMOTE_OPEN_END == remote_command) ||
        (ET_REMOTE_CLOSE_END == remote_command)) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 4;
      sys_ctrl.delay0 = 100;
    }
  }
  /***stop motor */
  else if (4 == sys_ctrl.sub_sts) {
    /* delay smoe time, then turn off relay */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
      sys_ctrl.sub_sts = 1;
      sys_ctrl.delay1 = 10000;

      if (sys_ctrl.times1_distance > ((PWM_FULL / 18) * 17)) {
        glb_ctrl.hall_bot = 0;
        glb_ctrl.hall_top = 0;
        sys_ctrl.sys_sts = SYS_ST_STANDBY;
        sys_ctrl.sub_sts = 0;
      }

      if (hallCnt <= sys_ctrl.start_position) {
        sys_ctrl.learn_up_distance += (sys_ctrl.start_position - hallCnt);
      } else {
        sys_ctrl.learn_up_distance +=
            sys_ctrl.start_position + (PWM_FULL - hallCnt);
      }
#ifdef LIMITSET_DEBUG
      // printf("up_dis : %d\r\n", sys_ctrl.learn_up_distance);
      printf("cur_hall : %d\r\n", hallCnt);
#endif
    }
  }

  /* move down */
  else if (5 == sys_ctrl.sub_sts) {
    /* move begin */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 6;
      sys_ctrl.times1_distance = 0;
      sys_ctrl.start_position = hallCnt;
      sys_ctrl.distance_integral = hallCnt;
    }
  } else if (6 == sys_ctrl.sub_sts) {
    /* turn up pwm ratio */
    sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_SET, 2);
    motors_pwms(sys_ctrl.cur_pwm);
    if ((sys_ctrl.distance_integral > PWM_FULL - LERAN_CRITICAL_RANGE) &&
        (hallCnt < LERAN_CRITICAL_RANGE)) {
      sys_ctrl.times1_distance +=
          hallCnt + (PWM_FULL - sys_ctrl.distance_integral);
      sys_ctrl.distance_integral = hallCnt;
    } else if ((sys_ctrl.distance_integral < hallCnt) &&
               (hallCnt - sys_ctrl.distance_integral > 3) &&
               (hallCnt - sys_ctrl.distance_integral < 100)) {
      sys_ctrl.times1_distance += hallCnt - sys_ctrl.distance_integral;
      sys_ctrl.distance_integral = hallCnt;
    }

    if ((ET_REMOTE_OPEN_END == remote_command) ||
        (ET_REMOTE_CLOSE_END == remote_command)) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 7;
      sys_ctrl.delay0 = 100;
    }
  }
  /***stop motor */
  else if (7 == sys_ctrl.sub_sts) {
    /* delay smoe time, then turn off relay */
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
      sys_ctrl.sub_sts = 1;
      sys_ctrl.delay1 = 10000;
      if (sys_ctrl.times1_distance > ((PWM_FULL / 18) * 17)) {
        sys_ctrl.sys_sts = SYS_ST_STANDBY;
        sys_ctrl.sub_sts = 0;
      }

      if (hallCnt >= sys_ctrl.start_position) {
        sys_ctrl.learn_down_distance += (hallCnt - sys_ctrl.start_position);
      } else {
        sys_ctrl.learn_down_distance +=
            hallCnt + (PWM_FULL - sys_ctrl.start_position);
      }
#ifdef LIMITSET_DEBUG
      // printf("dn_dis : %d\r\n", sys_ctrl.learn_down_distance);
      printf("cur_hall : %d\r\n", hallCnt);
#endif
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
  U08ET adcValue;
  U16ET hallCnt, hallSmp, index;
  //  LOCAL_COMMAND_LIST remote_command;

#ifdef TRAIN_DEBUG
  U16ET buf[8];
#endif

  //  remote_command = remote_hits(&iflearn_get);
  hallCnt = position_get(&install_lr_get, &iflearn_get);
  adcValue = getadc_data(ADC_CH_MOT);

  hallSmp = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1) /
            UP_FORCE_SAMP;

  if (hallSmp == 0)
    hallSmp = 1;

  /* iniltal parameter for open set */
  if (0 == sys_ctrl.sub_sts) {
#ifdef TRAIN_DEBUG
    printf("func <train open>\r\n");
#endif
    /* clean all data */
    for (index = 0; index < UP_FORCE_SAMP; index++)
      glb_ctrl.upForce[index] = 0;

    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    glb_ctrl.ufor_max = 0;
    motors_ctrl(MOT_CTS_UPUP, &install_lr_get);
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
    if (hallCnt > glb_ctrl.hall_top + 600)
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
    else
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_SLOW_STOP, 2);
    motors_pwms(sys_ctrl.cur_pwm);

    /* beyond top side, stop motor */
    if (hallCnt <= glb_ctrl.hall_top) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;

#ifdef TRAIN_DEBUG

      position_buf_get(buf);
      for (index = 0; index < 10; index++) {
        printf("%d ", buf[index]);
      }
      printf("hallcnt: %d\r\n", hallCnt);
#endif
      return;
    }
    /* learn force of the door close duration */
    if ((hallCnt >= glb_ctrl.hall_top) && (hallCnt <= glb_ctrl.hall_bot)) {

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
      if ((hallCnt <= glb_ctrl.hall_top + HAL_CNT_SLOW_STOP) &&
          (hallCnt > glb_ctrl.hall_top + HAL_CNT_SLOW_STOP - 60)) {
        if (glb_ctrl.upForce[index] <
            glb_ctrl.upForce[((glb_ctrl.hall_bot - glb_ctrl.hall_top -
                               HAL_CNT_SLOW_STOP) /
                              hallSmp) -
                             2]) {
          glb_ctrl.upForce[index] =
              glb_ctrl.upForce[((glb_ctrl.hall_bot - glb_ctrl.hall_top -
                                 HAL_CNT_SLOW_STOP) /
                                hallSmp) -
                               2];
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

      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
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
      printf("hallcnt : %d\r\n", hallCnt);
      printf("glb_ctrl.hall_top : %d\r\n", glb_ctrl.hall_top);
      printf("glb_ctrl.hall_bot : %d\r\n", glb_ctrl.hall_bot);
#endif
      sys_ctrl.sub_sts = 5;
      sys_ctrl.delay1 = 10000;
    }
  }
  /**wait for long press of key_DN*/
  else if (5 == sys_ctrl.sub_sts) {
    sys_ctrl.sys_sts = SYS_ST_TRAIN_CLOSE;
    sys_ctrl.sub_sts = 0;

#ifdef TRAIN_DEBUG
    printf("open force:\r\n");
    printf("open max 0x%x:\r\n", glb_ctrl.ufor_max);
    for (index = 0; index < UP_FORCE_SAMP; index++)
      printf("0x%x\r\n", glb_ctrl.upForce[index]);
#endif
  }
}

/**
 * @brief  System train close
 * @param  NONE
 * @retval NONE
 *
 */
VODET sys_train_close(VODET) {
  U08ET adcValue;
  U16ET hallCnt, hallSmp, index;
  //  LOCAL_COMMAND_LIST remote_command;

  //  remote_command = remote_hits(&iflearn_get);
  hallCnt = position_get(&install_lr_get, &iflearn_get);
  adcValue = getadc_data(ADC_CH_MOT);
  hallSmp = (glb_ctrl.hall_bot - glb_ctrl.hall_top + DN_FORCE_SAMP - 1) /
            DN_FORCE_SAMP;

  if (hallSmp == 0)
    hallSmp = 1;

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
    motors_ctrl(MOT_CTS_DOWN, &install_lr_get);
  } else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
    }
  } else if (2 == sys_ctrl.sub_sts) {
    /* blink when touch ref hal */
    if (hallCnt < glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP)
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
    else
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_SLOW_STOP, 2);

    motors_pwms(sys_ctrl.cur_pwm);
    /* hit bot side, stop motor */
    if (hallCnt >= glb_ctrl.hall_bot) {
      motors_pwms(0);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;
      return;
    }
    /* learn force */
    if ((hallCnt >= glb_ctrl.hall_top) && (hallCnt <= glb_ctrl.hall_bot)) {

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

      if ((hallCnt >= (glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP)) &&
          (hallCnt < (glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP + 60))) {
        if (glb_ctrl.dnForce[index] <
            glb_ctrl.dnForce[((glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP -
                               glb_ctrl.hall_top) /
                              hallSmp) -
                             2]) {
          glb_ctrl.dnForce[index] =
              glb_ctrl.dnForce[((glb_ctrl.hall_bot - HAL_CNT_SLOW_STOP -
                                 glb_ctrl.hall_top) /
                                hallSmp) -
                               2];
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

      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
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
VODET sys_standby(VODET) {
  U08ET infra_status;
  U16ET hallCnt;
  LOCAL_COMMAND_LIST remote_command;

  hallCnt = position_get(&install_lr_get, &iflearn_get);
  remote_command = remote_hits(&iflearn_get);
  infra_status = getkey_stabe(KEY_INFRA);

  if (0 == sys_ctrl.sub_sts) {
#ifdef STANDBY_DEBUG
    printf("func <standby>\r\n");
#endif
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay1 = 1500;
  } else if (1 == sys_ctrl.sub_sts) {
    /* get config from switch */
    sys_ctrl.force_level = config_force_level_set();

    if (glb_ctrl.install_lr_mode != config_install_rf()) {
      glb_ctrl.hall_bot = 0;
      glb_ctrl.hall_top = 0;
      glb_ctrl.install_lr_mode = config_install_rf();
    }

    /* get event<remote set button> */
    if (KEY_ST_HIGH == getkey_stabe(KEY_BUTTON)) {
      sys_ctrl.sys_sts = SYS_ST_REMOTE_ST;
      sys_ctrl.sub_sts = 0;
      return;
    }

    /* remote signal prcess */
    /* limit set entry */
    if (ET_REMOTE_TRAIN_ENTRY == remote_command) {
      LedTaskCtrl(LED_ID_REMOTE, 1, 1, 1000);
      sys_ctrl.sys_sts = SYS_ST_LIMITTOP_SET;
      sys_ctrl.sub_sts = 0;
    } else if ((ET_REMOTE_CTL == remote_command) ||
               (KEY_ST_HIGH == getkey_stabe(KEY_PB_WALL))) {
      #ifdef STANDBY_DEBUG
          printf("get ctrl\r\n");
      #endif
      if ((0 != glb_ctrl.hall_bot) && (0 != glb_ctrl.hall_top)) {
        if (KEY_ST_HIGH == infra_status) {
          if (hallCnt > glb_ctrl.hall_top) {
            sys_ctrl.sys_sts = SYS_ST_EXCUTE_OPEN;
            sys_ctrl.hall_dest = glb_ctrl.hall_top;
            sys_ctrl.sub_sts = 0;
          } else {
            #ifdef STANDBY_DEBUG
                printf("infra move none\r\n");
            #endif
          }
        }  
        else {
          if (MOT_CTS_DOWN == sys_ctrl.lst_dir) {
            if (hallCnt > glb_ctrl.hall_top) {
              sys_ctrl.sys_sts = SYS_ST_EXCUTE_OPEN;
              sys_ctrl.hall_dest = glb_ctrl.hall_top;
              sys_ctrl.sub_sts = 0;
            } else {
              sys_ctrl.sys_sts = SYS_ST_EXCUTE_CLOSE;
              sys_ctrl.hall_dest = glb_ctrl.hall_bot;
              sys_ctrl.sub_sts = 0;
            }
          } else if (MOT_CTS_UPUP == sys_ctrl.lst_dir) {
            if (hallCnt < glb_ctrl.hall_bot) {
              sys_ctrl.sys_sts = SYS_ST_EXCUTE_CLOSE;
              sys_ctrl.hall_dest = glb_ctrl.hall_bot;
              sys_ctrl.sub_sts = 0;
            } else {
              sys_ctrl.sys_sts = SYS_ST_EXCUTE_OPEN;
              sys_ctrl.hall_dest = glb_ctrl.hall_top;
              sys_ctrl.sub_sts = 0;
            }
          }
        }
      }
    }
  }
}

/**
 * @brief  System excute open
 * @param  NONE
 * @retval NONE
 *
 */
VODET sys_excute_open(VODET) {
  U08ET adcValue;
  U16ET hallCnt, hallSmp, index, force_res, calcForce;
  LOCAL_COMMAND_LIST remote_command;

  remote_command = remote_hits(&iflearn_get);
  hallCnt = position_get(&install_lr_get, &iflearn_get);
  adcValue = getadc_data(ADC_CH_MOT);
  hallSmp = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1) /
            UP_FORCE_SAMP;

  if (hallSmp == 0)
    hallSmp = 1;

  if ((ET_REMOTE_CTL == remote_command) ||
      (KEY_ST_HIGH == getkey_stabe(KEY_PB_WALL))) {
#ifdef EXCUTE_DEBUG
    printf("st_open get stop signal\r\n\r\n");
#endif
    motors_pwms(0);
    sys_ctrl.sub_sts = 3;
    sys_ctrl.delay0 = 100;
    return;
  }

  /* motor direction up */
  if (0 == sys_ctrl.sub_sts) {
#ifdef EXCUTE_DEBUG
    printf("func <excute up>\r\n\r\n");
#endif
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    sys_ctrl.delay2 = 0;
    motors_ctrl(MOT_CTS_UPUP, &install_lr_get);
    // LedTaskCtrl(LED_ID_REMOTE, 1, 0, 750);
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
      sys_ctrl.move_blink = hallCnt;
      return;
    }
  }
  /* run until top side */
  else if (2 == sys_ctrl.sub_sts) {
    if ((sys_ctrl.move_blink > hallCnt) &&
        (sys_ctrl.move_blink - hallCnt > 40)) {
      sys_ctrl.move_blink = hallCnt;
      LedTaskCtrl(LED_ID_REMOTE, 1, 1, 80);
    }

    /* calculate force sampling position */
    if (hallCnt > glb_ctrl.hall_bot)
      index = 0;
    else if (hallCnt < glb_ctrl.hall_top)
      index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
    else
      index = (glb_ctrl.hall_bot - hallCnt) / hallSmp;

    /* normal route overforce judge */
    if (hallCnt > sys_ctrl.hall_dest + HAL_CNT_SLOW_STOP) {
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);

      if (sys_ctrl.delay0 > 0) {
        sys_ctrl.delay0--;

        /* Resistance rebound in starting duration */
        calcForce = glb_ctrl.ufor_max;
        force_res =
            ((calcForce + bk_force[sys_ctrl.force_level][3]) > AD_MAX)
                ? AD_MAX
                : (glb_ctrl.ufor_max + bk_force[sys_ctrl.force_level][3]);

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
        calcForce = sub_maxi_values(glb_ctrl.upForce[index - 1],
                                    glb_ctrl.upForce[index],
                                    glb_ctrl.upForce[index + 1]);
        force_res = ((calcForce + bk_force[sys_ctrl.force_level][4]) > AD_MAX)
                        ? AD_MAX
                        : (calcForce + bk_force[sys_ctrl.force_level][4]);
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
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_SLOW_STOP, 2);
      /* Resistance rebound in slow stop duration */
      if ((sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0)) {

        if (sys_ctrl.delay0 > 0)
          sys_ctrl.delay0--;

        if (sys_ctrl.delay1 > 0)
          sys_ctrl.delay1--;

        calcForce = glb_ctrl.ufor_max;
        force_res =
            (calcForce + bk_force[sys_ctrl.force_level][3]) > AD_MAX
                ? AD_MAX
                : (glb_ctrl.ufor_max + bk_force[sys_ctrl.force_level][3]);

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
        calcForce = sub_maxi_values(glb_ctrl.upForce[index - 1],
                                    glb_ctrl.upForce[index],
                                    glb_ctrl.upForce[index + 1]);
        force_res = (calcForce + bk_force[sys_ctrl.force_level][5]) > AD_MAX
                        ? AD_MAX
                        : (calcForce + bk_force[sys_ctrl.force_level][5]);
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
      sys_ctrl.delay0 = 100;
      return;
    }
  }
  /*------------ normal stop : delay sometime to stop motor -----------*/
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
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
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
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

/**
 * @brief  System excute close
 * @param
 *   none;
 * @retval
 *   none;
 *
 */
VODET sys_excute_close(VODET) {
  U08ET adcValue, infra_status;
  U16ET hallCnt, hallSmp, index, force_res, calcForce;
  LOCAL_COMMAND_LIST remote_command;

  infra_status = getkey_stabe(KEY_INFRA);
  remote_command = remote_hits(&iflearn_get);
  hallCnt = position_get(&install_lr_get, &iflearn_get);
  adcValue = getadc_data(ADC_CH_MOT);
  hallSmp = (glb_ctrl.hall_bot - glb_ctrl.hall_top + UP_FORCE_SAMP - 1) /
            UP_FORCE_SAMP;

  if (hallSmp == 0)
    hallSmp = 1;

  if ((ET_REMOTE_CTL == remote_command) ||
      (KEY_ST_HIGH == getkey_stabe(KEY_PB_WALL))) {
#ifdef EXCUTE_DEBUG
    printf("st_close get stop signal\r\n\r\n");
#endif
    motors_pwms(0);
    sys_ctrl.sub_sts = 3;
    sys_ctrl.delay0 = 100;
    return;
  }

  /* motor direction up */
  if (0 == sys_ctrl.sub_sts) {
#ifdef EXCUTE_DEBUG
    printf("excute close\r\n");
#endif
    motors_ctrl(MOT_CTS_DOWN, &install_lr_get);
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 10;
    return;
  }
  /* start motor power */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sub_clrs_status();
      sys_ctrl.delay0 = 1500;
      sys_ctrl.delay1 = 300;
      sys_ctrl.cur_pwm = PWM_VAL_MIN_ALL;
      sys_ctrl.sub_sts = 2;
      sys_ctrl.move_blink = hallCnt;
      return;
    }
  }
  /* run until top side */
  else if (2 == sys_ctrl.sub_sts) {
    if ((hallCnt > sys_ctrl.move_blink) &&
        (hallCnt - sys_ctrl.move_blink > 40)) {
      sys_ctrl.move_blink = hallCnt;
      LedTaskCtrl(LED_ID_REMOTE, 1, 1, 80);
    }

    /* calculate force sampling position */
    if (hallCnt < glb_ctrl.hall_top)
      index = 0;
    else if (hallCnt > glb_ctrl.hall_bot)
      index = (glb_ctrl.hall_bot - glb_ctrl.hall_top) / hallSmp;
    else
      index = (hallCnt - glb_ctrl.hall_top) / hallSmp;

    /* normal route overforce judge */
    if (hallCnt < sys_ctrl.hall_dest - HAL_CNT_SLOW_STOP) {
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_MAX_RUN, 2);
      if (sys_ctrl.delay0 > 0) {
        sys_ctrl.delay0--;
        /* Resistance rebound in starting duration */
        calcForce = glb_ctrl.dfor_max;
        force_res =
            (calcForce + bk_force[sys_ctrl.force_level][0]) > AD_MAX
                ? AD_MAX
                : (glb_ctrl.dfor_max + bk_force[sys_ctrl.force_level][0]);
        if (adcValue > force_res) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
          sys_ctrl.sub_sts = 0;
          return;
        }
      } else {
        /* Resistance rebound in normal duration */
        calcForce = sub_maxi_values(glb_ctrl.dnForce[index - 1],
                                    glb_ctrl.dnForce[index],
                                    glb_ctrl.dnForce[index + 1]);
        force_res = (calcForce + bk_force[sys_ctrl.force_level][1]) > AD_MAX
                        ? AD_MAX
                        : (calcForce + bk_force[sys_ctrl.force_level][1]);
        if (adcValue > force_res) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
          sys_ctrl.sub_sts = 0;
          return;
        }
      }
    }
    /* stop route overforce judge */
    else {
      sub_sped_raises(&sys_ctrl.cur_pwm, PWM_VAL_SLOW_STOP, 2);
      /* Resistance rebound in sOUT_ST_OFF stop duration */
      if ((sys_ctrl.delay0 > 0) || (sys_ctrl.delay1 > 0)) {

        if (sys_ctrl.delay0 > 0)
          sys_ctrl.delay0--;

        if (sys_ctrl.delay1 > 0)
          sys_ctrl.delay1--;

        calcForce = glb_ctrl.dfor_max;
        force_res =
            (calcForce + bk_force[sys_ctrl.force_level][0]) > AD_MAX
                ? AD_MAX
                : (glb_ctrl.dfor_max + bk_force[sys_ctrl.force_level][0]);

        if (adcValue > force_res) {
          sys_ctrl.sys_sts = SYS_ST_EXCUTE_BACK;
          sys_ctrl.sub_sts = 0;
          return;
        }
      } else {
        calcForce = sub_maxi_values(glb_ctrl.dnForce[index - 1],
                                    glb_ctrl.dnForce[index],
                                    glb_ctrl.dnForce[index + 1]);
        force_res = ((calcForce + bk_force[sys_ctrl.force_level][2]) > AD_MAX)
                        ? AD_MAX
                        : (calcForce + bk_force[sys_ctrl.force_level][2]);

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
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 100;
      return;
    }
  }
  /* delay sometime to stop motor */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
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
      sys_ctrl.lst_dir = MOT_CTS_DOWN;
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
VODET sys_excute_back(VODET) {
  /* stop motor power */
  U16ET hallCnt;

  hallCnt = position_get(&install_lr_get, &iflearn_get);

  if (0 == sys_ctrl.sub_sts) {
#ifdef EXCUTE_DEBUG
    printf("func <back >\r\n");
#endif
    motors_pwms(0);
    sys_ctrl.sub_sts = 1;
    sys_ctrl.delay0 = 250;
  }

  /* stop motor action  */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
      sys_ctrl.sub_sts = 2;
      sys_ctrl.delay0 = 60;
    }
  }

  /* motor direction up */
  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_UPUP, &install_lr_get);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 10;
    }
  }

  /* wait sometime to start power motor */
  else if (3 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
#ifdef EXCUTE_DEBUG
      printf("back to open\r\n\r\n");
#endif
      sub_clrs_status();
      if ((hallCnt - 300 > glb_ctrl.hall_top) && (hallCnt > 300))
        sys_ctrl.hall_dest = hallCnt - 300;
      else
        sys_ctrl.hall_dest = glb_ctrl.hall_top;

      sys_ctrl.delay0 = 700;
      sys_ctrl.delay1 = 300;
      sys_ctrl.cur_pwm = PWM_VAL_DFT_REV;
      sys_ctrl.sys_sts = SYS_ST_EXCUTE_OPEN;
      sys_ctrl.sub_sts = 2;
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
  U08ET key[RBUF_SIZE], index;

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
      LedTaskCtrl(LED_ID_REMOTE, 1, 0, 750);
      return;
    }

    if (KEY_ST_LOW == getkey_stabe(KEY_BUTTON)) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }

  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      remote_clr(CLEAR_ALL);
      LedTaskCtrl(LED_ID_REMOTE, 1, 10, 200);
      sys_ctrl.delay0 = 3800;
      sys_ctrl.sub_sts = 5;
    }

    if (KEY_ST_LOW == getkey_stabe(KEY_BUTTON)) {
      remote_hits(&iflearn_get);
      sys_ctrl.sub_sts = 3;
      sys_ctrl.delay0 = 20000;
    }
  }

  else if (3 == sys_ctrl.sub_sts) {
    if (ET_TRUE == remote_learn(sys_ctrl.rem_key)) {
      LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
      sys_ctrl.sub_sts = 4;
      sys_ctrl.delay0 = 20000;
      return;
    }

    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.err_num = ERR_PROGRAM_OUTTIME;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }

  else if (4 == sys_ctrl.sub_sts) {
    if (ET_TRUE == remote_learn(key)) {
      for (index = 2; index < RBUF_SIZE; index++) {
        if (key[index] != sys_ctrl.rem_key[index])
          break;
      }

      if (RBUF_SIZE == index) {
        sys_ctrl.sub_sts = 5;
        sys_ctrl.delay0 = 800;
        remote_save(key);
        LedTaskCtrl(LED_ID_REMOTE, 1, 4, 80);
        return;
      }
    }

    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
      sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
      sys_ctrl.err_num = ERR_PROGRAM_OUTTIME;
      sys_ctrl.sub_sts = 0;
      return;
    }
  }

  else if (5 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
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
#endif
    motors_pwms(0);
    LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
    sys_ctrl.delay0 = 4;
    sys_ctrl.sub_sts = 1;
  }

  /* safe to stop motor */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
      sys_ctrl.delay0 = 15;
      sys_ctrl.sub_sts = 2;
    }
  }

  /* save current position */
  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      if (ET_TRUE == sys_ctrl.bit_flg.bit0) {
        sub_save_sysctl(0);
#ifdef POWER_OFF_DEBUG
        printf("save suc1\r\n\r\n");
#endif
        
        remote_Write_FlashMemory();
#ifdef POWER_OFF_DEBUG
        printf("save suc2\r\n\r\n");
#endif
      }
      sys_ctrl.delay0 = 1000;
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
  LOCAL_COMMAND_LIST remote_command;

  remote_command = remote_hits(&iflearn_get);
  if (0 == sys_ctrl.sub_sts) {
#ifdef POWER_OFF_DEBUG
    printf("FUNC <sys err >\r\n\r\n");
#endif
    motors_pwms(0);
    LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
    sys_ctrl.delay0 = 5;
    sys_ctrl.sub_sts = 1;
  }
  /* safe to stop motor */
  else if (1 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      motors_ctrl(MOT_CTS_STOP, &install_lr_get);
      sys_ctrl.delay0 = 40;
      sys_ctrl.sub_sts = 2;
    }
  }
  /* save current position */
  else if (2 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      if (ERR_HAL_MAIN == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
        sys_ctrl.sub_sts = 6;
        sys_ctrl.delay0 = 0;
        sys_ctrl.delay1 = 10;
      } else if (ERR_HAL_RFER == sys_ctrl.err_num) {
        sys_ctrl.sub_sts = 4;
      } else if (ERR_INFRA_DURING_TRAINING == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        sys_ctrl.sub_sts = 4;
      } else if (ERR_EXCUTE_WITHOUT_ROUTE == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 1, 2, 500);
        sys_ctrl.delay0 = 1000;
        sys_ctrl.sub_sts = 5;
      } else if (ERR_ROUTE_OUTTIME == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
        glb_ctrl.hall_top = 0;
        glb_ctrl.hall_bot = 0;
        sys_ctrl.sub_sts = 4;
      } else if (ERR_ROUTE_INVALID == sys_ctrl.err_num ||
                 ERR_HAL_LEARN_FAIL == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
        sys_ctrl.sub_sts = 4;
      } else if (ERR_EXOPEN_OVERFORCE == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 1, 3, 500);
        sys_ctrl.sub_sts = 5;
        sys_ctrl.delay0 = 1500;
      } else if (ERR_PROGRAM_OUTTIME == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 1, 4, 500);
        sys_ctrl.sub_sts = 5;
        sys_ctrl.delay0 = 2000;
      } else if (ERR_HALL_DURING_MOVE == sys_ctrl.err_num) {
        LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
        sys_ctrl.sub_sts = 7;
        sys_ctrl.delay0 = 0;
        sys_ctrl.delay1 = 10;
      } else {
        LedTaskCtrl(LED_ID_REMOTE, 0, 0, 0);
        sys_ctrl.sub_sts = 4;
      }
    }
  }
  /* infinity loop */
  else if (3 == sys_ctrl.sub_sts) {

  }

  else if (4 == sys_ctrl.sub_sts) {
    sys_ctrl.sys_sts = SYS_ST_STANDBY;
    sys_ctrl.sub_sts = 0;
  }

  else if (5 == sys_ctrl.sub_sts) {
    sys_ctrl.delay0--;
    if (0 == sys_ctrl.delay0) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
    }
  }

  else if (6 == sys_ctrl.sub_sts) {
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
      LedTaskCtrl(LED_ID_REMOTE, 1, 2, 80);
      sys_ctrl.delay1 = 750;
    }

    if (0 == hall_overflow_cnt()) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
    }
  }

  else if (7 == sys_ctrl.sub_sts) {
    sys_ctrl.delay1--;
    if (0 == sys_ctrl.delay1) {
      LedTaskCtrl(LED_ID_REMOTE, 1, 3, 80);
      sys_ctrl.delay1 = 750;
    }

    if (ET_REMOTE_CTL == remote_command) {
      sys_ctrl.sys_sts = SYS_ST_STANDBY;
      sys_ctrl.sub_sts = 0;
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
#if 0
  U08ET hall_err;
  hall_err = halsig_gerr();
#endif
  /* power off */
  if ((SYS_ST_POWER_OFF != sys_ctrl.sys_sts) &&
      (ET_TRUE == getkey_stabe(KEY_POWER_DOWN))) {

    if ((sys_ctrl.sys_sts >= SYS_ST_LIMITTOP_SET) &&
        (sys_ctrl.sys_sts <= SYS_ST_TRAIN_CLOSE)) {
      glb_ctrl.hall_top = 0;
      glb_ctrl.hall_bot = 0;
    }

    sys_ctrl.sys_sts = SYS_ST_POWER_OFF;
    sys_ctrl.sub_sts = 0;
    return;
  }

  /* state : power off or error process, ignore all */
  if ((SYS_ST_POWER_OFF == sys_ctrl.sys_sts) ||
      (SYS_ST_ERR_PROCESS == sys_ctrl.sys_sts))
    return;

  /* hall_hard_ware err */
  if (hall_overflow_cnt() > 20) {
    sys_ctrl.err_num = ERR_HAL_MAIN;
    sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
    sys_ctrl.sub_sts = 0;
    return;
  }

  /* hall_move err */
  if (ET_TRUE == hall_move_err()) {
    sys_ctrl.err_num = ERR_HALL_DURING_MOVE;
    sys_ctrl.sys_sts = SYS_ST_ERR_PROCESS;
    sys_ctrl.sub_sts = 0;
    return;
  }
}
