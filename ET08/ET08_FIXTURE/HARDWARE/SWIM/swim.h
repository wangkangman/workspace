#ifndef __SWIM_H
#define __SWIM_H

#include "stm32f4xx_conf.h"

#define USE_PROGRESS (0)

typedef struct
{
	int	 Page;							    //mcu的名字索引
	int  EepromNum;							//eeprom的大小
	int  EepromAddr;						//eeprom的起始地址
	int  ProgramNum;
	int  ProgramAddr;
	char *FileAddr;
}MCU_INFO;

typedef struct
{
	u8 FactoryState;
	u8 UsbState;
	u8 PowerState;
	char FileName[20];
	char FileType;
	char Password[10];
	char DownTimes[6];
	int	McuIndex;
}STORE_INFO;

/*----------------------SWIM----------------------------*/
#define SWIM_CSR			0x00007F80
#define DM_CSR2				0x00007F99
#define STM8_FLASH_KEY		0x00005062
#define STM8_FLASH_CR2		0x0000505B
#define STM8_FLASH_IAPCSR	0x0000505f

#define STM8_FLASH_ROP	    0x00004800

#define STM8_FLASH_EOF		0x04





#define STM8_EEPROM_KEY		0x00005064


/**-------------  file type  -----------**/
#define BIN_FILE				0x00
#define HEX_FILE				0x01
#define S19_FILE				0x02


#define SWIM_MAX_DLY            0xfffff
#define SWIM_MAX_RESEND_CNT     20

#define SWIM_CMD_BITLEN         3
#define SWIM_CMD_SRST           0x00	 //复位
#define SWIM_CMD_ROTF           0x01	 //SWIM 飞速读
#define SWIM_CMD_WOTF           0x02	 //SWIM 飞速写

#define SWIM_SYNC_CYCLES		128

#define GPIO_SWIM_LED_PIN       GPIO_Pin_6    //PD.6 SWIM LED 输出
#define GPIO_SWIM_RSTOUT_PIN    GPIO_Pin_11		//PC.5 SWIM RST 输出
#define GPIO_SWIM_OUT_PIN       GPIO_Pin_6		//Tim3_CH1映射到PC.6作为SWIM输出
#define GPIO_SWIM_IN1_PIN       GPIO_Pin_6		//PB,6,7 2根线都是作为SWIM输入
#define GPIO_SWIM_IN2_PIN       GPIO_Pin_7

#define _SYS_FREQUENCY                          168
//#define SYNCSWPWM_OUT_TIMER_MHZ			        _SYS_FREQUENCY
#define SYNCSWPWM_OUT_TIMER_MHZ			        84
#define SYNCSWPWM_OUT_TIMER				        TIM3
#define SWIM_PWM_OUT_DMA_CHANNEL                DMA_Channel_5
#define SWIM_PWM_OUT_DMA_STREAM                 DMA1_Stream4
#define SWIM_PWM_IN_DMA_CHANNEL                 DMA_Channel_2
#define SYNCSWPWM_IN_TIMER				        TIM4
#define SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM		DMA1_Stream0
#define SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM		DMA1_Stream3
#define SYNCSWPWM_IN_TIMER_DMA_CHANNEL		    DMA_Channel_2




#define SWIM_PWM_OUT_TIMER TIM3


#define SWIM_RST_HIGH()	(GPIO_WriteBit(GPIOB, GPIO_SWIM_RSTOUT_PIN, Bit_SET))			 
#define SWIM_RST_LOW() 	(GPIO_WriteBit(GPIOB, GPIO_SWIM_RSTOUT_PIN, Bit_RESET))

#define SWIM_HIGH() (GPIO_WriteBit(GPIOC, GPIO_SWIM_OUT_PIN, Bit_SET))			 
#define SWIM_LOW() 	(GPIO_WriteBit(GPIOC, GPIO_SWIM_OUT_PIN, Bit_RESET))
#define SWIM_IN1()	(GPIO_ReadInputDataBit(GPIOB, GPIO_SWIM_IN1_PIN))
#define SWIM_IN2()	(GPIO_ReadInputDataBit(GPIOB, GPIO_SWIM_IN2_PIN))


#define SET_U16_MSBFIRST(p, v)		\
	do{\
		*((uint8_t *)(p) + 0) = (((uint16_t)(v)) >> 8) & 0xFF;\
		*((uint8_t *)(p) + 1) = (((uint16_t)(v)) >> 0) & 0xFF;\
	} while (0)
