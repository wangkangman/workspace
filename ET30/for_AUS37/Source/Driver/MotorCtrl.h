#ifndef _MOTORCTRL_H
#define _MOTORCTRL_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define motor control code                                       */
/*-----------------------------------------------------------------*/
#define MOT_CT_XXX    (0)
#define MOT_CT_UP     (1)
#define MOT_CT_DOWN   (2)
#define MOT_CT_STOP   (3)
#define MOT_CT_MAX    (4)

#define POS_SLOW_STOP     (60)   

/*-----------------------------------------------------------------*/
/* Define motor control parameter                                  */
/*-----------------------------------------------------------------*/
#define POS_VAL_MIN   		(14  )
#define POS_VAL_MAX    		(1010)
#define POS_ERR       		(1088) 

#define TOP_TOLERANCE    	(5 )
#define AUTOCLOSE_TOLERANCE     (10)
#define BOT_TOLERANCE    	(5 )

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET status;

    U16ET delay0;
    U16ET delay1;
}PWM_CTRL;


void motors_init(void);
void motors_ctrl(U08ET ctrls);
void motors_pwms(U08ET duty);
U16ET pos_get(void);
void Motor_PWM_Raise(U08ET *pcur_pwm, U08ET max_pwm, U08ET step);
void Motor_PWM_ctrl_init(void);
U08ET max(U08ET val0, U08ET val1, U08ET val2);
void AbnormalDetective(void);

#endif /* _MOTORS_H */
