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
void sub_clrs_status(void)
{
    sub_ctrl.status = 0;
    sub_ctrl.delay0 = 0;
    sub_ctrl.delay1 = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Save system parameters                                   */
/* PARA :                                                          */
/*   flg0 : 1-force value include   0-just controls                */
/*   fla1 : 1-current position      0-no current position          */
/* RETN :                                                          */
/*   always return ET_TRUE                                         */
/*-----------------------------------------------------------------*/
void sub_save_sysctl(u8 flg0, u8 flg1)
{
    u8 *pSysPara;
    u8 size, index;

    ET_DI();

    if (0 != flg0)
    {
        /* save force parameters */
        for (index = 0; index < UP_FORCE_SAMP / EEP_BLK_SIZE; index++)
        {
            eepblk_writ(index + 2, glb_ctrl.upForce + EEP_BLK_SIZE * index, EEP_BLK_SIZE);
            wdtdog_feed();
        }
        for (index = 0; index < DN_FORCE_SAMP / EEP_BLK_SIZE; index++)
        {
            eepblk_writ(index + 3, glb_ctrl.dnForce + EEP_BLK_SIZE * index, EEP_BLK_SIZE);
            wdtdog_feed();
        }
    }

    /* save the other parameters */
    glb_ctrl.save_flg = EEP_SAVE_FLG;
    if (0 == flg1)
    {
        glb_ctrl.hall_cur = 0;
    }
    else
    {
        glb_ctrl.hall_cur = halsig_gcnt();
    }
    if (0 == glb_ctrl.auto_clo)
    {
        glb_ctrl.just_opn = 0;
    }
    pSysPara = (u8 *)(&(glb_ctrl.save_flg));
    size = (u8 *)(&(glb_ctrl.save_end)) - (u8 *)(&(glb_ctrl.save_flg));
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
void sub_sped_raises(u8 *pcur_pwm, u8 max_pwm, u8 step)
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
            if (*pcur_pwm < (max_pwm - step))
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
void sub_sped_stable(u8 *pcur_pwm, u8 max_pwm, u8 min_pwm, u16 set_duty)
{
    u16 hallDuty;

    hallDuty = halsig_duty();

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

            if (hallDuty > (set_duty + 20))
            {
                if (*pcur_pwm < (max_pwm - 4))
                {
                    *pcur_pwm += 4;
                }
                else
                {
                    *pcur_pwm = max_pwm;
                }
            }
            else if (hallDuty < (set_duty - 20))
            {
                if (*pcur_pwm > min_pwm)
                {
                    *pcur_pwm -= 1;
                }
                else
                {
                    *pcur_pwm = min_pwm;
                }
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
u8 sub_maxi_values(u8 val0, u8 val1, u8 val2)
{
    u8 max;

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

/*-----------------------------------------------------------------*/
/* FUNC : Sub hall verify                                          */
/* PARA :                                                          */
/*   hall : current main hall count                                */
/* RETN :                                                          */
/*   if success, return ET_TRUE, else return ET_FALSE              */
/*-----------------------------------------------------------------*/
u8 sub_hall_verify(u16 hall)
{
    u16 calcHall, difHall;
    float kValue;

    if (hall > glb_ctrl.hsub_oft)
    {
        return ET_TRUE;
    }

    if (ET_TRUE == halsub_gets())
    {
        kValue = (glb_ctrl.hsub_oft - hall) * 1.0 / glb_ctrl.hsub_dif;
        calcHall = glb_ctrl.hsub_oft - ((u16)(kValue + 0.4)) * glb_ctrl.hsub_dif;
        if (glb_ctrl.hsub_lst > calcHall)
        {
            difHall = glb_ctrl.hsub_lst - calcHall;
        }
        else
        {
            difHall = calcHall - glb_ctrl.hsub_lst;
        }
        if (difHall > 2 * glb_ctrl.hsub_dif)
        {
            return ET_FALSE;
        }
        glb_ctrl.hsub_lst = calcHall;
        halsig_scnt(calcHall);
    }

    return ET_TRUE;
}
