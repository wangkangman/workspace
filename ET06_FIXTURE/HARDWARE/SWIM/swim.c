#include "swim.h"
#include "common.h"
#include "string_process.h"

MCU_INFO *CurStm8;
STORE_INFO StoreInfo;
STORE_INFO CopyInfo;

MCU_INFO STM8S_xx = {128, 1024, 0x4000, 32, 0x8000, 0};

FIL FilSys;
u8 FileData[1024];
UINT ReadCount;

union u32Tou8 {
  vu32 ulongx;
  vu16 u16x[2];
  vu8 u8x[4];
};
union u32Tou8 tmpVar;

static uint8_t SWIM_Inited;
uint8_t RecCmdFlag;
uint8_t Receive_Buffer[256];
uint8_t Transceive_Buffer[256];
u16 SWIM_SYNC_CNT;

static u16 SWIM_PULSE_0;
static u16 SWIM_PULSE_1;
static u16 SWIM_PULSE_Threshold;
static u16 SWIM_DMA_IN_Buffer[12];
static u16 SWIM_DMA_OUT_Buffer[12];
static uint16_t SWIM_clock_div = 0;
static uint8_t ReadBuff[1024];
static uint8_t WriteBuff[1024];
static uint8_t cmdrtv;

void SWIM_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOF时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOF时钟
  // SWIM RST
  GPIO_InitStructure.GPIO_Pin = GPIO_SWIM_RSTOUT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  // SWIM OUT
  GPIO_InitStructure.GPIO_Pin = GPIO_SWIM_OUT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     //输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;    //开漏输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
  // SWIM IN
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

  GPIO_WriteBit(GPIOC, GPIO_SWIM_OUT_PIN, Bit_SET);
//  SYNCSWPWM_IN_TIMER_INIT();
//  SYNCSWPWM_OUT_TIMER_INIT(0);
//  APPLY_VOLTAGE();
}

static void SYNCSWPWM_PORT_OD_INIT(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_WriteBit(GPIOC, GPIO_SWIM_OUT_PIN, Bit_SET);
  GPIO_InitStructure.GPIO_Pin = GPIO_SWIM_OUT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
}

#if 0 
static void SYNCSWPWM_PORT_PP_INIT(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_WriteBit(GPIOC, GPIO_SWIM_OUT_PIN, Bit_SET);
  GPIO_InitStructure.GPIO_Pin = GPIO_SWIM_OUT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
}
#endif

