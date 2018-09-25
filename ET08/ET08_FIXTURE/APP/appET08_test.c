#include "appET08_test.h"
#include "os_cpu.h"
#include "slave_command.h"
#include "string_process.h"
#include "text.h"
#include "swim.h"

u8 *unit[] = {"mA", "v", "v", "v", "v", "v"};

u8 *ad_display_str[] = {
    "总电流:", "主电压:", "电池电压:", "24v电压:", "警示灯电压:", "5v电压:"};

u8 *section_display_str[] = {"P侧供电", "N侧供电", "电池供电"};

u8 *ad_section_str[] = {"main_P", "main_N", "battery"};

u8 *ad_key_str[] = {"current",     "voltage_main", "voltage_bat",
                    "voltage_24v", "voltage_lamp", "voltage_5v"};

const u8 *str_type[] = {"standard", "mutifunction"};

const u8 *str_type_display[] = {"标准版", "多功能版"};



static void test_init(void);
static void test_bin_set(void);
static void test_board_set(void);
static void test_standby(void);
static void test_power(void);
static void test_program(void);
static void test_signal(void);
static void test_seg(void);
static void test_button(void);
static void test_rf(void);
static void test_358_pwm(void);
static void test_motor(void);
static void app_program(void);
static void test_final(void);

static void standby_init(u8 relay);
void show_value_float(u8 *string, float val, u8 *unit, u16 f_color);
void show_signal_res(u8 *string, int res);
void show_seg_val(u8 *string, int val, int piont);
void show_motor_res(u8 x, u8 *string, u8 res);
static u8 check_sum_bin(u8 *path);
static u32 get_checksum_from_name(u8 *path);

TEST_ST tcb_test;

u8 plcd[100];
static u16 j = 0;
static u16 ypos = 2;
static u8 fsize = 24;
static u16 okoffset = 370;

// static u16 ypos_mark;
FUNC test_func[] = {test_init,   test_bin_set, test_board_set, test_standby,
                    test_power,  test_program, test_signal,    test_seg,
                    test_button, test_rf,      test_358_pwm,   test_motor,
                    app_program, test_final};

void ET_08_test_init(void){
  u8 *pdata;
	u16 index;
  pdata = (u8*)&tcb_test;
	
	for (index = 0; index < sizeof(TEST_ST); index++) {
	  pdata[index] = 0;
	}
}


#if 1
void ET_08_test_app(void) { test_func[tcb_test.cur_state](); }
#endif

#if 0
void ET_08_test_app(void) { 
  u8 button_res;

  digit.init();
  power.init();  
  while (1){
		button_res = KEY_Scan(0);

		if (1 == button_res) {
			power.ctrl(POW_ON);
		}
		else if (2 == button_res) {
			power.ctrl(POW_OFF);
		}
		else if (3 == button_res) {
			power.level(LEVEL_HIGH);
		}
		else if (4 == button_res) {
			power.level(LEVEL_LOW);
		}
		delay_ms(10);
  }
}
#endif

static void test_init(void) {
  FIL *f_txt;
  u8 *val;
  u8 *buf;
  u8 *text;
  TCHAR *path = "1:/SYSTEM/CONFIG/config.txt";
  u8 rval;
  u8 error;
  u8 index_section, index_key, button_res;


  if (0 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = WHITE;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    /********** swim port initial ***********/
    SWIM_GPIO_Init();
    /********** 输出端口初始化  **************/
    digit.init();
    /********** 低电压控制初始化**************/
    power.init();
    /********** 电机检测端口初始化**************/
    motor.init();
    /********** pwm采样端口初始化**************/
    pwm.init();

    f_txt = memin_malloc(sizeof(FIL));
    val = memin_malloc(100);
    buf = memin_malloc(100);
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "测试参数初始化...", 24,
             0);

    rval = f_open(f_txt, (const TCHAR *)path, FA_READ);
    text = memin_malloc(f_txt->fsize + 1);
    mymemset(text, 0, f_txt->fsize + 1);
    rval = f_read(f_txt, text, f_txt->fsize, (UINT *)&br);
    if (0 == rval)
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "配置文件打开成功...",
               24, 0);
    else {
      error = 0x01;
      goto ERROR;
    }

    /*----------------------GET value from configuration file
     * ------------------------*/
    rval = GetSectionVal(text, "information", "object", val);
    if (0 != rval) {
      error = 0x02;
      goto ERROR;
    } else {
      sprintf((char*)tcb_test.object, "%s", (char*)val);
      Show_Str(5, ypos + fsize * j, lcddev.width, 24, "项目:", 24, 0);
      POINT_COLOR = GREEN;
      Show_Str(190, ypos + fsize * j++, lcddev.width, 24, tcb_test.object, 24,
               0);
      POINT_COLOR = WHITE;
    }

    rval = GetSectionVal(text, "information", "testfirm", val);
    if (0 != rval) {
      error = 0x02;
      goto ERROR;
    } else {
      sprintf((char*)tcb_test.test_firm, "%s", (char*)val);
      Show_Str(5, ypos + fsize * j, lcddev.width, 24, "测试程序:", 24, 0);
      POINT_COLOR = GREEN;
      Show_Str(190, ypos + fsize * j++, lcddev.width, 24, tcb_test.test_firm,
               24, 0);
      POINT_COLOR = WHITE;
    }

    for (index_section = 0; index_section < 3; index_section++) {
      sprintf((char *)buf, "[%s]", section_display_str[index_section]);
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24, buf, 24, 0);
      POINT_COLOR = WHITE;
      for (index_key = 0; index_key < 6; index_key++) {
        rval = GetSectionVal(text, ad_section_str[index_section],
                             ad_key_str[index_key], val);
        if (0 != rval) {
          error = 0x02;
          goto ERROR;
        }

        rval = GetLimitVal(
            val,
            &(((SUB_RATIO *)(((u32)&tcb_test.ratio) +
                             (index_section * sizeof(TEST_AD_RATIO)) +
                             (index_key * sizeof(SUB_RATIO))))
                  ->low),
            &(((SUB_RATIO *)(((u32)&tcb_test.ratio) +
                             (index_section * sizeof(TEST_AD_RATIO)) +
                             (index_key * sizeof(SUB_RATIO))))
                  ->upper));
        if (0 != rval) {
          error = 0x02;
          goto ERROR;
        }

        sprintf((char *)buf, "%0.2f%s - %0.2f%s",
                ((SUB_RATIO *)(((u32)&tcb_test.ratio) +
                               (index_section * sizeof(TEST_AD_RATIO)) +
                               (index_key * sizeof(SUB_RATIO))))
                    ->low,
                unit[index_key],
                ((SUB_RATIO *)(((u32)&tcb_test.ratio) +
                               (index_section * sizeof(TEST_AD_RATIO)) +
                               (index_key * sizeof(SUB_RATIO))))
                    ->upper,
                unit[index_key]);
        Show_Str(5, ypos + fsize * j, lcddev.width, 24,
                 ad_display_str[index_key], 24, 0);
        POINT_COLOR = GREEN;
        Show_Str(190, ypos + fsize * j++, lcddev.width, 24, buf, 24, 0);
        POINT_COLOR = WHITE;
      }
    }

    f_close(f_txt);
    memin_free(text);
    memin_free(val);
    memin_free(buf);
    memin_free(f_txt);

    tcb_test.sub_state = 1;
    return;
  } else if (1 == tcb_test.sub_state) {
    POINT_COLOR = YELLOW;
    Show_Str(30, ypos + fsize * (j + 3), lcddev.width, 24, "数据正确请按KEY4",
             24, 0);
    tcb_test.sub_state = 2;
    return;
  } else if (2 == tcb_test.sub_state) {
    button_res = KEY_Scan(0);
    if (4 == button_res) {
      tcb_test.cur_state = TEST_BIN_SET;
      tcb_test.sub_state = 0;
      BEEP_OPEN();
      delay_ms(100);
      BEEP_CLOSE();
    }
    return;
  }

