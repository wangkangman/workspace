#include "usmart.h"
#include "usmart_str.h"
#include "main.h"

/**
  ******************************************************************************
  * @file    usmart.c
  * @author  Kimi
  * @version V0.0.1
  * @date    2018Äê3ÔÂ25ÈÕ 10:24:58
  * @brief    This file provide interface for user, to config the functions of 
  *           usmart.         
  *           + user can config the functions which should be call. 
  *           
  *           
  ******************************************************************************
  * @attention
  *           Must include the headfile of the function which you called.
  *           
  *
  * 
  ******************************************************************************
  */

extern void led_set(u8 sta);
extern void test_fun(void (*ledset)(u8), u8 sta);

/**
  * @brief  config the functions and func string.
  *
  */
struct _m_usmart_nametab usmart_nametab[] =
    {
#if USMART_USE_WRFUNS == 1 //if r/w enable
        (void *)read_addr,
        "u32 read_addr(u32 addr)",
        (void *)write_addr,
        "void write_addr(u32 addr,u32 val)",
#endif
				(void *)usmart_getcurrent,
				"u32 usmart_getcurrent(void)",
				(void *)usmart_getkeyad,
        "u32 usmart_getkeyad(void)",
        (void *)usmart_segctrl,
        "void usmart_segctrl(u32 digit, u32 piont)",
        (void *)usmart_motorctrl,
        "void usmart_motorctrl(u32 dir)",
        (void *)usmart_pwmsctrl,
        "void usmart_pwmsctrl(u32 pwmval)",
        (void *)usmart_remotekey,
        "void usmart_remotekey(void)",
        (void *)usmart_infra_sta,
        "u32 usmart_infra_sta(void)",
        (void *)usmart_wall_sta,
        "u32 usmart_wall_sta(void)",
        (void *)usmart_door_sta,
        "u32 usmart_door_sta(void)",
        (void *)usmart_power_sta,
        "u32 usmart_power_sta(void)",
        (void *)usmart_main_hall_sta,
        "u32 usmart_main_hall_sta(void)",
        (void *)usmart_ref_hall_sta,
        "u32 usmart_ref_hall_sta(void)",
};

/**
  * @brief  module func init.
  *
  */
struct _m_usmart_dev usmart_dev =
    {
        usmart_nametab,
        usmart_init,
        usmart_cmd_rec,
        usmart_exe,
        usmart_scan,
        usmart_task,
        sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), //amount of fucntions
        0,                                                         //amount of parameters
        0,                                                         //func ID
        1,                                                         //display : 0-dex. 1-hex.
        0,                                                         //parameter type 0-digit 1-string
        0,                                                         //length of parameter, temp.
        0,                                                         //parameter of function
};
