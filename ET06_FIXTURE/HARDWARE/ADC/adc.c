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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //ʹ��GPIOCʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //ʹ��GPIOFʱ��

  /*---------------------- gpio for adc3 ---------------------------*/
  GPIO_InitStructure.GPIO_Pin =
      GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����������
  GPIO_Init(GPIOF, &GPIO_InitStructure);           //��ʼ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //��ʼ��

  /*---------------------- gpio for adc1 ---------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //��ʼ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);           //��ʼ��
}

void ADCcommon_init(void) {
  ADC_CommonInitTypeDef ADC_CommonInitStructure;

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; //����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay =
      ADC_TwoSamplingDelay_5Cycles; //���������׶�֮����ӳ�5��ʱ��(����ģʽ��˫��ģʽ��ʹ��)
  ADC_CommonInitStructure.ADC_DMAAccessMode =
      ADC_DMAAccessMode_Disabled; // DMAʧ��(���ڶ��ADCģʽ)
  ADC_CommonInitStructure.ADC_Prescaler =
      ADC_Prescaler_Div8; //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure); //��ʼ��
}

void ADC1_ADC_init(void) {
  ADC_InitTypeDef ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC1��λ
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); //��λ����

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; /*12λģʽ*/
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;           /*ɨ��ģʽ*/
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     /*����ת��*/
  ADC_InitStructure.ADC_ExternalTrigConvEdge =
      ADC_ExternalTrigConvEdge_None; /*��ֹ������� ʹ���������*/
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /*�Ҷ���*/
  ADC_InitStructure.ADC_NbrOfConversion = 4;             /*4ͨ�� */
  ADC_Init(ADC1, &ADC_InitStructure);                    /*��ʼ��*/
  ADC_Cmd(ADC1,ENABLE);/*����ת��*/

  //����ģʽ��,ͨ��������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 3, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_480Cycles);
  ADC_SoftwareStartConv(ADC1);/*�������ת��*/
  ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);//Դ���ݱ仯ʱ����DMA����
  ADC_DMACmd(ADC1,ENABLE);//ʹ��ADC����
}

void ADC1_DMA_init(void) {
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); /*DMA2��ʱ��ʹ��*/
  while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE)
    ; /*�ȴ�DMA��������*/

  DMA_InitStructure.DMA_Channel = DMA_Channel_0; /*DMAͨ��0*/
  DMA_InitStructure.DMA_PeripheralBaseAddr =
      (uint32_t)(u32)&ADC1->DR;                             /*�����ַ*/
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC1_val; /*��ȡ����ַ*/
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; /*��������赽�ڴ�*/
  DMA_InitStructure.DMA_BufferSize = ADC1_NUM * ADC_FILTER_NUM; /*���ݴ��������Ϊ1*/
  DMA_InitStructure.DMA_PeripheralInc =
      DMA_PeripheralInc_Disable;                          /*��ַ������*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /*��ַ����*/
  DMA_InitStructure.DMA_PeripheralDataSize =
      DMA_PeripheralDataSize_HalfWord; /*���ݳ��Ȱ���*/
  DMA_InitStructure.DMA_MemoryDataSize =
      DMA_MemoryDataSize_HalfWord;                       /*���ݳ��Ȱ���*/
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;    /*�����ȼ�*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;        /*ѭ��ģʽ*/
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; /*��ֹFIFO*/
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; /*FIFO��ֵ*/
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /*���δ���*/
  DMA_InitStructure.DMA_PeripheralBurst =
      DMA_PeripheralBurst_Single;             /*���δ���*/

  DMA_Init(DMA2_Stream4, &DMA_InitStructure); /**/
  DMA_Cmd(DMA2_Stream4, ENABLE);              //����DMA����
}

void ADC3_ADC_init(void) {
  ADC_InitTypeDef ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC1ʱ��

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);  // ADC3��λ
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE); //��λ����

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; /*12λģʽ*/
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;           /*ɨ��ģʽ*/
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     /*����ת��*/
  ADC_InitStructure.ADC_ExternalTrigConvEdge =
      ADC_ExternalTrigConvEdge_None; /*��ֹ������� ʹ���������*/
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /*�Ҷ���*/
  ADC_InitStructure.ADC_NbrOfConversion = 6;             /*6ͨ�� */
  ADC_Init(ADC3, &ADC_InitStructure);                    /*��ʼ��*/
  ADC_Cmd(ADC3,ENABLE);/*����ת��*/

  //����ģʽ��,ͨ��������
  ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 2, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 3, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 4, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 5, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 6, ADC_SampleTime_480Cycles);

  ADC_SoftwareStartConv(ADC3);/*�������ת��*/
  ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE);//Դ���ݱ仯ʱ����DMA����
  ADC_DMACmd(ADC3,ENABLE);//ʹ��ADC����
}

void ADC3_DMA_init(void) {
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); /*DMA2��ʱ��ʹ��*/
  while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE)
    ; /*�ȴ�DMA��������*/

  DMA_InitStructure.DMA_Channel = DMA_Channel_2; /*DMAͨ��0*/
  DMA_InitStructure.DMA_PeripheralBaseAddr =
      (uint32_t)(u32)&ADC3->DR;                             /*�����ַ*/
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC3_val; /*��ȡ����ַ*/
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; /*��������赽�ڴ�*/
  DMA_InitStructure.DMA_BufferSize = ADC3_NUM * ADC_FILTER_NUM; /*���ݴ��������Ϊ1*/
  DMA_InitStructure.DMA_PeripheralInc =
      DMA_PeripheralInc_Disable;                          /*��ַ������*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /*��ַ����*/
  DMA_InitStructure.DMA_PeripheralDataSize =
      DMA_PeripheralDataSize_HalfWord; /*���ݳ��Ȱ���*/
  DMA_InitStructure.DMA_MemoryDataSize =
      DMA_MemoryDataSize_HalfWord;                       /*���ݳ��Ȱ���*/
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;    /*�����ȼ�*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;        /*ѭ��ģʽ*/
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; /*��ֹFIFO*/
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; /*FIFO��ֵ*/
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /*���δ���*/
  DMA_InitStructure.DMA_PeripheralBurst =
      DMA_PeripheralBurst_Single;             /*���δ���*/

  DMA_Init(DMA2_Stream1, &DMA_InitStructure); /**/
  DMA_Cmd(DMA2_Stream1, ENABLE);              //����DMA����
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
