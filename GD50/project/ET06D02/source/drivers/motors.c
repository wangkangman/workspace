#include "regdef.h"
#include "timwdt.h"
#include "motors.h"

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
VODET motors_init(VODET)
{
    /* set control port to stop level as default */
    motors_ctrl(MOT_CTS_STOP);

    /* motor control IO init */
    GPIO_PD_DDR |=  (1<<3);  /* direction output */
    GPIO_PD_CR1 |=  (1<<3);  /* push-pull        */
    GPIO_PD_CR2 &= ~(1<<3);  /* max speed 2MHz   */

    GPIO_PD_DDR |=  (1<<2);  /* direction output */
    GPIO_PD_CR1 |=  (1<<2);  /* push-pull        */
    GPIO_PD_CR2 &= ~(1<<2);  /* max speed 2MHz   */

    /* main hall sampling IO init */
    GPIO_PB_DDR &= ~(1<<5);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<5);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<5);  /* max speed 2MHz   */

    /* subs hall sampling IO init */
    GPIO_PB_DDR &= ~(1<<4);  /* direction input  */
    GPIO_PB_CR1 &= ~(1<<4);  /* float input      */
    GPIO_PB_CR2 &= ~(1<<4);  /* max speed 2MHz   */

    /* motor hall structure init */
    et_hall.main_lst = (GPIO_PB_IDR & 0x20) ? 1:0;
    et_hall.main_cts = (GPIO_PB_IDR & 0x20) ? 1:0;
    et_hall.subs_lst = (GPIO_PB_IDR & 0x10) ? 1:0;
    et_hall.subs_cts = (GPIO_PB_IDR & 0x10) ? 1:0;

    et_hall.hall_cnt = 0;
    et_hall.duty_val = HAL_DTY_MAX;
    et_hall.duty_cnt = HAL_DTY_MAX;
    et_hall.hall_dir = HAL_DIR_UPUP;

    et_hall.herr_max = 40000;
    et_hall.herr_cnt = 0;
    et_hall.herr_ena = ET_FALSE;

    et_hall.hcnt_sub = 0;
    et_hall.hevt_sub = ET_FALSE;
    et_hall.herr_sub = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Control motor up or down and its speed                   */
