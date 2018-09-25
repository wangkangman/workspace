#include "main.h"
#include "subs.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static SUB_CTRL sub_ctrl;

/*-----------------------------------------------------------------*/
/* FUNC : Clear sub system control structure                       */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sub_clrs_status(VODET)
{
    sub_ctrl.status = 0;

    sub_ctrl.delay0 = 0;
    sub_ctrl.delay1 = 0;

    sub_ctrl.pid_k0 = 0;
    sub_ctrl.pid_k1 = 0;
    sub_ctrl.pid_k2 = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Save system parameters                                   */
/* PARA :                                                          */
/*   flg0 : 1-force value include   0-just controls                */
/*   fla1 : 1-current position      0-no current position          */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
VODET sub_save_sysctl(U08ET flg0, U08ET flg1)
{
    U08ET *pSysPara;
    U08ET size,index;

    ET_DI();

    if( 0 != flg0 ) {
        /* save force parameters */
        for(index=0; index<UP_FORCE_SAMP/EEP_BLK_SIZE; index++) {
            eepblk_writ(index+2, glb_ctrl.upForce+EEP_BLK_SIZE*index, EEP_BLK_SIZE);
        }
        for(index=0; index<DN_FORCE_SAMP/EEP_BLK_SIZE; index++) {
            eepblk_writ(index+3, glb_ctrl.dnForce+EEP_BLK_SIZE*index, EEP_BLK_SIZE);
        }
    }

    /* save the other parameters */
    glb_ctrl.save_flg = EEP_SAVE_FLG;
    if( 0 == flg1 ) {
        glb_ctrl.hall_cur = 0;
    }
    else {
        glb_ctrl.hall_cur = halsig_gcnt();
    }
    pSysPara = (U08ET *)(&(glb_ctrl.save_flg));
    size = (U08ET *)(&(glb_ctrl.save_end)) - (U08ET *)(&(glb_ctrl.save_flg));
    eepblk_writ(0, pSysPara, size);

    ET_EI();
}

/*-----------------------------------------------------------------*/
/* FUNC : Motor speed up                                           */
/* PARA :                                                          */
/*   pcur_pwm : current pwm value                                  */
/*   max_pwm  : maximum pwm value                                  */
/*   step_pwm : raise step value                                   */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
VODET sub_sped_raises(U08ET *pcur_pwm, U08ET max_pwm, U08ET step)
{
    if(0 == sub_ctrl.status) {
        sub_ctrl.delay0 = 10;
        sub_ctrl.status = 1;
    }
    else if(1 == sub_ctrl.status) {
        sub_ctrl.delay0--;
        if(0 == sub_ctrl.delay0) {
            sub_ctrl.delay0 = 10;
            if( *pcur_pwm < (max_pwm-step) ) {
                *pcur_pwm += step;
            }
            else {
                *pcur_pwm = max_pwm;
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Motor speed stable                                       */
/* PARA :                                                          */
/*   pcur_pwm : current pwm value                                  */
/*   max_pwm  : maximum pwm value                                  */
/*   min_pwm  : minimum pwm valuw                                  */
/*   set_duty : target hall duty                                   */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
VODET sub_sped_stable(U08ET *pcur_pwm, U08ET max_pwm, U08ET min_pwm, U16ET set_duty)
{
    U16ET hallDuty;

    hallDuty = halsig_duty();

    if(0 == sub_ctrl.status) {
        sub_ctrl.delay0 = 10;
        sub_ctrl.status = 1;
    }
    else if(1 == sub_ctrl.status) {
        sub_ctrl.delay0--;
        if(0 == sub_ctrl.delay0) {
            sub_ctrl.delay0 = 10;

            if( hallDuty > (set_duty+20) ) {
                if( *pcur_pwm < (max_pwm-4) ) {
                    *pcur_pwm += 4;
                }
                else {
                    *pcur_pwm = max_pwm;
                }
            }
            else if( hallDuty < (set_duty-20) ) {
                if( *pcur_pwm > min_pwm ) {
                    *pcur_pwm -= 1;
                }
                else {
                    *pcur_pwm = min_pwm;
                }
            }
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Motor speed pid control                                  */
/* PARA :                                                          */
/*   pcur_pwm : current pwm value                                  */
/*   max_pwm  : maximum pwm value                                  */
/*   min_pwm  : minimum pwm valuw                                  */
/*   set_duty : target hall duty                                   */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
VODET sub_sped_pidctl(U08ET *pcur_pwm, U08ET max_pwm, U08ET min_pwm, U16ET set_duty)
{
    S16ET pwmDiff;

    if(0 == sub_ctrl.status) {
        sub_ctrl.delay0 = 20;
        sub_ctrl.status = 1;
    }
    else if(1 == sub_ctrl.status) {
        sub_ctrl.delay0--;
        if(0 == sub_ctrl.delay0) {
            sub_ctrl.delay0 = 20;

            sub_ctrl.pid_k0 = set_duty - halsig_duty();
            pwmDiff = 0.2*sub_ctrl.pid_k0 - 0.1*sub_ctrl.pid_k1 + 0.1*sub_ctrl.pid_k2;

            if( pwmDiff > 4 ) {
                pwmDiff = 4;
            }
            if( pwmDiff < -4 ) {
                pwmDiff = -4;
            }

            pwmDiff = *pcur_pwm - pwmDiff;
            if( pwmDiff < min_pwm ) {
                *pcur_pwm = min_pwm;
            }
            else if( pwmDiff > max_pwm ) {
                *pcur_pwm = max_pwm;
            }
            else {
                *pcur_pwm = pwmDiff;
            }

            sub_ctrl.pid_k2 = sub_ctrl.pid_k1;
            sub_ctrl.pid_k1 = sub_ctrl.pid_k0;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Get maximum value of the input parameters                */
/* PARA :                                                          */
/*   val0 : parameter 0                                            */
/*   val1 : parameter 1                                            */
/*   val2 : parameter 2                                            */
/* RETN :                                                          */
/*   return maximum value of the input parameters                  */
/*-----------------------------------------------------------------*/
U08ET sub_maxi_values(U08ET val0, U08ET val1, U08ET val2)
{
    U08ET max;

    max = val0;
    if( val1 > max ) {
        max = val1;
    }
    if( val2 > max ) {
        max = val2;
    }

    return max;
}

/*-----------------------------------------------------------------*/
/* FUNC : Sub hall verify                                          */
/* PARA :                                                          */
/*   hall : current main hall count                                */
/* RETN :                                                          */
/*   if success, return ET_TRUE, else return ET_FALSE              */
/*-----------------------------------------------------------------*/
U08ET sub_hall_verify(U16ET hall)
{
    U16ET calcHall, difHall;
    FLTET kValue;

    if( hall > glb_ctrl.hsub_oft ) {
        return ET_TRUE;
    }

    if( ET_TRUE == halsub_gets() ) {
        kValue = (glb_ctrl.hsub_oft-hall)*1.0/glb_ctrl.hsub_dif;
        calcHall = glb_ctrl.hsub_oft - ((U16ET)(kValue+0.4)) * glb_ctrl.hsub_dif;
        if( glb_ctrl.hsub_lst > calcHall ) {
            difHall = glb_ctrl.hsub_lst - calcHall;
        }
        else {
            difHall = calcHall - glb_ctrl.hsub_lst;
        }
        if( difHall > 2*glb_ctrl.hsub_dif ) {
            return ET_FALSE;
        }
        glb_ctrl.hsub_lst = calcHall;
        halsig_scnt(calcHall);
    }

    return ET_TRUE;
}
