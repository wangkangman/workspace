#include "common.h"

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


//PC2LCD2002字体取模方法:逐列式,顺向(高位在前),阴码.C51格式.																		    
//特殊字体:
//数码管字体:ASCII集+℃(' '+95)
//普通字体:ASCII集
u8*const APP_ASCII_S6030="1:/SYSTEM/APP/COMMON/fonts60.fon";	//数码管字体60*30大数字字体路径 
u8*const APP_ASCII_5427="1:/SYSTEM/APP/COMMON/font54.fon";		//普通字体54*27大数字字体路径 
u8*const APP_ASCII_3618="1:/SYSTEM/APP/COMMON/font36.fon";		//普通字体36*18大数字字体路径
u8*const APP_ASCII_2814="1:/SYSTEM/APP/COMMON/font28.fon";		//普通字体28*14大数字字体路径 

u8* asc2_s6030=0;	//数码管字体60*30大字体点阵集
u8* asc2_5427=0;	//普通字体54*27大字体点阵集
u8* asc2_3618=0;	//普通字体36*18大字体点阵集
u8* asc2_2814=0;	//普通字体28*14大字体点阵集


static u8 GetPrivateString(u8* text, u8* key, u8* val);




//ALIENTEK logo 图标(18*24大小)
//PCtoLCD2002取模方式:阴码,逐行式,顺向
const u8 APP_ALIENTEK_ICO1824[]=
{
0x03,0xF0,0x00,0x0F,0xF8,0x00,0x1F,0xFC,0x00,0x3F,0xFE,0x00,0x7F,0xFF,0x00,0x7F,
0xFF,0x00,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,
0x80,0xFF,0xFF,0x80,0xCF,0xF9,0x80,0xC7,0xF1,0x80,0x43,0xE1,0x00,0x61,0xC3,0x00,
0x31,0xC6,0x00,0x39,0xCE,0x00,0x1F,0xFC,0x00,0x0F,0xF8,0x00,0x07,0xF0,0x00,0x03,
0xE0,0x00,0x01,0xC0,0x00,0x00,0x80,0x00
};
//ALIENTEK logo 图标(24*32大小)
//PCtoLCD2002取模方式:阴码,逐行式,顺向
const u8 APP_ALIENTEK_ICO2432[]=
{
0x00,0x7F,0x00,0x01,0xFF,0xC0,0x03,0xFF,0xE0,0x07,0xFF,0xF0,0x0F,0xFF,0xF8,0x1F,
0xFF,0xFC,0x1F,0xFF,0xFC,0x3F,0xFF,0xFE,0x3F,0xFF,0xFE,0x7F,0xFF,0xFF,0x7F,0xFF,
0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,
0x73,0xFF,0xE7,0x70,0xFF,0x87,0x30,0xFF,0x86,0x30,0x7F,0x06,0x38,0x3E,0x0E,0x1C,
0x3E,0x1C,0x1E,0x1C,0x3C,0x0F,0x1C,0x78,0x07,0xFF,0xF0,0x07,0xFF,0xF0,0x03,0xFF,
0xE0,0x01,0xFF,0xC0,0x00,0xFF,0x80,0x00,0x7F,0x00,0x00,0x3E,0x00,0x00,0x1C,0x00,
};
//ALIENTEK logo 图标(36*48大小)
//PCtoLCD2002取模方式:阴码,逐行式,顺向
const u8 APP_ALIENTEK_ICO3648[]=
{
0x00,0x01,0xFC,0x00,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x1F,0xFF,0xC0,0x00,0x00,
0x7F,0xFF,0xF0,0x00,0x00,0xFF,0xFF,0xF8,0x00,0x01,0xFF,0xFF,0xFC,0x00,0x03,0xFF,
0xFF,0xFE,0x00,0x07,0xFF,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,
0xFF,0x80,0x0F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,
0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xE0,0x1F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,
0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,
0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3C,0xFF,0xFF,0xFD,0xE0,0x38,0x3F,0xFF,
0xF0,0xE0,0x18,0x1F,0xFF,0xC0,0xE0,0x1C,0x0F,0xFF,0x80,0xC0,0x1C,0x07,0xFF,0x01,
0xC0,0x0C,0x03,0xFF,0x01,0xC0,0x0E,0x03,0xFE,0x03,0x80,0x07,0x01,0xFC,0x07,0x80,
0x07,0x81,0xFC,0x07,0x00,0x03,0xC0,0xFC,0x0F,0x00,0x03,0xE0,0xF8,0x3E,0x00,0x01,
0xF0,0xF8,0x7E,0x00,0x01,0xFE,0xFD,0xFC,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x7F,
0xFF,0xF8,0x00,0x00,0x3F,0xFF,0xF0,0x00,0x00,0x3F,0xFF,0xE0,0x00,0x00,0x1F,0xFF,
0xC0,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x07,0xFF,0x00,0x00,0x00,0x03,0xFE,0x00,
0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
////////////////////////////////伪随机数产生办法////////////////////////////////
u32 random_seed=1;
void app_srand(u32 seed)
{
	random_seed=seed;
}


//将磁盘路径按要求修改
//des:目标缓存区
//diskx:新的磁盘编号
//src:原路径
void app_system_file_chgdisk(u8 *des,u8 *diskx,u8 *src)
{ 
	//static u8 t=0;
	strcpy((char*)des,(const char*)src);
	des[0]=diskx[0]; 
	//printf("path[%d]:%s\r\n",t++,des);	//打印文件名
}
//系统总共需要的图标/图片/系统文件有170个,其中SPB部分占20个.其余150个为APP各功能占用
//这还不包括SYSTEM/FONT文件夹内的字体文件.
//170个文件总大小:1,033,804 字节(2.95MB)
//3个字体文件总大小:1,514,984 字节(3.08MB)
//检测所有的系统文件是否正常
//返回值:0,正常
//    其他,错误代码
#if 1
u8 app_system_file_check(u8* diskx){
	u8 res = 0;
	FIL *f_check;
	u8 *path;
	u8 *text;
	u8 val[20];

	f_check=(FIL *)memin_malloc(sizeof(FIL));
	path = memin_malloc(200);

    if(!f_check || !path)res=0XFF;//申请失败

    sprintf((char*)path,"%s:%s", diskx,"/SYSTEM/CONFIG/config.txt");
	res = f_open(f_check,(const TCHAR*)path,FA_READ);
    if(res) {
		goto END;
	}

	text = memin_malloc(f_check->fsize + 1);
	mymemset(text, 0, f_check->fsize + 1); 
    res = f_read(f_check, text, f_check->fsize, (UINT*)&br);
	if(res) {
		res = 1;
		goto END;
    }

    
	GetPrivateString(text, "testfirm", val);

    sprintf((char*)path,"%s:%s%s", diskx, "/SYSTEM/TESTBIN/", val);

	res = f_open(f_check,(const TCHAR*)path,FA_READ);
	if(res) {
		res = 2;
		goto END;
    } else {
      f_close(f_check);
	}

#if 0
	GetPrivateString(text, "appfirm", val);

    sprintf((char*)path,"%s:%s%s", diskx, "/SYSTEM/BIN/", val);

	res = f_open(f_check,(const TCHAR*)path,FA_READ);
	if(res) {
		res = 3;
		goto END;
    }
#endif

END:
    f_close(f_check);
    memin_free(path);
	memin_free(f_check);
	memin_free(text);
	printf("check res: %d \r\n", res);
	return res;
}
#endif



#if 0
u8 app_system_file_check(u8* diskx)
{
	u8 i,j,k;
	u8 rval=0;
	u8 res=0;
	FIL *f_check;
	u8 *path;
	f_check=(FIL *)memin_malloc(sizeof(FIL));	//开辟FIL字节的内存区域 
	path = memin_malloc(200);						//为path申请内存 
	if(!f_check || !path)rval=0XFF;//申请失败
	while(rval==0)
	{	
		//检测5个SPB背景图片
		for(j=0;j<3;j++)
		{
			for(i=0;i<2;i++)
			{
				app_system_file_chgdisk(path,diskx,spb_bkpic_path_tbl[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
				if(res){rval++;break;}	//打开失败,rval强制赋值     
				f_close(f_check);		//关闭文件 
				rval++;
				if(j==2)break;
			}
			if(res)break;
		}
		if(rval!=5)break;
		//检测48个SPB界面图标
		for(i=0;i<3;i++)
		{
			for(j=0;j<2;j++)
			{
				for(k=0;k<8;k++)
				{
					app_system_file_chgdisk(path,diskx,spb_icos_path_tbl[i][j][k]); 
					res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
					if(res)break;		 //打开失败       
					f_close(f_check);    //关闭文件 
					rval++;
				}
				if(res)break;//打开失败   
			} 
			if(res)break;//打开失败   
		}
		if(rval!=5+48)break;
		//检测9个SPB界面图标
		for(j=0;j<3;j++)
		{
			for(i=0;i<3;i++)
			{
				app_system_file_chgdisk(path,diskx,spb_micos_path_tbl[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
				if(res)break;			//打开失败      
				f_close(f_check);		//关闭文件 
				rval++;
			}
			if(res)break;
		}
		if(j!=3&&i!=3)break;		
		//APP/COMMON文件夹内9个文件检测
		app_system_file_chgdisk(path,diskx,APP_OK_PIC); 
 		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		
		app_system_file_chgdisk(path,diskx,APP_CANCEL_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;	
		app_system_file_chgdisk(path,diskx,APP_UNSELECT_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_SELECT_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_VOL_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;
		app_system_file_chgdisk(path,diskx,APP_ASCII_S6030); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_ASCII_5427); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_ASCII_3618); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_ASCII_2814); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;	
		//APP/AUDIO 文件夹内15个文件检测
		for(j=0;j<2;j++)
		{
			for(i=0;i<5;i++)
			{
				app_system_file_chgdisk(path,diskx,AUDIO_BTN_PIC_TBL[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
				if(res)break;			//打开失败      
				f_close(f_check);		//关闭文件 
				rval++;
			}
			if(res)break;
		}
		if(j!=2&&i!=5)break;
		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,AUDIO_BACK_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
			if(res)break;			//打开失败      
			f_close(f_check);		//关闭文件 
			rval++; 
		}
		if(j!=3)break;	   
		app_system_file_chgdisk(path,diskx,AUDIO_PLAYR_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,AUDIO_PAUSEP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;	 
		//APP/PHONE 文件夹内9个文件检测
 		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,PHONE_CALL_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
			if(res)break;			//打开失败      
			f_close(f_check);		//关闭文件 
			rval++; 
		}
		if(j!=3)break;
 		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,PHONE_HANGUP_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
			if(res)break;			//打开失败      
			f_close(f_check);		//关闭文件 
			rval++; 
		}
		if(j!=3)break;		
 		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,PHONE_DEL_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
			if(res)break;			//打开失败      
			f_close(f_check);		//关闭文件 
			rval++; 
		}
		if(j!=3)break;	 
		//检测录音机7个图标
		app_system_file_chgdisk(path,diskx,RECORDER_DEMO_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
  		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_RECR_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_RECP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_PAUSER_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_PAUSEP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_STOPR_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_STOPP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件     
  		rval++;		  
		//检测应用程序48个图标
		for(j=0;j<3;j++)
		{
			for(i=0;i<16;i++)
			{
				app_system_file_chgdisk(path,diskx,appplay_icospath_tbl[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
				if(res)break;			//打开失败      
				f_close(f_check);		//关闭文件 
				rval++;
			}
			if(res)break;
		}
		if(j!=3&&i!=16)break;    
 		//检测画图1个图标
		app_system_file_chgdisk(path,diskx,PAINT_COLOR_TBL_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
		if(res)break;			//打开失败      
    	f_close(f_check);		//关闭文件 
 		rval++;		  
		//LWIP/WebServer文件夹内文件统计
 		for(j=0;j<19;j++)
		{ 
			app_system_file_chgdisk(path,diskx,lwip_webserver_path_tbl[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//只读方式打开文件
			if(res)break;		//打开失败 
			f_close(f_check);	//关闭文件 
			rval++; 
		}
		if(j!=19)break;   
		printf("\r\ntotal system files:%d\r\n",rval);
		rval=0;
		break;    
	}
	memin_free(f_check);//释放内存
	memin_free(path);//释放内存
	return rval; 
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
//以下代码仅用于实现SYSTEM文件夹更新
u8*const APP_SYSTEM_APPPATH="/SYSTEM/APP";	//APP文件夹路径 径
u8*const APP_SYSTEM_SPBPATH="/SYSTEM/SPB";	//SPB文件夹路径
u8*const APP_SYSTEM_LWIPPATH="/SYSTEM/LWIP";//LWIP文件夹路径
u8*const APP_SYSTEM_DSTPATH="1:/SYSTEM";	//系统文件目标路径

 
//文件复制信息提示坐标和字体
static u16 cpdmsg_x;
static u16 cpdmsg_y;
static u8  cpdmsg_fsize;	


//系统启动的时候,用于显示更新进度
//*pname:更新文件名字
//pct:百分比
//mode:模式
//[0]:更新文件名
//[1]:更新百分比pct
//[2]:更新文件夹
//[3~7]:保留
//返回值:0,正常;
//       1,结束复制
u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode)
{													 
	if(mode&0X01)
	{
		LCD_Fill(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,cpdmsg_x+(9+16)*(cpdmsg_fsize/2),cpdmsg_y+cpdmsg_fsize,BLACK);	//填充底色
		LCD_ShowString(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,16*(cpdmsg_fsize/2),cpdmsg_fsize,cpdmsg_fsize,pname);	//显示文件名,最长16个字符宽度
		printf("\r\nCopy File:%s\r\n",pname);  
	}
	if(mode&0X04)printf("Copy Folder:%s\r\n",pname);
	if(mode&0X02)//更新百分比
	{
		LCD_ShowString(cpdmsg_x+(9+16+1+3)*(cpdmsg_fsize/2),cpdmsg_y,lcddev.width,lcddev.height,cpdmsg_fsize,"%");		
 		LCD_ShowNum(cpdmsg_x+(9+16+1)*(cpdmsg_fsize/2),cpdmsg_y,pct,3,cpdmsg_fsize);//显示数值
		printf("File Copyed:%d\r\n",pct);
	}
	return 0;	
}
//设置app_boot_cpdmsg的显示坐标
//x,y:坐标.
//fisze:字体大小
void app_boot_cpdmsg_set(u16 x,u16 y,u8 fsize)
{
	cpdmsg_x=x;
	cpdmsg_y=y;
	cpdmsg_fsize=fsize;
}
//系统更新
//fcpymsg:百分比显示函数
//src:文件来源,"0:",SD卡;"2:",U盘.
//返回值:0,正常
//    其他,错误,0XFF强制退出了
#if 1
u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8* src){
	u32 totsize=0;
	u32 cpdsize=0;
	u8 res=0;
	u8 *p_testbin;
	u8 *p_appbin;
	u8 *p_config; 
	u8 *p_picture;  
	u8 rval;

	printf("updata file system 1");
	p_testbin=memin_malloc(100);
	p_appbin=memin_malloc(100);
	p_config=memin_malloc(100);
	p_picture=memin_malloc(100); 

	if(p_picture==NULL)
	{
		memin_free(p_testbin);
		memin_free(p_appbin);
		memin_free(p_config);
		memin_free(p_picture);
		return 1;//内存申请失败
	}
	sprintf((char*)p_testbin,"%s%s",src,"/SYSTEM/TESTBIN");
	sprintf((char*)p_appbin,"%s%s",src,"/SYSTEM/APPBIN");
	sprintf((char*)p_config,"%s%s",src,"/SYSTEM/CONFIG");
	sprintf((char*)p_picture,"%s%s",src,"/SYSTEM/PICTURE");
	printf("updata file system 2");
	totsize+=exf_fdsize(p_testbin);//得到三个文件夹的总大小
	totsize+=exf_fdsize(p_appbin);
	totsize+=exf_fdsize(p_config);
	totsize+=exf_fdsize(p_picture);
	printf("totsize:%d\r\n",totsize);

    f_mkdir((const TCHAR *)"1:/SYSTEM");//强制创建目标文件夹
 	res =exf_fdcopy(fcpymsg, p_testbin, (u8*)"1:/SYSTEM", &totsize, &cpdsize, 1);
        if (res) {
          if (res != 0xff)
            rval = 1; //第一阶段复制出错
          else
            rval = 0XFF; //强制退出
        } else           //第一阶段OK,启动第二阶段
        {
          res = exf_fdcopy(fcpymsg, p_appbin, (u8 *)"1:/SYSTEM", &totsize,
                           &cpdsize, 1);
          if (res) {
            if (res != 0xff)
              rval = 2; //第二阶段复制出错
            else
              rval = 0XFF; //强制退出
          } else           //第二阶段OK,启动第三阶段
          {
            res = exf_fdcopy(fcpymsg, p_config, (u8 *)"1:/SYSTEM", &totsize,
                             &cpdsize, 1);
            if (res) {
              if (res != 0xff)
                rval = 3; //第三阶段复制出错
              else
                rval = 0XFF; //强制退出
            } else           //第三阶段OK,启动第四阶段
            {
              res = exf_fdcopy(fcpymsg, p_picture, (u8 *)"1:/SYSTEM", &totsize,
                               &cpdsize, 1);
              if (res) {
                if (res != 0xff)
                  rval = 4; //第四阶段复制出错
                else
                  rval = 0XFF; //强制退出
              } else {         //第四阶段OK,启动第五阶段
                rval = 0;      //整个过程无错误,无强制退出
              }
            }
          }
        }

    memin_free(p_testbin);
	memin_free(p_appbin);
	memin_free(p_config);
	memin_free(p_picture); 
	return rval;
}
#endif



#if 0
u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8* src)
{
	u32 totsize=0;
	u32 cpdsize=0;
	u8 res=0;
	u8 *p_app;
	u8 *p_spb; 
	u8 *p_lwip;  
	u8 rval;
	p_app=memin_malloc(100);
	p_spb=memin_malloc(100);
	p_lwip=memin_malloc(100); 
	if(p_lwip==NULL)
	{
		memin_free(p_app);
		memin_free(p_spb);
		memin_free(p_lwip);
		return 1;//内存申请失败
	}
	sprintf((char*)p_app,"%s%s",src,APP_SYSTEM_APPPATH);
	sprintf((char*)p_spb,"%s%s",src,APP_SYSTEM_SPBPATH);
	sprintf((char*)p_lwip,"%s%s",src,APP_SYSTEM_LWIPPATH); 
	totsize+=exf_fdsize(p_app);//得到三个文件夹的总大小
	totsize+=exf_fdsize(p_spb);
	totsize+=exf_fdsize(p_lwip); 
	printf("totsize:%d\r\n",totsize);
	f_mkdir((const TCHAR *)APP_SYSTEM_DSTPATH);//强制创建目标文件夹
 	res=exf_fdcopy(fcpymsg,p_app,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)rval=1;			//第一阶段复制出错
		else rval=0XFF;					//强制退出
	}else								//第一阶段OK,启动第二阶段
	{
		res=exf_fdcopy(fcpymsg,p_spb,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
		if(res)
		{
			if(res!=0xff)rval=2;		//第二阶段复制出错
			else rval=0XFF;				//强制退出
		}else							//第二阶段OK,启动第三阶段
		{		
			res=exf_fdcopy(fcpymsg,p_lwip,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
			if(res)
			{
				if(res!=0xff)rval=3;	//第三阶段复制出错
				else rval=0XFF;			//强制退出
			}else						//第三阶段OK,启动第四阶段
			{	 
				rval=0;			//整个过程无错误,无强制退出 
			}			 
		}
	}
	memin_free(p_app);
	memin_free(p_spb);
	memin_free(p_lwip); 
	return rval;
}
#endif
//得到STM32的序列号
//sn0,sn1,sn2:3个固有序列号
void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2)
{
	*sn0=*(vu32*)(0x1FFF7A10);
	*sn1=*(vu32*)(0x1FFF7A14);
	*sn2=*(vu32*)(0x1FFF7A18);    
}
//打印SN
void app_usmart_getsn(void)
{
	u32 sn0,sn1,sn2;
	app_getstm32_sn(&sn0,&sn1,&sn2);
	printf("\r\nSerial Number:%X%X%X\r\n",sn0,sn1,sn2);
}
//得到版本信息
//buf:版本缓存区(最少需要6个字节,如保存:V1.00)
//ver:版本宏定义
//len:版本位数.1,代表只有1个位的版本,V1;2,代表2个位版本,V1.0;3,代表3个位版本,V1.00;
void app_get_version(u8*buf,u32 ver,u8 len)
{
	u8 i=0;
	buf[i++]='V';
	if(len==1)
	{
		buf[i++]=ver%10+'0';
	}else if(len==2)
	{ 	
		buf[i++]=(ver/10)%10+'0';
		buf[i++]='.';
		buf[i++]=ver%10+'0';
	}else
	{
		buf[i++]=(ver/100)%10+'0';
		buf[i++]='.';
		buf[i++]=(ver/10)%10+'0';
		buf[i++]=ver%10+'0';	 
	}
	buf[i]=0;//加入结束符	 			   
}

//初始化各个系统参数
//包括系统设置,闹钟数据,WM8978数据等
u8 app_system_parameter_init(void)
{ 
//	u8 i;

	TIM12_CH2_PWM_Init(100,8400-1);		//初始化TIM12_CH2 PWM输出,作为LCD的背光控制
 	app_lcd_auto_bklight();	//自动背光
//	calendar_alarm_init((_alarm_obj*)&alarm,&calendar);	//初始化闹钟
	return 0;	
} 
//LCD背光自动控制
void app_lcd_auto_bklight(void)
{
	u8 lsens;
	lsens = 35;
	if(lcddev.id==0X1963)LCD_SSD_BackLightSet(30+lsens);
	else LCD_BLPWM_VAL=30+lsens;
}


u8 GetPrivateString(u8* text, u8* key, u8* val){
  u8* temp_key;
  u8* temp_text;

  temp_key = key;
  temp_text = text;

  while(1) {
    if ('\0' == *text) {
      return 1;
    }

    temp_text = text;
    temp_key = key;
    while (*temp_text == *temp_key) {
      temp_text++;
      temp_key++;
      if ('\0' == *temp_key)
        goto SUCCESS;
    }

    text++;
  }

SUCCESS:	
  while(*text++ != '='){
    if (*text == '\r' || *text == '\n' || *text == '\0' )
      return 2;
  }

  while(*text == ' '){
    text += 1;
  }

  while(*text != '\r' && *text != '\n' && *text != '\0' && *text != ' ') {
    *val++ = *text++;
  }
  *val = '\0';

  return 0;
}