ERROR:
  f_close(f_txt);
  memin_free(text);
  memin_free(val);
  memin_free(buf);
  memin_free(f_txt);

  POINT_COLOR = RED;
  if (0x01 == error) {
    Show_Str(30, ypos + fsize * j++, lcddev.width, 24, "配置文件打开失败", 24,
             0);
  }
  if (0x02 == error) {
    Show_Str(30, ypos + fsize * j++, lcddev.width, 24, "参数初始化失败", 24, 0);
  }

  Show_Str(30, ypos + fsize * j++, lcddev.width, 24, "请更新系统文件", 24, 0);
  tcb_test.sub_state = 8;
  while (1)
    ;
}

static void test_bin_set(void) {
  FIL *f_txt;
  u8 *buf;
  u8 *temp_buf;
  TCHAR *dir_path = "1:/SYSTEM/APPBIN";
  TCHAR *config_file_path = "1:/SYSTEM/CONFIG/download_bin.txt";
  u8 rval = 0;
  DIR *srcdir = 0;    //源目录
  FILINFO *finfo = 0; //文件信息
  BIN_ST *current_list, *free_temp;
  u32 index;
  u8 button_res;
  u8 *load_path;

  if (0 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = WHITE;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "请选择出货程序", 24, 0);
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "上:KEY2  下:KEY3  确认:KEY4", 24, 0);
    j++;
    POINT_COLOR = WHITE;
    tcb_test.sub_state = 1;
  }
  // strcmp
  else if (1 == tcb_test.sub_state) {
    /***************************   读取配置文件  ***************************/
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "读取配置信息", 24, 0);
    f_txt = memin_malloc(sizeof(FIL));
    rval = f_open(f_txt, (const TCHAR *)config_file_path, FA_READ);
    buf = memin_malloc(f_txt->fsize);
    rval = f_read(f_txt, buf, f_txt->fsize, (UINT *)&br);
    if (0 != rval) {
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
               "配置文件读取失败，请更新系统文件", 24, 0);
      f_close(f_txt);
      memin_free(buf);
      memin_free(f_txt);
      while (1)
        ;
    }
    /********* 初始化文件，尾部加结束符*************/
    temp_buf = buf;
    while (*temp_buf != 0x20 && *temp_buf != 0x0)
      temp_buf++;
    *temp_buf = 0x0;
    /********* 复制字符串到全局结构体中 ************/
    tcb_test.current_bin = memin_malloc(100);
    mymemset(tcb_test.current_bin, 0, 100);
    strcpy((char*)tcb_test.current_bin, (char*)buf);
    tcb_test.bin_config_txt = memin_malloc(100);
    mymemset(tcb_test.bin_config_txt, 0, 100);
    strcpy((char*)tcb_test.bin_config_txt, (char*)buf);
    /****************释放全部资源******************/
    f_close(f_txt);
    memin_free(buf);
    memin_free(f_txt);

    /************读取bin文件夹下信息****************/
    srcdir = (DIR *)mymalloc(SRAMIN, sizeof(DIR)); //申请内存
    finfo = (FILINFO *)mymalloc(SRAMIN, sizeof(FILINFO));
    finfo->lfsize = _MAX_LFN * 2 + 1;
    finfo->lfname = mymalloc(SRAMIN, finfo->lfsize);   //申请内存
    rval = f_opendir(srcdir, (const TCHAR *)dir_path); //打开源目录
    if (0 != rval) {
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
               "程序目录打开失败，请更新系统文件", 24, 0);
      while (1)
        ;
    }

    tcb_test.list_bin = memin_malloc(sizeof(BIN_ST));
    tcb_test.list_bin->pre = NULL;
    tcb_test.list_bin->next = NULL;
    current_list = tcb_test.list_bin;
    do {
      rval = f_readdir(srcdir, finfo);
      if (finfo->fname[0] != 0) {
        current_list->name = memin_malloc(strlen(finfo->lfname) + 1);
        strcpy((char*)current_list->name, (char*)finfo->lfname);
        current_list->next = memin_malloc(sizeof(BIN_ST));
        current_list->next->pre = current_list;
        current_list = current_list->next;
      } else {
        if (NULL != current_list->pre) {
          current_list = current_list->pre;
          memin_free(current_list->next);
          current_list->next = NULL;
        } else {
          memin_free(tcb_test.list_bin);
          tcb_test.list_bin = NULL;
        }
        break;
      }
    } while (rval == 0);

    if (NULL == tcb_test.list_bin) {
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
               "程序目录为空，请更新系统文件", 24, 0);
      while (1)
        ;
    }
    /*************切换至下一阶段******************/
    tcb_test.sub_state = 2;
    return;
  } else if (2 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = WHITE;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "请选择出货程序", 24, 0);
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "上:KEY2  下:KEY3  确认:KEY4", 24, 0);
    j++;
    POINT_COLOR = WHITE;

    current_list = tcb_test.list_bin;
    rval = 0;
    index = j;
    while (NULL != current_list) {
      POINT_COLOR = WHITE;
      if (0 == strcmp((char*)current_list->name, (char*)tcb_test.current_bin)) {
        POINT_COLOR = GREEN;
        rval = 1;
      }
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24, current_list->name, 24,
               0);
      current_list = current_list->next;
    }
    if (0 == rval) {
      POINT_COLOR = GREEN;
      strcpy((char*)tcb_test.current_bin, (char*)tcb_test.list_bin->name);
      Show_Str(5, ypos + fsize * index, lcddev.width, 24, tcb_test.current_bin,
               24, 0);
    }

    /*************切换至下一阶段******************/
    tcb_test.sub_state = 3;
    return;
  } else if (3 == tcb_test.sub_state) {
    button_res = KEY_Scan(0);
    if (2 == button_res) {
      current_list = tcb_test.list_bin;
      while (0 != strcmp((char*)current_list->name, (char*)tcb_test.current_bin)) {
        current_list = current_list->next;
      }
      if (NULL != current_list->pre) {
        strcpy((char*)tcb_test.current_bin, (char*)current_list->pre->name);
        tcb_test.sub_state = 2;
        return;
      }
    } else if (3 == button_res) {
      current_list = tcb_test.list_bin;
      while (0 != strcmp((char*)current_list->name, (char*)tcb_test.current_bin)) {
        current_list = current_list->next;
      }
      if (NULL != current_list->next) {
        strcpy((char*)tcb_test.current_bin, (char*)current_list->next->name);
        tcb_test.sub_state = 2;
        return;
      }
    } else if (4 == button_res) {
      tcb_test.sub_state = 4;
      return;
    }
  } else if (4 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = GREEN;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "更新配置文件...", 24, 0);
    delay_ms(100);
    if (0 != strcmp((char*)tcb_test.bin_config_txt, (char*)tcb_test.current_bin)) {
      f_txt = memin_malloc(sizeof(FIL));
      rval = f_open(f_txt, (const TCHAR *)config_file_path, FA_WRITE);
      if (0 != rval) {
        POINT_COLOR = RED;
        Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "配置文件无法写入",
                 24, 0);
        while (1)
          ;
      }
      strcpy((char*)tcb_test.bin_config_txt, (char*)tcb_test.current_bin);
      f_write(f_txt, tcb_test.bin_config_txt, 50, (UINT *)&br);
      f_close(f_txt);
      memin_free(f_txt);
    }
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "配置文件更新成功", 24,
             0);
    delay_ms(100);

    /***********释放bin程序链表****************/
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "释放系统资源...", 24, 0);
    delay_ms(100);
    current_list = tcb_test.list_bin;
    while (current_list != NULL) {
      memin_free(current_list->name);
      free_temp = current_list->next;
      memin_free(current_list);
      current_list = free_temp;
    }
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "资源释放成功", 24, 0);
    delay_ms(100);

    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "加载测试程序...", 24, 0);
    delay_ms(100);
    /*****************加载bin文件*************/
    load_path = memin_malloc(100);
    sprintf((char*)load_path, "1:/SYSTEM/TESTBIN/%s", (char*)tcb_test.test_firm);
    rval = check_sum_bin(load_path);
    if (0 != rval) {
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
               "测试程序损坏，请更新系统文件", 24, 0);
      memin_free(load_path);
      while (1)
        ;
    }
    POINT_COLOR = GREEN;
    tcb_test.testbin = memin_malloc(strlen((char*)load_path) + 1);
    strcpy((char*)tcb_test.testbin, (char*)load_path);
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "测试程序加载成功", 24,
             0);
    delay_ms(100);

    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "加载出货程序...", 24, 0);
    delay_ms(100);
    sprintf((char*)load_path, "1:/SYSTEM/APPBIN/%s", (char*)tcb_test.current_bin);
    rval = check_sum_bin(load_path);
    if (0 != rval) {
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
               "出货程序损坏，请更新系统文件", 24, 0);
      memin_free(load_path);
      while (1)
        ;
    }
    POINT_COLOR = GREEN;
    tcb_test.appbin = memin_malloc(strlen((char*)load_path) + 1);
    strcpy((char*)tcb_test.appbin, (char*)load_path);
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "出货程序加载成功", 24,
             0);

    memin_free(load_path);
    tcb_test.cur_state = TEST_BOARD_SET;
    tcb_test.sub_state = 0;
    BEEP_OPEN();
    delay_ms(100);
    BEEP_CLOSE();



  }
}

