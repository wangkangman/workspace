#ifndef _SUBS_H
#define _SUBS_H

/*--------------------------------------------------------------*/
/* Define sub system control structure                          */
/*--------------------------------------------------------------*/
typedef struct
{
    U08ET status;

    U16ET delay0;
    U16ET delay1;

    S16ET pid_k0;
    S16ET pid_k1;
    S16ET pid_k2;
}SUB_CTRL;

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/
VODET sub_clrs_status(VODET);
VODET sub_save_sysctl(U08ET flg0, U08ET flg1);
VODET sub_sped_raises(U08ET *pcur_pwm, U08ET max_pwm, U08ET step);
VODET sub_sped_stable(U08ET *pcur_pwm, U08ET max_pwm, U08ET min_pwm, U16ET set_duty);
VODET sub_sped_pidctl(U08ET *pcur_pwm, U08ET max_pwm, U08ET min_pwm, U16ET set_duty);
U08ET sub_maxi_values(U08ET val0, U08ET val1, U08ET val2);
U08ET sub_hall_verify(U16ET hall);

#endif /* _SUBS_H */
