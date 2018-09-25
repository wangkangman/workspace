#ifndef _RKE_DEMO_H
#define _RKE_DEMO_H
/*------------------------------------------------------------------------------
 *                          Silicon Laboratories, Inc.
 *                           http://www.silabs.com
 *                               Copyright 2010
 *------------------------------------------------------------------------------
 *
 *    FILE:       rke_demo.h
 *    TARGET:     Si4010 RevC
 *    TOOLCHAIN:  Keil
 *    DATE:       March 31, 2012,
 *    RELEASE:    3.0 (Tamas Nagy), ROM version 02.00
 *
 *------------------------------------------------------------------------------
 *
 *    DESCRIPTION:
 *      Header file for the rke_demo module.
 *
 *------------------------------------------------------------------------------
 *
 *    INCLUDES:
 */
#include "si4010_types.h"



//define pinselect if frequency will be selected by gpio pins
#define PINSELECT
#define AESUSED
//define crystal if external crystal is used 
//#define CRYSTAL				
//defines GPIO pins used for button input (see button vector mapping in AN370)
#define bButtonMask_c   0x1F;	// GPIO0, GPIO1, GPIO2, GPIO3, GPIO4 

#define bEncodeNoneNrz_c       	0   /* No encoding */
#define bEncodeManchester_c    	1   /* Manchester encoding */
#define bEncode4b5b_c          	2   /* 4b-5b encoding */
#define bModOOK_c				0
#define bModFSK_c				1

#define bPreambleSize_c		   	13	
#define bSyncSize_c                     2       
#define bPayloadSize_c		   	25      
#define bCrcSize_c                      2	
#define bRke_FrameSize_c		(bPreambleSize_c + bSyncSize_c + bPayloadSize_c + bCrcSize_c)
#define bPayloadStartIndex_c            bPreambleSize_c + bSyncSize_c
#define bPreambleNrz_c			0xaa
#define bPreambleManch_c		0xff
#define bSync1_c			0x2d
#define bSync2_c			0xd4

  #define f_315_RkeFreqOOK_c		316660000.0	// for RKEdemo OOK
  #define f_315_RkeFreqFSK_c		316703093.0	// for RKEdemo FSK, upper frequency
  #define b_315_RkeFskDev_c			102	// deviation 43kHz
  #define b_315_PaLevel_c			60
  #define b_315_PaMaxDrv_c			0
  #define b_315_PaNominalCap_c		        270

  #define f_433_RkeFreqOOK_c		433920000.0	// for RKEdemo OOK
  #define f_433_RkeFreqFSK_a		433479050.0	// for RKEdemo FSK, upper frequency
  #define f_433_RkeFreqFSK_b		433979050.0	// for RKEdemo FSK, upper frequency
  #define f_433_RkeFreqFSK_c		434479050.0	// for RKEdemo FSK, upper frequency
  #define b_433_RkeFskDev_c			102	// deviation 59kHz
  #define b_433_PaLevel_c			77
  #define b_433_PaMaxDrv_c			1
  #define b_433_PaNominalCap_c		        192

  #define f_868_RkeFreqOOK_c		868300000.0	// for  OOK
  #define f_868_RkeFreqFSK_c		868418922.0	// for  FSK
  #define b_868_RkeFskDev_c			103     // deviation 119kHz
  #define b_868_PaLevel_c			77
  #define b_868_PaMaxDrv_c			1
  #define b_868_PaNominalCap_c		        81

  #define f_915_RkeFreqOOK_c		917000000.0	// for RKEdemo OOK
  #define f_915_RkeFreqFSK_c		917120194.0	// for RKEdemo FSK
  #define b_915_RkeFskDev_c			93      // deviation 120kHz
  #define b_915_PaLevel_c			77
  #define b_915_PaMaxDrv_c			1
  #define b_915_PaNominalCap_c		        69

#define iLowBatMv_c				2500


#define bRightButton_c          0x01
#define bMiddleButton_c         0x02	
#define bLeftButton_c 		0x04
#define bRearButton_c           0x08
#define bFrontButton_c          0x10
#define SELOUT                  GPIO6   //pin8 (SOIC)
#define FREQSEL0                GPIO8   //pin14 (SOIC)
#define FREQSEL1                GPIO9   //pin1 (SOIC)
#define SetToOokGPIO		GPIO1	//hold down this GPIO (middle button) while inserting battery to switch to OOK mode
#define PACAPPIN		GPIO7	//if this GPIO is pulled down to gnd, keyfob will transmit only two bytes, that contains the PACap value.