#if 0
static void test_board_set(void) {
    tcb_test.type = MUTIFUNC;
    tcb_test.cur_state = TEST_STANDBY;
    tcb_test.sub_state = 0;
    return;
}
#endif

#if 1
static void test_board_set(void) {
  FIL *f_txt;
  u8 *buf;
  u8 *temp_buf;
  TCHAR *config_file_path = "1:/SYSTEM/CONFIG/board_type.txt";
  u8 rval = 0;
  u8 index_1, index_2;
  u8 button_res;

  if (0 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = WHITE;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "请选择基板型号", 24, 0);
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "上:KEY2  下:KEY3  确认:KEY4", 24, 0);
    j++;
    POINT_COLOR = WHITE;
    tcb_test.sub_state = 1;
    return;
  } else if (1 == tcb_test.sub_state) {
    /***************************   读取配置文件  ***************************/
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "读取配置信息", 24, 0);
    f_txt = memin_malloc(sizeof(FIL));
    rval = f_open(f_txt, (const TCHAR *)config_file_path, FA_READ);
    buf = memin_malloc(f_txt->fsize);
    rval = f_read(f_txt, buf, f_txt->fsize, (UINT *)&br);
    if (0 != rval) {
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
               "配置文件读取失败，请更新系统文件", 24, 0);
      f_close(f_txt);
      memin_free(buf);
      memin_free(f_txt);
      while (1)
        ;
    }
    /********* 初始化文件，尾部加结束符*************/
    temp_buf = buf;
    while (*temp_buf != 0x20 && *temp_buf != 0x0)
      temp_buf++;
    *temp_buf = 0x0;
    /********* 复制字符串到全局结构体中 ************/
    tcb_test.current_type = memin_malloc(100);
    mymemset(tcb_test.current_type, 0, 100);
    strcpy((char*)tcb_test.current_type, (char*)buf);
    tcb_test.type_config_txt = memin_malloc(100);
    mymemset(tcb_test.type_config_txt, 0, 100);
    strcpy((char*)tcb_test.type_config_txt, (char*)buf);
    /****************释放全部资源******************/
    f_close(f_txt);
    memin_free(buf);
    memin_free(f_txt);
    tcb_test.sub_state = 2;
    return;
  } else if (2 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = WHITE;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "请选择基板型号", 24, 0);
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "上:KEY2  下:KEY3  确认:KEY4", 24, 0);
    j++;
    POINT_COLOR = WHITE;

    rval = 0;
    index_2 = j;
    for (index_1 = 0; index_1 < 2; index_1++) {
      POINT_COLOR = WHITE;
      if (0 == strcmp((char*)str_type[index_1], (char*)tcb_test.current_type)) {
        POINT_COLOR = GREEN;
        rval = 1;
      }
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
               (u8*)str_type_display[index_1], 24, 0);
    }
    if (0 == rval) {
      POINT_COLOR = GREEN;
      strcpy((char*)tcb_test.current_type, (char*)str_type[0]);
      Show_Str(5, ypos + fsize * index_2, lcddev.width, 24, (u8*)str_type_display[0],
               24, 0);
    }
    tcb_test.sub_state = 3;
    return;
  } else if (3 == tcb_test.sub_state) {
    button_res = KEY_Scan(0);
    if (2 == button_res) {
      if (0 == strcmp((char*)str_type[1], (char*)tcb_test.current_type)) {
        strcpy((char*)tcb_test.current_type, (char*)str_type[0]);
        tcb_test.sub_state = 2;
        return;
      }
    } else if (3 == button_res) {
      if (0 == strcmp((char*)str_type[0], (char*)tcb_test.current_type)) {
        strcpy((char*)tcb_test.current_type, (char*)str_type[1]);
        tcb_test.sub_state = 2;
        return;
      }
    } else if (4 == button_res) {
      tcb_test.sub_state = 4;
      return;
    }
  } else if (4 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = GREEN;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "更新配置文件...", 24, 0);
    delay_ms(800);

    if (0 != strcmp((char*)tcb_test.type_config_txt, (char*)tcb_test.current_type)) {
      f_txt = memin_malloc(sizeof(FIL));
      rval = f_open(f_txt, (const TCHAR *)config_file_path, FA_WRITE);
      if (0 != rval) {
        POINT_COLOR = RED;
        Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "配置文件无法写入",
                 24, 0);
        while (1)
          ;
      }
      strcpy((char*)tcb_test.type_config_txt, (char*)tcb_test.current_type);
      f_write(f_txt, tcb_test.type_config_txt, 50, (UINT *)&br);
      f_close(f_txt);
      memin_free(f_txt);
    }
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "配置文件更新成功", 24,
             0);

    if (0 == strcmp((const char*)str_type[0], (const char*)tcb_test.current_type))
      tcb_test.type = STANDARD;
    else if (0 == strcmp((const char*)str_type[1], (const char*)tcb_test.current_type))
      tcb_test.type = MUTIFUNC;
    else {
      POINT_COLOR = RED;
      Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "产品类型无效", 24, 0);
      tcb_test.type = UNKNOW;
      while (1)
        ;
    }

    tcb_test.cur_state = TEST_STANDBY;
    tcb_test.sub_state = 0;
    BEEP_OPEN();
    delay_ms(100);
    BEEP_CLOSE();
  }
}
#endif

static void test_standby(void) {
  if (0 == tcb_test.sub_state) {
    standby_init(1);
    tcb_test.sub_state = 1;
  } else if (1 == tcb_test.sub_state) {
    POINT_COLOR = MAGENTA;
    j++;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "请确认以上信息是否正确，", 24, 0);
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "并核对出货程序版本。",
             24, 0);
    j++;
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "如信息全部正确且基板已正确放置，", 24, 0);
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "按KEY1启动测试程序。",
             24, 0);
    tcb_test.sub_state = 2;
  } else if (2 == tcb_test.sub_state) {
    if (1 == KEY_Scan(0)) {
      #if 1
      tcb_test.cur_state = TEST_POWER;
      #else
      tcb_test.cur_state = TEST_358_PWM;
      
      power.ctrl(POW_OFF);
      delay_ms(200);
      power.bat_main(CH_MAIN);
      delay_ms(200);
      power.ac_channel(CH_P);
      delay_ms(200);
      power.level(HIGH_POWER);
      delay_ms(200); 
      power.ctrl(POW_ON);
      delay_ms(500);
      #endif
    
      tcb_test.sub_state = 0;
    }
  }
}