/* PARA :                                                          */
/*   ctrl : motor control code                                     */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET motors_ctrl(U08ET ctrl)
{
    switch(ctrl) {
        case MOT_CTS_UPUP:
            GPIO_PD_ODR |=  (1<<3);
            GPIO_PD_ODR |=  (1<<2);
            break;
        case MOT_CTS_DOWN:
            GPIO_PD_ODR &= ~(1<<3);
            GPIO_PD_ODR |=  (1<<2);
            break;
        case MOT_CTS_STOP:
            GPIO_PD_ODR &= ~(1<<3);
            GPIO_PD_ODR &= ~(1<<2);
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Control motor pwm signal                                 */
/* PARA :                                                          */
/*   pwms : motor pwm duty                                         */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET motors_pwms(U08ET pwms)
{
    if(0 == pwms) {
        timer2_ctrl(ET_FALSE);
        et_hall.herr_ena = ET_FALSE;
        et_hall.herr_cnt = 0;
    }
    else {
        timer2_ctrl(ET_TRUE);
        et_hall.herr_ena = ET_TRUE;
    }

    timer2_duty(pwms);
}

/*-----------------------------------------------------------------*/
/* FUNC : Motor hall signal sampling                               */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET halsig_samp(VODET)
{
    /* hall signal IO detection */
    et_hall.main_lst = et_hall.main_cts;
    et_hall.main_cts = (GPIO_PB_IDR & 0x20) ? 1:0;
    et_hall.subs_lst = et_hall.subs_cts;
    et_hall.subs_cts = (GPIO_PB_IDR & 0x10) ? 1:0;

    /* hall duty detection */
    if( (0 == et_hall.main_lst) && (1 == et_hall.main_cts) ) {
        et_hall.duty_val = et_hall.duty_cnt;
        et_hall.duty_cnt = 0;
    }
    else {
        if(et_hall.duty_cnt < HAL_DTY_MAX) {
            et_hall.duty_cnt++;
        }
        else {
            et_hall.duty_val = HAL_DTY_MAX;
        }
    }

    /* main hall number and error detection */
    if( et_hall.main_lst != et_hall.main_cts ) {
        et_hall.herr_cnt = 0;

        if( (et_hall.hall_dir == HAL_DIR_UPUP) && (et_hall.hall_cnt > HAL_CNT_MIN) ) {
            et_hall.hall_cnt--;
        }

        if( (et_hall.hall_dir == HAL_DIR_DOWN) && (et_hall.hall_cnt < HAL_CNT_MAX) ) {
            et_hall.hall_cnt++;
        }
    }
    else {
        if( (ET_TRUE == et_hall.herr_ena) && (et_hall.herr_cnt < et_hall.herr_max) ) {
            et_hall.herr_cnt++;
        }
    }

    /* sub hall number and error detection */
    if( (0 == et_hall.subs_lst) && (1 == et_hall.subs_cts) && (HAL_DIR_UPUP == et_hall.hall_dir) ) {
        et_hall.hevt_sub = ET_TRUE;
        if( et_hall.hcnt_sub == et_hall.hall_cnt ) {
            et_hall.herr_sub++;
        }
        else {
            et_hall.herr_sub = 0;
        }
        et_hall.hcnt_sub = et_hall.hall_cnt;
    }
    if( (1 == et_hall.subs_lst) && (0 == et_hall.subs_cts) && (HAL_DIR_DOWN == et_hall.hall_dir) ) {
        et_hall.hevt_sub = ET_TRUE;
        if( et_hall.hcnt_sub == et_hall.hall_cnt ) {
            et_hall.herr_sub++;
        }
        else {
            et_hall.herr_sub = 0;
        }
        et_hall.hcnt_sub = et_hall.hall_cnt;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Set hall count direction                                 */
/* PARA :                                                          */
/*   dir : hall signal count direction, MOT_CT_UP or MOT_CT_DOWN   */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET halsig_sdir(U08ET dir)
{
    et_hall.hall_dir = dir;
}

/*-----------------------------------------------------------------*/
/* FUNC : Set current hall count                                   */
/* PARA :                                                          */
/*   cnt : target hall count value                                 */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET halsig_scnt(U16ET cnt)
{
    et_hall.hall_cnt = cnt;
    et_hall.hcnt_sub = cnt;
}

/*-----------------------------------------------------------------*/
/* FUNC : Get current hall count                                   */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return current hall count value                               */
/*-----------------------------------------------------------------*/
U16ET halsig_gcnt(VODET)
{
    return et_hall.hall_cnt;
}

/*-----------------------------------------------------------------*/
/* FUNC : get hall duty value                                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return current hall duty value                                */
/*-----------------------------------------------------------------*/
U16ET halsig_duty(VODET)
{
    return et_hall.duty_val;
}

/*-----------------------------------------------------------------*/
/* FUNC : Clear sub hall flag                                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET halsub_clrs(VODET)
{
    et_hall.hevt_sub = ET_FALSE;
}

/*-----------------------------------------------------------------*/
/* FUNC : Get sub hall flag                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   return sub hall flag                                          */
/*-----------------------------------------------------------------*/
U08ET halsub_gets(VODET)
{
    if( ET_TRUE == et_hall.hevt_sub ) {
        et_hall.hevt_sub = ET_FALSE;
        return ET_TRUE;
    }
    else {
        return ET_FALSE;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Get hall error state                                     */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   If hall error exist, return ET_TRUE; else return ET_FALSE     */
/*-----------------------------------------------------------------*/
U08ET halsig_gerr(VODET)
{
    S16ET dval_hall;

    if( ET_FALSE == et_hall.herr_ena ) {
        return ERR_HAL_NONE;
    }

    if( et_hall.herr_cnt >= et_hall.herr_max ) {
        et_hall.herr_cnt = 0;
        return ERR_HAL_MAIN;
    }

    dval_hall = (S16ET)(et_hall.hcnt_sub) - (S16ET)(et_hall.hall_cnt);
    if( (dval_hall > 400) || (dval_hall < -400) ) {
        return ERR_HAL_SUBS;
    }

    if(et_hall.herr_sub >= 60) {
        et_hall.herr_sub = 0;
        return ERR_HAL_MUTX;
    }

    return ERR_HAL_NONE;
}
