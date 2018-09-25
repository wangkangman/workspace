#pragma sfr
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP
/* Start user code for include. Do not edit comment generated here */
#include "main.h"
#include "BuzTask.h"
#include "global.h"
#include "KeyTask.h"
#include "Timer_Wdg.h"
#include "dataflash.h"
#include "SendDataMake.h"
#include "LowPowerConsum.h"
#include "Timer_Wdg.h"
#include "ClkSet.h"
#include "regdef.h"
/* End user code. Do not edit comment generated here */



static SYS_CTRL sys_ctrl;

const FUNC func[SYS_ST_INVALID] = 
{
    sys_power_on,
    sys_read,
    sys_stay_stop,
    sys_pw_change,
    sys_rf_send,
    sys_power_off,
};

//void R_MAIN_UserInit(void);
void main(void)
{
    SysPreInit();
    /* Start user code. Do not edit comment generated here */

    while (1U)
    {
	if (ET_FALSE == tskskm_rdy())
           continue;

	func[sys_ctrl.sys_sts]();
	BUZ_task();
	key_task();
	wdtdog_feed();
    }
    /* End user code. Do not edit comment generated here */
}

void SysPreInit(void)
{
    /* Start user code. Do not edit comment generated here */
    clkrst_init();
    timwdt_init();
    BuzInit();
    key_init();
    LowPowInit();
	//Enable_TM02();
    sys_ctrl.sys_sts = SYS_ST_POWER_ON;
    sys_ctrl.sys_lts = SYS_ST_INVALID;
    sys_ctrl.sub_sts = 0;
    sys_ctrl.sub_lts = 0;
    
    EI();
    /* End user code. Do not edit comment generated here */
}


void sys_power_on(void){
	if (0 == sys_ctrl.sub_sts) {
		sys_ctrl.delay0 = 5;
		sys_ctrl.sub_sts = 1;	
	}
	else if (1 == sys_ctrl.sub_sts) {
		if (sys_ctrl.delay0 > 0)
			--sys_ctrl.delay0;
		else {
			sys_ctrl.sys_sts = SYS_ST_READ;
			sys_ctrl.sub_sts = 0;
		}		
	}
}

void sys_read(void){
	u8 index;
	u8 num = getkey();
	
	if (0 == sys_ctrl.sub_sts) {
		sys_ctrl.delay0 = 5;
		sys_ctrl.sub_sts = 1;
		read_data_from_dataflash((u8*)(&glb_ctr.PassWord), 0x0, (u8)((u8*)(&glb_ctr.EndAddress) - (u8*)(&glb_ctr.PassWord)));
	}
	if (1 == sys_ctrl.sub_sts) {
		if ( 0x4046 != glb_ctr.data_flag) {
			sys_ctrl.sub_sts = 2;
		}
		else {
			sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
			sys_ctrl.sub_sts = 0;
		}
	}
	if (2 == sys_ctrl.sub_sts) {
		if(num != KEY_ID_MAX) {
			BUZ_ctrl(50, 0);
			for (index = 0; index < 4; ++index) {
				glb_ctr.PassWord[0][index] = index + 1;
				glb_ctr.PassWord[1][index] = index + 1;
				glb_ctr.PassWord[2][index] = index + 1;
				glb_ctr.PassWord[3][index] = index + 1;
			}
			glb_ctr.send_counter = 0x0;
			glb_ctr.data_flag = 0x4046;
			sys_ctrl.sub_sts = 3;
		}	
	}
	if (3 == sys_ctrl.sub_sts) {
		write_data_to_dataflash((u8*)(&glb_ctr.PassWord), 0x0, (u8)((u8*)(&glb_ctr.EndAddress) - (u8*)(&glb_ctr.PassWord)));
		sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
		BUZ_ctrl(100, 3);
		sys_ctrl.sub_sts = 0;
	}
}

