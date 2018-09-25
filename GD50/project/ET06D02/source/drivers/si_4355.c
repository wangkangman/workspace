#include "regdef.h"
#include "ClkRst.h"
#include "timwdt.h"
#include "si_4355.h"
#include <stdio.h>

/**
  * @brief  local variable;
  *
  */
static ETST_4355DATA      et_4355data_st;
static ETST_Receiver      Receiver_st;
static ETST_RadioPacket   RadioPacket;
static ETST_RadioPacket   lst_RadioPacket;


/**
  * @brief  inital code of si4355;
  *
  */
const  ETST_EzConfigArray EzConfigArray_433M92_FSK = { 
   {0xDD, 0xCA, 0x36, 0x9E, 0x76, 0x53, 0xDB, 0x5A, 0xFB, 0xC7, 0xAB, 0x79, 
    0x39, 0x18, 0xC5, 0x84, 0x86, 0x06, 0x21, 0x4A, 0x0D, 0x48, 0x9D, 0x9B, 
    0xCB, 0x76, 0x3F, 0x00, 0x13, 0x2A, 0xBC, 0x58, 0xA8, 0x24, 0xBF, 0xFF, 
    0x6E, 0x4B, 0x96, 0xA8, 0x76, 0xC1, 0x9C, 0xE2, 0x39, 0x0C, 0x50, 0x6A, 
    0x5D, 0xB3, 0xC7, 0xA8, 0x0B, 0x6F, 0x57, 0xC7, 0xF3, 0x04, 0x13, 0x39, 
    0x0B, 0xE1, 0x49, 0xF2, 0xA3, 0x17, 0x76, 0xD1, 0x5C, 0x6B, 0x4E, 0x3B, 
    0x98, 0xE4, 0x2D, 0x99, 0xB6, 0x12, 0xFD, 0x7F, 0xF7, 0x37, 0xA8, 0xC0, 
    0xBC, 0x16, 0x48, 0x0B, 0x72, 0x05, 0xEA, 0x88, 0xBA, 0x63, 0x19, 0xF3, 
    0xE9, 0x5E, 0x34, 0xC3, 0x9A, 0x9E, 0xE8, 0x6A, 0xEA, 0xF6, 0x1B, 0xC5, 
    0xCC, 0xA9, 0x7B, 0x8D, 0x21, 0xA0, 0x40, 0xEA, 0xA3, 0xAD, 0x5A, 0xD2, 
    0x15, 0x61, 0x01, 0x17, 0xFD, 0x7A, 0x32, 0x42, 0xEB, 0xEC, 0x35, 0x11, 
    0x20, 0x4B, 0x55, 0x07, 0x3B, 0xB0, 0x79, 0x38, 0x5B, 0x79, 0xFB, 0xE0, 
    0x95, 0xEA, 0x57, 0x07, 0xE0, 0x11, 0x0E, 0xF7, 0x12, 0x9C, 0x01, 0xEE, 
    0x93, 0x06, 0x50, 0x51, 0xCE, 0x73, 0x06, 0x35, 0x52, 0xE1, 0x8E, 0x31, 
    0x70, 0x8D, 0x2A, 0x7B, 0x11, 0x7B, 0x51, 0x5C, 0xDA, 0x1C, 0xBF, 0x69, 
    0x09, 0x4D, 0xB5, 0xB0, 0xE6, 0xFF, 0x72, 0x8B, 0xE2, 0x78, 0x78, 0xA5, 
    0x30, 0x70, 0xA8, 0x7A, 0x1E, 0x5F, 0xA1, 0xA4, 0xE6, 0x87 }, //Radio_Configuration[EZCONFIG_RADIO_CFG_SIZE];
    0x88FE,        // Radio_Configuration_CRC;
    0x01,          // Radio_BOOT_OPTIONS;
    0x00,          // Radio_XTAL_OPTIONS;
    30000000,     // Radio_XO_FREQ;
    0x05,          // Radio_INT_CTL_ENABLE;
    0x10,          // Radio_INT_CTL_PH_ENABLE;
    0x00,          // Radio_INT_CTL_MODEM_ENABLE;
    0x04,          // Radio_INT_CTL_CHIP_ENABLE;
    0x01,          // Radio_FRR_CTL_A_MODE;
    0x02,          // Radio_FRR_CTL_B_MODE;
    0x09,          // Radio_FRR_CTL_C_MODE;
    0x00,          // Radio_FRR_CTL_D_MODE;
    0x2D,          // Radio_SYNC_BITS_31_24;
    0xD4,          // Radio_SYNC_BITS_23_16;
    0X7F,          // Radio_PA_PWR_LVL;
    0X02,          // Radio_EZCONFIG_MODULATION;
    0X40,          // Radio_EZCONFIG_XO_TUNE;
    0X3C,          // Radio_FREQ_CONTROL_INTE;
    0X08,          // Radio_FREQ_CONTROL_FRAC_2;
    0X00,          // Radio_FREQ_CONTROL_FRAC_1;
    0x00,          // Radio_FREQ_CONTROL_FRAC_0;
    0x00,          // Radio_FREQ_CONTROL_CHANNEL_STEP_SIZE_1;
    0x00,          // Radio_FREQ_CONTROL_CHANNEL_STEP_SIZE_0;
    0x01,          // Radio_GPIO0_PIN_CFG;
    0x01,          // Radio_GPIO1_PIN_CFG;
    0x01,          // Radio_GPIO2_PIN_CFG;
    0x01,          // Radio_GPIO3_PIN_CFG;
    0x27,          // Radio_GPIO_NIRQ_MODE;
    0x01,          // Radio_GPIO_SDO_MODE;
    0x00,          // Radio_GPIO_GEN_CONFIG;
    0x01,          // Radio_Mode_After_Power_Up;
    0x1000,        // Radio_Delay_Cnt_After_Reset;
};