static void SYNCSWPWM_PORT_ODPP_FINI(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_SWIM_OUT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

ErrorStatus HSEStartUpStatus;
FLASH_Status FlashStatus;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;

static void SWIM_Init() {
  if (!SWIM_Inited) {
    SWIM_Inited = 1;
    SYNCSWPWM_OUT_TIMER_INIT(0);
    SYNCSWPWM_PORT_OD_INIT(); //端口切换到第二功能输出口
  }
}

static void SWIM_Fini() {
  SYNCSWPWM_PORT_ODPP_FINI();
  SYNCSWPWM_OUT_TIMER_FINI();
  SYNCSWPWM_IN_TIMER_FINI();
  SWIM_Inited = 0;
}

static void SWIM_EnableClockInput(void) {
  SWIM_clock_div = 0;
  SYNCSWPWM_IN_TIMER_INIT();
}

static uint8_t SWIM_EnterProgMode(void) {
  uint8_t i;
  uint32_t dly;

  SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(10, SWIM_DMA_IN_Buffer); //分配DMA存储空间
  SWIM_LOW();
  delay_us(1000);
  
  for (i = 0; i < 4; i++) {
    SWIM_HIGH();
    delay_us(500);
    SWIM_LOW();
    delay_us(500);
  }

  for (i = 0; i < 4; i++) {
    SWIM_HIGH();
    delay_us(250);
    SWIM_LOW();
    delay_us(250);
  }
  SWIM_HIGH();

  dly = SWIM_MAX_DLY;
  SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(dly);
  if (!dly) {
    return 1;
  } else {
    return 0;
  }
}

//设置SWIM通讯时钟参数
//默认SWIM时钟为HSI/2=8MHz,周期=0.125us x 22拍 =2.75us
static uint8_t SWIM_SetClockParam(uint8_t mHz, uint8_t cnt0,
                                  uint8_t cnt1) // cnt0和cnt1构成一个周期
{
  uint16_t clock_div;

  if (SWIM_clock_div) {
    clock_div = SWIM_clock_div;
  } else {
    clock_div = SYNCSWPWM_OUT_TIMER_MHZ / mHz;
    if ((SYNCSWPWM_OUT_TIMER_MHZ % mHz) >= (mHz / 2)) {
      clock_div++;
    }
    clock_div *= SWIM_SYNC_CYCLES; // 9x128=1152
  }

  SWIM_PULSE_0 = cnt0 * clock_div / SWIM_SYNC_CYCLES;
  if ((cnt0 * clock_div % SWIM_SYNC_CYCLES) >= SWIM_SYNC_CYCLES / 2) {
    SWIM_PULSE_0++;
  }
  SWIM_PULSE_1 = cnt1 * clock_div / SWIM_SYNC_CYCLES;
  if ((cnt1 * clock_div % SWIM_SYNC_CYCLES) >= SWIM_SYNC_CYCLES / 2) {
    SWIM_PULSE_1++;
  }
  SWIM_PULSE_Threshold = SWIM_PULSE_0 + SWIM_PULSE_1;

  SYNCSWPWM_OUT_TIMER_SetCycle(SWIM_PULSE_Threshold);
  SWIM_PULSE_Threshold >>= 1;
  SWIM_PULSE_Threshold = 0x3c;
  return 0;
}

static u8 SWIM_HW_Out(u8 cmd, u8 bitlen, u16 retry_cnt) {
  int8_t i, p;
  u32 dly;
  u16 *ptr = &SWIM_DMA_OUT_Buffer[0];
	
	u8 debug_index;

retry:
	delay_us(2);
  SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(bitlen + 3, SWIM_DMA_IN_Buffer);

  *ptr++ = SWIM_PULSE_0;

  p = 0;
  for (i = bitlen - 1; i >= 0; i--) {
    if ((cmd >> i) & 1) {
      *ptr++ = SWIM_PULSE_1;
      p++;
    } else {
      *ptr++ = SWIM_PULSE_0;
    }
  }
  // parity bit
  if (p & 1) {
    *ptr++ = SWIM_PULSE_1;
  } else {
    *ptr++ = SWIM_PULSE_0;
  }
	
  // wait for last waveform -- parity bit
  *ptr++ = 0;
  for (debug_index = 0; debug_index < 12; debug_index++)
    SWIM_DMA_IN_Buffer[debug_index] = 0;

  SYNCSWPWM_OUT_TIMER_DMA_INIT(bitlen + 3, SWIM_DMA_OUT_Buffer);
  SYNCSWPWM_OUT_TIMER_DMA_WAIT();

  dly = SWIM_MAX_DLY;
  SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(dly);
  SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(bitlen + 3, SWIM_DMA_IN_Buffer);

  if (!dly) {
    // timeout
    return 2;
  } else if (
      SWIM_DMA_IN_Buffer[bitlen + 2] >
      SWIM_PULSE_Threshold) //判断最后一个ACK应答是否为"1",小于半个周期的低电平在前
  {
    // nack
    if (retry_cnt) {
      retry_cnt--;
      goto retry;
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}
static u8 SWIM_HW_In(u8 *data, u8 bitlen) {
  u8 ret = 0, index;
  u32 dly;

  dly = SWIM_MAX_DLY;
  for (index = 0; index < 12; index++)
    SWIM_DMA_IN_Buffer[index] = 0;

  *data = 0;
  dly = SWIM_MAX_DLY;
  SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(10, SWIM_DMA_IN_Buffer);
  SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(
      dly); //先接收引导bit,目标到主机，这个位必须是1
  if (!dly) {
    return 2;
  }

  for (dly = 0; dly < 8; dly++) {
    if (SWIM_DMA_IN_Buffer[1 + dly] < SWIM_PULSE_Threshold) {
      *data |= 1 << (7 - dly);
    }
  }

  SWIM_DMA_OUT_Buffer[0] = SWIM_PULSE_1;
  SWIM_DMA_OUT_Buffer[1] = 0;
  SYNCSWPWM_OUT_TIMER_DMA_INIT(2, SWIM_DMA_OUT_Buffer);
  SYNCSWPWM_OUT_TIMER_DMA_WAIT();
  SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(1, SWIM_DMA_IN_Buffer);
  SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(
      dly); //先接收引导bit,目标到主机，这个位必须是1


  if (dly && (SWIM_DMA_IN_Buffer[0] <
              SWIM_PULSE_Threshold)) //如果=1,低电平时间小于4个脉冲
  {

  } else {
    ret = 1;
  }

  return ret;
}

#if 0
static u8 SWIM_HW_In(u8 *data, u8 bitlen) {
  u8 ret = 0, index;
  u32 dly;

  dly = SWIM_MAX_DLY;
	for (index = 0; index < 12; index++) SWIM_DMA_IN_Buffer[index] = 0;
  
  SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(1, SWIM_DMA_IN_Buffer);
  SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(dly); //先接收引导bit,目标到主机，这个位必须是1
  *data = 0;
  if (dly && (SWIM_DMA_IN_Buffer[0] < SWIM_PULSE_Threshold)) //如果=1,低电平时间小于4个脉冲
  {
    dly = SWIM_MAX_DLY;
    SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(9, SWIM_DMA_IN_Buffer);
    SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(dly); //先接收引导bit,目标到主机，这个位必须是1
    if (!dly) {
      return 2;
    }

    for (dly = 0; dly < 8; dly++) {
      if (SWIM_DMA_IN_Buffer[1 + dly] < SWIM_PULSE_Threshold) {
        *data |= 1 << (7 - dly);
      }
    }

    SWIM_DMA_OUT_Buffer[0] = SWIM_PULSE_1;
    SWIM_DMA_OUT_Buffer[1] = 0;
    SYNCSWPWM_OUT_TIMER_DMA_INIT(2, SWIM_DMA_OUT_Buffer);
    SYNCSWPWM_OUT_TIMER_DMA_WAIT();
  } else {
    ret = 1;
  }

  return ret;
}
#endif


static uint8_t SWIM_SRST(void) {
  return SWIM_HW_Out(SWIM_CMD_SRST, SWIM_CMD_BITLEN, SWIM_MAX_RESEND_CNT);
}

static uint8_t SWIM_WOTF(uint32_t addr, uint16_t len, uint8_t *data) {
  uint16_t processed_len;
  uint8_t cur_len, i, debug;
  uint32_t cur_addr, addr_tmp;
  u8 rtv2;
	u32 debug_data;

  if ((0 == len) || ((uint8_t *)0 == data)) {
    return 1;
  }

  processed_len = 0;
  cur_addr = addr;
  while (processed_len < len) {
    if ((len - processed_len) > 255) {
      cur_len = 255;
    } else {
      cur_len = len - processed_len;
    }

    SET_LE_U32(&addr_tmp, cur_addr);

    if (SWIM_HW_Out(SWIM_CMD_WOTF, SWIM_CMD_BITLEN, SWIM_MAX_RESEND_CNT)) {
      return 1;
    }
    if (SWIM_HW_Out(cur_len, 8, 0)) {
      return 2;
    }
    rtv2 = SWIM_HW_Out((addr_tmp >> 16) & 0xFF, 8, 0); // retry=0,出错后不重发
    if (rtv2) {
      return 3;
    }
    if (SWIM_HW_Out((addr_tmp >> 8) & 0xFF, 8, 0)) {
      return 4;
    }
    if (SWIM_HW_Out((addr_tmp >> 0) & 0xFF, 8, 0)) {
      return 5;
    }
		debug = 0;
    for (i = 0; i < cur_len; i++) {
			debug_data = data[processed_len + i];
			debug = SWIM_HW_Out(debug_data, 8, SWIM_MAX_RESEND_CNT);
      if (debug) {
        return 6;
      }
    }

    cur_addr += cur_len;
    processed_len += cur_len;
  }

  return 0;
}

static OS_CPU_SR cpu_sr=0; 
static uint8_t SWIM_ROTF(uint32_t addr, uint16_t len, uint8_t *data) {
  uint16_t processed_len;
  uint8_t cur_len, i, res;
  uint32_t cur_addr, addr_tmp;

  OS_ENTER_CRITICAL();
  res = 0;
  if ((0 == len) || ((uint8_t *)0 == data)) {
    res = 7;
    goto END;
  }

  processed_len = 0;
  cur_addr = addr;
  while (processed_len < len) {
    if ((len - processed_len) > 255) {
      cur_len = 255;
    } else {
      cur_len = len - processed_len;
    }

    SET_LE_U32(&addr_tmp, cur_addr);

    if (SWIM_HW_Out(SWIM_CMD_ROTF, SWIM_CMD_BITLEN, SWIM_MAX_RESEND_CNT)) {
      res = 6;
      goto END;
    }
    if (SWIM_HW_Out(cur_len, 8, 0)) {
      res = 5;
      goto END;
    }
    if (SWIM_HW_Out((addr_tmp >> 16) & 0xFF, 8, 0)) {
      res = 4;
      goto END;
    }
    if (SWIM_HW_Out((addr_tmp >> 8) & 0xFF, 8, 0)) {
      res = 3;
      goto END;
    }
    if (SWIM_HW_Out((addr_tmp >> 0) & 0xFF, 8, 0)) {
      res = 2;
      goto END;
    }
    for (i = 0; i < cur_len; i++) {
      if (SWIM_HW_In(&data[processed_len + i], 8)) {
      res = 1;
      goto END;
      }
    }

    cur_addr += cur_len;
    processed_len += cur_len;
  }
END:  
  OS_EXIT_CRITICAL();
  return res;
}
#if 0
static uint8_t SWIM_ROTF(uint32_t addr, uint16_t len, uint8_t *data) {
  uint16_t processed_len;
  uint8_t cur_len, i;
  uint32_t cur_addr, addr_tmp;

  if ((0 == len) || ((uint8_t *)0 == data)) {
    return 7;
  }

  processed_len = 0;
  cur_addr = addr;
  while (processed_len < len) {
    if ((len - processed_len) > 255) {
      cur_len = 255;
    } else {
      cur_len = len - processed_len;
    }

    SET_LE_U32(&addr_tmp, cur_addr);

    if (SWIM_HW_Out(SWIM_CMD_ROTF, SWIM_CMD_BITLEN, SWIM_MAX_RESEND_CNT)) {
      return 6;
    }
    if (SWIM_HW_Out(cur_len, 8, 0)) {
      return 5;
    }
    if (SWIM_HW_Out((addr_tmp >> 16) & 0xFF, 8, 0)) {
      return 4;
    }
    if (SWIM_HW_Out((addr_tmp >> 8) & 0xFF, 8, 0)) {
      return 3;
    }
    if (SWIM_HW_Out((addr_tmp >> 0) & 0xFF, 8, 0)) {
      return 2;
    }
    for (i = 0; i < cur_len; i++) {
      if (SWIM_HW_In(&data[processed_len + i], 8)) {
        return 1;
      }
    }

    cur_addr += cur_len;
    processed_len += cur_len;
  }
  return 0;
}
#endif

u8 SendData[512];
u8 Erase_Flash(void) {
  u16 blk;
  u16 Range;
  u16 index;
  u16 retry_times;

  MCU_INFO *PMcu;
#if USE_PROGRESS
  u32 ProgSize;
	#endif
  PMcu = &STM8S_xx;
#if USE_PROGRESS
  ProgSize = PMcu->ProgramNum * 1024;
#endif
  for (index = 0; index < 512; index++){
    SendData[index] = 0;
  }

  Range = PMcu->ProgramNum * 1024 / PMcu->Page;
   for (blk = 0; blk < Range; blk++) {
    WriteBuff[0] = 0x01; // 0000505B,Flash_CR2 ,Bit0=1,标准块编程方式
    WriteBuff[1] = 0xFE; // 0000505C,Flash_NCR2
    cmdrtv = SWIM_WOTF( STM8_FLASH_CR2, 2, WriteBuff); //每次写块数据之前都要重新配置Flash_CR2，NCR2,块写结束后被硬件自动清0
    if (cmdrtv > 0) {
      SWIM_Fini();
      return 1;
    }
    cmdrtv =
        SWIM_WOTF(PMcu->ProgramAddr + (blk * PMcu->Page), PMcu->Page, SendData);
    if (cmdrtv > 0) {
      SWIM_Fini();
      return 2;
    }
    ReadBuff[0] = 0;

#if USE_PROGRESS
    PROGBAR_SetValue(WM_GetDialogItem(MainHwin, GUI_ID_PROGBAR0),
                     (blk * PMcu->Page * 100) / ProgSize);
    WM_Paint(WM_GetDialogItem(MainHwin, GUI_ID_PROGBAR0));
#endif

    retry_times = 50;
    while (!(ReadBuff[0] & STM8_FLASH_EOF)) {
      cmdrtv = SWIM_ROTF(STM8_FLASH_IAPCSR, 1, ReadBuff);
      if (cmdrtv > 0) {
        SWIM_Fini();
        return 9;
      }
      delay_us(200);
      retry_times--;
      if (0 == retry_times)
        break;
    }

    if (0 == retry_times)
      return 10;
  }
  return 0;
}

u8 Erase_EEPROM(void) {
  u16 blk;
  u16 Range;
  u16 index;
  u16 retry_times;

  MCU_INFO *PMcu;
#if USE_PROGRESS
  u32 ProgSize;
	#endif
  PMcu = &STM8S_xx;
#if USE_PROGRESS
  ProgSize = PMcu->EepromNum;
#endif
  for (index = 0; index < 512; index++){
    SendData[index] = 0;
  }

  Range = PMcu->EepromNum / PMcu->Page;
  for (blk = 0; blk < Range; blk++) {
    WriteBuff[0] = 0x01; // 0000505B,Flash_CR2 ,Bit0=1,标准块编程方式
    WriteBuff[1] = 0xFE; // 0000505C,Flash_NCR2
    cmdrtv = SWIM_WOTF(
        STM8_FLASH_CR2, 2,
        WriteBuff); //每次写块数据之前都要重新配置Flash_CR2，NCR2,块写结束后被硬件自动清0
    if (cmdrtv > 0) {
      SWIM_Fini();
      return 1;
    }
    cmdrtv = SWIM_WOTF(PMcu->EepromAddr + (blk * 128), 128, SendData);
    if (cmdrtv > 0) {
      SWIM_Fini();
      return 2;
    }
    ReadBuff[0] = 0;

#if USE_PROGRESS
    PROGBAR_SetValue(WM_GetDialogItem(MainHwin, GUI_ID_PROGBAR0),
                     (blk * 64 * 100) / ProgSize);
    WM_Paint(WM_GetDialogItem(MainHwin, GUI_ID_PROGBAR0));
#endif

    retry_times = 50;
    while (!(ReadBuff[0] & STM8_FLASH_EOF)) {
      cmdrtv = SWIM_ROTF(STM8_FLASH_IAPCSR, 1, ReadBuff);
      if (cmdrtv > 0) {
        SWIM_Fini();
        return 9;
      }
      delay_us(200);
      retry_times--;
      if (0 == retry_times)
        break;
    }

    if (0 == retry_times)
      return 10;
  }
  return 0;
}

uint8_t Pro_Init(void) {
  SWIM_RST_HIGH();
  delay_ms(200);
  SWIM_RST_LOW();
  delay_us(10);
  SWIM_EnableClockInput();
  cmdrtv = SWIM_EnterProgMode();
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 1;
  }
  delay_ms(1);
  SWIM_Inited = 0;
  SWIM_Init();
  SWIM_SetClockParam(8, 20, 2); // SWIM初始时钟为HSI/2=8MHz

  cmdrtv = SWIM_SRST();
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 2;
  }
  delay_ms(1);
  WriteBuff[0] = 0xA0;
  cmdrtv =
      SWIM_WOTF(SWIM_CSR, 1, WriteBuff); // SWIM_CSR,控制寄存器中写入0B1010,0000
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 3;
  }
  delay_ms(10);
  SWIM_RST_HIGH();
  delay_ms(10);
  WriteBuff[0] = 0xB0;
  cmdrtv =
      SWIM_WOTF(SWIM_CSR, 1, WriteBuff); // SWIM_CSR,控制寄存器中写入0B1010,0000
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 4;
  }
  delay_ms(10);
