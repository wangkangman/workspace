#ifndef _SUBS_H
#define _SUBS_H

/*--------------------------------------------------------------*/
/* Function declare                                             */
/*--------------------------------------------------------------*/
U16ET subs_calc_crcrst(U08ET *pbuf, U16ET size);
U08ET subs_crcs_verify(U08ET *pbuf);
VODET subs_crcs_fillin(U08ET *pbuf, U16ET size);
U08ET subs_file_verify(VODET);


/*-----------------------------------------------------------------*/
/* Define SUB system control structure                             */
/*-----------------------------------------------------------------*/
typedef struct
{
    U08ET buff[512];
}SUB_CTRL;

#endif /* _SUBS_H */
