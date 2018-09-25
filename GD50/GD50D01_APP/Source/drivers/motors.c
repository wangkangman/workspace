#include "regdef.h"
#include "motors.h"
#include "stdio.h"
/**
  ******************************************************************************
  * @file    motor.c
  * @author  ETdoor team
  * @version V0.0.0
  * @date    2018/4/13
  * @brief   This file provides all application of motor and hall, 
  *          functionalities of the inital and listen for uart:
  *           + hardware setting & irq function,
  *           + hall val calculation.
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
static ETST_HALL et_hall;

/*-----------------------------------------------------------------*/
/* FUNC : Motor control initialize                                 */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET motors_init(VODET) {
  /* set control port to stop level as default */
  motors_ctrl(MOT_CTS_STOP);
  /* PWM3  <--> P4.3 */
  GCR_P4MFP |= ((U32ET)1 << 3);
  GCR_P4MFP &= ~((U32ET)1 << 11);
  /* select HCLK as PWM23 clock source */
  CLK_SEL1 |= ((U32ET)2 << 30);
  /* enable PWM23 clock */
  CLK_APB |= ((U32ET)1 << 21);
  /* set PWM23 clock pre divider to 11 */
  PWMA_PPR &= 0x00ff;
  PWMA_PPR |= ((U32ET)10 << 8);
  /* set PWM3 clock divider to 1  [14:12]=100 */
  PWMA_CSR &= ~((U32ET)0x7 << 12);
  PWMA_CSR |= ((U32ET)0x4 << 12);
  /* set PWM3 to auto reload mode and disable inverter */
  PWMA_PCR &= ~((U32ET)0x7 << 25);
  PWMA_PCR |= ((U32ET)0x4 << 25);
  /* set PWM3 cycle count to 250 */
  PWMA_CNR3 = 250;
  /* set PWM3 default duty to 0 */
  PWMA_CMR3 = 0;
  /* PWM3 irq disable */
  PWMA_PIER &= ~((U32ET)0x1 << 3);
  PWMA_PIER &= ~((U32ET)0x1 << 11);
  PWMA_PIIR &= ~((U32ET)0x1 << 3);
  PWMA_PIIR &= ~((U32ET)0x1 << 11);
  /* motor control M00+ <--> P2.2 */
  GPIO_P2_PMD |= ((U32ET)1 << 4);
  GPIO_P2_PMD &= ~((U32ET)1 << 5);
  /* motor control M00- <--> P2.3 */
  GPIO_P2_PMD |= ((U32ET)1 << 6);
  GPIO_P2_PMD &= ~((U32ET)1 << 7);
  /* main hall <--> P3.6 */
  GPIO_P3_PMD &= ~((U32ET)1 << 12);
  GPIO_P3_PMD &= ~((U32ET)1 << 13);
  /* subs hall <--> P3.7 */
  GPIO_P3_PMD &= ~((U32ET)1 << 14);
  GPIO_P3_PMD &= ~((U32ET)1 << 15);
  /* motor hall structure init */
  et_hall.main_lst = PORT_READ(ET_PORT3, 6);
  et_hall.main_cur = PORT_READ(ET_PORT3, 6);
  et_hall.rfer_lst = PORT_READ(ET_PORT3, 7);
  et_hall.rfer_cur = PORT_READ(ET_PORT3, 7);
  et_hall.hall_cnt = 0;
  et_hall.duty_val = HAL_DTY_MAX;
  et_hall.duty_cnt = HAL_DTY_MAX;
  et_hall.hall_dir = HAL_DIR_UPUP;
  et_hall.mainerr_max = 40000;
  et_hall.mainerr_cnt = 0;
  et_hall.rfererr_max = 200;
  et_hall.rfererr_cnt = 0;
  et_hall.errcnt_ena = ET_FALSE;
  /* start PWM3 */
  PWMA_PCR |= (1 << 24);
  /* enable PWM3 output */
  PWMA_POE |= ((U32ET)1 << 3);
}

/**
  * @brief  Control motor up or down and its speed 
  * @param  
  *    ctrl : motor control code
  * @retval NONE
  *
  */