//  SWIM_SetClockParam(8, 10, 2);
  WriteBuff[0] = 0x08;
  cmdrtv = SWIM_WOTF(DM_CSR2, 1, WriteBuff); // DM_CSR2,Bit4 STALL =1
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 5;
  }

  #if 1
  WriteBuff[0] = 0x56;                              // MASSKEY1
  cmdrtv = SWIM_WOTF(STM8_FLASH_KEY, 1, WriteBuff); //解锁Flash区域保护
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 6;
  }
  delay_ms(1);
  WriteBuff[0] = 0xAE; // MASSKEY2
  cmdrtv = SWIM_WOTF(STM8_FLASH_KEY, 1, WriteBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 7;
  }

  delay_ms(1);
  WriteBuff[0] = 0xAE;                              // MASSKEY1
  cmdrtv = SWIM_WOTF(STM8_EEPROM_KEY, 1, WriteBuff); //解锁Flash区域保护
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 8;
  }
  delay_ms(1);
  WriteBuff[0] = 0x56; // MASSKEY2
  cmdrtv = SWIM_WOTF(STM8_EEPROM_KEY, 1, WriteBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 9;
  }
  #endif

  return 0;
}



u8 End_Pro(void) {
  MCU_INFO *PMcu;
  PMcu = &STM8S_xx;
  cmdrtv = SWIM_ROTF(PMcu->ProgramAddr, PMcu->Page, ReadBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 1;
  }
  delay_ms(10);


  WriteBuff[0] = 0x00;
  cmdrtv = SWIM_WOTF(DM_CSR2, 1, WriteBuff); // DM_CSR2,Bit4 STALL =0
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 4;
  }
  SWIM_Fini();
  return 0;
}



