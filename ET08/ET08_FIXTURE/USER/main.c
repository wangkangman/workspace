#include "common.h"
#include "includes.h"
#include "malloc.h"
#include "sys.h"
#include "usb_app.h"
#include "usart3.h"
#include "text.h"


/////////////////////////UCOSII任务设置///////////////////////////////////
// START 任务
//设置任务优先级
#define START_TASK_PRIO 10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE 64
//任务堆栈，8字节对齐
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//串口任务
//设置任务优先级
#define USART_TASK_PRIO 7
//设置任务堆栈大小
#define USART_STK_SIZE 128
//任务堆栈，8字节对齐
__align(8) static OS_STK USART_TASK_STK[USART_STK_SIZE];
//任务函数
void usart_task(void *pdata);

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO 6
//设置任务堆栈大小
#define MAIN_STK_SIZE 1200
//任务堆栈，8字节对齐
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

//监视任务
//设置任务优先级
#define WATCH_TASK_PRIO 3
//设置任务堆栈大小
#define WATCH_STK_SIZE 256
//任务堆栈，8字节对齐
__align(8) static OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
//任务函数
void watch_task(void *pdata);

//外部内存测试(最大支持1M字节内存测试)
// x,y:坐标
// fsize:字体大小
//返回值:0,成功;1,失败.
u8 system_exsram_test(u16 x, u16 y, u8 fsize) {
  u32 i = 0;
  u16 temp = 0;
  u16 sval = 0; //在地址0读到的数据
  LCD_ShowString(x, y, lcddev.width, y + fsize, fsize,
                 (unsigned char *)"Ex Memory Test:   0KB");
  //每隔1K字节,写入一个数据,总共写入1024个数据,刚好是1M字节
  for (i = 0; i < 1024 * 1024; i += 1024) {
    FSMC_SRAM_WriteBuffer((u8 *)&temp, i, 2);
    temp++;
  }
  //依次读出之前写入的数据,进行校验
  for (i = 0; i < 1024 * 1024; i += 1024) {
    FSMC_SRAM_ReadBuffer((u8 *)&temp, i, 2);
    if (i == 0)
      sval = temp;
    else if (temp <= sval)
      break; //后面读出的数据一定要比第一次读到的数据大.
    LCD_ShowxNum(x + 15 * (fsize / 2), y, (u16)(temp - sval + 1), 4, fsize,
                 0); //显示内存容量
  }
  if (i >= 1024 * 1024) {
    LCD_ShowxNum(x + 15 * (fsize / 2), y, i / 1024, 4, fsize, 0); //显示内存值
    return 0; //内存正常,成功
  }
  return 1; //失败
}
//显示错误信息
// x,y:坐标
// fsize:字体大小
// x,y:坐标.err:错误信息
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
//擦除整个SPI FLASH(即所有资源都删除),以快速更新系统.
// x,y:坐标
// fsize:字体大小
// x,y:坐标.err:错误信息
//返回值:0,没有擦除;1,擦除了
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
    if (key == 1) //不擦除,用户取消了
    {
      POINT_COLOR = WHITE;
      LED0 = 1;
      return 0;
    }
    if (key == 4) //要擦除,要重新来过
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
//字库更新确认提示.
// x,y:坐标
// fsize:字体大小
//返回值:0,不需要更新;1,确认要更新
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
      break; //不更新
    if (key == 4) {
      res = 1;
      break;
    } //要更新
    delay_ms(10);
  }
  LED0 = 1;
  POINT_COLOR = WHITE;
  return res;
}

//系统文件更新确认提示.
// x,y:坐标
// fsize:字体大小
//返回值:0,不需要更新;1,确认要更新
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
      break; //不更新
    if (key == 4) {
      res = 1;
      break;
    } //要更新
    delay_ms(10);
  }
  LED0 = 1;
  POINT_COLOR = WHITE;
  return res;
}

//系统初始化
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

  delay_init(168);   //延时初始化
  KEY_Init();       //按键初始化
  uart_init(115200); //初始化串口波特率为115200
  usart3_init(115200);
	