/**
  * @brief  local inital struct of si4355;
  *
  */
static ETST_EzConfigArray* pEzConfigArray;
static U08ET radioCmd[16];
static U08ET ctsWentHigh = 0;
static union si4355_cmd_reply_union Si4355Cmd;


/**
  * @brief  spi module inital;
  * @param  NONE
  * @retval NONE
  *
  */
VODET Si4355_Configuration(VODET) 
{ 
  U08ET index;
  sysclk_ctrl(CLK_ID_SPI, CLK_CT_ENA);
  
  /* 4355_SDN     <--> PC4  output */
  GPIO_PC_DDR |=  (1<<4);  /* direction output */
  GPIO_PC_CR1 |=  (1<<4);  /* float input      */
  GPIO_PC_CR2 &= ~(1<<4);  /* max speed 2MHz   */

  /* 4355_CS      <--> PE5  output */
  GPIO_PE_DDR |=  (1<<5);  /* direction output */
  GPIO_PE_CR1 |=  (1<<5);  /* float input      */
  GPIO_PE_CR2 &= ~(1<<5);  /* max speed 2MHz   */

  /* 4355_SCK     <--> PC5  output */
  GPIO_PC_DDR |=  (1<<5);  /* direction output */
  GPIO_PC_CR1 |=  (1<<5);  /* float input      */
  GPIO_PC_CR2 &= ~(1<<5);  /* max speed 2MHz   */

  /* 4355_MOSI    <--> PC6  output */
  GPIO_PC_DDR |=  (1<<6);  /* direction output */
  GPIO_PC_CR1 |=  (1<<6);  /* float input      */
  GPIO_PC_CR2 &= ~(1<<6);  /* max speed 2MHz   */

  /* 4355_MISO    <--> PC7  input  */
  GPIO_PC_DDR &= ~(1<<7);  /* direction input  */
  GPIO_PC_CR1 &= ~(1<<7);  /* float input      */
  GPIO_PC_CR2 &= ~(1<<7);  /* max speed 2MHz   */
  
  /* 4355_NIRQ    <--> PD0  input  */
  GPIO_PD_DDR &= ~(1<<0);  /* direction input  */
  GPIO_PD_CR1 &= ~(1<<0);  /* float input      */
  GPIO_PD_CR2 &= ~(1<<0);  /* max speed 2MHz   */


  /* SPI disable    */
  SPI_CR1 &= ~(1<<6);    
  /* MSB 1st       */
  SPI_CR1 &= ~(1<<7);  
  //SPI_CR1 |= (1<<7); 
  /* SPI baudrate-2M: [5:3]=010 div=8 bt = 16M/8 */
  SPI_CR1 &= ~(7<<3);
  SPI_CR1 |=  (3<<3);
  /* SPI Master */
  SPI_CR1 |=  (1<<2);
  /* SPI CPOL */
  SPI_CR1 &= ~(1<<1);
  //SPI_CR1 |= (1<<1);
  /* SPI CPHA */
  SPI_CR1 &= ~(1<<0);
  //SPI_CR1 |=  (1<<0);
  
  /* SSM SSI == 1 */
  SPI_CR2 |=  (1<<0);
  SPI_CR2 |=  (1<<1);
  
  SPI_ICR = 0x0;
  
  /* SPI enable    */
  SPI_CR1 |=  (1<<6);  
  
  
  Receiver_st.Receiver_State = SM_START;
  ctsWentHigh = 0;
  radio_hal_SetNsel();
  
  for(index = 0; index < 8; index ++) {
  	et_4355data_st.data[index] = 0x00;
  }
} 