static void test_power(void) {
  u16 index;
  float val, low, upper;

  if (0 == tcb_test.sub_state) {
    POINT_COLOR = WHITE;
    BACK_COLOR = BLACK;
    tcb_test.result.AD_falg = 0;
    tcb_test.sub_state = 1;
    return;
  } else if (1 == tcb_test.sub_state) {
    standby_init(0);
    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "[P侧供电测试]", 24, 0);
             
    POINT_COLOR = WHITE;
    power.ctrl(POW_OFF);
    delay_ms(50);
    power.bat_main(CH_MAIN);
    delay_ms(50);
    power.ac_channel(CH_P);
    delay_ms(50);
    power.level(HIGH_POWER);
    delay_ms(50); 
    power.ctrl(POW_ON);
    delay_ms(100);

    adc.get(&tcb_test.result.main_ch_p);

    for (index = 0; index < 6; index++) {
      if (4 == index)
        continue;
      val = *(
          (float *)(((u32)&tcb_test.result.main_ch_p) + index * sizeof(float)));
      low = ((SUB_RATIO *)(((u32)&tcb_test.ratio.main_ch_p) +
                           (index * sizeof(SUB_RATIO))))
                ->low;
      upper = ((SUB_RATIO *)(((u32)&tcb_test.ratio.main_ch_p) +
                             (index * sizeof(SUB_RATIO))))
                  ->upper;

      if (val > low && val < upper)
        show_value_float(ad_display_str[index], val, unit[index], GREEN);
      else {
        show_value_float(ad_display_str[index], val, unit[index], RED);
        tcb_test.result.AD_falg++;

      }
    }

    delay_ms(50);
    tcb_test.sub_state = 2;
    return;
  } else if (2 == tcb_test.sub_state) {

    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "[N侧供电测试]", 24, 0);
    POINT_COLOR = WHITE;
    power.ctrl(POW_OFF);
    delay_ms(50);
    power.bat_main(CH_MAIN);
    delay_ms(50);
    power.ac_channel(CH_N);
    delay_ms(50);
    power.level(HIGH_POWER);
    delay_ms(50);
    power.ctrl(POW_ON);
    delay_ms(100);

    adc.get(&tcb_test.result.main_ch_n);
    for (index = 0; index < 6; index++) {
      if (4 == index)
        continue;
      val = *(
          (float *)(((u32)&tcb_test.result.main_ch_n) + index * sizeof(float)));
      low = ((SUB_RATIO *)(((u32)&tcb_test.ratio.main_ch_n) +
                           (index * sizeof(SUB_RATIO))))
                ->low;
      upper = ((SUB_RATIO *)(((u32)&tcb_test.ratio.main_ch_n) +
                             (index * sizeof(SUB_RATIO))))
                  ->upper;

      if (val > low && val < upper)
        show_value_float(ad_display_str[index], val, unit[index], GREEN);
      else {
        show_value_float(ad_display_str[index], val, unit[index], RED);
        tcb_test.result.AD_falg++;
      }
    }

    delay_ms(10);
    tcb_test.sub_state = 3;
    return;
  } else if (3 == tcb_test.sub_state) {
    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "[电池供电测试]", 24, 0);
    POINT_COLOR = WHITE;
    power.ctrl(POW_OFF);
    delay_ms(20);
    power.bat_main(CH_BAT);
    delay_ms(20);
    power.ac_channel(CH_N);
    delay_ms(20);
    power.level(HIGH_POWER);
    delay_ms(20);
    power.ctrl(POW_ON);
    delay_ms(80);

    adc.get(&tcb_test.result.battery);
    for (index = 0; index < 6; index++) {
      if (4 == index)
        continue;
      val =
          *((float *)(((u32)&tcb_test.result.battery) + index * sizeof(float)));
      low = ((SUB_RATIO *)(((u32)&tcb_test.ratio.battery) +
                           (index * sizeof(SUB_RATIO))))
                ->low;
      upper = ((SUB_RATIO *)(((u32)&tcb_test.ratio.battery) +
                             (index * sizeof(SUB_RATIO))))
                  ->upper;

      if (val > low && val < upper)
        show_value_float(ad_display_str[index], val, unit[index], GREEN);
      else {
        show_value_float(ad_display_str[index], val, unit[index], RED);
        tcb_test.result.AD_falg++;
      }
    }

    delay_ms(10);
    if (0 == tcb_test.result.AD_falg)
      tcb_test.sub_state = 9;
    else 
      tcb_test.sub_state = 10;
    return;
  }

  else if (9 == tcb_test.sub_state) {
    power.ctrl(POW_OFF);
    delay_ms(20);
    power.bat_main(CH_MAIN);
    delay_ms(20);
    power.ac_channel(CH_P);
    delay_ms(20);
    power.level(HIGH_POWER);
    delay_ms(20);
    power.ctrl(POW_ON);
    delay_ms(80);

    tcb_test.cur_state = TEST_PROGRAM;
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "电系测试通过...", 24, 0);
    delay_ms(150);
    return;
  } else if (10 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_FINAL;
		sprintf(tcb_test.err_code, "基板电系故障");
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "基板电系测试故障", 24, 0);
    
    delay_ms(1500);
    return;
  }
}

static void test_program(void) {
  u8 res, retry;
  if (0 == tcb_test.sub_state) {
    standby_init(0);
    POINT_COLOR = GREEN;
    tcb_test.sub_state = 1;
  }

  else if (1 == tcb_test.sub_state) {
    retry = 0;
    delay_ms(100);
    POINT_COLOR = BLUE;
//    sprintf((char *)bin, "1:/SYSTEM/BIN/%s", tcb_test.test_firm);
    sprintf((char *)plcd, "测试程序:<%s>", tcb_test.test_firm);
    j += 1;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             plcd, 24, 0);
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "下载中...", 24, 0);


    j += 1;
    res = Begin_Pro((char *)tcb_test.testbin, 0);

    if (0 == res) {
      tcb_test.sub_state = 9;
      return;
    }
    while (retry < 3) {
      power.ctrl(POW_OFF);
      delay_ms(2500);
      power.ctrl(POW_ON);
      delay_ms(500);
      POINT_COLOR = YELLOW;
      if (0 == retry)
        sprintf((char *)plcd, "第一次下载重试!");
      else if (1 == retry)
        sprintf((char *)plcd, "第二次下载重试!");
      else
        sprintf((char *)plcd, "第三次下载重试!");

      Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             plcd, 24, 0);


      res = Begin_Pro((char*)tcb_test.testbin, 0);

      if (0 == res)
        break;
      retry++;
    }

    if (retry >= 3) {
      tcb_test.sub_state = 10;
      return;
    } else {
      tcb_test.sub_state = 9;
      return;
    }
  }

  else if (9 == tcb_test.sub_state) {
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "测试程序下载成功！", 24, 0);
    delay_ms(100);

    tcb_test.cur_state = TEST_SIGNAL;
    tcb_test.sub_state = 0;
  }

  else if (10 == tcb_test.sub_state) {
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24,
             "测试程序下载失败！", 24, 0);
    delay_ms(1000);

		sprintf(tcb_test.err_code, "测试程序下载失败");
    tcb_test.cur_state = TEST_FINAL;
    tcb_test.sub_state = 0;
  }
}

