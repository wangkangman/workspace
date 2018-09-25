#include "common.h"
#include "includes.h"
#include "malloc.h"
#include "sys.h"
#include "usb_app.h"
#include "usart3.h"
#include "text.h"


/////////////////////////UCOSII��������///////////////////////////////////
// START ����
//�����������ȼ�
#define START_TASK_PRIO 10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE 64
//�����ջ��8�ֽڶ���
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);

//��������
//�����������ȼ�
#define USART_TASK_PRIO 7
//���������ջ��С
#define USART_STK_SIZE 128
//�����ջ��8�ֽڶ���
__align(8) static OS_STK USART_TASK_STK[USART_STK_SIZE];
//������
void usart_task(void *pdata);

//������
//�����������ȼ�
#define MAIN_TASK_PRIO 6
//���������ջ��С
#define MAIN_STK_SIZE 1200
//�����ջ��8�ֽڶ���
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

//��������
//�����������ȼ�
#define WATCH_TASK_PRIO 3
//���������ջ��С
#define WATCH_STK_SIZE 256
//�����ջ��8�ֽڶ���
__align(8) static OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
//������
void watch_task(void *pdata);

//�ⲿ�ڴ����(���֧��1M�ֽ��ڴ����)
// x,y:����
// fsize:�����С
//����ֵ:0,�ɹ�;1,ʧ��.
u8 system_exsram_test(u16 x, u16 y, u8 fsize) {
  u32 i = 0;
  u16 temp = 0;
  u16 sval = 0; //�ڵ�ַ0����������
  LCD_ShowString(x, y, lcddev.width, y + fsize, fsize,
                 (unsigned char *)"Ex Memory Test:   0KB");
  //ÿ��1K�ֽ�,д��һ������,�ܹ�д��1024������,�պ���1M�ֽ�
  for (i = 0; i < 1024 * 1024; i += 1024) {
    FSMC_SRAM_WriteBuffer((u8 *)&temp, i, 2);
    temp++;
  }
  //���ζ���֮ǰд�������,����У��
  for (i = 0; i < 1024 * 1024; i += 1024) {
    FSMC_SRAM_ReadBuffer((u8 *)&temp, i, 2);
    if (i == 0)
      sval = temp;
    else if (temp <= sval)
      break; //�������������һ��Ҫ�ȵ�һ�ζ��������ݴ�.
    LCD_ShowxNum(x + 15 * (fsize / 2), y, (u16)(temp - sval + 1), 4, fsize,
                 0); //��ʾ�ڴ�����
  }
  if (i >= 1024 * 1024) {
    LCD_ShowxNum(x + 15 * (fsize / 2), y, i / 1024, 4, fsize, 0); //��ʾ�ڴ�ֵ
    return 0; //�ڴ�����,�ɹ�
  }
  return 1; //ʧ��
}
//��ʾ������Ϣ
// x,y:����
// fsize:�����С
// x,y:����.err:������Ϣ
void system_error_show(u16 x, u16 y, u8 *err, u8 fsize) {
  POINT_COLOR = RED;
  while (1) {
    LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, err);
    delay_ms(400);
    LCD_Fill(x, y, lcddev.width, y + fsize, BLACK);
    delay_ms(100);
    LED0 = !LED0;
  }
}
//��������SPI FLASH(��������Դ��ɾ��),�Կ��ٸ���ϵͳ.
// x,y:����
// fsize:�����С
// x,y:����.err:������Ϣ
//����ֵ:0,û�в���;1,������
u8 system_files_erase(u16 x, u16 y, u8 fsize) {
  u8 key;
  u8 t = 0;
  POINT_COLOR = RED;
  LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize,
                 "Erase all system files?");
  while (1) {
    t++;
    if (t == 20)
      LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize,
                     "KEY1:NO / KEY4:YES");
    if (t == 40) {
      t = 0;
      LED0 = !LED0;
    }
    key = KEY_Scan(0);
    if (key == 1) //������,�û�ȡ����
    {
      POINT_COLOR = WHITE;
      LED0 = 1;
      return 0;
    }
    if (key == 4) //Ҫ����,Ҫ��������
    {
      LED0 = 1;
      LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize,
                     "Erasing SPI FLASH...");
      W25QXX_Erase_Chip();
      LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize,
                     "Erasing SPI FLASH OK");
      delay_ms(600);
      return 1;
    }
    delay_ms(10);
  }
}
//�ֿ����ȷ����ʾ.
// x,y:����
// fsize:�����С
//����ֵ:0,����Ҫ����;1,ȷ��Ҫ����
u8 system_font_update_confirm(u16 x, u16 y, u8 fsize) {
  u8 key;
  u8 t = 0;
  u8 res = 0;
  POINT_COLOR = RED;
  LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, "Update font?");
  while (1) {
    t++;
    if (t == 20)
      LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize,
                     "KEY1:NO / KEY4:YES");
    if (t == 40) {
      t = 0;
      LED0 = !LED0;
    }
    key = KEY_Scan(0);
    if (key == 1)
      break; //������
    if (key == 4) {
      res = 1;
      break;
    } //Ҫ����
    delay_ms(10);
  }
  LED0 = 1;
  POINT_COLOR = WHITE;
  return res;
}