u8 Pro_Bin(void) {
  u16 i, blk = 0, retry_times;
  u8 *Point;
  MCU_INFO *PMcu;
	#if USE_PROGRESS
  u32 ProgSize;
	#endif
  PMcu = &STM8S_xx;
#if USE_PROGRESS
  ProgSize = PMcu->ProgramNum * 1024;
#endif

  if (f_read(&FilSys, FileData, 1024, &ReadCount) != FR_OK) {
    return 1;
  }
  while (ReadCount) {
    Point = FileData;
    for (i = 0; i < ReadCount / PMcu->Page; i++) {
      WriteBuff[0] = 0x01; // 0000505B,Flash_CR2 ,Bit0=1,标准块编程方式
      WriteBuff[1] = 0xFE; // 0000505C,Flash_NCR2
      cmdrtv = SWIM_WOTF(
          STM8_FLASH_CR2, 2,
          WriteBuff); //每次写块数据之前都要重新配置Flash_CR2，NCR2,块写结束后被硬件自动清0
      if (cmdrtv > 0) {
        SWIM_Fini();
        return 2;
      }
      cmdrtv =
          SWIM_WOTF(PMcu->ProgramAddr + (blk * PMcu->Page), PMcu->Page, Point);
      if (cmdrtv > 0) {
        SWIM_Fini();
        return 3;
      }
      blk++;
      Point += PMcu->Page;
      ReadBuff[0] = 0;

      retry_times = 50;
      while (!(ReadBuff[0] & STM8_FLASH_EOF)) {
        cmdrtv = SWIM_ROTF(STM8_FLASH_IAPCSR, 1, ReadBuff);
        if (cmdrtv > 0) {
          SWIM_Fini();
          return 4;
        }
        delay_us(200);
        retry_times--;
        if (0 == retry_times)
          break;
      }

      if (0 == retry_times)
        return 4;
    }
    if (ReadCount % PMcu->Page) {
      WriteBuff[0] = 0x01; // 0000505B,Flash_CR2 ,Bit0=1,标准块编程方式
      WriteBuff[1] = 0xFE; // 0000505C,Flash_NCR2
      cmdrtv = SWIM_WOTF(
          STM8_FLASH_CR2, 2,
          WriteBuff); //每次写块数据之前都要重新配置Flash_CR2，NCR2,块写结束后被硬件自动清0
      if (cmdrtv > 0) {
        SWIM_Fini();
        return 5;
      }
      cmdrtv = SWIM_WOTF(PMcu->ProgramAddr + (blk * PMcu->Page),
                         ReadCount % PMcu->Page, Point);
      if (cmdrtv > 0) {
        SWIM_Fini();
        return 6;
      }
      cmdrtv = SWIM_WOTF(PMcu->ProgramAddr + (blk * PMcu->Page) +
                             ReadCount % PMcu->Page,
                         PMcu->Page - ReadCount % PMcu->Page, SendData);
      blk++;
      if (cmdrtv > 0) {
        SWIM_Fini();
        return 7;
      }
      ReadBuff[0] = 0;

      retry_times = 50;
      while (!(ReadBuff[0] & STM8_FLASH_EOF)) {
        cmdrtv = SWIM_ROTF(STM8_FLASH_IAPCSR, 1, ReadBuff);
        if (cmdrtv > 0) {
          SWIM_Fini();
          return 8;
        }
        delay_us(200);
        retry_times--;
        if (0 == retry_times)
          break;
      }

      if (0 == retry_times)
        return 8;
    }
    if (ReadCount >= 1024) {
      if (f_lseek(&FilSys, FilSys.fptr) != FR_OK) {
        return 9;
      }
      if (f_read(&FilSys, FileData, 1024, &ReadCount) != FR_OK) {
        return 10;
      }
    } else {
      ReadCount = 0;
    }
  }
  return 0;
}