#define SET_U24_MSBFIRST(p, v)		\
	do{\
		*((uint8_t *)(p) + 0) = (((uint32_t)(v)) >> 16) & 0xFF;\
		*((uint8_t *)(p) + 1) = (((uint32_t)(v)) >> 8) & 0xFF;\
		*((uint8_t *)(p) + 2) = (((uint32_t)(v)) >> 0) & 0xFF;\
	} while (0)
#define SET_U32_MSBFIRST(p, v)		\
	do{\
		*((uint8_t *)(p) + 0) = (((uint32_t)(v)) >> 24) & 0xFF;\
		*((uint8_t *)(p) + 1) = (((uint32_t)(v)) >> 16) & 0xFF;\
		*((uint8_t *)(p) + 2) = (((uint32_t)(v)) >> 8) & 0xFF;\
		*((uint8_t *)(p) + 3) = (((uint32_t)(v)) >> 0) & 0xFF;\
	} while (0)
#define SET_U16_LSBFIRST(p, v)		\
	do{\
		*((uint8_t *)(p) + 0) = (((uint16_t)(v)) >> 0) & 0xFF;\
		*((uint8_t *)(p) + 1) = (((uint16_t)(v)) >> 8) & 0xFF;\
	} while (0)
#define SET_U24_LSBFIRST(p, v)		\
	do{\
		*((uint8_t *)(p) + 0) = (((uint32_t)(v)) >> 0) & 0xFF;\
		*((uint8_t *)(p) + 1) = (((uint32_t)(v)) >> 8) & 0xFF;\
		*((uint8_t *)(p) + 2) = (((uint32_t)(v)) >> 16) & 0xFF;\
	} while (0)
#define SET_U32_LSBFIRST(p, v)		\
	do{\
		*((uint8_t *)(p) + 0) = (((uint32_t)(v)) >> 0) & 0xFF;\
		*((uint8_t *)(p) + 1) = (((uint32_t)(v)) >> 8) & 0xFF;\
		*((uint8_t *)(p) + 2) = (((uint32_t)(v)) >> 16) & 0xFF;\
		*((uint8_t *)(p) + 3) = (((uint32_t)(v)) >> 24) & 0xFF;\
	} while (0)

#define SET_LE_U16(p, v)			SET_U16_LSBFIRST(p, v)
#define SET_LE_U24(p, v)			SET_U24_LSBFIRST(p, v)
#define SET_LE_U32(p, v)			SET_U32_LSBFIRST(p, v)
#define SET_BE_U16(p, v)			SET_U16_MSBFIRST(p, v)
#define SET_BE_U24(p, v)			SET_U24_MSBFIRST(p, v)
#define SET_BE_U32(p, v)			SET_U32_MSBFIRST(p, v)

#define SYNCSWPWM_IN_TIMER_INIT()		do{\
											DMA_InitTypeDef DMA_InitStructure;\
											TIM_ICInitTypeDef TIM_ICInitStructure;\
											\
											RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);\
											RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);\
											\
											DMA_DeInit(SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM);\
											DMA_StructInit(&DMA_InitStructure);\
											DMA_InitStructure.DMA_Channel = SWIM_PWM_IN_DMA_CHANNEL;\
											DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SYNCSWPWM_IN_TIMER->CCR1);\
											DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;\
											DMA_InitStructure.DMA_BufferSize = 0;\
											DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;\
											DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;\
											DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;\
											DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;\
											DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;\
											DMA_InitStructure.DMA_Priority = DMA_Priority_High;\
											DMA_Init(SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM, &DMA_InitStructure);\
											DMA_Cmd(SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM, ENABLE);\
											\
											DMA_DeInit(SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM);\
											DMA_StructInit(&DMA_InitStructure);\
											DMA_InitStructure.DMA_Channel = SWIM_PWM_IN_DMA_CHANNEL;\
											DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SYNCSWPWM_IN_TIMER->CCR2);\
											DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;\
											DMA_InitStructure.DMA_BufferSize = 0;\
											DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;\
											DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;\
											DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;\
											DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;\
											DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;\
											DMA_InitStructure.DMA_Priority = DMA_Priority_High;\
											DMA_Init(SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM, &DMA_InitStructure);\
											DMA_Cmd(SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM, ENABLE);\
											\
											TIM_ICStructInit(&TIM_ICInitStructure);\
											TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;\
											TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;\
											TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;\
											TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;\
											TIM_ICInitStructure.TIM_ICFilter    = 0;\
											TIM_PWMIConfig(SYNCSWPWM_IN_TIMER, &TIM_ICInitStructure);\
											\
											TIM_SelectInputTrigger(SYNCSWPWM_IN_TIMER, TIM_TS_TI2FP2);\
											TIM_SelectSlaveMode(SYNCSWPWM_IN_TIMER, TIM_SlaveMode_Reset);\
											TIM_SelectMasterSlaveMode(SYNCSWPWM_IN_TIMER, TIM_MasterSlaveMode_Enable);\
											TIM_DMACmd(SYNCSWPWM_IN_TIMER, TIM_DMA_CC2, ENABLE);\
											TIM_DMACmd(SYNCSWPWM_IN_TIMER, TIM_DMA_CC1, ENABLE);\
											\
											TIM_PrescalerConfig(SYNCSWPWM_IN_TIMER, 0, TIM_PSCReloadMode_Immediate);\
											TIM_Cmd(SYNCSWPWM_IN_TIMER, ENABLE);\
										}while(0)
