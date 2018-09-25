#ifndef _SPI_H
#define _SPI_H

#include "global.h"

//#define SI4355DEBUG

#define SPI_LEN_MAX 8
#define SPI_OVERTIME_CNT 50

#define SI4355_CMD_ID_POWER_UP                      (0x02)
#define SI4355_CMD_ARG_COUNT_POWER_UP               (7)
#define RADIO_CTS_TIMEOUT                           (5000)
#define SI4355_CMD_ID_WRITE_TX_FIFO                 (0x66)
#define SI4355_CMD_ID_EZCONFIG_CHECK                (0x19)


#define SI4355_CMD_ID_READ_RX_FIFO                  (0x77)
#define SI4355_CMD_ID_PART_INFO                     (0x01)
#define SI4355_CMD_ARG_COUNT_PART_INFO              (0x01)
#define SI4355_CMD_REPLY_COUNT_PART_INFO            (9)

#define SI4355_CMD_ARG_COUNT_GET_INT_STATUS         (4)
#define SI4355_CMD_REPLY_COUNT_GET_INT_STATUS       (8)

#define SI4355_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT  (0x10)

#define SI4355_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_RX   (8)
#define SI4355_CMD_START_RX_ARG_RXVALID_STATE_ENUM_SLEEP  (1)
#define SI4355_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX   (8)

#define SI4355_CMD_ID_GET_INT_STATUS                      (0x20)


#define SI4355_CMD_ID_SET_PROPERTY                        (0x11)
#define SI4355_PROP_GRP_ID_INT_CTL                        (1)
#define SI4355_PROP_GRP_LEN_INT_CTL                       (4)
#define SI4355_PROP_GRP_INDEX_INT_CTL_ENABLE              (0)
#define SI4355_PROP_GRP_ID_FRR_CTL                        (2)
#define SI4355_PROP_GRP_LEN_FRR_CTL                       (4)
#define SI4355_PROP_GRP_INDEX_FRR_CTL_A_MODE              (0)

#define SI4355_CMD_ID_START_RX                            (0x32)
#define SI4355_CMD_ARG_COUNT_START_RX                     (8)
#define SI4355_CMD_ID_FIFO_INFO                           (0x15)
#define SI4355_CMD_ARG_COUNT_FIFO_INFO                    (2)
#define SI4355_CMD_REPLY_COUNT_FIFO_INFO                  (2)

#define SI4455_CMD_ID_READ_RX_FIFO                        (0x77)




#define va_start(ap,v) ap = (va_list)&v + sizeof(v)
#define va_arg(ap,t) (((t *)ap)++[0])
#define va_end(ap)

typedef  struct {
  U08ET data[8];
  U08ET flag;
}ETST_4355DATA;

/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
#define EZCONFIG_ARRAY_SIZE     256u
#define EZCONFIG_RADIO_CFG_SIZE 202u

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
    U08ET   Radio_Configuration[EZCONFIG_RADIO_CFG_SIZE];
    U16ET   Radio_Configuration_CRC;

    U08ET   Radio_BOOT_OPTIONS;
    U08ET   Radio_XTAL_OPTIONS;
    U32ET   Radio_XO_FREQ;

    U08ET   Radio_INT_CTL_ENABLE;
    U08ET   Radio_INT_CTL_PH_ENABLE;
    U08ET   Radio_INT_CTL_MODEM_ENABLE;
    U08ET   Radio_INT_CTL_CHIP_ENABLE;

    U08ET   Radio_FRR_CTL_A_MODE;
    U08ET   Radio_FRR_CTL_B_MODE;
    U08ET   Radio_FRR_CTL_C_MODE;
    U08ET   Radio_FRR_CTL_D_MODE;

    U08ET   Radio_SYNC_BITS_31_24;
    U08ET   Radio_SYNC_BITS_23_16;

    U08ET   Radio_PA_PWR_LVL;

    U08ET   Radio_EZCONFIG_MODULATION;
    U08ET   Radio_EZCONFIG_XO_TUNE;

    U08ET   Radio_FREQ_CONTROL_INTE;
    U08ET   Radio_FREQ_CONTROL_FRAC_2;
    U08ET   Radio_FREQ_CONTROL_FRAC_1;
    U08ET   Radio_FREQ_CONTROL_FRAC_0;

    U08ET   Radio_FREQ_CONTROL_CHANNEL_STEP_SIZE_1;
    U08ET   Radio_FREQ_CONTROL_CHANNEL_STEP_SIZE_0;

    U08ET   Radio_GPIO0_PIN_CFG;
    U08ET   Radio_GPIO1_PIN_CFG;
    U08ET   Radio_GPIO2_PIN_CFG;
    U08ET   Radio_GPIO3_PIN_CFG;
    U08ET   Radio_GPIO_NIRQ_MODE;
    U08ET   Radio_GPIO_SDO_MODE;
    U08ET   Radio_GPIO_GEN_CONFIG;

    U08ET   Radio_Mode_After_Power_Up;

    U16ET   Radio_Delay_Cnt_After_Reset;
} ETST_EzConfigArray;