void sys_stay_stop(void){
	u8 num = getkey();
	if (0 == sys_ctrl.sub_sts){
		sys_ctrl.delay1  = 0;
		sys_ctrl.sub_sts = 1;		
		glb_ctr.sign_in_pw[0] = 20;
		glb_ctr.sign_in_pw[1] = 20;
		glb_ctr.sign_in_pw[2] = 20;
		glb_ctr.sign_in_pw[3] = 20;
		glb_ctr.sign_in_step = 0;
		glb_ctr.channel = 5;
	}
	
	/*********auto power off**********/
	if (num != KEY_ID_MAX){
		sys_ctrl.delay1  = 0;
	}
	if (sys_ctrl.delay1 < AUTO_CLOSE) {
		++sys_ctrl.delay1;
	} else {
		sys_ctrl.sys_sts = SYS_ST_POWER_OFF;
		sys_ctrl.sub_sts = 0;
	}	
	/*********judge if pass word is right then jump to send_rf or change_pw*************/
	if (num == KEY_ID_ENTER || num == KEY_ID_X) { //KEY_ID_ENTER send  @  KEY_ID_X change
		if (string_compare(glb_ctr.PassWord[glb_ctr.channel], glb_ctr.sign_in_pw, 4) \
		    && glb_ctr.sign_in_step == 4 \
		    && glb_ctr.channel < 4){
			BUZ_ctrl(400, 1);
			sys_ctrl.sys_sts = (num == KEY_ID_ENTER )? SYS_ST_RF_SEND : SYS_ST_PW_CHANGE;
			sys_ctrl.sub_sts = 0;
		}
		else {
			BUZ_ctrl(50, 5);
			sys_ctrl.sub_sts = 0;
		}
	}
	/***********set channel **************/
	if (sys_ctrl.sub_sts == 1) {
		if (num < 5 && num > 0) {
			glb_ctr.channel  = num - 1;
			glb_ctr.sign_in_step = 0;
			sys_ctrl.sub_sts = 2;			
		}		
	}
	/***********import pass word**********/
	else if (sys_ctrl.sub_sts == 2) {
		if (num < 10 && num > 0) {
			if (glb_ctr.sign_in_step < 4)
				glb_ctr.sign_in_pw[glb_ctr.sign_in_step] = num;
			++glb_ctr.sign_in_step;
		}
	}
}

void sys_pw_change(void){
	u8 num = getkey();
	if (0 == sys_ctrl.sub_sts) {
		sys_ctrl.delay0 = 20;
		sys_ctrl.sub_sts = 1;
		glb_ctr.sign_in_step = 0;
		sys_ctrl.delay1 = 0;
	}
	else if (1 == sys_ctrl.sub_sts) {
		if (0 == sys_ctrl.delay0)
			sys_ctrl.sub_sts = 2;
		--sys_ctrl.delay0;
	}
	/***************import new_pw once*************/
	else if (2 == sys_ctrl.sub_sts) {
		if (num < 10 && num > 0) {
			if (glb_ctr.sign_in_step < 4)
				glb_ctr.new_pw_1[glb_ctr.sign_in_step] = num;
			++glb_ctr.sign_in_step;
		}
		else if (num == KEY_ID_ENTER) {
			BUZ_ctrl(400, 1);
			sys_ctrl.sub_sts = 3;
			glb_ctr.sign_in_step = 0;		
		}
	}
	/***************import new_pw twice @ judge if new_pws are equal*************/
	else if (3 == sys_ctrl.sub_sts) {
		if (num < 10 && num > 0) {
			if (glb_ctr.sign_in_step < 4)
				glb_ctr.new_pw_2[glb_ctr.sign_in_step] = num;
			++glb_ctr.sign_in_step;
		}
		else if (num == KEY_ID_ENTER) {
			if (string_compare(glb_ctr.new_pw_1, glb_ctr.new_pw_2, 4) \
		    	    && glb_ctr.sign_in_step == 4 \
		    	    && glb_ctr.channel < 4){
				BUZ_ctrl(400, 1);
				glb_ctr.PassWord[glb_ctr.channel][0] = glb_ctr.new_pw_1[0];
				glb_ctr.PassWord[glb_ctr.channel][1] = glb_ctr.new_pw_1[1];
				glb_ctr.PassWord[glb_ctr.channel][2] = glb_ctr.new_pw_1[2];
				glb_ctr.PassWord[glb_ctr.channel][3] = glb_ctr.new_pw_1[3];	
				write_data_to_dataflash((u8*)(&glb_ctr.PassWord), 0x0, (u8)((u8*)(&glb_ctr.EndAddress) - (u8*)(&glb_ctr.PassWord)));
				sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
				sys_ctrl.sub_sts  = 0;				    
			}
			else {
				BUZ_ctrl(50, 5);
				sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
				sys_ctrl.sub_sts  = 0;
			}
		}
	}
	if (num != KEY_ID_MAX){
		sys_ctrl.delay1  = 0;
	}
	if (sys_ctrl.delay1 < AUTO_CLOSE) {
		++sys_ctrl.delay1;
	} else {
		sys_ctrl.sys_sts = SYS_ST_POWER_OFF;
		sys_ctrl.sub_sts = 0;
	}
}