#if 0
@far @interrupt VODET SPI_IRQHandler(VODET){
  if ((SPI_SR & 0X01) != 0x00) {
    SPI_ICR &= (~0x40);  //Disable receive interrupt
    if(et_spi.step < SPI_LEN_MAX) {
      et_spi.buf[et_spi.step] = SPI_DR;//Receive date
      et_spi.step++;
    }
    et_spi.overtime = 0;
  }
  SPI_ICR |= 0x40; //Enable interrupt  
}

VODET SPI_overtime_task(VODET) {
  et_spi.overtime++;
  if (et_spi.overtime > SPI_OVERTIME_CNT) {
      et_spi.step = 0;
      et_spi.overtime = 0;
  }
}

U08ET SPI_get_data(U08ET* pbuf) {
  U08ET index;
  if (et_spi.step != 8) return ET_FALSE;
  
  for (index = 0; index < SPI_LEN_MAX; index++)
    pbuf[index] = et_spi.buf[index];
    
  et_spi.step = 0;
  et_spi.overtime = 0;
  return ET_TRUE;
}
#endif


/**
 *  Receiver Demo application poll handler.
 *
 *  @author Sz. Papp
 *
 *  @note   Must be called periodically.
 *
 *****************************************************************************/
#if 0
VODET Receiver_sys_task(VODET){
  radio_hal_ClearNsel();
  SpiReadWrite(0x01);
  radio_hal_SetNsel();
} 
#endif

 #if 0
 VODET Receiver_sys_task(VODET){
   U08ET index, buf[9];
   U16ET delay,err;  
    vRadio_PowerUp();

   //vRadio_SetChip();

   si4355_reset();
   for (delay = 0; delay< 0x1000; delay++);

   err = 5000;
   while(err--){
       radio_hal_ClearNsel();
       SpiReadWrite(0x44);
       if(0xff ==  SpiReadWrite(0xff)) {
           radio_hal_SetNsel();
        	 break;
       }
         radio_hal_SetNsel();
   }    
    radio_hal_ClearNsel();
    SpiReadWrite(0x01);
    radio_hal_SetNsel();
    #ifdef SI4355DEBUG
    printf("err_1: %d \r\n",err);
    #endif

    err = 5000;
    while(err--){
        radio_hal_ClearNsel();
        SpiReadWrite(0x44);
        if(0xff == SpiReadWrite(0xff)) {
          for (index = 0; index < 9; index++){
            buf[index] = SpiReadWrite(0xFF);
		  }
		  radio_hal_SetNsel();
          break;
        }
        radio_hal_SetNsel();
    }  
    
#ifdef SI4355DEBUG
    printf("err_2: %d \r\n",err);
    printf("info:");
    for (index = 0; index < 9; index++){
           printf("0x%x ",(U16ET)buf[index]);
    }
                    printf("\r\n");
#endif
	err = 222;
 }
#endif
 
