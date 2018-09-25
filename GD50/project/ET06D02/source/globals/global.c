#include "global.h"

/*-----------------------------------------------------------------*/
/* Global variable define                                          */
/*-----------------------------------------------------------------*/
GLB_CTRL glb_ctrl;
GLB_CFGS glb_cfgs;

/*-----------------------------------------------------------------*/
/* FUNC : Initialization global configuration                      */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET glbcfg_init(VODET)
{
    U08ET *pbuf;
    U16ET size, index;

    /* read configuration area */
    pbuf = (U08ET *)(&glb_cfgs.flg_cfgs);
    size = (U08ET *)(&glb_cfgs.end_cfgs) - (U08ET *)(&glb_cfgs.flg_cfgs);
    for(index=0; index<size; index++) {
        pbuf[index] = *((volatile U08ET *)(0x0000FF00+index));
    }

    /* default configuration table */
    if( 0x5A != glb_cfgs.flg_cfgs ) {
        glb_cfgs.prg_vers = 0;
        glb_cfgs.rmt_type = 0;
        glb_cfgs.clo_auto = 60;
        glb_cfgs.siz_ropn = 0;
    }

    /* software version */
    if( glb_cfgs.prg_vers > 99 ) {
        glb_cfgs.prg_vers = 0;
    }
    /* remote controller type */
    if( glb_cfgs.rmt_type > 3 ) {
        glb_cfgs.rmt_type = 0;
    }
    /* auto close seconds */
    if( 0 == glb_cfgs.clo_auto ) {
        glb_cfgs.clo_auto = 60;
    }
    /* resistance level */
    if( (0 == glb_cfgs.max_levl) || (glb_cfgs.max_levl > 9) ) {
        glb_cfgs.max_levl = 9;
        glb_cfgs.dft_levl = 3;
    }
}

