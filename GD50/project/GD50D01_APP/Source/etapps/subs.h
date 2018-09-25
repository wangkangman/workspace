#ifndef _SUBS_H
#define _SUBS_H

/**
  * @brief  Define sub system control structure 
  */
typedef struct
{
  U08ET status;

  U16ET delay0;
  U16ET delay1;
} SUB_CTRL;
/**
  * @brief  Function declare
  */
VODET sub_clrs_status(VODET);
VODET sub_save_sysctl(U08ET flag);
VODET sub_sped_raises(U08ET *pcur_pwm, U08ET max_pwm, U08ET step);
VODET sub_sped_stable(U08ET *pcur_pwm, U08ET max_pwm, U08ET min_pwm, U16ET hall_duty);
U08ET sub_maxi_values(U08ET val0, U08ET val1, U08ET val2);

#endif /* _SUBS_H */
