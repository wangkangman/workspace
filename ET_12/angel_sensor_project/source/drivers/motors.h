#ifndef _MOTORS_H
#define _MOTORS_H

#include "global.h"

/*-----------------------------------------------------------------*/
/* Define motor control code                                       */
/*-----------------------------------------------------------------*/
#define MOT_CTS_UPUP    (1)
#define MOT_CTS_DOWN    (2)
#define MOT_CTS_STOP    (3)

/*-----------------------------------------------------------------*/
/* Define hall detection parameter                                 */
/*-----------------------------------------------------------------*/
#define HAL_DIR_UPUP    (1)
#define HAL_DIR_DOWN    (2)

#define HAL_DTY_MAX     (2000)

/*-----------------------------------------------------------------*/
/* Define hall error status                                        */
/*-----------------------------------------------------------------*/
#define ERR_HAL_NONE    (0)
#define ERR_HAL_MAIN    (1)
#define ERR_HAL_SUBS    (2)
#define ERR_HAL_MUTX    (3)

/*-----------------------------------------------------------------*/
/* Define hall detection structure                                 */
/*-----------------------------------------------------------------*/
typedef struct
{
    U16ET duty_val;     /* real time hall duty value */
    U16ET duty_cnt;     /* duty count to calcute duty value */
    U08ET hall_dir;     /* current hall move direction */

    U16ET herr_max;     /* main hall error maximum time count */
    U16ET herr_cnt;     /* main hall error time counter */
    U08ET herr_ena;     /* main hall error enable or not */
}ETST_HALL;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
VODET motors_init(VODET);
VODET motors_ctrl(U08ET ctrl);
VODET motors_pwms(U08ET pwms);
VODET halsig_samp(VODET);
VODET halsig_sdir(U08ET dir);
VODET halsig_scnt(U16ET cnt);
U16ET halsig_gcnt(VODET);
U16ET halsig_duty(VODET);
U08ET halsig_gerr(VODET);


#endif /* _MOTORS_H */