#define SYNCSWPWM_IN_TIMER_FINI()		do{\
											TIM_DeInit(SYNCSWPWM_IN_TIMER);\
											RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);\
											DMA_DeInit(SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM);\
											DMA_DeInit(SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM);\
											RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, DISABLE);\
										}while(0)


#define SYNCSWPWM_IN_TIMER_RISE_DMA_INIT(l, a)	do{\
													SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM->CR &= ~1;\
													SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM->CR &= ~1;\
													SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM->CR &= ~1;\
													SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM->NDTR = (l);\
													SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM->M0AR = (uint32_t)(a);\
													DMA1->LIFCR = DMA_LIFCR_CTCIF0;\
													SYNCSWPWM_IN_TIMER_RISE_DMA_STREAM->CR |= 1;\
												}while(0)
#define SYNCSWPWM_IN_TIMER_RISE_DMA_READY()		(DMA1->LISR & DMA_LISR_TCIF0)
#define SYNCSWPWM_IN_TIMER_RISE_DMA_RESET()		(DMA1->LIFCR = DMA_LIFCR_CTCIF0)
#define SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(dly)	do{\
													while((!SYNCSWPWM_IN_TIMER_RISE_DMA_READY()) && --dly);\
													SYNCSWPWM_IN_TIMER_RISE_DMA_RESET();\
												}while(0)


#define SYNCSWPWM_IN_TIMER_FALL_DMA_INIT(l, a)	do{\
													SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM->CR &= ~1;\
													SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM->CR &= ~1;\
													SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM->CR &= ~1;\
													SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM->NDTR = (l);\
													SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM->M0AR = (uint32_t)(a);\
													DMA1->LIFCR = DMA_LIFCR_CTCIF3;\
													SYNCSWPWM_IN_TIMER_FALL_DMA_STREAM->CR |= 1;\
												}while(0)
#define SYNCSWPWM_IN_TIMER_FALL_DMA_READY()		(DMA1->LISR & DMA_LISR_TCIF3)
#define SYNCSWPWM_IN_TIMER_FALL_DMA_RESET()		(DMA1->IFCR = DMA_LIFCR_CTCIF3)
#define SYNCSWPWM_IN_TIMER_FALL_DMA_WAIT(dly)	do{\
													while((!SYNCSWPWM_IN_TIMER_FALL_DMA_READY()) && --dly);\
													SYNCSWPWM_IN_TIMER_FALL_DMA_RESET();\
												}while(0)


#define SYNCSWPWM_IN_TIMER_DMA_INIT(l, a, b)	do{\
													SYNCSWPWM_IN_TIMER_RISE_DMA_INIT((l), (a));\
													SYNCSWPWM_IN_TIMER_FALL_DMA_INIT((l), (b));\
												}while(0)
#define SYNCSWPWM_IN_TIMER_DMA_WAIT(dly)	do{\
												SYNCSWPWM_IN_TIMER_RISE_DMA_WAIT(dly);\
												SYNCSWPWM_IN_TIMER_FALL_DMA_WAIT(dly);\
											}while(0)