//ϵͳ�ļ�����ȷ����ʾ.
// x,y:����
// fsize:�����С
//����ֵ:0,����Ҫ����;1,ȷ��Ҫ����
u8 system_sysfile_update_confirm(u16 x, u16 y, u8 fsize) {
  u8 key;
  u8 t = 0;
  u8 res = 0;
  POINT_COLOR = RED;
  LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize,
                 "Update system file?");
  while (1) {
    t++;
    if (t == 20)
      LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize,
                     "KEY1:NO / KEY4:YES");
    if (t == 40) {
      t = 0;
      LED0 = !LED0;
    }
    key = KEY_Scan(0);
    if (key == 1)
      break; //������
    if (key == 4) {
      res = 1;
      break;
    } //Ҫ����
    delay_ms(10);
  }
  LED0 = 1;
  POINT_COLOR = WHITE;
  return res;
}

//ϵͳ��ʼ��
void system_init(void) {
  u16 okoffset = 162;
  u16 ypos = 0;
  u16 j = 0;
  u16 temp = 0;
  u8 res;
  u32 dtsize, dfsize;
  u8 *stastr = 0;
  u8 *version = 0;
  u8 verbuf[12];
  u8 fsize;
  u8 icowidth;

  delay_init(168);   //��ʱ��ʼ��
  KEY_Init();       //������ʼ��
  uart_init(115200); //��ʼ�����ڲ�����Ϊ115200
  usart3_init(115200);
	
//  power.init();
  LED_Init(); //��ʼ��LED
  adc.init();

//  digit.init();
//  motor.init();

  LCD_Init(); // LCD��ʼ��


//  FSMC_SRAM_Init(); //��ʼ���ⲿSRAM
  W25QXX_Init();    //��ʼ��W25Q128

  my_mem_init(SRAMIN);  //��ʼ���ڲ��ڴ��
  my_mem_init(SRAMCCM); //��ʼ��CCM�ڴ��

  usbapp_init();
  exfuns_init(); // FATFS �����ڴ�

  version = mymalloc(SRAMIN, 31); //����31���ֽ��ڴ�
REINIT:                           //���³�ʼ��
  LCD_Clear(BLACK);               //����
  POINT_COLOR = WHITE;
  BACK_COLOR = BLACK;
  j = 0;

  ypos = 2;
  fsize = 24;
  icowidth = 36;
  okoffset = 370;

  LCD_ShowString(icowidth + 5 * 2, ypos + fsize * j++, lcddev.width,
                 lcddev.height, fsize, (unsigned char *)"ETdoor fixture ");
  LCD_ShowString(icowidth + 5 * 2, ypos + fsize * j++, lcddev.width,
                 lcddev.height, fsize,
                 (unsigned char *)"Copyright (C) 2018-2028");
  app_get_version(verbuf, HARDWARE_VERSION, 3);
  strcpy((char *)version, "HARDWARE:");
  strcat((char *)version, (const char *)verbuf);
  strcat((char *)version, ", SOFTWARE:");
  app_get_version(verbuf, SOFTWARE_VERSION, 3);
  strcat((char *)version, (const char *)verbuf);
  LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize,
                 version);
  sprintf((char *)verbuf, "LCD ID:%04X", lcddev.id); // LCD ID��ӡ��verbuf����
  LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize,
                 verbuf); //��ʾLCD ID

  LED0 = 0;
  LED1 = 0; //ͬʱ��������LED
  LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"CPU:STM32F407ZGT6 168Mhz");
  LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"FLASH:1024KB  SRAM:192KB");