u8 Read_Init(void){
  SWIM_RST_HIGH();
  delay_ms(200);
  SWIM_RST_LOW();
  delay_us(200);
  SWIM_EnableClockInput();
  cmdrtv = SWIM_EnterProgMode();
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 1;
  }
  delay_ms(1);
  SWIM_Inited = 0;
  SWIM_Init();
  SWIM_SetClockParam(8, 20, 2); // SWIM初始时钟为HSI/2=8MHz

  cmdrtv = SWIM_SRST();
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 2;
  }
  delay_ms(1);
  WriteBuff[0] = 0xA0;
  cmdrtv =
      SWIM_WOTF(SWIM_CSR, 1, WriteBuff); // SWIM_CSR,控制寄存器中写入0B1010,0000
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 3;
  }
  delay_ms(10);
  SWIM_RST_HIGH();
  delay_ms(10);
  WriteBuff[0] = 0xB0;
  cmdrtv =
      SWIM_WOTF(SWIM_CSR, 1, WriteBuff); // SWIM_CSR,控制寄存器中写入0B1010,0000
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 4;
  }
  delay_ms(10);
//  SWIM_SetClockParam(8, 10, 2);
  WriteBuff[0] = 0x08;
  cmdrtv = SWIM_WOTF(DM_CSR2, 1, WriteBuff); // DM_CSR2,Bit4 STALL =1
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 5;
  }
  delay_ms(10);