//  power.init();
  LED_Init(); //初始化LED
  adc.init();

//  digit.init();
//  motor.init();

  LCD_Init(); // LCD初始化


//  FSMC_SRAM_Init(); //初始化外部SRAM
  W25QXX_Init();    //初始化W25Q128

  my_mem_init(SRAMIN);  //初始化内部内存池
  my_mem_init(SRAMCCM); //初始化CCM内存池

  usbapp_init();
  exfuns_init(); // FATFS 申请内存

  version = mymalloc(SRAMIN, 31); //申请31个字节内存
REINIT:                           //重新初始化
  LCD_Clear(BLACK);               //黑屏
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
  sprintf((char *)verbuf, "LCD ID:%04X", lcddev.id); // LCD ID打印到verbuf里面
  LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize,
                 verbuf); //显示LCD ID

  LED0 = 0;
  LED1 = 0; //同时点亮两个LED
  LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"CPU:STM32F407ZGT6 168Mhz");
  LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"FLASH:1024KB  SRAM:192KB");
/*  if (system_exsram_test(5, ypos + fsize * j, fsize))
    system_error_show(5, ypos + fsize * j++,
                      (unsigned char *)"EX Memory Error!", fsize);
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK");
  my_mem_init(SRAMEX); //初始化外部内存池,必须放在内存检测之后*/
  LED0 = 1;
  LED1 = 1; //同时关闭两个LED

  if (W25QXX_ReadID() != W25Q128) //检测不到W25Q128
  {
    system_error_show(5, ypos + fsize * j++,
                      (unsigned char *)"Ex Flash Error!!", fsize);
  } else
    temp = 16 * 1024; // 16M字节大小
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"Ex Flash:     KB");
  LCD_ShowxNum(5 + 9 * (fsize / 2), ypos + fsize * j, temp, 5, fsize,
               0); //显示flash大小
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK");

  /****************************************是否擦除SPI FALSH*********************************************/
  res = KEY_Scan(1); //
  if (res == 3) {
    res = system_files_erase(5, ypos + fsize * j, fsize);
    if (res)
      goto REINIT;
  }

  /****************************************检测SPI FALSH文件系统*********************************************/
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"FATFS Check..."); // FATFS检测
  f_mount(fs[1], "1:", 1);                           //挂载挂载FLASH.
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK");
  // W25Q128检测,如果不存在文件系统,则先创建.
  temp = 0;
  do {
    temp++;
    res = exf_getfree((unsigned char *)"1:", &dtsize,
                      &dfsize); //得到FLASH剩余容量和总容量
    delay_ms(200);
  } while (res && temp < 20); //连续检测20次
  if (res == 0X0D)            //文件系统不存在
  {
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"Flash Disk Formatting..."); //格式化FLASH
    res = f_mkfs("1:", 1,
                 4096); //格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
    if (res == 0) {
      f_setlabel((const TCHAR *)"1:ALIENTEK"); //设置Flash磁盘的名字为：ALIENTEK
      LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                     lcddev.height, fsize,
                     (unsigned char *)"OK"); //标志格式化成功
      res = exf_getfree((unsigned char *)"1:", &dtsize, &dfsize); //重新获取容量
    }
  }
  if (res == 0) //得到FLASH卡剩余容量和总容量
  {
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"Flash Disk:     KB"); // FATFS检测
    temp = dtsize;
  } else
    system_error_show(5, ypos + fsize * (j + 1),
                      (unsigned char *)"Flash Fat Error!",
                      fsize); // flash 文件系统错误
  LCD_ShowxNum(5 + 11 * (fsize / 2), ypos + fsize * j, temp, 5, fsize,
               0); //显示FLASH容量大小
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK"); // FLASH卡状态

  /****************************************    U盘检测    *********************************************/
  usbapp_mode_set(0); //设置为U盘模式
  temp = 0;
  while (usbx.hdevclass == 0 && temp < 1000) //等待U盘被检测,最多等待5秒
  {
    usbapp_pulling();
    if ((usbx.bDeviceState & (1 << 6)) == 0 && temp > 300)
      break; // 1.5秒钟之内,没有检测到设备插入,则直接跳过,不再等待
    delay_ms(5);
    temp++;
  }
  if (usbx.hdevclass == 1) //检测到了U盘
  {
    fs[2]->drv = 2;
    f_mount(fs[2], "2:", 1); //挂载挂载U盘
    res = exf_getfree((unsigned char *)"2:", &dtsize,
                      &dfsize); //得到U盘剩余容量和总容量
  } else
    res = 0XFF;
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"U Disk:     MB"); // U盘容量大小
  if (res == 0)                                      //得到容量正常
  {
    temp = dtsize >> 10; //单位转换为MB
    stastr = (unsigned char *)"OK";
  } else {
    temp = 0; //出错了,单位为0
    stastr = (unsigned char *)"ERROR";
  }
  LCD_ShowxNum(5 + 7 * (fsize / 2), ypos + fsize * j, temp, 5, fsize,
               0); //显示U盘容量大小
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, stastr); // U盘状态

  /****************************************    字库检测    ********************************************/
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"Font Check...");
  res = KEY_Scan(1); //检测按键
  if (res == 2)      //更新？确认
  {
    res = system_font_update_confirm(5, ypos + fsize * (j + 1), fsize);
  } else
    res = 0;

  if (font_init() || (res == 1)) //检测字体,如果字体不存在/强制更新,则更新字库
  {
    res = 0; //按键无效
    TIM2_Int_Init(100 - 1, 8400 - 1);
    if (update_font(5, ypos + fsize * j, fsize, (unsigned char *)"2:") !=
        0) //从U盘更新
    {
      system_error_show(5, ypos + fsize * (j + 1),
                        (unsigned char *)"Font Error!", fsize); //字体错误
    }
    TIM2->CR1 &= ~(1 << 0);
    LCD_Fill(5, ypos + fsize * j, lcddev.width, ypos + fsize * (j + 1),
             BLACK); //填充底色
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"Font Check...");
  }
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, (unsigned char *)"OK"); //字库检测OK

  /****************************************    系统文件    ********************************************/
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"sysfile Check...");
  res = KEY_Scan(1); //检测按键
  if (res == 1)      //更新？确认
  {
    res = system_sysfile_update_confirm(5, ypos + fsize * (j + 1), fsize);
  } else
    res = 0;

  if (res == 0)
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"SYSTEM Files Check...");

  while (app_system_file_check((unsigned char *)"1") ||
         (res == 1)) //系统文件检测
  {
    LCD_Fill(5, ypos + fsize * j, lcddev.width, ypos + fsize * (j + 1),
             BLACK); //填充底色
    LCD_ShowString(5, ypos + fsize * j, (fsize / 2) * 8, fsize, fsize,
                   (unsigned char *)"Updating");     //显示updating
    app_boot_cpdmsg_set(5, ypos + fsize * j, fsize); //设置到坐标
    temp = 0;

    TIM2_Int_Init(100 - 1, 8400 - 1);
    if (app_system_file_check((unsigned char *)"2"))
      res = 9; //标记为不可用的盘
    else
      res = 2;                //标记为U盘
    if (res == 0 || res == 2) //完整了才更新
    {
      sprintf((char *)verbuf, "%d:", res);
      if (app_system_update(app_boot_cpdmsg, verbuf)) //更新?
      {
        system_error_show(5, ypos + fsize * (j + 1),
                          (unsigned char *)"SYSTEM File Error!", fsize);
      }
    }
    TIM2->CR1 &= ~(1 << 0);
    LCD_Fill(5, ypos + fsize * j, lcddev.width, ypos + fsize * (j + 1),
             BLACK); //填充底色
    LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                   (unsigned char *)"SYSTEM Files Check...");
    if (app_system_file_check((
            unsigned char
                *)"1")) //更新了一次，再检测，如果还有不全，说明SD卡文件就不全！
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

  //系统参数加载
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"SYSTEM Parameter Load...");
  if (app_system_parameter_init())
    system_error_show(5, ypos + fsize * (j + 1),
                      (unsigned char *)"Parameter Load Error!",
                      fsize); //参数加载
  else
    LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                   lcddev.height, fsize, (unsigned char *)"OK");
	
  LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize,
                 (unsigned char *)"SYSTEM Starting...");
	Show_Str(30, 430, 200, 24, "设备启动成功!!", 24, 0);
  //蜂鸣器短叫,提示正常启动
  BEEP_Init(); //蜂鸣器初始化
  POINT_COLOR = GREEN;
  BEEP_OPEN();
  delay_ms(100);
  BEEP_CLOSE();
  myfree(SRAMIN, version);
  delay_ms(1500);
}