static void test_signal(void) {

  u32 pintval;
  if (0 == tcb_test.sub_state) {
    standby_init(0);
    j++;
    POINT_COLOR = BLUE;
    tcb_test.sub_state = 1;

    tcb_test.result.lowpower = 0;
    tcb_test.result.infra = 0;
    tcb_test.result.doorindoor = 0;
    tcb_test.result.wall_switch = 0;
    tcb_test.result.hal_main = 0;
    tcb_test.result.hal_ref = 0;
		digit.uart(1);
  }

  else if (1 == tcb_test.sub_state) {
    /* power down test */
//    power.ac_channel(CH_N);
    power.level(LEVEL_LOW);
    delay_ms(800);
    if (slave.signal(POWER, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (1 == pintval)
      tcb_test.result.lowpower++;

    //power.ac_channel(CH_P);
    power.level(LEVEL_HIGH);
    delay_ms(90);
    if (slave.signal(POWER, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (0 == pintval)
      tcb_test.result.lowpower++;

    if (2 == tcb_test.result.lowpower)
      show_signal_res("掉电信号测试:", 1);
    else {
      show_signal_res("掉电信号测试:", 0);
      sprintf(tcb_test.err_code, "掉电信号故障");
      tcb_test.sub_state = 10;
      return;
    }

    /* infra test */
    digit.infra(1);
    delay_ms(150);
    if (slave.signal(INFRA, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (1 == pintval)
      tcb_test.result.infra++;

    digit.infra(0);
    delay_ms(150);
    if (slave.signal(INFRA, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (0 == pintval)
      tcb_test.result.infra++;
		
    if (2 == tcb_test.result.infra)
      show_signal_res("红外线信号测试:", 1);
    else {
      show_signal_res("红外线信号测试:", 0);
      sprintf(tcb_test.err_code, "红外线信号故障");
      tcb_test.sub_state = 10;
      return;
    }

    /* in door test */
    if (MUTIFUNC == tcb_test.type) {
      digit.door(1);
      delay_ms(50);
      if (slave.signal(DOOR, &pintval) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
      if (1 == pintval)
        tcb_test.result.doorindoor++;

      digit.door(0);
      delay_ms(50);
      if (slave.signal(DOOR, &pintval) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
      if (0 == pintval)
        tcb_test.result.doorindoor++;
      if (2 == tcb_test.result.doorindoor)
        show_signal_res("门中门信号测试:", 1);
      else {
        show_signal_res("门中门信号测试:", 0);
        sprintf(tcb_test.err_code, "门中门信号故障");
        tcb_test.sub_state = 10;
        return;
      }
    } else if (STANDARD == tcb_test.type) {
      digit.door(1);
      delay_ms(50);
      if (slave.signal(DOOR, &pintval) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
      if (1 == pintval)
        tcb_test.result.doorindoor++;

      digit.door(0);
      delay_ms(50);
      if (slave.signal(DOOR, &pintval) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
      if (1 == pintval)
        tcb_test.result.doorindoor++;
      if (2 == tcb_test.result.doorindoor)
        show_signal_res("门中门信号测试:", 1);
      else {
        show_signal_res("门中门信号测试:", 0);
        sprintf(tcb_test.err_code, "门中门信号故障");
        tcb_test.sub_state = 10;
        return;
      }
    }


    /* press button test */
    digit.wall(1);
    delay_ms(50);
    if (slave.signal(WALL, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (1 == pintval)
      tcb_test.result.wall_switch++;

    digit.wall(0);
    delay_ms(50);
    if (slave.signal(WALL, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (0 == pintval)
      tcb_test.result.wall_switch++;
    if (2 == tcb_test.result.wall_switch)
      show_signal_res("墙开关信号测试:", 1);
    else {
      show_signal_res("墙开关信号测试:", 0);
      sprintf(tcb_test.err_code, "墙开关信号故障");
      tcb_test.sub_state = 10;
      return;
    }
    /* main hall test */
    digit.hall_main(1);
    delay_ms(30);
    if (slave.signal(MAIN_HALL, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (1 == pintval)
      tcb_test.result.hal_main++;

    digit.hall_main(0);
    delay_ms(30);
    if (slave.signal(MAIN_HALL, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }

    if (0 == pintval)
      tcb_test.result.hal_main++;

    if (2 == tcb_test.result.hal_main)
      show_signal_res("主霍尔信号测试:", 1);
    else {
      show_signal_res("主霍尔信号测试:", 0);
      sprintf(tcb_test.err_code, "主霍尔信号故障");
      tcb_test.sub_state = 10;
      return;
    }
    /* ref hall test */
    digit.hall_ref(1);
    delay_ms(30);
    if (slave.signal(REF_HALL, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (1 == pintval)
      tcb_test.result.hal_ref++;

    digit.hall_ref(0);
    delay_ms(30);
    if (slave.signal(REF_HALL, &pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }

    if (0 == pintval)
      tcb_test.result.hal_ref++;

    if (2 == tcb_test.result.hal_ref)
      show_signal_res("副霍尔信号测试:", 1);
    else {
      show_signal_res("副霍尔信号测试:", 0);
      sprintf(tcb_test.err_code, "副霍尔信号故障");
      tcb_test.sub_state = 10;
      return;
    }

    tcb_test.sub_state = 9;
    return;
  }

  else if (9 == tcb_test.sub_state) {
    j += 3;
    tcb_test.cur_state = TEST_SEG;
    tcb_test.sub_state = 0;
    POINT_COLOR = GREEN;    
    Show_Str(5, ypos + fsize * j, lcddev.width, 24, "信号测试通过", 24, 0);
    delay_ms(100);
    return;
  } else if (10 == tcb_test.sub_state) {
    j += 3;
    tcb_test.cur_state = TEST_FINAL;    

    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j, lcddev.width, 24, "信号测试故障", 24, 0);
    delay_ms(1500);
    tcb_test.sub_state = 0;
  }
}

static void test_seg(void) {
  u8 button_res;

  button_res = KEY_Scan(0);

  if (3 == button_res && tcb_test.sub_state >= 2) {
    tcb_test.sub_state = 10;
    return;
  }

  if (0 == tcb_test.sub_state) {
    standby_init(0);
    j++;
    POINT_COLOR = BLUE;
    tcb_test.sub_state = 1;

    tcb_test.result.seg = 0;
  }

  else if (1 == tcb_test.sub_state) {
    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "按键1改变数码管显示数字", 24, 0);
    POINT_COLOR = RED;
    j++;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "如果显示错误，按3", 24, 0);
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "如果显示全部正确，按2", 24, 0);
    j += 3;
    tcb_test.sub_state = 2;
    POINT_COLOR = BLUE;
    return;
  }

  else if (2 == tcb_test.sub_state) {
    if (1 == button_res) {
      if (slave.seg(1, 1) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
      show_seg_val("当前显示:", 1, 1);

      tcb_test.sub_state = 3;
    }
  }

  else if (3 == tcb_test.sub_state) {
    if (1 == button_res) {
      if (slave.seg(2, 0) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
      show_seg_val("当前显示:", 2, 0);
      tcb_test.sub_state = 4;
    }
  }

  else if (4 == tcb_test.sub_state) {
    if (1 == button_res) {
      if (slave.seg(5, 1) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
      show_seg_val("当前显示:", 5, 1);
      tcb_test.sub_state = 5;
    }
  }

  else if (5 == tcb_test.sub_state) {
    if (2 == button_res) {
      tcb_test.sub_state = 9;
    }
    return;
  }

  else if (9 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_BUTTON;
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = GREEN;

    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "数码管测试通过", 24, 0);
    delay_ms(100);
    return;
  } else if (10 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_FINAL;
		sprintf(tcb_test.err_code, "基板数码管显示故障");
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "数码管显示错误", 24, 0);
    delay_ms(1500);
    return;
  }
}

static void test_button(void) {
  u8 button_res, index;
  u32 pintval;
  button_res = KEY_Scan(0);

  if (3 == button_res && tcb_test.sub_state >= 2) {
    tcb_test.sub_state = 10;
    return;
  }

  if (0 == tcb_test.sub_state) {
    standby_init(0);
    j++;
    POINT_COLOR = BLUE;
    tcb_test.sub_state = 1;

    tcb_test.result.button_1 = 0;
    tcb_test.result.button_2 = 0;
    tcb_test.result.button_3 = 0;
    tcb_test.result.button_4 = 0;
  }

  else if (1 == tcb_test.sub_state) {
    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "触发被测基板上所有按键", 24, 0);
    POINT_COLOR = RED;
    j++;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "若触发后治具无反应，按KEY3报错", 24, 0);
    j += 3;
    tcb_test.sub_state = 2;
    POINT_COLOR = BLUE;
    /* clear stable buf */
    for (index = 0; index < 4; index++) {
      if (slave.button(&pintval) > 0) {
        tcb_test.sub_state = 10;
        return;
      }
    }
    return;
  }

  else if (2 == tcb_test.sub_state) {
    if (slave.button(&pintval) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    if (1 == pintval) {
      tcb_test.result.button_1 = 1;
      //show_seg_val("get current button:", 1, 0);
      show_seg_val("当前触发:", 1, 0);
    }
    if (2 == pintval) {
      tcb_test.result.button_2 = 1;
      //show_seg_val("get current button:", 2, 0);
      show_seg_val("当前触发:", 2, 0);
    }
    if (3 == pintval) {
      tcb_test.result.button_3 = 1;
      //show_seg_val("get current button:", 3, 0);
      show_seg_val("当前触发:", 3, 0);
    }
    if (4 == pintval) {
      tcb_test.result.button_4 = 1;
      //show_seg_val("get current button:", 4, 0);
      show_seg_val("当前触发:", 4, 0);
    }

    if (1 == tcb_test.result.button_1 && 1 == tcb_test.result.button_2 &&
        1 == tcb_test.result.button_3 && 1 == tcb_test.result.button_4) {
      tcb_test.sub_state = 9;
    }
  }

  else if (9 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_RF;
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = GREEN;
    
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "按键测试通过", 24, 0);
    delay_ms(100);
    return;
  } else if (10 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_FINAL;    
		sprintf(tcb_test.err_code, "基板按键故障");
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "按键测试未通过", 24, 0);
    delay_ms(1500);
    return;
  }
}

static void test_rf(void) {
  u32 res;
  if (0 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = GREEN;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "RF信号测试", 24, 0);
    j += 1;
    tcb_test.sub_state = 1;
    return;
  }

  else if (1 == tcb_test.sub_state) {
		
    slave.rf(&res);
    delay_ms(10);
    digit.rf(1);    
    delay_ms(1500);
    digit.rf(0);    
    if (slave.rf(&res) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
		
    if (1 == res){
      tcb_test.sub_state = 9;
      return;
    } else {
      tcb_test.sub_state = 10;
      return;
    }
  }

  if (9 == tcb_test.sub_state) {
		POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "RF信号测试通过", 24, 0);
    tcb_test.cur_state = TEST_358_PWM;    delay_ms(100);
    tcb_test.sub_state = 0;
  }  
  else if (10 == tcb_test.sub_state) {
		POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "RF信号测试未通过", 24, 0);
    tcb_test.cur_state = TEST_FINAL;    delay_ms(1500);
    tcb_test.sub_state = 0;
  }
}


static void test_358_pwm(void){

    tcb_test.cur_state = TEST_MOTOR;
    tcb_test.sub_state = 0;
    return;

}

#if 0
static void test_358_pwm(void){
  float slave_current;
  u32 pwm_val;
  if (0 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = GREEN;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "运放系数及PWM频率测试", 24, 0);
    j += 1;
    tcb_test.sub_state = 1;
    return;
  }

  else if (1 == tcb_test.sub_state) {
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "0v电压基准值运放测试", 24, 0);
    
		POINT_COLOR = WHITE;
    motor.ref(0);
		delay_ms(80);
    
    if (slave.current(&slave_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
		if (slave_current > 50){
      tcb_test.sub_state = 10;
      show_motor_res(10, "0v基准测试:", 0);
		  sprintf(tcb_test.err_code, "基板运放电路故障");
      return;
    }    
    show_motor_res(10, "0v基准测试:", 1);
		

		
		
		j += 1;
		POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "160mv电压基准值运放测试", 24, 0);
		motor.ref(1);		
		delay_ms(80);
		POINT_COLOR = WHITE;
    if (slave.current(&slave_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
		if (slave_current < 2500 || slave_current > 3500){
      tcb_test.sub_state = 10;
      show_motor_res(10, "160mv基准测试:", 0);
		  sprintf(tcb_test.err_code, "基板运放电路故障");
      return;
    }    
    show_motor_res(10, "16mv基准测试:", 1);
		
		motor.ref(0);
    tcb_test.sub_state = 2;
    return;
  }

	else if (2 == tcb_test.sub_state) {
    pwm.ctrl(1);
		
    if (slave.pwm(127) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }		

		j += 1;
		POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "50%%pwm脉冲测试", 24, 0);
    pwm.start();
    delay_ms(80);
		POINT_COLOR = WHITE;
		pwm_val = pwm.get();
    if (pwm_val > 130 || pwm_val < 100) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "pwm50%%测试:", 0);
		  sprintf(tcb_test.err_code, "pwm驱动回路故障");
      return;
    }
    show_motor_res(10, "pwm50%%测试:", 1);
	

    if (slave.pwm(0) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }
		j += 1;
		POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "0%%pwm脉冲测试", 24, 0);
    pwm.start();
    delay_ms(80);
		POINT_COLOR = WHITE;		
		pwm_val = pwm.get();
    if (pwm_val != 0) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "pwm0%%测试:", 0);
		  sprintf(tcb_test.err_code, "pwm驱动回路故障");
      return;
    }
    show_motor_res(10, "pwm0%%测试:", 1);
		
		
	  pwm.ctrl(0);
		delay_ms(80);
		tcb_test.sub_state = 9;
    return;
	}

  else if (9 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_MOTOR;
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "运放&pwm测试通过", 24, 0);
    delay_ms(100);
    return;
  } else if (10 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_FINAL;
    tcb_test.sub_state = 0;
    j += 1;
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "运放&pwm测试未通过", 24, 0);
    delay_ms(1500);
    return;
  }

}
#endif

static void test_motor(void) {
  ADCn_FLOAT adc_st;
  u8 res;
  float motor_current;

  if (0 == tcb_test.sub_state) {
    LCD_Clear(BLACK); //黑屏
    POINT_COLOR = GREEN;
    BACK_COLOR = BLACK;
    j = 0;
    ypos = 2;
    fsize = 24;
    okoffset = 370;

    j++;
    tcb_test.sub_state = 1;

    tcb_test.result.motor_p = 0;
    tcb_test.result.motor_n = 0;
    tcb_test.result.pwm = 0;
    tcb_test.result.current_motor = 0;
  } else if (1 == tcb_test.sub_state) {
    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "马达动作测试", 24, 0);
    j += 1;
    tcb_test.sub_state = 2;
    return;
  }

  /*--------------------------motor stop test-----------------------*/
  else if (2 == tcb_test.sub_state) {
    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "继电器状态-停止", 24, 0);
    POINT_COLOR = WHITE;
    if (slave.motor(MOT_CTS_STOP) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);

    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "pwm0%%测试", 24, 0);
    if (slave.pwm(0) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);
    POINT_COLOR = WHITE;
    /* test lamp */
    if (MUTIFUNC == tcb_test.type) {
      adc.get(&adc_st);
      if (adc_st.vot_lamp > 5) {
        show_motor_res(10, "警示灯:", 0);
        tcb_test.sub_state = 10;
        return;
      }
      show_motor_res(10, "警示灯:", 1);
    }
    /* current test */
    if (slave.current(&motor_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
    if (motor_current > MOTOR_FLOW_THRESHOLD) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "驱动电流:", 0);
      return;
    }
    show_motor_res(10, "驱动电流:", 1);

    /*  motor test */
    res = motor.up();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "正向信号采样:", 0);
      return;
    }
    show_motor_res(10, "正向信号采样:", 1);

    res = motor.down();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "负向信号采样:", 0);
      return;
    }
    show_motor_res(10, "负向信号采样:", 1);

    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "pwm100%%测试", 24, 0);
    if (slave.pwm(255) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);
    POINT_COLOR = WHITE;
    /* test lamp */
    if (MUTIFUNC == tcb_test.type) {
      adc.get(&adc_st);
      if (adc_st.vot_lamp > 5) {
        show_motor_res(10, "警示灯:", 0);
        tcb_test.sub_state = 10;
        return;
      }
      show_motor_res(10, "警示灯:", 1);
    }
    /* current test */
    if (slave.current(&motor_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
    if (motor_current > MOTOR_FLOW_THRESHOLD) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "驱动电流测试:", 0);
      return;
    }
    show_motor_res(10, "驱动电流测试:", 1);

    /*  motor test */
    res = motor.up();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "正向信号采样:", 0);
      return;
    }
    show_motor_res(10, "正向信号采样:", 1);

    res = motor.down();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "负向信号采样:", 0);
      return;
    }
    show_motor_res(10, "负向信号采样:", 1);

    tcb_test.sub_state = 3;
    j += 1;
  }

  else if (3 == tcb_test.sub_state) {
    /*--------------------------motor stop test-----------------------*/
    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "继电器状态-上升", 24, 0);
    POINT_COLOR = WHITE;
    if (slave.motor(MOT_CTS_UPUP) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);

    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "pwm0%%测试", 24, 0);
    if (slave.pwm(0) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);
    POINT_COLOR = WHITE;
    /* test lamp */
    if (MUTIFUNC == tcb_test.type) {
      adc.get(&adc_st);
      if (adc_st.vot_lamp < 15) {
        show_motor_res(10, "警示灯:", 0);
        tcb_test.sub_state = 10;
        return;
      }
      show_motor_res(10, "警示灯:", 1);
    }
    /* current test */
    if (slave.current(&motor_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
    if (motor_current > MOTOR_FLOW_THRESHOLD) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "驱动电流:", 0);
      return;
    }
    show_motor_res(10, "驱动电流:", 1);

    /*  motor test */
    res = motor.up();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "正向信号采样:", 0);
      return;
    }
    show_motor_res(10, "正向信号采样:", 1);

    res = motor.down();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "负向信号采样:", 0);
      return;
    }
    show_motor_res(10, "负向信号采样:", 1);

    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "pwm100%%测试", 24, 0);
    if (slave.pwm(255) > 0) { // change pwm to 0/255
      //    if (slave.pwm(127) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(300);
    POINT_COLOR = WHITE;
    /* test lamp */
    if (MUTIFUNC == tcb_test.type) {
      adc.get(&adc_st);
      if (adc_st.vot_lamp < 15) {
        show_motor_res(10, "警示灯:", 0);
        tcb_test.sub_state = 10;
        return;
      }
      show_motor_res(10, "警示灯:", 1);
    }
    /* current test */
    if (slave.current(&motor_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
    if (motor_current < MOTOR_UPER_THRESHOLD) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "驱动电流测试:", 0);
      return;
    }
    show_motor_res(10, "驱动电流测试:", 1);

    /*  motor test */
    res = motor.up();
    if (1 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "正向信号采样:", 0);
      return;
    }
    show_motor_res(10, "正向信号采样:", 1);

    res = motor.down();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "负向信号采样:", 0);
      return;
    }
    show_motor_res(10, "负向信号采样:", 1);

    tcb_test.sub_state = 4;
    j += 1;
  }

  else if (4 == tcb_test.sub_state) {
    /*--------------------------motor stop test-----------------------*/
    LCD_Clear(BLACK); //黑屏
    BACK_COLOR = BLACK;
    j = 1;
    ypos = 2;
    fsize = 24;
    okoffset = 370;
    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "马达动作测试", 24, 0);
    j += 1;

    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "继电器状态-下降", 24, 0);
    POINT_COLOR = WHITE;
    if (slave.motor(MOT_CTS_DOWN) > 0) {
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);

    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "pwm0%%测试", 24, 0);
    if (slave.pwm(0) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);
    POINT_COLOR = WHITE;
    /* test lamp */
    if (MUTIFUNC == tcb_test.type) {
      adc.get(&adc_st);
      if (adc_st.vot_lamp < 15) {
        show_motor_res(10, "警示灯:", 0);
        tcb_test.sub_state = 10;
        return;
      }
      show_motor_res(10, "警示灯:", 1);
    }
    /* current test */
    if (slave.current(&motor_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
    if (motor_current > MOTOR_FLOW_THRESHOLD) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "驱动电流:", 0);
      return;
    }
    show_motor_res(10, "驱动电流:", 1);

    /*  motor test */
    res = motor.up();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "正向信号采样:", 0);
      return;
    }
    show_motor_res(10, "正向信号采样:", 1);

    res = motor.down();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "负向信号采样:", 0);
      return;
    }
    show_motor_res(10, "负向信号采样:", 1);

    POINT_COLOR = YELLOW;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "pwm100%%测试", 24, 0);
    if (slave.pwm(255) > 0) { // change pwm to 0/255
      tcb_test.sub_state = 10;
      return;
    }
    delay_ms(100);
    POINT_COLOR = WHITE;
    /* test lamp */
    if (MUTIFUNC == tcb_test.type) {
      adc.get(&adc_st);
      if (adc_st.vot_lamp < 15) {
        show_motor_res(10, "警示灯:", 0);
        tcb_test.sub_state = 10;
        return;
      }
      show_motor_res(10, "警示灯:", 1);
    }
    /* current test */
    if (slave.current(&motor_current) > 0) { // read ad val from ET08
      tcb_test.sub_state = 10;
      return;
    }
    if (motor_current < MOTOR_UPER_THRESHOLD) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "驱动电流测试:", 0);
      return;
    }
    show_motor_res(10, "驱动电流测试:", 1);

    /*  motor test */
    res = motor.up();
    if (0 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "正向信号采样:", 0);
      return;
    }
    show_motor_res(10, "正向信号采样:", 1);

    res = motor.down();
    if (1 != res) {
      tcb_test.sub_state = 10;
      show_motor_res(10, "负向信号采样:", 0);
      return;
    }
    show_motor_res(10, "负向信号采样:", 1);

    tcb_test.sub_state = 9;
    j += 1;
  }

  else if (9 == tcb_test.sub_state) {
    tcb_test.cur_state = APP_PROGRAM;
    tcb_test.sub_state = 0;
    j += 3;
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "电机动作测试通过", 24, 0);
    delay_ms(150);
    return;
  } else if (10 == tcb_test.sub_state) {
    tcb_test.cur_state = TEST_FINAL;
		sprintf(tcb_test.err_code, "电机运动故障");
    tcb_test.sub_state = 0;
    j += 1;
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "电机动作测试未通过", 24, 0);
    delay_ms(1500);
    return;
  }
}