typedef enum
{
  SM_START,
  SM_STEP1,
  SM_STEP2,
  SM_STEP3,
  SM_STEP4,
  SM_STEP5,
  SM_STEP6,
  SM_STEP7,
  SM_STEP8,
  SM_STEP9,
  SM_STEP10,
  SM_STEP11,
  SM_STEP12,
  SM_STEP13,
  SM_STEP14,
  SM_STEP15
} eSMStates;

typedef struct
{
  eSMStates       Receiver_State;
  eSMStates       Receiver_ReturnState;
  volatile U08ET  Receiver_BeepLength;
} ETST_Receiver;

typedef struct
{
  U32ET       ChipID;
  U08ET       Flags;          /* 4 bit - Buttons */
  U16ET       RollingCounter;
} ETST_RadioPacket;

/* This section contains command map declarations */
struct si4355_reply_GENERIC_map {
    U08ET  REPLY1;
    U08ET  REPLY2;
    U08ET  REPLY3;
    U08ET  REPLY4;
    U08ET  REPLY5;
    U08ET  REPLY6;
    U08ET  REPLY7;
    U08ET  REPLY8;
    U08ET  REPLY9;
    U08ET  REPLY10;
    U08ET  REPLY11;
    U08ET  REPLY12;
    U08ET  REPLY13;
    U08ET  REPLY14;
    U08ET  REPLY15;
    U08ET  REPLY16;
};

struct si4355_reply_PART_INFO_map {
    U08ET  CHIPREV;
    UU16 PART;
    U08ET  PBUILD;
    UU16 ID;
    U08ET  CUSTOMER;
    U08ET  ROMID;
    U08ET  BOND;
};

struct si4355_reply_TEST_DATA_map {
    U08ET  TEST0;
    U08ET  TEST1;
    U08ET  TEST2;
    U08ET  TEST3;
    U08ET  TEST4;
    U08ET  TEST5;
    U08ET  TEST6;
    U08ET  TEST7;
    U08ET  TEST8;
    U08ET  TEST9;
    U08ET  TESTA;
    U08ET  TESTB;
    U08ET  TESTC;
    U08ET  TESTD;
    U08ET  TESTE;
    U08ET  TESTF;
};

struct si4355_reply_FUNC_INFO_map {
    U08ET  REVEXT;
    U08ET  REVBRANCH;
    U08ET  REVINT;
    UU16 PATCH;
    U08ET  FUNC;
    U08ET  SVNFLAGS;
    UU32 SVNREV;
};

struct si4355_reply_GET_PROPERTY_map {
    U08ET  DATA0;
    U08ET  DATA1;
    U08ET  DATA2;
    U08ET  DATA3;
    U08ET  DATA4;
    U08ET  DATA5;
    U08ET  DATA6;
    U08ET  DATA7;
    U08ET  DATA8;
    U08ET  DATA9;
    U08ET  DATA10;
    U08ET  DATA11;
    U08ET  DATA12;
    U08ET  DATA13;
    U08ET  DATA14;
    U08ET  DATA15;
};

struct si4355_reply_GPIO_PIN_CFG_map {
    U08ET  GPIO0;
    U08ET  GPIO1;
    U08ET  GPIO2;
    U08ET  GPIO3;
    U08ET  NIRQ;
    U08ET  SDO;
    U08ET  GEN_CONFIG;
};

struct si4355_reply_FIFO_INFO_map {
    U08ET  RX_FIFO_COUNT;
    U08ET  TX_FIFO_SPACE;
};

struct si4355_reply_EZCONFIG_CHECK_map {
    U08ET  RESULT;
};

struct si4355_reply_GET_INT_STATUS_map {
    U08ET  INT_PEND;
    U08ET  INT_STATUS;
    U08ET  PH_PEND;
    U08ET  PH_STATUS;
    U08ET  MODEM_PEND;
    U08ET  MODEM_STATUS;
    U08ET  CHIP_PEND;
    U08ET  CHIP_STATUS;
};

struct si4355_reply_REQUEST_DEVICE_STATE_map {
    U08ET  CURR_STATE;
    U08ET  CURRENT_CHANNEL;
};

struct si4355_reply_READ_CMD_BUFF_map {
    U08ET  CMD_BUFF0;
    U08ET  CMD_BUFF1;
    U08ET  CMD_BUFF2;
    U08ET  CMD_BUFF3;
    U08ET  CMD_BUFF4;
    U08ET  CMD_BUFF5;
    U08ET  CMD_BUFF6;
    U08ET  CMD_BUFF7;
    U08ET  CMD_BUFF8;
    U08ET  CMD_BUFF9;
    U08ET  CMD_BUFF10;
    U08ET  CMD_BUFF11;
    U08ET  CMD_BUFF12;
    U08ET  CMD_BUFF13;
    U08ET  CMD_BUFF14;
    U08ET  CMD_BUFF15;
};

struct si4355_reply_FRR_A_READ_map {
    U08ET  FRR_A_VALUE;
    U08ET  FRR_B_VALUE;
    U08ET  FRR_C_VALUE;
    U08ET  FRR_D_VALUE;
};

