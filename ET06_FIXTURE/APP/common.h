#ifndef __COMMON_H
#define __COMMON_H 	
#include "sys.h"
#include "includes.h"
#include "appET08_test.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//APP通用 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/2/16
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//V1.1 20140216
//新增对各种分辨率LCD的支持.
//V1.2 20140727
//修改app_show_float函数的一个bug
////////////////////////////////////////////////////////////////////////////////// 	   

//硬件平台软硬件版本定义	   	
#define HARDWARE_VERSION	   		1		//硬件版本,放大10倍,如1.0表示为10
#define SOFTWARE_VERSION	    	1		//软件版本,放大100倍,如1.00,表示为100

void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2);
void app_get_version(u8*buf,u32 ver,u8 len);//得到版本号

u8 app_system_file_check(u8* diskx);//系统文件检测
u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode);
void app_boot_cpdmsg_set(u16 x,u16 y,u8 fsize);
u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8* src);
void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2);
void app_get_version(u8*buf,u32 ver,u8 len);//得到版本号

u8 app_system_parameter_init(void);	//系统信息初始化


void app_lcd_auto_bklight(void);
#endif