static void app_program(void) {
	#if 1
  u8 res, retry;
  if (0 == tcb_test.sub_state) {
    standby_init(1);
    delay_ms(500);
    POINT_COLOR = GREEN;
    tcb_test.sub_state = 1;
  }

  else if (1 == tcb_test.sub_state) {
    retry = 0;    
    power.ctrl(POW_ON);
    delay_ms(100);
    POINT_COLOR = BLUE;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "出货程序下载...", 24, 0);
    Show_Str(5, ypos + fsize * j, lcddev.width, 24, "程序：", 24, 0);
    Show_Str(120, ypos + fsize * j++, lcddev.width, 24, tcb_test.current_bin, 24, 0);
    j += 1;
    res = Begin_Pro((char *)tcb_test.appbin, 0);

    if (0 == res) {
      tcb_test.sub_state = 9;
      return;
    }
    while (retry < 3) {
      power.ctrl(POW_OFF);
      delay_ms(2500);
      power.ctrl(POW_ON);
      delay_ms(500);
      POINT_COLOR = YELLOW;
      if (0 == retry)
        sprintf((char *)plcd, "第一次重试!");
      else if (1 == retry)
        sprintf((char *)plcd, "第二次重试!");
      else
        sprintf((char *)plcd, "第三次重试!");


      Show_Str(120, ypos + fsize * j++, lcddev.width, 24, plcd, 24, 0);
      res = Begin_Pro(tcb_test.appbin, 0);

      if (0 == res)
        break;
      retry++;
    }

    if (retry >= 3) {
      tcb_test.sub_state = 10;
      return;
    } else {
      tcb_test.sub_state = 9;
      return;
    }
  }

  else if (9 == tcb_test.sub_state) {
    POINT_COLOR = GREEN;

    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "出货程序下载成功", 24, 0);
    delay_ms(3000);

    tcb_test.cur_state = TEST_FINAL;
		sprintf(tcb_test.err_code, "测试通过");
    slave.motor(MOT_CTS_STOP);
    tcb_test.sub_state = 0;
  }

  else if (10 == tcb_test.sub_state) {
    POINT_COLOR = RED;
    Show_Str(5, ypos + fsize * j++, lcddev.width, 24, "出货程序下载失败", 24, 0);
    delay_ms(1000);

		sprintf(tcb_test.err_code, "出货程序下载失败");
    slave.motor(MOT_CTS_STOP);
    tcb_test.cur_state = TEST_FINAL;
    tcb_test.sub_state = 0;
  }
	#endif
	
}

