#include "adc.h"

__IO u16 ADC1_val[ADC_FILTER_NUM][ADC1_NUM] = {0};

__IO u16 ADC3_val[ADC_FILTER_NUM][ADC3_NUM] = {0};


static void ADC_gpio_init(void);
static void Adc_Init(void);
static u16 get_adc_val(ADCn_Type channel);
static void get_result(ADCn_FLOAT* pval);

ADC_API adc = {Adc_Init, get_result};

void ADC_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //使能GPIOF时钟

  /*---------------------- gpio for adc3 ---------------------------*/
  GPIO_InitStructure.GPIO_Pin =
      GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);           //初始化

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化

  /*---------------------- gpio for adc1 ---------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);           //初始化
}

void ADCcommon_init(void) {
  ADC_CommonInitTypeDef ADC_CommonInitStructure;

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; //独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay =
      ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟(三重模式或双重模式下使用)
  ADC_CommonInitStructure.ADC_DMAAccessMode =
      ADC_DMAAccessMode_Disabled; // DMA失能(对于多个ADC模式)
  ADC_CommonInitStructure.ADC_Prescaler =
      ADC_Prescaler_Div8; //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure); //初始化
}

void ADC1_ADC_init(void) {
  ADC_InitTypeDef ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC1复位
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); //复位结束

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; /*12位模式*/
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;           /*扫描模式*/
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     /*连续转换*/
  ADC_InitStructure.ADC_ExternalTrigConvEdge =
      ADC_ExternalTrigConvEdge_None; /*禁止触发检测 使用软件触发*/
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /*右对齐*/
  ADC_InitStructure.ADC_NbrOfConversion = 4;             /*4通道 */
  ADC_Init(ADC1, &ADC_InitStructure);                    /*初始化*/
  ADC_Cmd(ADC1,ENABLE);/*开启转换*/

  //连续模式下,通道的配置
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 3, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_480Cycles);
  ADC_SoftwareStartConv(ADC1);/*启动软件转换*/
  ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);//源数据变化时开启DMA传输
  ADC_DMACmd(ADC1,ENABLE);//使能ADC传输
}

void ADC1_DMA_init(void) {
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); /*DMA2的时钟使能*/
  while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE)
    ; /*等待DMA可以配置*/

  DMA_InitStructure.DMA_Channel = DMA_Channel_0; /*DMA通道0*/
  DMA_InitStructure.DMA_PeripheralBaseAddr =
      (uint32_t)(u32)&ADC1->DR;                             /*外设地址*/
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC1_val; /*存取器地址*/
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; /*方向从外设到内存*/
  DMA_InitStructure.DMA_BufferSize = ADC1_NUM * ADC_FILTER_NUM; /*数据传输的数量为1*/
  DMA_InitStructure.DMA_PeripheralInc =
      DMA_PeripheralInc_Disable;                          /*地址不增加*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /*地址增加*/
  DMA_InitStructure.DMA_PeripheralDataSize =
      DMA_PeripheralDataSize_HalfWord; /*数据长度半字*/
  DMA_InitStructure.DMA_MemoryDataSize =
      DMA_MemoryDataSize_HalfWord;                       /*数据长度半字*/
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;    /*高优先级*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;        /*循环模式*/
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; /*禁止FIFO*/
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; /*FIFO的值*/
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /*单次传输*/
  DMA_InitStructure.DMA_PeripheralBurst =
      DMA_PeripheralBurst_Single;             /*单次传输*/

  DMA_Init(DMA2_Stream4, &DMA_InitStructure); /**/
  DMA_Cmd(DMA2_Stream4, ENABLE);              //开启DMA传输
}

void ADC3_ADC_init(void) {
  ADC_InitTypeDef ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC1时钟

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);  // ADC3复位
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE); //复位结束

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; /*12位模式*/
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;           /*扫描模式*/
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     /*连续转换*/
  ADC_InitStructure.ADC_ExternalTrigConvEdge =
      ADC_ExternalTrigConvEdge_None; /*禁止触发检测 使用软件触发*/
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /*右对齐*/
  ADC_InitStructure.ADC_NbrOfConversion = 6;             /*6通道 */
  ADC_Init(ADC3, &ADC_InitStructure);                    /*初始化*/
  ADC_Cmd(ADC3,ENABLE);/*开启转换*/

  //连续模式下,通道的配置
  ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 2, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 3, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 4, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 5, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 6, ADC_SampleTime_480Cycles);

  ADC_SoftwareStartConv(ADC3);/*启动软件转换*/
  ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE);//源数据变化时开启DMA传输
  ADC_DMACmd(ADC3,ENABLE);//使能ADC传输
}

