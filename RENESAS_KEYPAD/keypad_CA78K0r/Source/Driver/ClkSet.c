#include "regdef.h"
#include "ClkSet.h"

/*-----------------------------------------------------------------*/
/* Local variable define                                           */
/*-----------------------------------------------------------------*/
static ETST_CLKRST et_clkrst;

/*-----------------------------------------------------------------*/
/* FUNC : Clock unit and reset unit initialize                     */
/* PARA :                                                          */
/*   NONE                                                          */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void clkrst_init(void)
{
    /* Clear reset flag by just read the register */
    et_clkrst.rstFlag = RST_RESF;

    /* IO redirection configuration */
    PORT_PIOR = 0x00;

    /* Set PIN8(X1) PIN7(X2) as input port mode */
    CLK_CMC = 0x00;

    /* Stop X1 oscillating circuit */
    CLK_CSC &= ~(1<<7);

    /* Disable low speed internal oscillator */
    CLK_OSMC &= ~(1<<4);

    /* Select high speed internal oscillator as main system clock */
    CLK_CKC &= ~(1<<4);

    /* Enable high speed internal oscillator */
    CLK_CSC &= ~(1<<0);
}

/*-----------------------------------------------------------------*/
/* FUNC : Enable of disable peripheral clock                       */
/* PARA :                                                          */
/*   clkid : clock unit ID                                         */
/*   ctrl  : clock control code                                    */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
void clkset_ctrl(u8 clkid, u8 ctrl)
{
    /* Undefined clock ID */
    if( (clkid<=CLK_ID_XXX) || (clkid>=CLK_ID_MAX) ) {
        return ;
    }

    /* Undefined clock control code */
    if( (ctrl<=CLK_CT_XXX) || (ctrl>=CLK_CT_MAX) ) {
        return ;
    }

    /* Enable of disable peripheral clock */
    switch(clkid) {
        case CLK_ID_TM0:
            if(CLK_CT_DIS == ctrl) {
                CLK_PER0 &= ~(1<<0);
            }
            if(CLK_CT_ENA == ctrl) {
                CLK_PER0 |=  (1<<0);
            }
            break;
        case CLK_ID_UART:
            if(CLK_CT_DIS == ctrl) {
                CLK_PER0 &= ~(1<<2);
            }
            if(CLK_CT_ENA == ctrl) {
                CLK_PER0 |=  (1<<2);
            }
            break;
        case CLK_ID_ADC:
            if(CLK_CT_DIS == ctrl) {
                CLK_PER0 &= ~(1<<5);
            }
            if(CLK_CT_ENA == ctrl) {
                CLK_PER0 |=  (1<<5);
            }
            break;
        default:
            break;
    }
}