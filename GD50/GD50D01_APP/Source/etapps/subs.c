#include "main.h"
#include "subs.h"

/**
  * @brief  local variable
  *
  */
static SUB_CTRL sub_ctrl;

/**
  * @brief  Clear sub system control structure
  * @param  NONE
  * @retval NONE
  *
  */
VODET sub_clrs_status(VODET)
{
  sub_ctrl.status = 0;
  sub_ctrl.delay0 = 0;
  sub_ctrl.delay1 = 0;
}

/**
  * @brief  Save system parameters  
  * @param  
  *   peep : pointer of eeprom control structure
  *   flag : 1-all   0-just controls   
  * @retval 
  *   always return ET_TRUE
  *
  */
VODET sub_save_sysctl(U08ET flag)
{
  U32ET *pbuf, size, pflash;

  /* save force data */
  if (flag)
  {
    size = UP_FORCE_SAMP / 4;
    dflash_writ_page((U32ET *)(&glb_ctrl.upForce[0]), DFMC_UPFORCE_OFST, size);
    wdtdog_feed();

    size = DN_FORCE_SAMP / 4;
    dflash_writ_page((U32ET *)(&glb_ctrl.dnForce[0]), DFMC_DNFORCE_OFST, size);
    wdtdog_feed();
  }

  /* save system control */
  size = (OFFSETOF(GLB_CTRL, save_end) + 1) / 4;
  pbuf = (U32ET *)(&glb_ctrl);
  pflash = DFMC_CTRLS_OFST;
  while (size > 128)
  {
    dflash_writ_page(pbuf, pflash, 128);
    size -= 128;
    pbuf += 128;
    pflash += 128;
    wdtdog_feed();
  }
  dflash_writ_page(pbuf, pflash, size);

  wdtdog_feed();
}

/**
  * @brief  Motor speed up
  * @param  
  *   pcur_pwm : current pwm value 
  *   max_pwm  : maximum pwm value
  *   step_pwm : raise step value
  * @retval
  *   always return ET_TRUE
  *
  */
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
        *pcur_pwm += step;
      else
        *pcur_pwm = max_pwm;
    }
  }
}

/**
  * @brief  Motor speed stable
  * @param  
  *   pcur_pwm : current pwm value  
  *   max_pwm  : maximum pwm value
  *   min_pwm  : minimum pwm valuw 
  *   set_duty : target hall duty
  * @retval
  *   always return ET_TRUE
  *
  */
VODET sub_sped_stable(U08ET *pcur_pwm, U08ET max_pwm, U08ET min_pwm, U16ET set_duty)
{
  U16ET hallDuty;
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
        *pcur_pwm += 4;
      else if (hallDuty < (set_duty - 20))
        *pcur_pwm -= 1;

      if (*pcur_pwm > max_pwm)
        *pcur_pwm = max_pwm;
      if (*pcur_pwm < min_pwm)
        *pcur_pwm = min_pwm;
    }
  }
}

/**
  * @brief  Get maximum value of the input parameters
  * @param  
  *   val0 : parameter 0  
  *   val1 : parameter 1
  *   val2 : parameter 2 
  * @retval
  *   return maximum value of the input parameters
  *
  */
U08ET sub_maxi_values(U08ET val0, U08ET val1, U08ET val2)
{
  U08ET max;

  max = val0;
  if (val1 > max)
    max = val1;
  if (val2 > max)
    max = val2;
  return max;
}