#define SYNCSWPWM_OUT_TIMER_INIT(p)		do{\
											DMA_InitTypeDef DMA_InitStructure;\
											TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;\
											TIM_OCInitTypeDef TIM_OCInitStructure;\
											\
											RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);\
											RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);\
											\
											DMA_DeInit(SWIM_PWM_OUT_DMA_STREAM);\
											DMA_StructInit(&DMA_InitStructure);\
                                            DMA_InitStructure.DMA_Channel = SWIM_PWM_OUT_DMA_CHANNEL;\
											DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SYNCSWPWM_OUT_TIMER->CCR1);\
											DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;\
											DMA_InitStructure.DMA_BufferSize = 0;\
											DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;\
											DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;\
											DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;\
											DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;\
											DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;\
											DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;\
											DMA_Init(SWIM_PWM_OUT_DMA_STREAM, &DMA_InitStructure);\
											DMA_Cmd(SWIM_PWM_OUT_DMA_STREAM, ENABLE);\
											\
											TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);\
											TIM_TimeBaseStructure.TIM_Prescaler = 0;\
											TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;\
											TIM_TimeBaseStructure.TIM_Period = 0;\
											TIM_TimeBaseStructure.TIM_ClockDivision = 0;\
											TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;\
											TIM_TimeBaseInit(SYNCSWPWM_OUT_TIMER, &TIM_TimeBaseStructure);\
											\
											TIM_OCStructInit(&TIM_OCInitStructure);\
											TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;\
											TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;\
											TIM_OCInitStructure.TIM_Pulse = 0;\
											TIM_OCInitStructure.TIM_OCPolarity = (p) ? TIM_OCPolarity_High : TIM_OCPolarity_Low;\
											TIM_OC1Init(SYNCSWPWM_OUT_TIMER, &TIM_OCInitStructure);\
											\
											TIM_OC1PreloadConfig(SYNCSWPWM_OUT_TIMER, TIM_OCPreload_Enable);\
											TIM_ARRPreloadConfig(SYNCSWPWM_OUT_TIMER, ENABLE);\
											TIM_DMACmd(SYNCSWPWM_OUT_TIMER, TIM_DMA_CC1, ENABLE);\
											TIM_Cmd(SYNCSWPWM_OUT_TIMER, ENABLE);\
											TIM_CtrlPWMOutputs(SYNCSWPWM_OUT_TIMER, ENABLE);\
										}while(0)
#define SYNCSWPWM_OUT_TIMER_FINI()		do{\
											TIM_DeInit(SYNCSWPWM_OUT_TIMER);\
                                            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);\
											DMA_DeInit(SWIM_PWM_OUT_DMA_STREAM);\
                                            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, DISABLE);\
										}while(0)

#define SYNCSWPWM_OUT_TIMER_SetCycle(cycle)	do {\
												SYNCSWPWM_OUT_TIMER->ARR = (cycle);\
												SYNCSWPWM_OUT_TIMER->EGR = TIM_PSCReloadMode_Immediate;\
											} while (0)
#define SYNCSWPWM_OUT_TIMER_GetCycle()	SYNCSWPWM_OUT_TIMER->ARR
#define SYNCSWPWM_OUT_TIMER_GetRate()	SYNCSWPWM_OUT_TIMER->CCR1
#define SYNCSWPWM_OUT_TIMER_SetRate(r)	SYNCSWPWM_OUT_TIMER->CCR1 = (r)
#define SYNCSWPWM_OUT_TIMER_DMA_INIT(l, a)	do{\
                        SYNCSWPWM_OUT_TIMER->EGR = TIM_PSCReloadMode_Immediate;\
												SWIM_PWM_OUT_DMA_STREAM->CR &= ~1;\
												SWIM_PWM_OUT_DMA_STREAM->CR &= ~1;\
												SWIM_PWM_OUT_DMA_STREAM->CR &= ~1;\
												SWIM_PWM_OUT_DMA_STREAM->NDTR = (l);\
												SWIM_PWM_OUT_DMA_STREAM->M0AR = (uint32_t)(a);\
												DMA1->HIFCR = DMA_HIFCR_CTCIF4;\
												SWIM_PWM_OUT_DMA_STREAM->CR |= 1;\
											}while(0)                      
#define SYNCSWPWM_OUT_TIMER_DMA_WAIT()	do{\
											while(!(DMA1->HISR & DMA_HISR_TCIF4));\
											DMA1->HIFCR = DMA_HIFCR_CTCIF4;\
										}while(0)


void SWIM_GPIO_Init(void);
u8 Begin_Pro(char *FileName, u8 lock_command);

#endif