struct si4355_reply_FRR_B_READ_map {
    U08ET  FRR_B_VALUE;
    U08ET  FRR_C_VALUE;
    U08ET  FRR_D_VALUE;
    U08ET  FRR_A_VALUE;
};

struct si4355_reply_FRR_C_READ_map {
    U08ET  FRR_C_VALUE;
    U08ET  FRR_D_VALUE;
    U08ET  FRR_A_VALUE;
    U08ET  FRR_B_VALUE;
};

struct si4355_reply_FRR_D_READ_map {
    U08ET  FRR_D_VALUE;
    U08ET  FRR_A_VALUE;
    U08ET  FRR_B_VALUE;
    U08ET  FRR_C_VALUE;
};


/* The union that stores the reply written back to the host registers */
union si4355_cmd_reply_union {
    U08ET                                                                     RAW[16];
    struct si4355_reply_GENERIC_map                                        GENERIC;
    struct si4355_reply_PART_INFO_map                                      PART_INFO;
    struct si4355_reply_TEST_DATA_map                                      TEST_DATA;
    struct si4355_reply_FUNC_INFO_map                                      FUNC_INFO;
    struct si4355_reply_GET_PROPERTY_map                                   GET_PROPERTY;
    struct si4355_reply_GPIO_PIN_CFG_map                                   GPIO_PIN_CFG;
    struct si4355_reply_FIFO_INFO_map                                      FIFO_INFO;
    struct si4355_reply_EZCONFIG_CHECK_map                                 EZCONFIG_CHECK;
    struct si4355_reply_GET_INT_STATUS_map                                 GET_INT_STATUS;
    struct si4355_reply_REQUEST_DEVICE_STATE_map                           REQUEST_DEVICE_STATE;
    struct si4355_reply_READ_CMD_BUFF_map                                  READ_CMD_BUFF;
    struct si4355_reply_FRR_A_READ_map                                     FRR_A_READ;
    struct si4355_reply_FRR_B_READ_map                                     FRR_B_READ;
    struct si4355_reply_FRR_C_READ_map                                     FRR_C_READ;
    struct si4355_reply_FRR_D_READ_map                                     FRR_D_READ;
};






VODET SPI_Slave_Configuration(VODET);
VODET SPI_IRQ_task(VODET);
VODET SPI_overtime_task(VODET);
U08ET SPI_get_data(U08ET* pbuf);
@far @interrupt VODET SPI_IRQHandler(VODET);

VODET Si4355_Configuration(VODET);
VODET Receiver_sys_task(VODET);




VODET vRadio_SetChip(VODET);
VODET vRadio_PowerUp(VODET);
VODET si4355_reset(VODET);
VODET si4355_power_up(U08ET BOOT_OPTIONS, U08ET XTAL_OPTIONS, U32ET XO_FREQ);
VODET radio_comm_SendCmd(U08ET byteCount, U08ET* pData);
VODET radio_hal_ClearNsel(VODET);
VODET radio_hal_SetNsel(VODET);
VODET radio_hal_SpiWriteData(U08ET byteCount, U08ET* pData);
U08ET SpiReadWrite(U08ET DataIn);
VODET radio_hal_SpiReadData(U08ET byteCount, U08ET* pData);
U08ET radio_comm_PollCTS(void);
U08ET radio_comm_GetResp(U08ET byteCount, U08ET* pData);
VODET si4355_write_ezconfig_array(U08ET numBytes, U08ET* pbuf);
VODET radio_comm_WriteData(U08ET cmd, U08ET pollCts, U08ET byteCount, U08ET* pData);
U08ET bRadio_Check_Ezconfig(U16ET crc);
VODET si4355_ezconfig_check(U16ET CHECKSUM);
VODET si4355_get_int_status(U08ET PH_CLR_PEND, U08ET MODEM_CLR_PEND, U08ET CHIP_CLR_PEND);
U08ET radio_comm_SendCmdGetResp(U08ET cmdByteCount, U08ET* pCmdData, U08ET respByteCount, U08ET* pRespData);
VODET si4355_set_property( U08ET GROUP, U08ET NUM_PROPS, U08ET START_PROP, ... );
VODET si4355_get_int_status(U08ET PH_CLR_PEND, U08ET MODEM_CLR_PEND, U08ET CHIP_CLR_PEND);
VODET si4355_start_rx(U08ET CHANNEL, U08ET CONDITION, U16ET RX_LEN, U08ET NEXT_STATE1, U08ET NEXT_STATE2, U08ET NEXT_STATE3);
VODET si4355_fifo_info(U08ET FIFO);
VODET Radio_StartRX(U08ET channel);
VODET radio_comm_ReadData(U08ET cmd, U08ET pollCts, U08ET byteCount, U08ET* pData);
U08ET gRadio_CheckReceived(VODET);
void si4355_part_info(void);
U08ET si4355_getkey(U08ET *pbuf);

#endif /* _UARTCT_H */