/*  if (system_exsram_test(5, ypos + fsize * j, fsize))
    system_error_show(5, ypos + fsize * j++,
                      (unsigned char *)"EX Memory Error!", fsize);
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK");
  my_mem_init(SRAMEX); //��ʼ���ⲿ�ڴ��,��������ڴ���֮��*/
  LED0 = 1;
  LED1 = 1; //ͬʱ�ر�����LED

  if (W25QXX_ReadID() != W25Q128) //��ⲻ��W25Q128
  {
    system_error_show(5, ypos + fsize * j++,
                      (unsigned char *)"Ex Flash Error!!", fsize);
  } else
    temp = 16 * 1024; // 16M�ֽڴ�С
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"Ex Flash:     KB");
  LCD_ShowxNum(5 + 9 * (fsize / 2), ypos + fsize * j, temp, 5, fsize,
               0); //��ʾflash��С
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK");

  /****************************************�Ƿ����SPI FALSH*********************************************/
  res = KEY_Scan(1); //
  if (res == 3) {
    res = system_files_erase(5, ypos + fsize * j, fsize);
    if (res)
      goto REINIT;
  }

  /****************************************���SPI FALSH�ļ�ϵͳ*********************************************/
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"FATFS Check..."); // FATFS���
  f_mount(fs[1], "1:", 1);                           //���ع���FLASH.
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK");
  // W25Q128���,����������ļ�ϵͳ,���ȴ���.
  temp = 0;
  do {
    temp++;
    res = exf_getfree((unsigned char *)"1:", &dtsize,
                      &dfsize); //�õ�FLASHʣ��������������
    delay_ms(200);
  } while (res && temp < 20); //�������20��
  if (res == 0X0D)            //�ļ�ϵͳ������
  {
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"Flash Disk Formatting..."); //��ʽ��FLASH
    res = f_mkfs("1:", 1,
                 4096); //��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
    if (res == 0) {
      f_setlabel((const TCHAR *)"1:ALIENTEK"); //����Flash���̵�����Ϊ��ALIENTEK
      LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                     lcddev.height, fsize,
                     (unsigned char *)"OK"); //��־��ʽ���ɹ�
      res = exf_getfree((unsigned char *)"1:", &dtsize, &dfsize); //���»�ȡ����
    }
  }
  if (res == 0) //�õ�FLASH��ʣ��������������
  {
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"Flash Disk:     KB"); // FATFS���
    temp = dtsize;
  } else
    system_error_show(5, ypos + fsize * (j + 1),
                      (unsigned char *)"Flash Fat Error!",
                      fsize); // flash �ļ�ϵͳ����
  LCD_ShowxNum(5 + 11 * (fsize / 2), ypos + fsize * j, temp, 5, fsize,
               0); //��ʾFLASH������С
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK"); // FLASH��״̬

  /****************************************    U�̼��    *********************************************/
  usbapp_mode_set(0); //����ΪU��ģʽ
  temp = 0;
  while (usbx.hdevclass == 0 && temp < 1000) //�ȴ�U�̱����,���ȴ�5��
  {
    usbapp_pulling();
    if ((usbx.bDeviceState & (1 << 6)) == 0 && temp > 300)
      break; // 1.5����֮��,û�м�⵽�豸����,��ֱ������,���ٵȴ�
    delay_ms(5);
    temp++;
  }
  if (usbx.hdevclass == 1) //��⵽��U��
  {
    fs[2]->drv = 2;
    f_mount(fs[2], "2:", 1); //���ع���U��
    res = exf_getfree((unsigned char *)"2:", &dtsize,
                      &dfsize); //�õ�U��ʣ��������������
  } else
    res = 0XFF;
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"U Disk:     MB"); // U��������С
  if (res == 0)                                      //�õ���������
  {
    temp = dtsize >> 10; //��λת��ΪMB
    stastr = (unsigned char *)"OK";
  } else {
    temp = 0; //������,��λΪ0
    stastr = (unsigned char *)"ERROR";
  }
  LCD_ShowxNum(5 + 7 * (fsize / 2), ypos + fsize * j, temp, 5, fsize,
               0); //��ʾU��������С
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, stastr); // U��״̬

  /****************************************    �ֿ���    ********************************************/
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"Font Check...");
  res = KEY_Scan(1); //��ⰴ��
  if (res == 2)      //���£�ȷ��
  {
    res = system_font_update_confirm(5, ypos + fsize * (j + 1), fsize);
  } else
    res = 0;

  if (font_init() || (res == 1)) //�������,������岻����/ǿ�Ƹ���,������ֿ�
  {
    res = 0; //������Ч
    TIM2_Int_Init(100 - 1, 8400 - 1);
    if (update_font(5, ypos + fsize * j, fsize, (unsigned char *)"2:") !=
        0) //��U�̸���
    {
      system_error_show(5, ypos + fsize * (j + 1),
                        (unsigned char *)"Font Error!", fsize); //�������
    }
    TIM2->CR1 &= ~(1 << 0);
    LCD_Fill(5, ypos + fsize * j, lcddev.width, ypos + fsize * (j + 1),
             BLACK); //����ɫ
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"Font Check...");
  }
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK"); //�ֿ���OK

  /****************************************    ϵͳ�ļ�    ********************************************/
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"sysfile Check...");
  res = KEY_Scan(1); //��ⰴ��
  if (res == 1)      //���£�ȷ��
  {
    res = system_sysfile_update_confirm(5, ypos + fsize * (j + 1), fsize);
  } else
    res = 0;

  if (res == 0)
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"SYSTEM Files Check...");

  while (app_system_file_check((unsigned char *)"1") ||
         (res == 1)) //ϵͳ�ļ����
  {
    LCD_Fill(5, ypos + fsize * j, lcddev.width, ypos + fsize * (j + 1),
             BLACK); //����ɫ
    LCD_ShowString(5, ypos + fsize * j, (fsize / 2) * 8, fsize, fsize,
                   (unsigned char *)"Updating");     //��ʾupdating
    app_boot_cpdmsg_set(5, ypos + fsize * j, fsize); //���õ�����
    temp = 0;

    TIM2_Int_Init(100 - 1, 8400 - 1);
    if (app_system_file_check((unsigned char *)"2"))
      res = 9; //���Ϊ�����õ���
    else
      res = 2;                //���ΪU��
    if (res == 0 || res == 2) //�����˲Ÿ���
    {
      sprintf((char *)verbuf, "%d:", res);
      if (app_system_update(app_boot_cpdmsg, verbuf)) //����?
      {
        system_error_show(5, ypos + fsize * (j + 1),
                          (unsigned char *)"SYSTEM File Error!", fsize);
      }
    }
    TIM2->CR1 &= ~(1 << 0);
    LCD_Fill(5, ypos + fsize * j, lcddev.width, ypos + fsize * (j + 1),
             BLACK); //����ɫ
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"SYSTEM Files Check...");
    if (app_system_file_check((
            unsigned char
                *)"1")) //������һ�Σ��ټ�⣬������в�ȫ��˵��SD���ļ��Ͳ�ȫ��
    {
      system_error_show(5, ypos + fsize * (j + 1),
                        (unsigned char *)"SYSTEM File Lost!", fsize);
    } else {
      res = 0;
      break;
    }
  }
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK");

  //ϵͳ��������
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"SYSTEM Parameter Load...");
  if (app_system_parameter_init())
    system_error_show(5, ypos + fsize * (j + 1),
                      (unsigned char *)"Parameter Load Error!",
                      fsize); //��������
  else
    LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                   lcddev.height, fsize, (unsigned char *)"OK");
	
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"SYSTEM Starting...");
	Show_Str(30, 430, 200, 24, "�豸�����ɹ�!!", 24, 0);
  //�������̽�,��ʾ��������
  BEEP_Init(); //��������ʼ��
  POINT_COLOR = GREEN;
  BEEP_OPEN();
  delay_ms(100);
  BEEP_CLOSE();
  myfree(SRAMIN, version);
  delay_ms(1500);
}

