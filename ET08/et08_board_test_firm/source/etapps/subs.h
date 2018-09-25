#ifndef _SUBS_H
#define _SUBS_H

/*--------------------------------------------------------------*/
/* Define sub system control structure                          */
/*--------------------------------------------------------------*/
typedef struct
{
    u8 status;

    u16 delay0;
    u16 delay1;
} SUB_CTRL;

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/
void sub_clrs_status(void);
void sub_save_sysctl(u8 flg0, u8 flg1);
void sub_sped_raises(u8 *pcur_pwm, u8 max_pwm, u8 step);
void sub_sped_stable(u8 *pcur_pwm, u8 max_pwm, u8 min_pwm, u16 hall_duty);
u8 sub_maxi_values(u8 val0, u8 val1, u8 val2);
u8 sub_hall_verify(u16 hall);

#endif /* _SUBS_H */
