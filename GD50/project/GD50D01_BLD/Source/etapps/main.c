#include <stdio.h>
#include "main.h"
#include "subs.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static SYS_CTRL sys_ctrl;

const FUNC func[SYS_ST_INVALID] = 
{
    sys_wait_evt,
    sys_file_get,
    sys_prog_end,
};

const U08ET led_sts[4][4] = 
{
    {  LED_ST_ON, LED_ST_OFF, LED_ST_OFF, LED_ST_OFF },
    { LED_ST_OFF,  LED_ST_ON, LED_ST_OFF, LED_ST_OFF },
    { LED_ST_OFF, LED_ST_OFF,  LED_ST_ON, LED_ST_OFF },
    { LED_ST_OFF, LED_ST_OFF, LED_ST_OFF,  LED_ST_ON },
};

/*-----------------------------------------------------------------*/
/* FUNC : MAIN function of the project                             */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
INTET main(VODET)
{
    sys_pre_init();

    while(1) {
        if( ET_FALSE == tskskm_flag() ) {
            continue;
        }

        ledseg_task();
        keysmp_task();
        adcsmp_task();

        func[sys_ctrl.sys_sts]();
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : System initialize                                        */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_pre_init(VODET)
{
    sysclk_init();
    ledseg_init();
    adcsmp_init();
    keysmp_init();
    timwdt_init();
    uartct_init();
	
    wdt_reset();
	

    sys_ctrl.sys_sts = SYS_ST_WAIT_EVT;
    sys_ctrl.sub_sts = SYS_ST_SUB0;
    sys_ctrl.delay0 = 100;
}

/*-----------------------------------------------------------------*/
/* FUNC : wait firmware program key event                          */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_wait_evt(VODET)
{
    /* excute time delay */
    if( SYS_ST_SUB0 == sys_ctrl.sub_sts ) {
        sys_ctrl.delay0--;
        if( 0 == sys_ctrl.delay0 ) {
            sys_ctrl.sub_sts = SYS_ST_SUB1;
        }
    }
    /* check if K1 is pressed */
    if( SYS_ST_SUB1 == sys_ctrl.sub_sts ) {
        if( KEY_ST_HIGH == getkey_stabe(KEY_ID_K1) ) {
            sys_ctrl.sub_sts = SYS_ST_SUB2;
            sys_ctrl.delay0 = 500;
        }
        else {
            sys_ctrl.sys_sts = SYS_ST_PROG_END;
            sys_ctrl.sub_sts = SYS_ST_SUB1;
        }
    }
    /* check if K1 keep press for 1 second */
    else if( SYS_ST_SUB2 == sys_ctrl.sub_sts ) {
        sys_ctrl.delay0--;
        if( KEY_ST_LOW == getkey_stabe(KEY_ID_K1) ) {
            sys_ctrl.sys_sts = SYS_ST_PROG_END;
            sys_ctrl.sub_sts = SYS_ST_SUB1;
        }
        if( 0 == sys_ctrl.delay0 ) {
            ledtsk_ctrl(LED_ID_LED1, LED_ST_ON, 0);
            ledtsk_ctrl(LED_ID_LED2, LED_ST_ON, 0);
            ledtsk_ctrl(LED_ID_LED3, LED_ST_ON, 0);
            ledtsk_ctrl(LED_ID_LED4, LED_ST_ON, 0);
            sys_ctrl.sys_sts = SYS_ST_FILE_GET;
            sys_ctrl.sub_sts = SYS_ST_SUB0;
        }
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : get firmware file from programer                         */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
VODET sys_file_get(VODET)
{
    U32ET addr, index;
    U08ET ret;

    if( ET_FALSE == uartct_recv(sys_ctrl.rcv_pkt, 640) ) {
        return ;
    }

    /* excute CRC verify */
    ret = subs_crcs_verify(sys_ctrl.rcv_pkt);
    if( ET_FALSE == ret ) {
        sys_ctrl.snd_pkt[0] = 0xFF;
        sys_ctrl.snd_pkt[1] = PKT_DEV_ID;
        sys_ctrl.snd_pkt[2] = 0x00;
        sys_ctrl.snd_pkt[3] = 0x07;
        sys_ctrl.snd_pkt[4] = 0x00;
        sys_ctrl.snd_pkt[5] = 0x00;
        sys_ctrl.snd_pkt[6] = PKT_ERR_CRC;
        subs_crcs_fillin(sys_ctrl.snd_pkt, 7);
        uartct_send(sys_ctrl.snd_pkt, 7);
        return ;
    }

    /* excute device ID verify */
    if( PKT_DEV_ID != sys_ctrl.rcv_pkt[PKT_DEV_IDX] ) {
        sys_ctrl.snd_pkt[0] = 0xFF;
        sys_ctrl.snd_pkt[1] = PKT_DEV_ID;
        sys_ctrl.snd_pkt[2] = 0x00;
        sys_ctrl.snd_pkt[3] = 0x07;
        sys_ctrl.snd_pkt[4] = 0x00;
        sys_ctrl.snd_pkt[5] = 0x00;
        sys_ctrl.snd_pkt[6] = PKT_ERR_DEV;
        subs_crcs_fillin(sys_ctrl.snd_pkt, 7);
        uartct_send(sys_ctrl.snd_pkt, 7);
        return ;
    }

    sys_ctrl.dis_sts = (sys_ctrl.dis_sts+1)%4;
    ledtsk_ctrl(LED_ID_LED1, led_sts[sys_ctrl.dis_sts][0], 0);
    ledtsk_ctrl(LED_ID_LED2, led_sts[sys_ctrl.dis_sts][1], 0);
    ledtsk_ctrl(LED_ID_LED3, led_sts[sys_ctrl.dis_sts][2], 0);
    ledtsk_ctrl(LED_ID_LED4, led_sts[sys_ctrl.dis_sts][3], 0);

    switch(sys_ctrl.rcv_pkt[6]) {
        case PKT_CMD_STA:
            sys_ctrl.snd_pkt[0] = 0xFF;
            sys_ctrl.snd_pkt[1] = PKT_DEV_ID;
            sys_ctrl.snd_pkt[2] = 0x00;
            sys_ctrl.snd_pkt[3] = 0x07;
            sys_ctrl.snd_pkt[4] = 0x00;
            sys_ctrl.snd_pkt[5] = 0x00;
            sys_ctrl.snd_pkt[6] = PKT_ACK_STA;
            subs_crcs_fillin(sys_ctrl.snd_pkt, 7);
            uartct_send(sys_ctrl.snd_pkt, 7);
            break;
        case PKT_CMD_PKT:
            sys_ctrl.snd_pkt[0] = 0xFF;
            sys_ctrl.snd_pkt[1] = PKT_DEV_ID;
            sys_ctrl.snd_pkt[2] = 0x00;
            sys_ctrl.snd_pkt[3] = 0x07;
            sys_ctrl.snd_pkt[4] = 0x00;
            sys_ctrl.snd_pkt[5] = 0x00;

            addr = (sys_ctrl.rcv_pkt[7]<<24) + (sys_ctrl.rcv_pkt[8]<<16) + (sys_ctrl.rcv_pkt[9]<<8) + (sys_ctrl.rcv_pkt[10]<<0);
            if( (addr < 0x2000) || (addr > 0x8000) || (0 != addr%0x200) ) {
                sys_ctrl.snd_pkt[6] = PKT_ERR_PAR;
            }
            else {
                for(index=0; index<512; index++) {
                    sys_ctrl.rcv_pkt[index] = sys_ctrl.rcv_pkt[index+11];
                }
                flashs_write(FLASH_TYPE_APROM, addr, (U32ET *)(sys_ctrl.rcv_pkt), 512);
                sys_ctrl.snd_pkt[6] = PKT_ACK_PKT;
            }

            subs_crcs_fillin(sys_ctrl.snd_pkt, 7);
            uartct_send(sys_ctrl.snd_pkt, 7);
            break;
        case PKT_CMD_END:
            sys_ctrl.snd_pkt[0] = 0xFF;
            sys_ctrl.snd_pkt[1] = PKT_DEV_ID;
            sys_ctrl.snd_pkt[2] = 0x00;
            sys_ctrl.snd_pkt[3] = 0x07;
            sys_ctrl.snd_pkt[4] = 0x00;
            sys_ctrl.snd_pkt[5] = 0x00;
            if( ET_TRUE == subs_file_verify() ) {
                sys_ctrl.snd_pkt[6] = PKT_ACK_END;
                subs_crcs_fillin(sys_ctrl.snd_pkt, 7);
                uartct_send(sys_ctrl.snd_pkt, 7);
                sys_ctrl.sys_sts = SYS_ST_PROG_END;
                sys_ctrl.sub_sts = SYS_ST_SUB0;
                sys_ctrl.delay0 = 500;
            }
            else {
                sys_ctrl.snd_pkt[6] = PKT_ERR_SUM;
                subs_crcs_fillin(sys_ctrl.snd_pkt, 7);
                uartct_send(sys_ctrl.snd_pkt, 7);
            }
            break;
        default:
            sys_ctrl.snd_pkt[0] = 0xFF;
            sys_ctrl.snd_pkt[1] = PKT_DEV_ID;
            sys_ctrl.snd_pkt[2] = 0x00;
            sys_ctrl.snd_pkt[3] = 0x07;
            sys_ctrl.snd_pkt[4] = 0x00;
            sys_ctrl.snd_pkt[5] = 0x00;
            sys_ctrl.snd_pkt[6] = PKT_ERR_CMD;
            subs_crcs_fillin(sys_ctrl.snd_pkt, 7);
            uartct_send(sys_ctrl.snd_pkt, 7);
            break;
    }

}

/*-----------------------------------------------------------------*/
/* FUNC : firmware program finished                                */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
__asm stack_remap(U32ET _sp)
{
    MSR MSP, r0
    BX lr
}

VODET sys_prog_end(VODET)
{
    volatile U32ET spVal;
    FUNC_PTR *func;

    if( SYS_ST_SUB0 == sys_ctrl.sub_sts ) {
        sys_ctrl.delay0--;
        if( 0 == sys_ctrl.delay0 ) {
            sys_ctrl.sub_sts = SYS_ST_SUB1;
        }
    }
    else if ( SYS_ST_SUB1 == sys_ctrl.sub_sts ) {
        uartct_stop();
        timwdt_stop();
        vector_remap(0x00002000);
			
			#if 1
        spVal = *(volatile U32ET *)(0x00002000);
        stack_remap(spVal);
        func = (FUNC_PTR *)(*(U32ET *)(0x2004));
        func();
			#endif
    }
}
