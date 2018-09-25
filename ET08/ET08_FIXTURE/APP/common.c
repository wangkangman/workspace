#include "common.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//APPͨ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/2/16
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//V1.1 20140216
//�����Ը��ֱַ���LCD��֧��.
//V1.2 20140727
//�޸�app_show_float������һ��bug
////////////////////////////////////////////////////////////////////////////////// 	   


//PC2LCD2002����ȡģ����:����ʽ,˳��(��λ��ǰ),����.C51��ʽ.																		    
//��������:
//���������:ASCII��+��(' '+95)
//��ͨ����:ASCII��
u8*const APP_ASCII_S6030="1:/SYSTEM/APP/COMMON/fonts60.fon";	//���������60*30����������·�� 
u8*const APP_ASCII_5427="1:/SYSTEM/APP/COMMON/font54.fon";		//��ͨ����54*27����������·�� 
u8*const APP_ASCII_3618="1:/SYSTEM/APP/COMMON/font36.fon";		//��ͨ����36*18����������·��
u8*const APP_ASCII_2814="1:/SYSTEM/APP/COMMON/font28.fon";		//��ͨ����28*14����������·�� 

u8* asc2_s6030=0;	//���������60*30���������
u8* asc2_5427=0;	//��ͨ����54*27���������
u8* asc2_3618=0;	//��ͨ����36*18���������
u8* asc2_2814=0;	//��ͨ����28*14���������


static u8 GetPrivateString(u8* text, u8* key, u8* val);




//ALIENTEK logo ͼ��(18*24��С)
//PCtoLCD2002ȡģ��ʽ:����,����ʽ,˳��
const u8 APP_ALIENTEK_ICO1824[]=
{
0x03,0xF0,0x00,0x0F,0xF8,0x00,0x1F,0xFC,0x00,0x3F,0xFE,0x00,0x7F,0xFF,0x00,0x7F,
0xFF,0x00,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,
0x80,0xFF,0xFF,0x80,0xCF,0xF9,0x80,0xC7,0xF1,0x80,0x43,0xE1,0x00,0x61,0xC3,0x00,
0x31,0xC6,0x00,0x39,0xCE,0x00,0x1F,0xFC,0x00,0x0F,0xF8,0x00,0x07,0xF0,0x00,0x03,
0xE0,0x00,0x01,0xC0,0x00,0x00,0x80,0x00
};
//ALIENTEK logo ͼ��(24*32��С)
//PCtoLCD2002ȡģ��ʽ:����,����ʽ,˳��
const u8 APP_ALIENTEK_ICO2432[]=
{
0x00,0x7F,0x00,0x01,0xFF,0xC0,0x03,0xFF,0xE0,0x07,0xFF,0xF0,0x0F,0xFF,0xF8,0x1F,
0xFF,0xFC,0x1F,0xFF,0xFC,0x3F,0xFF,0xFE,0x3F,0xFF,0xFE,0x7F,0xFF,0xFF,0x7F,0xFF,
0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,
0x73,0xFF,0xE7,0x70,0xFF,0x87,0x30,0xFF,0x86,0x30,0x7F,0x06,0x38,0x3E,0x0E,0x1C,
0x3E,0x1C,0x1E,0x1C,0x3C,0x0F,0x1C,0x78,0x07,0xFF,0xF0,0x07,0xFF,0xF0,0x03,0xFF,
0xE0,0x01,0xFF,0xC0,0x00,0xFF,0x80,0x00,0x7F,0x00,0x00,0x3E,0x00,0x00,0x1C,0x00,
};
//ALIENTEK logo ͼ��(36*48��С)
//PCtoLCD2002ȡģ��ʽ:����,����ʽ,˳��
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
////////////////////////////////α����������취////////////////////////////////
u32 random_seed=1;
void app_srand(u32 seed)
{
	random_seed=seed;
}