#if 1
  WriteBuff[0] = 0x56;                              // MASSKEY1
  cmdrtv = SWIM_WOTF(STM8_FLASH_KEY, 1, WriteBuff); //解锁Flash区域保护
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 6;
  }
  delay_ms(1);
  WriteBuff[0] = 0xAE; // MASSKEY2
  cmdrtv = SWIM_WOTF(STM8_FLASH_KEY, 1, WriteBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 7;
  }

  delay_ms(1);
  WriteBuff[0] = 0xAE;                              // MASSKEY1
  cmdrtv = SWIM_WOTF(STM8_EEPROM_KEY, 1, WriteBuff); //解锁Flash区域保护
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 8;
  }
  delay_ms(1);
  WriteBuff[0] = 0x56; // MASSKEY2
  cmdrtv = SWIM_WOTF(STM8_EEPROM_KEY, 1, WriteBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 9;
  }
#endif

  return 0;
}

u8 Opetion_lock(void){

  WriteBuff[0] = 0x80;
  WriteBuff[1] = 0x7F;
  cmdrtv = SWIM_WOTF(
      STM8_FLASH_CR2, 2,
      WriteBuff); 
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 1;
  }

  WriteBuff[0] = 0xAA;
  cmdrtv = SWIM_WOTF(STM8_FLASH_ROP, 1, WriteBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 2;
  }
  return 0;
}