static void test_final(void) {
  u8 res_button;

  res_button = KEY_Scan(0);

  if (0 == tcb_test.sub_state) {
    standby_init(1);
    j += 4;

    if (0 == strcmp("测试通过", tcb_test.err_code))
      tcb_test.sub_state = 1;
    else
      tcb_test.sub_state = 5;

    delay_ms(20);
    return;
  }

  /* pass */
  else if (1 == tcb_test.sub_state) {
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j, lcddev.width, 24, "测试通过", 24, 0);
    POINT_COLOR = GREEN;
    Show_Str(5, ypos + fsize * j, lcddev.width, 24, "按键3进入待机", 24, 0);
    tcb_test.sub_state = 2;

    BEEP_OPEN();
    delay_ms(500);
    BEEP_CLOSE();

    return;
  } else if (2 == tcb_test.sub_state) {
    if (3 == res_button) {
      tcb_test.cur_state = TEST_STANDBY;
      tcb_test.sub_state = 0;
    }
    return;
  }

  /* fail */
  else if (5 == tcb_test.sub_state) {
    tcb_test.cnt = 0;
    POINT_COLOR = RED;
    tcb_test.sub_state = 6;
    return;
  }

  else if (6 == tcb_test.sub_state) {
    if (tcb_test.cnt < 200)
      tcb_test.cnt++;
    else
      tcb_test.cnt = 0;

    if (50 == tcb_test.cnt) {
      Show_Str(5, ypos + fsize * (j + 1), lcddev.width, 24, tcb_test.err_code, 24, 0);
      Show_Str(5, ypos + fsize * (j + 2), lcddev.width, 24, "按键4结束报警", 24, 0);
      BEEP_OPEN();
    } else if (150 == tcb_test.cnt) {
      LCD_ShowString(5, ypos + fsize * (j + 1), lcddev.width, lcddev.height,
                     fsize, "                          ");
      LCD_ShowString(5, ypos + fsize * (j + 2), lcddev.width, lcddev.height,
                     fsize, "                          ");
      BEEP_CLOSE();
    }

    if (4 == res_button) {
      tcb_test.cur_state = TEST_STANDBY;
      tcb_test.sub_state = 0;
      BEEP_CLOSE();
      return;
    }
  }
}