void ADC3_DMA_init(void) {
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); /*DMA2的时钟使能*/
  while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)
    ; /*等待DMA可以配置*/

  DMA_InitStructure.DMA_Channel = DMA_Channel_2; /*DMA通道0*/
  DMA_InitStructure.DMA_PeripheralBaseAddr =
      (uint32_t)(u32)&ADC3->DR;                             /*外设地址*/
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC3_val; /*存取器地址*/
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; /*方向从外设到内存*/
  DMA_InitStructure.DMA_BufferSize = ADC3_NUM * ADC_FILTER_NUM; /*数据传输的数量为1*/
  DMA_InitStructure.DMA_PeripheralInc =
      DMA_PeripheralInc_Disable;                          /*地址不增加*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /*地址增加*/
  DMA_InitStructure.DMA_PeripheralDataSize =
      DMA_PeripheralDataSize_HalfWord; /*数据长度半字*/
  DMA_InitStructure.DMA_MemoryDataSize =
      DMA_MemoryDataSize_HalfWord;                       /*数据长度半字*/
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;    /*高优先级*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;        /*循环模式*/
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; /*禁止FIFO*/
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; /*FIFO的值*/
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /*单次传输*/
  DMA_InitStructure.DMA_PeripheralBurst =
      DMA_PeripheralBurst_Single;             /*单次传输*/

  DMA_Init(DMA2_Stream1, &DMA_InitStructure); /**/
  DMA_Cmd(DMA2_Stream1, ENABLE);              //开启DMA传输
}

void Adc_Init(void) {
  ADC_gpio_init();
  ADCcommon_init();
  ADC1_DMA_init();
  ADC1_ADC_init();
  ADC3_DMA_init();
  ADC3_ADC_init();
}


u16 get_adc_val(ADCn_Type channel) {
  u16 *p_val, step, temp;
  u16 index, min, max, sum;

  if (channel >= ADC_CHANNEL_NUM) return (u16)0x1ffffff;
  
  if (channel < ADC1_NUM) {
    p_val = (u16*)ADC1_val;
    step = ADC1_NUM;
  } else {
    p_val = (u16*)ADC3_val;
    step = ADC3_NUM;
    channel -= ADC1_NUM;
  }

  for (index = 0, min = (u16)0xffffff, max = 0, sum = 0
       ; index < ADC_FILTER_NUM
       ; index++) {
    temp = *(p_val + (index * step) + channel);
    min = temp < min ? temp : min;
    max = temp > max ? temp : max;
    sum += temp;
  }

  return ((sum - min - max) / (ADC_FILTER_NUM - 2));

}

void get_result(ADCn_FLOAT* pval) {
  u16 adc_data;
  double gnd, res;
  adc_data = get_adc_val(ADC1_CURRENT);
  gnd = ((double)adc_data * 3.3 / 4096) / 6 / 16;
//  gnd = ((double)adc_data * 3.3 / 4096) / 16;


  res = gnd / 0.1;
  res *= 1000;
  pval->current = res;

#if 1
  adc_data = get_adc_val(VOT1_MAIN);
  res = (double)adc_data * 3.3 / 4096;
  res *= 11;
  res -= gnd;
  pval->vot_main = res;
#endif


  adc_data = get_adc_val(VOT2_BAT);
  res = (double)adc_data * 3.3 / 4096;
  res *= 11;
  res -= gnd;
  pval->vot_bat = res;

#if 0
  adc_data = get_adc_val(VOT3_24V);
  res = (double)adc_data * 3.3 / 4096;
  res *= 11;
  res -= gnd;
  pval->vot_24v = res;
  #endif

  adc_data = get_adc_val(VOT4_LAMP);
  res = (double)adc_data * 3.3 / 4096;
  res *= 11;
  res -= gnd;
  pval->vot_lamp = res;

  adc_data = get_adc_val(VOT5_12V);
  res = (double)adc_data * 3.3 / 4096;
  res *= 4.03;
  res -= gnd;
  pval->vot_12v = res;


#if 0
  pval->vot_main = res;
#endif

  adc_data = get_adc_val(VOT6_5V);
  res = (double)adc_data * 3.3 / 4096;
  res *= 2;
//  res /= 3.3;
  res -= gnd;
  pval->vot_5v = res;

  return;

}
