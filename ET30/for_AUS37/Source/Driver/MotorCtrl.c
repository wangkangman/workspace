#include "regdef.h"
#include "Timer_Wdg.h"
#include "MotorCtrl.h"
#include "AdcFunc.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static PWM_CTRL pwm_ctrl;



/*-----------------------------------------------------------------*/
/* FUNC : Motor control initialize                                 */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void motors_init(void)
{
    /* set control port to default low */
    PORT_PD6 &= ~(1<<0);
    PORT_PD6 &= ~(1<<1);

    /* motor control IO init */
    PORT_PM6 &= ~(1<<0);  /* direction output */
    PORT_PM6 &= ~(1<<1);  /* direction output */

    /* hall signal IO init */
    /* P121 only can be digtal input */
}

/*-----------------------------------------------------------------*/
/* FUNC : Control motor up or down and its speed                   */
/* PARA :                                                          */
/*   ctrls : motor control code                                    */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void motors_ctrl(U08ET ctrls)
{
	
    /* undefined motor control code */
    if( (ctrls<=MOT_CT_XXX) || (ctrls>=MOT_CT_MAX) ) {
        return ;
    }
    if (1 == glb_ctrl.LR_set) {
    	if(MOT_CT_UP == ctrls)
		ctrls = MOT_CT_DOWN;
        else if (MOT_CT_DOWN == ctrls)
		ctrls = MOT_CT_UP;
    }
    
    /* control motor up or down or stop */
    switch(ctrls) {
        case MOT_CT_DOWN:
            PORT_PD6 &= ~(1<<0);
            PORT_PD6 |=  (1<<1);
            break;
        case MOT_CT_UP:
            PORT_PD6 |=  (1<<0);
            PORT_PD6 &= ~(1<<1);
            break;
        case MOT_CT_STOP:
            PORT_PD6 &= ~(1<<0);
            PORT_PD6 &= ~(1<<1);
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Control motor pwm signal                                 */
/* PARA :                                                          */
/*   duty : motor pwm duty                                         */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void motors_pwms(U08ET duty)
{
    if(0 == duty) {
        pwmctl_ctrl(ET_FALSE);
        pwmctl_duty(0);
    }
    else {
        pwmctl_ctrl(ET_TRUE);
        pwmctl_duty(duty);
    }
}
/*-----------------------------------------------------------------*/
/* FUNC : position get                                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   pos val                                                       */
/*-----------------------------------------------------------------*/
U16ET pos_get(void)
{
    if (1 == glb_ctrl.LR_set) {
    	return 1024 - getadc_data(ADC_CH_POS);
    }
    return getadc_data(ADC_CH_POS);
}

/*-----------------------------------------------------------------*/
/* FUNC : Clear PWM control structure                       */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void Motor_PWM_ctrl_init(void){
    pwm_ctrl.status = 0;
    pwm_ctrl.delay0 = 0;
    pwm_ctrl.delay1 = 0;
}

/*-----------------------------------------------------------------*/
/* FUNC : Motor speed up                                           */
/* PARA :                                                          */
/*   pcur_pwm : current pwm value                                  */
/*   max_pwm  : maximum pwm value                                  */
/*   step_pwm : raise step value                                   */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void Motor_PWM_Raise(U08ET *pcur_pwm, U08ET max_pwm, U08ET step){
    if (0 == pwm_ctrl.status)
    {
        pwm_ctrl.delay0 = 10;
        pwm_ctrl.status = 1;
    }
    else if (1 == pwm_ctrl.status)
    {
        pwm_ctrl.delay0--;
        if (0 == pwm_ctrl.delay0)
        {
            pwm_ctrl.delay0 = 10;
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
/* FUNC : get max val
/* PARA : 
/*   val0 : U08ET val 
/*   val1 : U08ET val
/*   val2 : U08ET val 
/* RETN : 
/*   U08ET max val 
/*-----------------------------------------------------------------*/
U08ET max(U08ET val0, U08ET val1, U08ET val2)
{
    U08ET max;
    max = val0 > val1 ? val0 : val1;
    max = max > val2 ? max : val2;
    return max;
}