#define bAssocButton_c			(bRightButton_c | bLeftButton_c)
#define bPaCapButton_c			(bRearButton_c | bFrontButton_c)

#define M_AesBit_c			0x40
#define M_AssocBit_c			0x80
#define M_ButtonBits_c			0x1F

#define wSys_16Bit_40ms_c		41737
#define bHvram_BatteryVoltageByteAddr_c	0x02
#define bPacketCountAddr_c		0x05	//word
#define bModAddress_c			0x07
  /* Amount of time, the application will wait after boot without
   * getting a qualified button push before giving up and shutting the chip
   * down. */
#define	bMaxWaitForPush_c		50
#define wRepeatInterval_c		100
#define bRepeatCount_c			3
#define bBatteryWait_c 			200
 // This specifies the number of times that the RTC ISR is called between 
 // each call to the button service routine.  The actual time between
 // calls is dependent on the setting of RTC_CTRL.RTC_DIV which dictates how
 // often the RTC ISR is called (5ms in the demo).It means that the interval
 // between calls to the vBsr_Service() function is 5*2=10ms.
#define	bDebounceInterval_c		2
  
//---------------------------------------------------------------------------
//    PROTOTYPES OF STATIC FUNCTIONS:
//-------------------------------------------------------------------------
void vRkePacketAssemble    /* Assemble packet for Rke demo receiver*/
      (
        void
      );
void vButtonCheck       /* Buttoons checking */
      (
        void
      );
void vRepeatTxLoop
      (
       void
      );
void vCopyInputCode     /* Copy AES input from CODE */
      (
       BYTE const code *pbiSource,
       BYTE idata *pboData
      );
void vCalculateCrc
	  (
	  void
	  );
void vConvertPacket     /* change bit order and invert if necessary*/
      (
       BYTE bModType
      );
void vCustomFirstPowerUp   /* Function to be run if power_1st_time is 1 */
      (
        BYTE bWaitTime
      );
  //---------------------------------------------------------------------------
  //    VARIABLES:

  BYTE bMtp_WrStatus = 0;
  BYTE bIsr_DebounceCount;
  LWORD xdata lLEDOnTime;
  BYTE xdata bRepeatCount;
  LWORD lTimestamp;
  BYTE bdata bPrevBtn = 0;
  BYTE xdata *pbRke_FrameHead;//Pointer to the head of the frame to be sent out.
  BYTE xdata bRke_FrameSize;
  xdata BYTE abFrame[bRke_FrameSize_c];
  xdata LWORD lSyncCounter;
  xdata LWORD lPartID;
  code BYTE abAssocKey[16] = {0x75,0xe0,0x10,0x0e,0x37,0x9b,0x0d,0x6b,0x1d,0x16,
  	0x3c,0x30,0x1f,0x29,0x80,0xa9};
  code BYTE abBtnKey[16] = {0xb0,0x86,0x9c,0x60,0x5f,0xfb,0x13,0x30,0xe3,0x4a,
  	0x41,0x66,0x31,0xc0,0xc9,0xad};
  idata BYTE abAES1[16];
  idata BYTE abAES2[16];
  BYTE bBatStatus;
  code BYTE abConvTable[16] = {0x00,0x88,0x44,0xcc,0x22,0xaa,0x66,0xee,0x11,0x99,
  	0x55,0xdd,0x33,0xbb,0x77,0xff};
  code int  iMaxBatteryMv_c = 3250;
  xdata int iCurrentTemp;
  int           iBatteryMv;
 
  BYTE bStatus;
  BYTE xdata bButtonState;

  float xdata fRkeDesiredFreqOOK;
  float xdata fRkeDesiredFreqFSK;
  float xdata fRkeDesiredFreq;
  BYTE  xdata bRkeFskDev;
  BYTE	bPreamble;
  /* Structure for setting up the ODS .. must be in XDATA */
  tOds_Setup xdata rOdsSetup;

  tFCast_XoSetup xdata rXoSetup;

  /* Structure for setting up the PA .. must be in XDATA */
  tPa_Setup xdata  rPaSetup;

  /* BSR control structure */
  tBsr_Setup xdata rBsrSetup;
  BYTE xdata abBsrPtsPlaceHolder [6] = {0};
  WORD wPacketCount;

#endif /* _RKE_DEMO_H */