//������·����Ҫ���޸�
//des:Ŀ�껺����
//diskx:�µĴ��̱��
//src:ԭ·��
void app_system_file_chgdisk(u8 *des,u8 *diskx,u8 *src)
{ 
	//static u8 t=0;
	strcpy((char*)des,(const char*)src);
	des[0]=diskx[0]; 
	//printf("path[%d]:%s\r\n",t++,des);	//��ӡ�ļ���
}
//ϵͳ�ܹ���Ҫ��ͼ��/ͼƬ/ϵͳ�ļ���170��,����SPB����ռ20��.����150��ΪAPP������ռ��
//�⻹������SYSTEM/FONT�ļ����ڵ������ļ�.
//170���ļ��ܴ�С:1,033,804 �ֽ�(2.95MB)
//3�������ļ��ܴ�С:1,514,984 �ֽ�(3.08MB)
//������е�ϵͳ�ļ��Ƿ�����
//����ֵ:0,����
//    ����,�������
#if 1
u8 app_system_file_check(u8* diskx){
	u8 res = 0;
	FIL *f_check;
	u8 *path;
	u8 *text;
	u8 val[20];

	f_check=(FIL *)memin_malloc(sizeof(FIL));
	path = memin_malloc(200);

    if(!f_check || !path)res=0XFF;//����ʧ��

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
	f_check=(FIL *)memin_malloc(sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	path = memin_malloc(200);						//Ϊpath�����ڴ� 
	if(!f_check || !path)rval=0XFF;//����ʧ��
	while(rval==0)
	{	
		//���5��SPB����ͼƬ
		for(j=0;j<3;j++)
		{
			for(i=0;i<2;i++)
			{
				app_system_file_chgdisk(path,diskx,spb_bkpic_path_tbl[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
				if(res){rval++;break;}	//��ʧ��,rvalǿ�Ƹ�ֵ     
				f_close(f_check);		//�ر��ļ� 
				rval++;
				if(j==2)break;
			}
			if(res)break;
		}
		if(rval!=5)break;
		//���48��SPB����ͼ��
		for(i=0;i<3;i++)
		{
			for(j=0;j<2;j++)
			{
				for(k=0;k<8;k++)
				{
					app_system_file_chgdisk(path,diskx,spb_icos_path_tbl[i][j][k]); 
					res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
					if(res)break;		 //��ʧ��       
					f_close(f_check);    //�ر��ļ� 
					rval++;
				}
				if(res)break;//��ʧ��   
			} 
			if(res)break;//��ʧ��   
		}
		if(rval!=5+48)break;
		//���9��SPB����ͼ��
		for(j=0;j<3;j++)
		{
			for(i=0;i<3;i++)
			{
				app_system_file_chgdisk(path,diskx,spb_micos_path_tbl[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
				if(res)break;			//��ʧ��      
				f_close(f_check);		//�ر��ļ� 
				rval++;
			}
			if(res)break;
		}
		if(j!=3&&i!=3)break;		
		//APP/COMMON�ļ�����9���ļ����
		app_system_file_chgdisk(path,diskx,APP_OK_PIC); 
 		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		
		app_system_file_chgdisk(path,diskx,APP_CANCEL_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;	
		app_system_file_chgdisk(path,diskx,APP_UNSELECT_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_SELECT_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_VOL_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;
		app_system_file_chgdisk(path,diskx,APP_ASCII_S6030); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_ASCII_5427); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_ASCII_3618); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_ASCII_2814); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;	
		//APP/AUDIO �ļ�����15���ļ����
		for(j=0;j<2;j++)
		{
			for(i=0;i<5;i++)
			{
				app_system_file_chgdisk(path,diskx,AUDIO_BTN_PIC_TBL[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
				if(res)break;			//��ʧ��      
				f_close(f_check);		//�ر��ļ� 
				rval++;
			}
			if(res)break;
		}
		if(j!=2&&i!=5)break;
		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,AUDIO_BACK_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
			if(res)break;			//��ʧ��      
			f_close(f_check);		//�ر��ļ� 
			rval++; 
		}
		if(j!=3)break;	   
		app_system_file_chgdisk(path,diskx,AUDIO_PLAYR_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,AUDIO_PAUSEP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;	 
		//APP/PHONE �ļ�����9���ļ����
 		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,PHONE_CALL_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
			if(res)break;			//��ʧ��      
			f_close(f_check);		//�ر��ļ� 
			rval++; 
		}
		if(j!=3)break;
 		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,PHONE_HANGUP_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
			if(res)break;			//��ʧ��      
			f_close(f_check);		//�ر��ļ� 
			rval++; 
		}
		if(j!=3)break;		
 		for(j=0;j<3;j++)
		{ 
			app_system_file_chgdisk(path,diskx,PHONE_DEL_PIC[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
			if(res)break;			//��ʧ��      
			f_close(f_check);		//�ر��ļ� 
			rval++; 
		}
		if(j!=3)break;	 
		//���¼����7��ͼ��
		app_system_file_chgdisk(path,diskx,RECORDER_DEMO_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
  		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_RECR_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_RECP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_PAUSER_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_PAUSEP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_STOPR_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,RECORDER_STOPP_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
  		rval++;		  
		//���Ӧ�ó���48��ͼ��
		for(j=0;j<3;j++)
		{
			for(i=0;i<16;i++)
			{
				app_system_file_chgdisk(path,diskx,appplay_icospath_tbl[j][i]); 
				res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
				if(res)break;			//��ʧ��      
				f_close(f_check);		//�ر��ļ� 
				rval++;
			}
			if(res)break;
		}
		if(j!=3&&i!=16)break;    
 		//��⻭ͼ1��ͼ��
		app_system_file_chgdisk(path,diskx,PAINT_COLOR_TBL_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ� 
 		rval++;		  
		//LWIP/WebServer�ļ������ļ�ͳ��
 		for(j=0;j<19;j++)
		{ 
			app_system_file_chgdisk(path,diskx,lwip_webserver_path_tbl[j]); 
			res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
			if(res)break;		//��ʧ�� 
			f_close(f_check);	//�ر��ļ� 
			rval++; 
		}
		if(j!=19)break;   
		printf("\r\ntotal system files:%d\r\n",rval);
		rval=0;
		break;    
	}
	memin_free(f_check);//�ͷ��ڴ�
	memin_free(path);//�ͷ��ڴ�
	return rval; 
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
//���´��������ʵ��SYSTEM�ļ��и���
u8*const APP_SYSTEM_APPPATH="/SYSTEM/APP";	//APP�ļ���·�� ��
u8*const APP_SYSTEM_SPBPATH="/SYSTEM/SPB";	//SPB�ļ���·��
u8*const APP_SYSTEM_LWIPPATH="/SYSTEM/LWIP";//LWIP�ļ���·��
u8*const APP_SYSTEM_DSTPATH="1:/SYSTEM";	//ϵͳ�ļ�Ŀ��·��

 
//�ļ�������Ϣ��ʾ���������
static u16 cpdmsg_x;
static u16 cpdmsg_y;
static u8  cpdmsg_fsize;	


//ϵͳ������ʱ��,������ʾ���½���
//*pname:�����ļ�����
//pct:�ٷֱ�
//mode:ģʽ
//[0]:�����ļ���
//[1]:���°ٷֱ�pct
//[2]:�����ļ���
//[3~7]:����
//����ֵ:0,����;
//       1,��������
u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode)
{													 
	if(mode&0X01)
	{
		LCD_Fill(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,cpdmsg_x+(9+16)*(cpdmsg_fsize/2),cpdmsg_y+cpdmsg_fsize,BLACK);	//����ɫ
		LCD_ShowString(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,16*(cpdmsg_fsize/2),cpdmsg_fsize,cpdmsg_fsize,pname);	//��ʾ�ļ���,�16���ַ����
		printf("\r\nCopy File:%s\r\n",pname);  
	}
	if(mode&0X04)printf("Copy Folder:%s\r\n",pname);
	if(mode&0X02)//���°ٷֱ�
	{
		LCD_ShowString(cpdmsg_x+(9+16+1+3)*(cpdmsg_fsize/2),cpdmsg_y,lcddev.width,lcddev.height,cpdmsg_fsize,"%");		
 		LCD_ShowNum(cpdmsg_x+(9+16+1)*(cpdmsg_fsize/2),cpdmsg_y,pct,3,cpdmsg_fsize);//��ʾ��ֵ
		printf("File Copyed:%d\r\n",pct);
	}
	return 0;	
}
//����app_boot_cpdmsg����ʾ����
//x,y:����.
//fisze:�����С
void app_boot_cpdmsg_set(u16 x,u16 y,u8 fsize)
{
	cpdmsg_x=x;
	cpdmsg_y=y;
	cpdmsg_fsize=fsize;
}
//ϵͳ����
//fcpymsg:�ٷֱ���ʾ����
//src:�ļ���Դ,"0:",SD��;"2:",U��.
//����ֵ:0,����
//    ����,����,0XFFǿ���˳���
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
		return 1;//�ڴ�����ʧ��
	}
	sprintf((char*)p_testbin,"%s%s",src,"/SYSTEM/TESTBIN");
	sprintf((char*)p_appbin,"%s%s",src,"/SYSTEM/APPBIN");
	sprintf((char*)p_config,"%s%s",src,"/SYSTEM/CONFIG");
	sprintf((char*)p_picture,"%s%s",src,"/SYSTEM/PICTURE");
	printf("updata file system 2");
	totsize+=exf_fdsize(p_testbin);//�õ������ļ��е��ܴ�С
	totsize+=exf_fdsize(p_appbin);
	totsize+=exf_fdsize(p_config);
	totsize+=exf_fdsize(p_picture);
	printf("totsize:%d\r\n",totsize);

    f_mkdir((const TCHAR *)"1:/SYSTEM");//ǿ�ƴ���Ŀ���ļ���
 	res =exf_fdcopy(fcpymsg, p_testbin, (u8*)"1:/SYSTEM", &totsize, &cpdsize, 1);
        if (res) {
          if (res != 0xff)
            rval = 1; //��һ�׶θ��Ƴ���
          else
            rval = 0XFF; //ǿ���˳�
        } else           //��һ�׶�OK,�����ڶ��׶�
        {
          res = exf_fdcopy(fcpymsg, p_appbin, (u8 *)"1:/SYSTEM", &totsize,
                           &cpdsize, 1);
          if (res) {
            if (res != 0xff)
              rval = 2; //�ڶ��׶θ��Ƴ���
            else
              rval = 0XFF; //ǿ���˳�
          } else           //�ڶ��׶�OK,���������׶�
          {
            res = exf_fdcopy(fcpymsg, p_config, (u8 *)"1:/SYSTEM", &totsize,
                             &cpdsize, 1);
            if (res) {
              if (res != 0xff)
                rval = 3; //�����׶θ��Ƴ���
              else
                rval = 0XFF; //ǿ���˳�
            } else           //�����׶�OK,�������Ľ׶�
            {
              res = exf_fdcopy(fcpymsg, p_picture, (u8 *)"1:/SYSTEM", &totsize,
                               &cpdsize, 1);
              if (res) {
                if (res != 0xff)
                  rval = 4; //���Ľ׶θ��Ƴ���
                else
                  rval = 0XFF; //ǿ���˳�
              } else {         //���Ľ׶�OK,��������׶�
                rval = 0;      //���������޴���,��ǿ���˳�
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
		return 1;//�ڴ�����ʧ��
	}
	sprintf((char*)p_app,"%s%s",src,APP_SYSTEM_APPPATH);
	sprintf((char*)p_spb,"%s%s",src,APP_SYSTEM_SPBPATH);
	sprintf((char*)p_lwip,"%s%s",src,APP_SYSTEM_LWIPPATH); 
	totsize+=exf_fdsize(p_app);//�õ������ļ��е��ܴ�С
	totsize+=exf_fdsize(p_spb);
	totsize+=exf_fdsize(p_lwip); 
	printf("totsize:%d\r\n",totsize);
	f_mkdir((const TCHAR *)APP_SYSTEM_DSTPATH);//ǿ�ƴ���Ŀ���ļ���
 	res=exf_fdcopy(fcpymsg,p_app,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)rval=1;			//��һ�׶θ��Ƴ���
		else rval=0XFF;					//ǿ���˳�
	}else								//��һ�׶�OK,�����ڶ��׶�
	{
		res=exf_fdcopy(fcpymsg,p_spb,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
		if(res)
		{
			if(res!=0xff)rval=2;		//�ڶ��׶θ��Ƴ���
			else rval=0XFF;				//ǿ���˳�
		}else							//�ڶ��׶�OK,���������׶�
		{		
			res=exf_fdcopy(fcpymsg,p_lwip,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
			if(res)
			{
				if(res!=0xff)rval=3;	//�����׶θ��Ƴ���
				else rval=0XFF;			//ǿ���˳�
			}else						//�����׶�OK,�������Ľ׶�
			{	 
				rval=0;			//���������޴���,��ǿ���˳� 
			}			 
		}
	}
	memin_free(p_app);
	memin_free(p_spb);
	memin_free(p_lwip); 
	return rval;
}
#endif
//�õ�STM32�����к�
//sn0,sn1,sn2:3���������к�
void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2)
{
	*sn0=*(vu32*)(0x1FFF7A10);
	*sn1=*(vu32*)(0x1FFF7A14);
	*sn2=*(vu32*)(0x1FFF7A18);    
}
//��ӡSN
void app_usmart_getsn(void)
{
	u32 sn0,sn1,sn2;
	app_getstm32_sn(&sn0,&sn1,&sn2);
	printf("\r\nSerial Number:%X%X%X\r\n",sn0,sn1,sn2);
}
//�õ��汾��Ϣ
//buf:�汾������(������Ҫ6���ֽ�,�籣��:V1.00)
//ver:�汾�궨��
//len:�汾λ��.1,����ֻ��1��λ�İ汾,V1;2,����2��λ�汾,V1.0;3,����3��λ�汾,V1.00;
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
	buf[i]=0;//���������	 			   
}

//��ʼ������ϵͳ����
//����ϵͳ����,��������,WM8978���ݵ�
u8 app_system_parameter_init(void)
{ 
//	u8 i;

	TIM12_CH2_PWM_Init(100,8400-1);		//��ʼ��TIM12_CH2 PWM���,��ΪLCD�ı������
 	app_lcd_auto_bklight();	//�Զ�����
//	calendar_alarm_init((_alarm_obj*)&alarm,&calendar);	//��ʼ������
	return 0;	
} 
//LCD�����Զ�����
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