#if 1
VODET Receiver_sys_task(VODET)
{
//	et_4355data_st.cnt = et_4355data_st.cnt < 550 ? (et_4355data_st.cnt + 1) : et_4355data_st.cnt;
  /* Receiver Demo state machine */
  switch (Receiver_st.Receiver_State)
  {

  case SM_START:
    pEzConfigArray = &EzConfigArray_433M92_FSK;
    Receiver_st.Receiver_State = SM_STEP1;
    break;
    
  case SM_STEP1:
    vRadio_SetChip();
    Receiver_st.Receiver_State = SM_STEP2;
    break;

  case SM_STEP2:
    Radio_StartRX(0u);
    si4355_fifo_info(0x02);
    Receiver_st.Receiver_State = SM_STEP3;
    break;

  case SM_STEP3:
    #if 1
    /* Wait for incoming RF frame */

    /* Incoming RF Packet */
    if (gRadio_CheckReceived() == ET_TRUE)
    {
      /* RF Packet received */

      if (RadioPacket.Flags&(1u << 4u)){
        #ifdef SI4355DEBUG
        printf("get key_1 \r\n");
        #endif
      }
      
      if (RadioPacket.Flags&(1u << 3u)){        
        #ifdef SI4355DEBUG
        printf("get key_2 \r\n");
        #endif
      }
      
      if (RadioPacket.Flags&(1u << 2u)){
        #ifdef SI4355DEBUG
        printf("get key_3 \r\n");
        #endif
      }
      
      if (RadioPacket.Flags&(1u << 1u)){
        #ifdef SI4355DEBUG
        printf("get key_4 \r\n");
        #endif
      }
      
      if (RadioPacket.Flags&(1u << 0u)){
        #ifdef SI4355DEBUG
        printf("get key_5 \r\n");
        #endif
      }

      Receiver_st.Receiver_State = SM_STEP2;
    }
    #endif
    break;

  default:
    Receiver_st.Receiver_State = SM_START;
    break;
  }
}
#endif



/**
 *  Sets the radio according to the given configuration.
 *
 *  @note
 *
 **/
VODET vRadio_SetChip(VODET){
  U08ET lTemp;
    #ifdef SI4355DEBUG
  U08ET index;
    #endif
  
  
  do
  {
    #ifdef SI4355DEBUG
    printf("vRadio_SetChip \r\n");
    #endif
    /* Power Up the radio chip */
    vRadio_PowerUp();
    /* Load the 1st part of the configuration array (128 bytes) */
    si4355_write_ezconfig_array(128u, (U08ET *)(&(pEzConfigArray->Radio_Configuration[0])));
    /* Load the final part of the configuration array (128 bytes) */
    si4355_write_ezconfig_array(EZCONFIG_RADIO_CFG_SIZE - 128u, (U08ET *)(&(pEzConfigArray->Radio_Configuration[128])));


    lTemp = bRadio_Check_Ezconfig(pEzConfigArray->Radio_Configuration_CRC);
    #ifdef SI4355DEBUG
    printf("lTemp: %d \r\n",(U16ET)lTemp);
    #endif
  } while(lTemp != 0u);
  
  // Read ITs, clear pending ones
  si4355_get_int_status(0u, 0u, 0u);

    #ifdef SI4355DEBUG    
    printf("Si4355Cmd : ");
    for (index = 0; index < 8; index++)
      printf("0x%x ",(U16ET)radioCmd[index]);    

    printf("\r\n");    
    #endif

  /* Enable ITs */
  radioCmd[0] = SI4355_CMD_ID_SET_PROPERTY;
  radioCmd[1] = SI4355_PROP_GRP_ID_INT_CTL;
  radioCmd[2] = SI4355_PROP_GRP_LEN_INT_CTL;
  radioCmd[3] = SI4355_PROP_GRP_INDEX_INT_CTL_ENABLE;
  radioCmd[4] = (U08ET)pEzConfigArray->Radio_INT_CTL_ENABLE;
  radioCmd[5] = (U08ET)pEzConfigArray->Radio_INT_CTL_PH_ENABLE;
  radioCmd[6] = (U08ET)pEzConfigArray->Radio_INT_CTL_MODEM_ENABLE;
  radioCmd[7] = (U08ET)pEzConfigArray->Radio_INT_CTL_CHIP_ENABLE;
  radio_comm_SendCmd( 8, radioCmd );  

  // Configure Fast response registers
  radioCmd[0] = SI4355_CMD_ID_SET_PROPERTY;
  radioCmd[1] = SI4355_PROP_GRP_ID_FRR_CTL;
  radioCmd[2] = SI4355_PROP_GRP_LEN_FRR_CTL;
  radioCmd[3] = SI4355_PROP_GRP_INDEX_FRR_CTL_A_MODE;
  radioCmd[4] = (U08ET)pEzConfigArray->Radio_FRR_CTL_A_MODE;
  radioCmd[5] = (U08ET)pEzConfigArray->Radio_FRR_CTL_B_MODE;
  radioCmd[6] = (U08ET)pEzConfigArray->Radio_FRR_CTL_C_MODE;
  radioCmd[7] = (U08ET)pEzConfigArray->Radio_FRR_CTL_D_MODE;
  radio_comm_SendCmd( 8, radioCmd );

  // not Configure GPIO pins


  // Put the Radio into SLEEP state
  radioCmd[0] = 0x34;
  radioCmd[1] = pEzConfigArray->Radio_Mode_After_Power_Up;

  radio_comm_SendCmd( 2, radioCmd );
    
  // Read ITs, clear pending ones
//  si4355_get_int_status(0u, 0u, 0u);
  // Get part info
//  si4355_part_info();
}