// main函数
int main(void) {

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
  system_init();                                  //系统初始化
	

  OSInit();
  OSTaskCreate(start_task, (void *)0,
               (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1],
               START_TASK_PRIO); //创建起始任务
  OSStart();
}

//开始任务
void start_task(void *pdata) {
  OS_CPU_SR cpu_sr = 0;
  pdata = pdata;
  OSStatInit();        //初始化统计任务.这里会延时1秒钟左右
                       // app_srand(OSTime);
  OS_ENTER_CRITICAL(); //进入临界区(无法被中断打断)
  OSTaskCreate(main_task, (void *)0,
               (OS_STK *)&MAIN_TASK_STK[MAIN_STK_SIZE - 1], MAIN_TASK_PRIO);
  OSTaskCreate(usart_task, (void *)0,
               (OS_STK *)&USART_TASK_STK[USART_STK_SIZE - 1], USART_TASK_PRIO);
  OSTaskCreate(watch_task, (void *)0,
               (OS_STK *)&WATCH_TASK_STK[WATCH_STK_SIZE - 1], WATCH_TASK_PRIO);
  OSTaskSuspend(START_TASK_PRIO); //挂起起始任务.
  OS_EXIT_CRITICAL();             //退出临界区(可以被中断打断)
}

//主任务
void main_task(void *pdata) {
  //	u8 selx;
  //	u16 tcnt=0;
  //	spb_init();			//初始化SPB界面
  //	spb_load_mui();		//加载SPB主界面
  //	slcd_frame_show(0);	//显示界面
	ET_08_test_init();
  while (1u) {
		
    ET_08_test_app();
    delay_ms(10);
  }
}
//执行最不需要时效性的代码
void usart_task(void *pdata) {
  pdata = pdata;
  while (1) {
    delay_ms(1000);
    app_lcd_auto_bklight(); //自动背光控制
//     printf("in:%d,ex:%d,ccm:%d\r\n",my_mem_perused(0),my_mem_perused(1),my_mem_perused(2));//打印内存占用率
    u3_printf("in:%d,ccm:%d\r\n",my_mem_perused(0),my_mem_perused(2));//打印内存占用率
  }
}