VODET motors_ctrl(U08ET ctrl) {
  switch (ctrl) {
  case MOT_CTS_DOWN: //M+ = 0; M- = 1
    PORT_SET(ET_PORT2, 2, LOW);
    PORT_SET(ET_PORT2, 3, HIGH);
    break;
  case MOT_CTS_UPUP:
    PORT_SET(ET_PORT2, 2, HIGH);
    PORT_SET(ET_PORT2, 3, HIGH);
    break;
  case MOT_CTS_STOP:
    PORT_SET(ET_PORT2, 3, LOW);
    break;
  default:
    break;
  }
}

/**
  * @brief  Control motor pwm signal 
  * @param  
  *    pwms : motor pwm duty 
  * @retval NONE
  *
  */
VODET motors_pwms(U08ET pwms) {  
  pwms = (pwms > 250) ? 250 : pwms;  
  if (0 == pwms) {
    et_hall.errcnt_ena = ET_FALSE;
    et_hall.mainerr_cnt = 0;
    et_hall.rfererr_cnt = 0;
  }
  else et_hall.errcnt_ena = ET_TRUE;
  PWMA_CMR3 = pwms;
}

/**
  * @brief  Motor hall signal sampling 
  * @param  NONE
  * @retval NONE
  *
  */
VODET halsig_samp(VODET) {
  U08ET mod;
  /* hall signal IO detection */
  et_hall.main_lst = et_hall.main_cur;
  et_hall.main_cur = PORT_READ(ET_PORT3, 6);
  et_hall.rfer_lst = et_hall.rfer_cur;
  et_hall.rfer_cur = PORT_READ(ET_PORT3, 7);
  /* hall duty detection and main hall cnt calculate  and main err count*/
  if (((HAL_DIR_UPUP == et_hall.hall_dir) && (0 == et_hall.main_lst && 1 == et_hall.main_cur)) 
     || ((HAL_DIR_DOWN == et_hall.hall_dir) && (1 == et_hall.main_lst && 0 == et_hall.main_cur))) {
    /* main duty calculate */
    et_hall.duty_val = et_hall.duty_cnt;
    et_hall.duty_cnt = 0;
    /*  main err count*/
    et_hall.mainerr_cnt = 0;
    /*  main cnt calculate */
    if ((et_hall.hall_dir == HAL_DIR_UPUP) 
       && (et_hall.hall_cnt > HAL_CNT_MIN))
      et_hall.hall_cnt--;
    
    if ((et_hall.hall_dir == HAL_DIR_DOWN) 
       && (et_hall.hall_cnt < HAL_CNT_MAX))
      et_hall.hall_cnt++;
    
    /*  rfer err count */
    if ((ET_TRUE == et_hall.errcnt_ena) 
       && (et_hall.rfererr_cnt < et_hall.rfererr_max))
      et_hall.rfererr_cnt++;
  } else {
    /* hall duty calculate */
    if (et_hall.duty_cnt < HAL_DTY_MAX) 
      et_hall.duty_cnt++;
    else et_hall.duty_val = HAL_DTY_MAX;
    
    /*  mian err count */
    if ((ET_TRUE == et_hall.errcnt_ena) 
       && (et_hall.mainerr_cnt < et_hall.mainerr_max))
      et_hall.mainerr_cnt++;
  }
  /* main hall regulate and err detect */
  if (0 == et_hall.rfer_lst && 1 == et_hall.rfer_cur) {
    /* rfer hall err detective */
    et_hall.rfererr_cnt = 0;
    if (HAL_MOD_NOR == et_hall.hall_mode) {
      /* main hall regulate*/
      mod = (et_hall.hall_cnt % et_hall.hall_ratio);
      if (mod > (et_hall.hall_ratio / 2))
        et_hall.hall_cnt = et_hall.hall_cnt 
                           - mod 
                           + et_hall.hall_ratio;
      else et_hall.hall_cnt = et_hall.hall_cnt - mod;
    }
    else if (HAL_MOD_LEARN == et_hall.hall_mode) {
      if (et_hall.hall_learn_index < HAL_LEARN_LEN) {
        et_hall.hall_learn_cnt[et_hall.hall_learn_index] 
                = et_hall.hall_cnt;
        et_hall.hall_learn_index++;
      }
    }
  }
}