void sys_rf_send(void){
	u8 num = getkey();
	if (0 == sys_ctrl.sub_sts) {
		get_source_code();
		Enable_TM02();
		sys_ctrl.sub_sts = 1;
		sys_ctrl.delay0 = 400;
		sys_ctrl.delay1 = 0;
	}
	else if (1 == sys_ctrl.sub_sts) {
		if (ET_TRUE == Send_Detcet()\
		     || 0 == sys_ctrl.delay0 ){
			    sys_ctrl.sub_sts = 2;
			    Disable_TM02();
		}
		--sys_ctrl.delay0;
	}
	else if (2 == sys_ctrl.sub_sts) {
		if (num == KEY_ID_ENTER) {
			BUZ_ctrl(400, 1);
			get_source_code();
			Enable_TM02();
			sys_ctrl.sub_sts = 3;
			sys_ctrl.delay0 = 400;
		}
		else if (num <= 4) {
			sys_ctrl.delay1  = 0;					
			glb_ctr.sign_in_pw[0] = 20;
			glb_ctr.sign_in_pw[1] = 20;
			glb_ctr.sign_in_pw[2] = 20;
			glb_ctr.sign_in_pw[3] = 20;
			glb_ctr.sign_in_step = 0;
			glb_ctr.channel = num - 1;
			sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
			sys_ctrl.sub_sts = 2;
		}
		else if (num < KEY_ID_MAX) {
			sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
			sys_ctrl.sub_sts  = 0;
		}
	}
	else if (3 == sys_ctrl.sub_sts) {
		if (ET_TRUE == Send_Detcet() || \
		    0 == sys_ctrl.delay0 ){
			sys_ctrl.sys_sts = SYS_ST_STAY_STOP;
			sys_ctrl.sub_sts  = 0;
			Disable_TM02();
		}
		--sys_ctrl.delay0;
	}
	
	
	if (num != KEY_ID_MAX){
		sys_ctrl.delay1  = 0;
	}
	if (sys_ctrl.delay1 < AUTO_CLOSE) {
		++sys_ctrl.delay1;
	} else {
		sys_ctrl.sys_sts = SYS_ST_POWER_OFF;
		sys_ctrl.sub_sts = 0;
	}
}

void sys_power_off(void){
	if (0 == sys_ctrl.sub_sts) {
		write_data_to_dataflash((u8*)(&glb_ctr.PassWord), 0x0, (u8)((u8*)(&glb_ctr.EndAddress) - (u8*)(&glb_ctr.PassWord)));
		sys_ctrl.sub_sts = 1;
		sys_ctrl.delay0 = 10;
	}
	else if (1 == sys_ctrl.sub_sts) {
		if (0 == sys_ctrl.delay0){
			CutPower();
		}
		--sys_ctrl.delay0;
	}
}

u8 string_compare(u8 *a, u8 *b, u8 length){
	u8 index;
	for (index = 0; index < length; ++index) {
		if (a[index] != b[index])
			break;
	}
	if (index == length)
		return 1;
	else
		return 0;
}