/**
 *  Power up the Radio, set GPIOs to HiZ state, get Part Info
 *  then set Sleep mode.
 *
 *  @note
 *
 **/
VODET vRadio_PowerUp(VODET){
  U16ET wDelay = 0;
  U08ET index = 0;  
    
  #ifdef SI4355DEBUG
  printf("si4355_reset wait \r\n");
  #endif
  si4355_reset();
  #ifdef SI4355DEBUG
  printf("si4355_reset over \r\n");
  #endif
  
  for (; wDelay < pEzConfigArray->Radio_Delay_Cnt_After_Reset; wDelay++);  

  

  si4355_power_up(pEzConfigArray->Radio_BOOT_OPTIONS,
                  pEzConfigArray->Radio_XTAL_OPTIONS,
                  pEzConfigArray->Radio_XO_FREQ);
                  
  #ifdef SI4355DEBUG
  printf("vRadio_PowerUp wait \r\n");
  #endif
  radio_comm_PollCTS();
  #ifdef SI4355DEBUG
  printf("vRadio_PowerUp wait over \r\n");
  #endif
}


/*!
 * This functions is used to reset the si4355 radio by applying shutdown and
 * releasing it.  After this function @ref si4355_boot should be called.  You
 * can check if POR has completed by waiting 4 ms or by polling GPIO 0, 2, or 3.
 * When these GPIOs are high, it is safe to call @ref si4355_boot.
 */
VODET si4355_reset(VODET)
{
    U08ET loopCount;
     /* Put radio in shutdown, wait then release */
    PORT_SET(ET_PORTC, 4, 1);
    //! @todo this needs to be a better delay function.
    for (loopCount = 255; loopCount != 0; loopCount--);    
    PORT_SET(ET_PORTC, 4, 0);
}

VODET si4355_power_up(U08ET BOOT_OPTIONS, U08ET XTAL_OPTIONS, U32ET XO_FREQ){
  
  
    radioCmd[0] = SI4355_CMD_ID_POWER_UP;
    radioCmd[1] = BOOT_OPTIONS;
    radioCmd[2] = XTAL_OPTIONS;
    radioCmd[3] = (U08ET)((XO_FREQ >> 24) & 0xff);
    radioCmd[4] = (U08ET)((XO_FREQ >> 16) & 0xff);
    radioCmd[5] = (U08ET)((XO_FREQ >> 8) & 0xff);
    radioCmd[6] = (U08ET)((XO_FREQ) & 0xff);
    #ifdef SI4355DEBUG
    printf("si4355_power_up radio_comm_SendCmd wait \r\n");
    #endif
    
    radio_comm_SendCmd( SI4355_CMD_ARG_COUNT_POWER_UP, radioCmd );
    
    #ifdef SI4355DEBUG
    printf("si4355_power_up radio_comm_SendCmd over \r\n");
    #endif
}