// main����
int main(void) {

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
  system_init();                                  //ϵͳ��ʼ��
	

  OSInit();
  OSTaskCreate(start_task, (void *)0,
               (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1],
               START_TASK_PRIO); //������ʼ����
  OSStart();
}

//��ʼ����
void start_task(void *pdata) {
  OS_CPU_SR cpu_sr = 0;
  pdata = pdata;
  OSStatInit();        //��ʼ��ͳ������.�������ʱ1��������
                       // app_srand(OSTime);
  OS_ENTER_CRITICAL(); //�����ٽ���(�޷����жϴ��)
  OSTaskCreate(main_task, (void *)0,
               (OS_STK *)&MAIN_TASK_STK[MAIN_STK_SIZE - 1], MAIN_TASK_PRIO);
  OSTaskCreate(usart_task, (void *)0,
               (OS_STK *)&USART_TASK_STK[USART_STK_SIZE - 1], USART_TASK_PRIO);
  OSTaskCreate(watch_task, (void *)0,
               (OS_STK *)&WATCH_TASK_STK[WATCH_STK_SIZE - 1], WATCH_TASK_PRIO);
  OSTaskSuspend(START_TASK_PRIO); //������ʼ����.
  OS_EXIT_CRITICAL();             //�˳��ٽ���(���Ա��жϴ��)
}