vu8 system_task_return; //任务强制返回标志.
//监视任务
void watch_task(void *pdata) {
  u8 t = 0;
  pdata = pdata;
  while (1) {
    if (t == 4)
      LED0 = 1;   //亮100ms左右
    if (t == 119) // 2.5秒钟左右亮一次
    {
      LED0 = 0;
      t = 0;
    }
    t++;
    delay_ms(10);
  }
}
//硬件错误处理
void HardFault_Handler(void) {
  u32 i;
  u8 t = 0;
  u32 temp;
  temp = SCB->CFSR; // fault状态寄存器(@0XE000ED28)包括:MMSR,BFSR,UFSR
  printf("CFSR:%8X\r\n", temp); //显示错误值
  temp = SCB->HFSR;             //硬件fault状态寄存器
  printf("HFSR:%8X\r\n", temp); //显示错误值
  temp = SCB->DFSR;             //调试fault状态寄存器
  printf("DFSR:%8X\r\n", temp); //显示错误值
  temp = SCB->AFSR;             //辅助fault状态寄存器
  printf("AFSR:%8X\r\n", temp); //显示错误值
  LED1 = !LED1;
  while (t < 5) {
    t++;
    LED0 = !LED0;
    // BEEP=!BEEP;
    for (i = 0; i < 0X1FFFFF; i++)
      ;
  }
}