u8 Opetion_unlock(void){
/*  WriteBuff[0] = 0x80;
  WriteBuff[1] = 0x7F;
  cmdrtv = SWIM_WOTF(
      STM8_FLASH_CR2, 2,
      WriteBuff); 
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 1;
  }
  
  cmdrtv = SWIM_ROTF(STM8_FLASH_ROP, 1, ReadBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 1;
  }*/

//  if (0x00 == ReadBuff[0]) return 0;

  WriteBuff[0] = 0x80; // 0000505B,Flash_CR2 ,Bit0=1,标准块编程方式
  WriteBuff[1] = 0x7F; // 0000505C,Flash_NCR2
  cmdrtv = SWIM_WOTF(
      STM8_FLASH_CR2, 2,
      WriteBuff); 
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 2;
  }

  WriteBuff[0] = 0x00;
  cmdrtv = SWIM_WOTF(STM8_FLASH_ROP, 1, WriteBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 3;
  }

  delay_ms(50);

  cmdrtv = SWIM_ROTF(STM8_FLASH_ROP, 1, ReadBuff);
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 4;
  }

  if (0x00 == ReadBuff[0]) return 0;

  return 5;  
}



u8 Read_Flash(void){
  u16 blk = 0;
  u16 index;
  u8 *Point;
  MCU_INFO *PMcu;
#if USE_PROGRESS
  u32 ProgSize;
	#endif
  PMcu = &STM8S_xx;
#if USE_PROGRESS
  ProgSize = PMcu->ProgramNum * 1024;
#endif

  if (f_read(&FilSys, FileData, 128, &ReadCount) != FR_OK) {
    return 1;
  }

  while (ReadCount) {
    Point = FileData;
    cmdrtv = SWIM_ROTF(PMcu->ProgramAddr + blk * 128, 128, ReadBuff);
    if (cmdrtv > 0) {
      SWIM_Fini();
      return 2;
    }

    for (index = 0; index < 128; index++){
      if (Point[index] != ReadBuff[index])
        return 3;
    }

    if (ReadCount >= 128) {
      if (f_lseek(&FilSys, FilSys.fptr) != FR_OK) {
        return 9;
      }
      if (f_read(&FilSys, FileData, 128, &ReadCount) != FR_OK) {
        return 10;
      }
    } else {
      ReadCount = 0;
    }

    blk++;
  }
  return 0;
}