//������
void main_task(void *pdata) {
  //	u8 selx;
  //	u16 tcnt=0;
  //	spb_init();			//��ʼ��SPB����
  //	spb_load_mui();		//����SPB������
  //	slcd_frame_show(0);	//��ʾ����
	ET_08_test_init();
  while (1u) {
		
    ET_08_test_app();
    delay_ms(10);
  }
}
//ִ�����ҪʱЧ�ԵĴ���
void usart_task(void *pdata) {
  pdata = pdata;
  while (1) {
    delay_ms(1000);
    app_lcd_auto_bklight(); //�Զ��������
//     printf("in:%d,ex:%d,ccm:%d\r\n",my_mem_perused(0),my_mem_perused(1),my_mem_perused(2));//��ӡ�ڴ�ռ����
    u3_printf("in:%d,ccm:%d\r\n",my_mem_perused(0),my_mem_perused(2));//��ӡ�ڴ�ռ����
  }
}

vu8 system_task_return; //����ǿ�Ʒ��ر�־.
//��������
void watch_task(void *pdata) {
  u8 t = 0;
  pdata = pdata;
  while (1) {
    if (t == 4)
      LED0 = 1;   //��100ms����
    if (t == 119) // 2.5����������һ��
    {
      LED0 = 0;
      t = 0;
    }
    t++;
    delay_ms(10);
  }
}
//Ӳ��������
void HardFault_Handler(void) {
  u32 i;
  u8 t = 0;
  u32 temp;
  temp = SCB->CFSR; // fault״̬�Ĵ���(@0XE000ED28)����:MMSR,BFSR,UFSR
  printf("CFSR:%8X\r\n", temp); //��ʾ����ֵ
  temp = SCB->HFSR;             //Ӳ��fault״̬�Ĵ���
  printf("HFSR:%8X\r\n", temp); //��ʾ����ֵ
  temp = SCB->DFSR;             //����fault״̬�Ĵ���
  printf("DFSR:%8X\r\n", temp); //��ʾ����ֵ
  temp = SCB->AFSR;             //����fault״̬�Ĵ���
  printf("AFSR:%8X\r\n", temp); //��ʾ����ֵ
  LED1 = !LED1;
  while (t < 5) {
    t++;
    LED0 = !LED0;
    // BEEP=!BEEP;
    for (i = 0; i < 0X1FFFFF; i++)
      ;
  }
}