/**
  * @brief  Set hall count direction
  * @param
  *   dir : hall signal count direction, MOT_CT_UP or MOT_CT_DOWN
  * @retval NONE
  *
  */
VODET halsig_sdir(U08ET dir) {
  et_hall.hall_dir = dir;
}

/**
  * @brief  Set current hall count
  * @param 
  *   cnt : target hall count value 
  * @retval NONE
  *
  */
VODET halsig_scnt(U16ET cnt) {
  et_hall.hall_cnt = cnt;
}

/**
  * @brief  Get current hall count
  * @param  NONE
  * @retval 
  *    return current hall count value 
  *
  */
U16ET halsig_gcnt(VODET) {
  return et_hall.hall_cnt;
}

/**
  * @brief  Set the ratio of main hall and refer hall
  * @param  NONE
  * @retval NONE
  *
  */
VODET halsig_ratio(U08ET ratio) {
  et_hall.hall_ratio = ratio;
}

/**
  * @brief  get hall duty value
  * @param  NONE
  * @retval 
  *   return current hall duty value  
  *
  */
U16ET halsig_duty(VODET) {
  return et_hall.duty_val;
}

/**
  * @brief  set hall count mode
  * @param  
  *   mode : new mode
  * @retval NONE
  *
  */
VODET halsig_mode(U08ET mode)
{
  et_hall.hall_mode = mode;
  
  if (HAL_MOD_LEARN == mode)
    et_hall.hall_learn_index = 0;
}

/**
  * @brief  Get hall error state
  * @param  NONE
  * @retval 
  *   If hall error exist, return ET_TRUE; else return ET_FALSE
  *
  */
U08ET halsig_gerr(VODET) {
  
  if (ET_FALSE == et_hall.errcnt_ena)
    return ERR_HAL_NONE;
  
  if (et_hall.mainerr_cnt >= et_hall.mainerr_max) {
    et_hall.mainerr_cnt = 0;
    return ERR_HAL_MAIN;
  }
  
  if (et_hall.rfererr_cnt >= et_hall.rfererr_max) {
    et_hall.rfererr_cnt = 0;
    return ERR_HAL_RFER;
  }
  
  return ERR_HAL_NONE;
}

/**
  * @brief  calculate the ratio and skew of hall;
  * @param  NONE;
  * @retval 
  *   st_ETST_HALL_LEARN: include ratio and skew information;
  *
  */
ETST_HALL_LEARN hall_learn(VODET)
{
  ETST_HALL_LEARN learn_st;
  U08ET index, max, min, temp;
  U16ET sum;
#ifdef TRAIN_DEBUG
  printf("learn data :\r\n");
  for (index = 0
		  ; index < et_hall.hall_learn_index
	    ; index++)
    printf("%d\r\n", (U16ET)et_hall.hall_learn_cnt[index]);
	
  printf("learn data over!!\r\n");
#endif
  /* capture data error */
  if (et_hall.hall_learn_index < 4) {
    learn_st.ratio = 0;
    learn_st.skew = 0;
    return learn_st;
  }
    
  /* calculate the ratio between main hall and refer hall */
  for (max = 0x0, min = 0xff, sum = 0, index = 0
      ; index < et_hall.hall_learn_index - 1
      ; index++) {
    temp = et_hall.hall_learn_cnt[index]
            - et_hall.hall_learn_cnt[index + 1];
    sum += temp;
    max = temp > max ? temp : max;
    min = temp < min ? temp : min;
  }
  sum -= max;
  sum -= min;
  learn_st.ratio = sum / (et_hall.hall_learn_index - 3);
  /* calculate the skew between main hall and refer hall */
  for (max = 0x0, min = 0xff, sum = 0, index = 0
      ; index < et_hall.hall_learn_index
      ; index++) {
    temp = et_hall.hall_learn_cnt[index]
           % (U16ET)learn_st.ratio;
    sum += temp;
    max = temp > max ? temp : max;
    min = temp < min ? temp : min;
  }
  sum -= max;
  sum -= min;
  learn_st.skew = sum / (et_hall.hall_learn_index - 2);
  return learn_st;
}