/*!
 * Sends a command to the radio chip
 *
 * @param byteCount     Number of bytes in the command to send to the radio device
 * @param pData         Pointer to the command to send.
 */
VODET radio_comm_SendCmd(U08ET byteCount, U08ET* pData){
    /* There was a bug in A1 hardware that will not handle 1 byte commands. 
       It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
    if (byteCount == 1)
        byteCount++;

    while(!ctsWentHigh){
       radio_comm_PollCTS();
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}


/**
 *  Read/Write one byte to/from the SPI peripheral.
 *
 *  @param[in]  DataIn The data byte needs to be sent.
 *
 *  @return     Byte received from SPI.
 *
 *  @note
 *
 **/
U08ET SpiReadWrite(U08ET DataIn) {
  U08ET tmp;
  SPI_DR = DataIn;
  /* wait on tx */
  while(!(SPI_SR & (1 << 1)));
  /* check if spi is busy */
  while(!(SPI_SR & (1 << 0)));
  tmp = SPI_DR;
  return tmp;
}

VODET radio_hal_ClearNsel(VODET){
    PORT_SET(ET_PORTE, 5, 0);
}

VODET radio_hal_SetNsel(VODET){
    PORT_SET(ET_PORTE, 5, 1);
}

VODET radio_hal_SpiWriteData(U08ET byteCount, U08ET* pData){
	U08ET index;

//  printf("SpiWriteData: \r\n");
	for (index = 0; index < byteCount; index++){
	  SpiReadWrite(pData[index]);
//	  printf("0x%x ",(U16ET)pData[index]);
	}
//	printf("\r\n");
/*  while (byteCount--){
    SpiReadWrite(*pData);
    pData++;
  }*/
}

VODET radio_hal_SpiReadData(U08ET byteCount, U08ET* pData)
{
	U08ET index;
	for (index = 0; index < byteCount; index++)
	  pData[index] = SpiReadWrite(0xff);
	
/*  while (byteCount--){
    *pData = SpiReadWrite(0xff);
    pData++;
  }*/
}

VODET si4355_write_ezconfig_array(U08ET numBytes, U08ET* pbuf){
	#ifdef SI4355DEBUG
	U08ET index;
  printf("si4355_write_ezconfig \r\n");
/*  for(index = 0; index < numBytes; index++)      
      printf("0x%x ",(U16ET)pbuf[index]);
  
  printf("\r\n");
  */      
#endif

  radio_comm_WriteData(SI4355_CMD_ID_WRITE_TX_FIFO, 1, numBytes, pbuf);
}

/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 */
VODET radio_comm_WriteData(U08ET cmd, U08ET pollCts, U08ET byteCount, U08ET* pData)
{
    if(pollCts){
        while(!ctsWentHigh){
            radio_comm_PollCTS();
        }
    }
    
    radio_hal_ClearNsel();
    SpiReadWrite(cmd);
    radio_hal_SpiWriteData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}


/*!
 * Waits for CTS to be high
 *
 * @return CTS value
 */
U08ET radio_comm_PollCTS(void){
    return radio_comm_GetResp(0, 0);
}


/*!
 * Sends a command to the radio chip and gets a response
 *
 * @param cmdByteCount  Number of bytes in the command to send to the radio device
 * @param pCmdData      Pointer to the command data
 * @param respByteCount Number of bytes in the response to fetch
 * @param pRespData     Pointer to where to put the response data
 *
 * @return CTS value
 */
U08ET radio_comm_SendCmdGetResp(U08ET cmdByteCount, U08ET* pCmdData, U08ET respByteCount, U08ET* pRespData)
{
    radio_comm_SendCmd(cmdByteCount, pCmdData);
    return radio_comm_GetResp(respByteCount, pRespData);
}


/*!
 * Gets a command response from the radio chip
 *
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 *
 * @return CTS value
 */
U08ET radio_comm_GetResp(U08ET byteCount, U08ET* pData){
  
    U08ET ctsVal = 0;
    U16ET errCnt = RADIO_CTS_TIMEOUT;

    while (errCnt != 0) {
    	  wdtdog_feed();
        radio_hal_ClearNsel();
        SpiReadWrite(0x44);    //read CMD buffer
        ctsVal = SpiReadWrite(0xff);
  
        if(ctsVal == 0xFF){
            if(byteCount){
                radio_hal_SpiReadData(byteCount, pData);
            }
            radio_hal_SetNsel();
            break;
        }
        radio_hal_SetNsel();
        errCnt--;
    }
    
    if (ctsVal == 0xFF){
        ctsWentHigh = 1;
    }
    return ctsVal;
}


/**
 *  Local function to check the loaded EzConfig Array CRC.
 *
 *  @param[in]  crc CRC of EzConfig array.
 *
 *  @return     Result of the command.
 *
 *  @note
 *
 *****************************************************************************/
U08ET bRadio_Check_Ezconfig(U16ET crc)
{
  si4355_ezconfig_check(crc);
  return radioCmd[0];
}

VODET si4355_ezconfig_check(U16ET CHECKSUM)
{
	#ifdef SI4355DEBUG
	   printf("CHECKSUM: 0x%x\r\n", CHECKSUM);
	#endif	
  /* Do not check CTS */
  radio_hal_ClearNsel();
  /* Command byte */
  SpiReadWrite(SI4355_CMD_ID_EZCONFIG_CHECK);
  /* CRC */
  SpiReadWrite((U08ET) ((CHECKSUM >> 8u) & 0xff));
  SpiReadWrite((U08ET) (CHECKSUM & 0xff));
  radio_hal_SetNsel();
  /* Get the respoonse from the radio chip */
  radio_comm_GetResp(1, &radioCmd[0]);
}


VODET si4355_get_int_status(U08ET PH_CLR_PEND, U08ET MODEM_CLR_PEND, U08ET CHIP_CLR_PEND){    
    radioCmd[0] = SI4355_CMD_ID_GET_INT_STATUS;
    radioCmd[1] = PH_CLR_PEND;
    radioCmd[2] = MODEM_CLR_PEND;
    radioCmd[3] = CHIP_CLR_PEND;
    radio_comm_SendCmdGetResp( SI4355_CMD_ARG_COUNT_GET_INT_STATUS,
                              radioCmd,
                              SI4355_CMD_REPLY_COUNT_GET_INT_STATUS,
                              radioCmd );
    Si4355Cmd.GET_INT_STATUS.INT_PEND       = radioCmd[0];
    Si4355Cmd.GET_INT_STATUS.INT_STATUS     = radioCmd[1];
    Si4355Cmd.GET_INT_STATUS.PH_PEND        = radioCmd[2];
    Si4355Cmd.GET_INT_STATUS.PH_STATUS      = radioCmd[3];
    Si4355Cmd.GET_INT_STATUS.MODEM_PEND     = radioCmd[4];
    Si4355Cmd.GET_INT_STATUS.MODEM_STATUS   = radioCmd[5];
    Si4355Cmd.GET_INT_STATUS.CHIP_PEND      = radioCmd[6];
    Si4355Cmd.GET_INT_STATUS.CHIP_STATUS    = radioCmd[7];
}

VODET si4355_start_rx(U08ET CHANNEL, U08ET CONDITION, U16ET RX_LEN, U08ET NEXT_STATE1, U08ET NEXT_STATE2, U08ET NEXT_STATE3)
{
    radioCmd[0] = SI4355_CMD_ID_START_RX;
    radioCmd[1] = CHANNEL;
    radioCmd[2] = CONDITION;
    radioCmd[3] = (U08ET)(RX_LEN >> 8);
    radioCmd[4] = (U08ET)(RX_LEN);
    radioCmd[5] = NEXT_STATE1;
    radioCmd[6] = NEXT_STATE2;
    radioCmd[7] = NEXT_STATE3;

    radio_comm_SendCmd( SI4355_CMD_ARG_COUNT_START_RX, radioCmd );
}

VODET si4355_fifo_info(U08ET FIFO)
{
    radioCmd[0] = SI4355_CMD_ID_FIFO_INFO;
    radioCmd[1] = FIFO;

    radio_comm_SendCmdGetResp( SI4355_CMD_ARG_COUNT_FIFO_INFO,
                              radioCmd,
                              SI4355_CMD_REPLY_COUNT_FIFO_INFO,
                              radioCmd );

    Si4355Cmd.FIFO_INFO.RX_FIFO_COUNT   = radioCmd[0];
    Si4355Cmd.FIFO_INFO.TX_FIFO_SPACE   = radioCmd[1];
}

/**
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 *****************************************************************************/
VODET Radio_StartRX(U08ET channel)
{
  // Read ITs, clear pending ones
  si4355_get_int_status(0u, 0u, 0u);
  /* Start Receiving packet, channel 0, START immediately, Packet size by PH */
  si4355_start_rx(channel, 0u, 0u,
                  SI4355_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_RX,
                  SI4355_CMD_START_RX_ARG_RXVALID_STATE_ENUM_SLEEP,
                  SI4355_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
}


/**
 *  Check if Packet received IT flag is pending.
 *
 *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
 *
 *  @note
 *
 **/
U08ET gRadio_CheckReceived(VODET)
{
	#ifdef SI4355DEBUG
	U08ET index, buf[8];
	#endif
  if (1 == PORT_READ(ET_PORTD, 0))
  {
    /* Read ITs, clear pending ones */
    si4355_get_int_status(0u, 0u, 0u);

    /* check the reason for the IT */
    if (Si4355Cmd.GET_INT_STATUS.PH_PEND & SI4355_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT){
      /* Packet RX */
      radio_comm_ReadData( SI4355_CMD_ID_READ_RX_FIFO, 0, sizeof(ETST_RadioPacket), (U08ET *) &RadioPacket );
  	    #ifdef SI4355DEBUG
  	    printf("cnt: 0x%x 0x%x\r\n",(U16ET)(RadioPacket.RollingCounter&0xff),(U16ET)((RadioPacket.RollingCounter>>8)&0xff));
  	    #endif
  	    
  	  if (lst_RadioPacket.RollingCounter != RadioPacket.RollingCounter){
	  	  et_4355data_st.data[0] = ((U08ET)((RadioPacket.ChipID) & 0xff));
	  	  et_4355data_st.data[1] = ((U08ET)((RadioPacket.ChipID >> 8) & 0xff));
	  	  et_4355data_st.data[2] = ((U08ET)((RadioPacket.ChipID >> 16) & 0xff));
	  	  et_4355data_st.data[3] = 0;
	  	  et_4355data_st.data[3] |= ((U08ET)((RadioPacket.ChipID >> 24) & 0xf));
	  	  et_4355data_st.data[3] |= ((U08ET)((RadioPacket.Flags << 4) & 0xf0));
	  	  
	      et_4355data_st.flag = 1;
	    }
      lst_RadioPacket.ChipID = RadioPacket.ChipID;
      lst_RadioPacket.Flags = RadioPacket.Flags;
      lst_RadioPacket.RollingCounter = RadioPacket.RollingCounter;
      return ET_TRUE;
    }
  }

  return ET_FALSE;
}



/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip.
 * @param pData         Pointer to where to put the data.
 */
VODET radio_comm_ReadData(U08ET cmd, U08ET pollCts, U08ET byteCount, U08ET* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    radio_hal_ClearNsel();
    SpiReadWrite(cmd);
    radio_hal_SpiReadData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

void si4355_part_info(void)
{
    radioCmd[0] = SI4355_CMD_ID_PART_INFO;

    radio_comm_SendCmdGetResp( SI4355_CMD_ARG_COUNT_PART_INFO,
                              radioCmd,
                              SI4355_CMD_REPLY_COUNT_PART_INFO,
                              radioCmd );

}

U08ET si4355_getkey(U08ET *pbuf){
	U08ET index;
  if (0 == et_4355data_st.flag ) return 0;
  	
  for (index = 0; index < 4; index++){
  	pbuf[index] = et_4355data_st.data[index];
	}
  	
  	et_4355data_st.flag = 0;
  	return 1;
}