static void standby_init(u8 relay) {

  if (relay) {
    power.ctrl(POW_OFF);
    motor.ref(0);
    pwm.ctrl(0);
    motor.ref(0);
		digit.uart(0);
    delay_ms(200);
  }

  LCD_Clear(BLACK); //黑屏
  POINT_COLOR = GREEN;
  BACK_COLOR = BLACK;
  j = 0;
  ypos = 2;
  fsize = 24;
  okoffset = 370;

  POINT_COLOR = WHITE;
  Show_Str(5, ypos + fsize * j, lcddev.width, 24, "项目名称:", 24, 0);
  POINT_COLOR = GREEN;
  Show_Str(120, ypos + fsize * j++, lcddev.width, 24, tcb_test.object, 24, 0);

  POINT_COLOR = WHITE;
  Show_Str(5, ypos + fsize * j, lcddev.width, 24, "基板类型:", 24, 0);
  POINT_COLOR = GREEN;
  if (STANDARD == tcb_test.type)
    Show_Str(120, ypos + fsize * j++, lcddev.width, 24, "标准版", 24, 0);
  else if (MUTIFUNC == tcb_test.type)
    Show_Str(120, ypos + fsize * j++, lcddev.width, 24, "多功能", 24, 0);

  POINT_COLOR = WHITE;
  Show_Str(5, ypos + fsize * j, lcddev.width, 24, "测试程序:", 24, 0);
  POINT_COLOR = GREEN;
  Show_Str(120, ypos + fsize * j++, lcddev.width, 24, tcb_test.test_firm, 24,
           0);

  POINT_COLOR = WHITE;
  Show_Str(5, ypos + fsize * j, lcddev.width, 24, "出货程序:", 24, 0);
  POINT_COLOR = GREEN;
  Show_Str(120, ypos + fsize * j++, lcddev.width, 24, tcb_test.current_bin, 24,
           0);
}

void show_value_float(u8 *string, float val, u8 *unit, u16 f_color) {
  u8 buf[10];
  u16 color;
  okoffset = 300;

  Show_Str(5, ypos + (fsize * j), lcddev.width, 24,
           string, 24, 0);

  sprintf((char *)buf, "%0.2f%s", val, unit);
  color = POINT_COLOR;
  POINT_COLOR = f_color;
  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, buf);
  POINT_COLOR = color;
}

void show_signal_res(u8 *string, int res) {
  u16 color;

  Show_Str(5, ypos + fsize * j, lcddev.width, 24, string, 24, 0);

  color = POINT_COLOR;
  if (res) {
    POINT_COLOR = GREEN;
    LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                   lcddev.height, fsize, "Pass");
  }

  else {
    POINT_COLOR = RED;
    LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                   lcddev.height, fsize, "Fail");
  }
  POINT_COLOR = color;
}

void show_seg_val(u8 *string, int val, int piont) {
  u8 buf[10];
  u16 color;
  Show_Str(5, ypos + fsize * j, lcddev.width, 24, string, 24, 0);

  color = POINT_COLOR;
  POINT_COLOR = GREEN;
  sprintf((char *)buf, "%d", val);
  if (piont)
    strcat((char *)buf, " .");

  LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height,
                 fsize, buf);

  POINT_COLOR = color;
}

void show_motor_res(u8 x, u8 *string, u8 res) {
  u16 color;
  
  Show_Str(5, ypos + fsize * j, lcddev.width, 24, string, 24, 0);
  color = POINT_COLOR;
  if (res) {
    POINT_COLOR = GREEN;
    LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                   lcddev.height, fsize, "Pass");
  }

  else {
    POINT_COLOR = RED;
    LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width,
                   lcddev.height, fsize, "Fail");
  }
  POINT_COLOR = color;
}

static u8 check_sum_bin(u8 *path) {
  FIL *f_bin;
  u8 *pbuf, *p_temp;
  u8 rval;
  u32 sum = 0;
  u32 index;

  f_bin = memin_malloc(sizeof(FIL));
  rval = f_open(f_bin, (const TCHAR *)path, FA_READ);
  if (rval != 0) {
    memin_free(f_bin);
    return 1;
  }

  pbuf = memin_malloc(f_bin->fsize + 0x200);
  mymemset(pbuf, 0, f_bin->fsize + 0x200);

  rval = f_read(f_bin, pbuf, f_bin->fsize, (UINT *)&br);
  if (rval != 0) {
    f_close(f_bin);
    memin_free(pbuf);
    memin_free(f_bin);
    return 2;
  }

  p_temp = pbuf;
  for (index = 0; index < f_bin->fsize; index++, p_temp++)
    sum += *p_temp;

  if (sum != get_checksum_from_name(path)) {
    f_close(f_bin);
    memin_free(pbuf);
    memin_free(f_bin);
    return 3;
  }

  f_close(f_bin);
  memin_free(pbuf);
  memin_free(f_bin);
  return 0;
}

static u32 get_checksum_from_name(u8 *path) {

  u8 buf[10], index, temp;
  u32 sum = 0;
  while (*path != (u8)'\0' && *path != (u8)EOF) {
    if (*path == '0') {
      if (*(path + 1) == 'x' || *(path + 1) == 'X') {
        break;
      }
    }
    path++;
  }

  for (index = 0; index < 8; index++) {
    buf[index] = path[index];
  }
  buf[index] = '\0';

  for (index = 2; index < 8; index++) {
    if (buf[index] >= '0' && buf[index] <= '9')
      temp = buf[index] - '0';
    else if (buf[index] >= 'a' && buf[index] <= 'f')
      temp = 10 + (buf[index] - 'a');
    else if (buf[index] >= 'A' && buf[index] <= 'F')
      temp = 10 + (buf[index] - 'A');

    sum = (sum << 4) + temp;
  }

  sum = sum;
  return sum;
}

