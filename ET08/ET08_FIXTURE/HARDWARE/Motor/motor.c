#include "motor.h"
#include "digit_out.h"


static void motor_signal_init(void);
static u8 up_signal_get(void);
static u8 down_signal_get(void);
static void referance_voltage(u8 status);



MOTOR_API motor={
  motor_signal_init,
  up_signal_get,
  down_signal_get,
  referance_voltage
};


static void timer1_capture_init(void);
static void pwm_capture_start(void);
static u32 pwm_capture_get(void);
static void pwm_module_ctrl(u8 mode);
PWM_API pwm = {
  timer1_capture_init,
	pwm_capture_start,
	pwm_capture_get,
	pwm_module_ctrl
};


void motor_signal_init(void)
{
	
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//GPIOE时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//GPIOA时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//GPIOB时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //MOTOR+ -
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化


  //if (mode)  GPIO_SetBits(GPIOG, GPIO_Pin_6);
  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //358基准电压
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50M  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //MOTOR+ -
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
} 



u8 up_signal_get(void){	
  return !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6); 
} 

u8 down_signal_get(void){	
  return !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5); 
} 


static void referance_voltage(u8 status){  
  digit.refvol(status);
}


static void timer1_capture_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA0

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_TIM1); //PA0复用位定时器5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); //PA0复用位定时器5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=167;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 0xffff;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	

	//初始化TIM1输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//下降沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x02;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM1, &TIM_ICInitStructure);

	TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); 
	TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset); 
		
//	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	


  /* 边沿触发中断 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
  /* 溢出中断 */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	TIM_Cmd(TIM1,DISABLE ); 	//使能定时器5
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM1CH1_CAPTURE_FLAG = 0;
u8  TIM1CH1_CAPTURE_STEP = 0;	//输入捕获状态
u32	TIM1CH1_CAPTURE_VAL[10];	//输入捕获值(TIM2/TIM5是32位)
//定时器5中断服务程序	 
void TIM1_CC_IRQHandler(void)
{ 		    

 	if(SET == TIM_GetITStatus(TIM1, TIM_IT_CC1))//还未成功捕获	
	{
    TIM1CH1_CAPTURE_VAL[TIM1CH1_CAPTURE_STEP] = TIM_GetCapture1(TIM1);
    TIM1CH1_CAPTURE_STEP = (TIM1CH1_CAPTURE_STEP + 1) % 10;
    TIM1CH1_CAPTURE_FLAG = (TIM1CH1_CAPTURE_FLAG < 20) 
		                       ? (TIM1CH1_CAPTURE_FLAG + 1)
													 : TIM1CH1_CAPTURE_FLAG;
  	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1); //清除中断标志位
 	}
}


void pwm_capture_start(void) {
  TIM1CH1_CAPTURE_FLAG = 0;
  TIM1CH1_CAPTURE_STEP = 0;	//输入捕获状态
  TIM_Cmd(TIM1,ENABLE );
}

u32 pwm_capture_get(void) {
  u32 res, max,min, sum;
  u8 index;

	if (20 == TIM1CH1_CAPTURE_FLAG){
		for (max = 0, min = 0xffffffff, index = 0, sum = 0; index < 10; index++){
			sum += TIM1CH1_CAPTURE_VAL[index];
			max = TIM1CH1_CAPTURE_VAL[index] > max
					? TIM1CH1_CAPTURE_VAL[index]
					: max;
			min = TIM1CH1_CAPTURE_VAL[index] < min
					? TIM1CH1_CAPTURE_VAL[index]
					: min;
		}
		res = (sum - max - min) / 8;
	} else {
    res = 0;
	}

  TIM_Cmd(TIM1,DISABLE );
	return res;
}


static void pwm_module_ctrl(u8 mode){
  digit.pwm(mode);
}




#if 0
void TIM1_UP_TIM10_IRQHandler(void){
  if (SET == TIM_GetITStatus(TIM1, TIM_IT_Update)){
    
	}




}
#endif