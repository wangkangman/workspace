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
}

/*-----------------------------------------------------------------*/
/* FUNC : Save system parameters                                   */
/* PARA :                                                          */
/*   peep : pointer of eeprom control structure                    */
/*   flag : 1-all   0-just controls                                */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
VODET sub_save_sysctl(U08ET flag)
{
    U08ET *pSysPara;
    U08ET size,index;

    ET_DI();

    if (0 != flag)
    {
        /* save force parameters */
        for (index = 0; index < UP_FORCE_SAMP/EEP_BLK_SIZE; index++)
        {
            eepblk_writ(index + 2, glb_ctrl.upForce + EEP_BLK_SIZE*index);
            wdtdog_feed();
        }
        for (index = 0; index<DN_FORCE_SAMP/EEP_BLK_SIZE; index++)
        {
            eepblk_writ(index + 3, glb_ctrl.dnForce + EEP_BLK_SIZE*index);
            wdtdog_feed();
        }
    }

    /* save the other parameters */
    glb_ctrl.save_flg = EEP_SAVE_FLG;
    glb_ctrl.hall_cur = halsig_gcnt();
    pSysPara = (U08ET *)(&(glb_ctrl.save_flg));
    size = (U08ET *)(&(glb_ctrl.save_end)) - (U08ET *)(&(glb_ctrl.save_flg));
    for (index = 0; index < size; index++)
    {
        eepbyt_writ(index, pSysPara[index] );
        wdtdog_feed();
    }

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
    if (0 == sub_ctrl.status)
    {
        sub_ctrl.delay0 = 10;
        sub_ctrl.status = 1;
    }
    else if (1 == sub_ctrl.status)
    {
        sub_ctrl.delay0--;
        if (0 == sub_ctrl.delay0)
        {
            sub_ctrl.delay0 = 10;
            if (*pcur_pwm < max_pwm) 
            {
                *pcur_pwm += step;
            }
            else 
            {
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

    if (0 == sub_ctrl.status) {
        sub_ctrl.delay0 = 10;
        sub_ctrl.status = 1;
    }
    else if (1 == sub_ctrl.status) {
        sub_ctrl.delay0--;
        if (0 == sub_ctrl.delay0) {
            sub_ctrl.delay0 = 10;

            if (hallDuty > (set_duty+20)) {
                *pcur_pwm += 4;
            }
            else if (hallDuty < (set_duty-20)) {
                *pcur_pwm -= 1;
            }

            if (*pcur_pwm > max_pwm) {
                 *pcur_pwm = max_pwm;
            }
            if (*pcur_pwm < min_pwm) {
                 *pcur_pwm = min_pwm;
            }
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
    if (val1 > max)
    {
        max = val1;
    }
    if (val2 > max)
    {
        max = val2;
    }

    return max;
}