u8 End_Read(){
  delay_ms(10);
  WriteBuff[0] = 0x00;
  cmdrtv = SWIM_WOTF(DM_CSR2, 1, WriteBuff); // DM_CSR2,Bit4 STALL =0
  if (cmdrtv > 0) {
    SWIM_Fini();
    return 4;
  }
  SWIM_Fini();
  return 0;
}


u8 Begin_Pro(char *FileName, u8 lock_command) {
  u8  res;
  u8 retu = 0;

  StoreInfo.McuIndex = 12;

	res = Pro_Init();
  if (res)
    return 1;

  res = Erase_Flash();
  if (res)
    return 2;

  res = Erase_EEPROM();
  if (res)
    return 3;

	res = f_open(&FilSys, FileName, FA_READ);
  if (res != FR_OK)
    return 4;

	res = Pro_Bin();
  if (res) {
    retu = 5;
    goto END;
  }

  res = End_Pro();
  if (res) {
    retu = 6;
    goto END;
  }
	
  f_lseek(&FilSys, 0);
  res = Read_Init();
  if (res) {
    retu = 7;
    goto END;
  }

  res = Read_Flash();
  if (res) {
    retu = 8;
    goto END;
  }

  if (lock_command){
    res = Opetion_lock();
    if (res){
      retu = 9;
      goto END;
    }
  }

  res = End_Read();
  if (res) {
    retu = 10;
    goto END;
  }


END:
  f_close(&FilSys);
  return retu;
}

