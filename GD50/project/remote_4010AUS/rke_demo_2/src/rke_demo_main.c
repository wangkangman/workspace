/*------------------------------------------------------------------------------
 *                          Silicon Laboratories, Inc.
 *                           http://www.silabs.com
 *                               Copyright 2010
 *------------------------------------------------------------------------------
 *
 *    FILE:       rke_demo_main.c
 *    TARGET:     Si4010 RevC
 *    TOOLCHAIN:  Keil
 *    DATE:       March 31, 2012,
 *    RELEASE:    3.0 (Tamas Nagy), ROM version 02.00
 *
 *------------------------------------------------------------------------------
 *
 *    DESCRIPTION:
 *      This file contains the main function for the Keil toolchain
 *      sample rke_demo_2 project.
 *
 *      BL51 linker directives for building this application:
 *
 *      BL51: PL(68) PW(78) IXREF RS(256)
 *            CODE (0x0-0x08FF)
 *            DATA (0x40)
 *            IDATA (0x70)
 *            XDATA (0x0900-0x107F)
 *            STACK (?STACK(0x90))
 *
 *------------------------------------------------------------------------------
 *
 *    INCLUDES:
 */
#include "stdlib.h"
#include "si4010.h"
#include "si4010_api_rom.h"

// Demo header
#include "rke_demo.h"

void  main (void)
{

/*------------------------------------------------------------------------------
 *    SETUP PHASE
 *------------------------------------------------------------------------------
 */
//Set DMD interrupt to high priority,
// any other interrupts have to stay low priority
  PDMD=1;
  // Disable the Matrix and Roff modes on GPIO[3:1] 
  PORT_CTRL &= ~(M_PORT_MATRIX | M_PORT_ROFF | M_PORT_STROBE);
  PORT_CTRL |=  M_PORT_STROBE;
  PORT_CTRL &= (~M_PORT_STROBE);
  // Turn LED control off 
  GPIO_LED = 0;
  vSys_Setup( 1 );
  vSys_SetMasterTime(0);
// Setup the bandgap, set to 2 because MTP used 
  vSys_BandGapLdo( 2 );

  // Check the first power up bit meaning keyfob is powered on by battery insertion
  if ( 0 != (SYSGEN & M_POWER_1ST_TIME) )
  {
   	vCustomFirstPowerUp(10); // Function will shutdown if part is in user or run mode.
  }



  // Set LED intensity .. acceptable values are 0 (off), 1, 2 and 3 
  vSys_LedIntensity( 3 );
  lLEDOnTime=20;
 
  //Get part ID (4byte factory burned unique serial number)
  lPartID = lSys_GetProdId();
  // Initialize isr call counter 
  bIsr_DebounceCount = 0;
  // BSR parameters initialization 
  rBsrSetup.bButtonMask = bButtonMask_c;
#ifdef CRYSTAL
  rBsrSetup.bButtonMask &= 0xFE;// clear bit0; GPIO0 used by crystal
#endif
  rBsrSetup.pbPtsReserveHead = abBsrPtsPlaceHolder;
  rBsrSetup.bPtsSize = 3;
  rBsrSetup.bPushQualThresh = 3;

  // Setup the BSR 
  vBsr_Setup( &rBsrSetup );

// Setup the RTC to tick every 5ms and clear it. Keep it disabled. 
  RTC_CTRL = (0x07 << B_RTC_DIV) | M_RTC_CLR;

// Enable the RTC 
  RTC_CTRL |= M_RTC_ENA;

// Enable the RTC interrupt and global interrupt enable 
  ERTC = 1;
  EA = 1;
//------------------------------------------------------------
// Setup the PA.
/*#ifdef PINSELECT
//set SELOUT to zero
SELOUT = 0;
SELOUT = 0;
//FREQSEL pins are open and pulled up internally (==1), or connected to SELOUT (==0)
if ((FREQSEL0==1) & (FREQSEL1==1))
{*/ //433MHz
    fRkeDesiredFreqOOK 	 = f_433_RkeFreqOOK_c;
	fRkeDesiredFreqFSK 	 = f_433_RkeFreqFSK_b;
	bRkeFskDev 		 = b_433_RkeFskDev_c;
	rPaSetup.bLevel      = b_433_PaLevel_c;
  	rPaSetup.wNominalCap = b_433_PaNominalCap_c;
  	rPaSetup.bMaxDrv     = b_433_PaMaxDrv_c;
/*}
if ((FREQSEL0==0) & (FREQSEL1==1))
{ //868MHz
	fRkeDesiredFreqOOK 	 = f_868_RkeFreqOOK_c;
	fRkeDesiredFreqFSK 	 = f_868_RkeFreqFSK_c;
	bRkeFskDev 		 = b_868_RkeFskDev_c;
	rPaSetup.bLevel      = b_868_PaLevel_c;
  	rPaSetup.wNominalCap = b_868_PaNominalCap_c;
  	rPaSetup.bMaxDrv     = b_868_PaMaxDrv_c;
}
if ((FREQSEL0==1) & (FREQSEL1==0))
{ //315MHz
	fRkeDesiredFreqOOK 	 = f_315_RkeFreqOOK_c;
	fRkeDesiredFreqFSK 	 = f_315_RkeFreqFSK_c;
	bRkeFskDev 		 = b_315_RkeFskDev_c;
	rPaSetup.bLevel      = b_315_PaLevel_c;
  	rPaSetup.wNominalCap = b_315_PaNominalCap_c;
  	rPaSetup.bMaxDrv     = b_315_PaMaxDrv_c;
}
if ((FREQSEL0==0) & (FREQSEL1==0))
{ //915MHz
	fRkeDesiredFreqOOK 	 = f_915_RkeFreqOOK_c;
	fRkeDesiredFreqFSK 	 = f_915_RkeFreqFSK_c;
	bRkeFskDev 		 = b_915_RkeFskDev_c;
	rPaSetup.bLevel      = b_915_PaLevel_c;
  	rPaSetup.wNominalCap = b_915_PaNominalCap_c;
  	rPaSetup.bMaxDrv     = b_915_PaMaxDrv_c;
}
SELOUT = 1;
#else		//if PINSELECT is not used, the following settings are used
	fRkeDesiredFreqOOK 	 = f_868_RkeFreqOOK_c;
	fRkeDesiredFreqFSK 	 = f_868_RkeFreqFSK_c;
	bRkeFskDev 		 = b_868_RkeFskDev_c;
	rPaSetup.bLevel      = b_868_PaLevel_c;
  	rPaSetup.wNominalCap = b_868_PaNominalCap_c;
  	rPaSetup.bMaxDrv     = b_868_PaMaxDrv_c;
#endif
*/
  rPaSetup.fAlpha      = 0.0;
  rPaSetup.fBeta       = 0.0;

  vPa_Setup( &rPaSetup );

/*  if (bHvram_Read(bModAddress_c) == bModOOK_c)	//read modulation setting from HVRAM
  {
  rOdsSetup.bModulationType = bModOOK_c;
// Setup the STL encoding for Manchester. No user encoding function therefore the pointer is NULL. 
  vStl_EncodeSetup( bEncodeManchester_c, NULL );
  fRkeDesiredFreq = fRkeDesiredFreqOOK;
  bPreamble = bPreambleManch_c;
  }
  if (bHvram_Read(bModAddress_c) == bModFSK_c)	//read modulation setting from HVRAM
  {
  */
  rOdsSetup.bModulationType = bModFSK_c;
// Setup the STL encoding for none. No user encoding function therefore the pointer is NULL.
  vStl_EncodeSetup( bEnc_NoneNrz_c, NULL );
  fRkeDesiredFreq = fRkeDesiredFreqFSK;
  bPreamble = bPreambleNrz_c;
  //}
/*#ifdef PACAPPIN
  if (PACAPPIN == 0)	
  {
  rOdsSetup.bModulationType = bModOOK_c;
// Setup the STL encoding for Manchester. No user encoding function therefore the pointer is NULL. 
  vStl_EncodeSetup( bEnc_NoneNrz_c, NULL );
  fRkeDesiredFreq = fRkeDesiredFreqOOK;
  }
#endif
*/
// Setup the ODS 
  // Set group width to 7, which means 8 bits/encoded byte to be transmitted.
  // The value must match the output width of the data encoding function
  // set by the vStl_EncodeSetup()! 
  rOdsSetup.bGroupWidth     = 7;
  rOdsSetup.bClkDiv         = 5;
  rOdsSetup.bEdgeRate       = 0;
  // Configure the warm up intervals 
  rOdsSetup.bLcWarmInt  = 0;
  rOdsSetup.bDivWarmInt = 5;
  rOdsSetup.bPaWarmInt  = 4;
  // 24MHz / (bClkDiv+1) / 9.6kbps = 417 
  rOdsSetup.wBitRate        = 417;
  // 24MHz / (bClkDiv+1) / 19.2kbps = 208 //by hugong
//  rOdsSetup.wBitRate        = 208;

  vOds_Setup( &rOdsSetup );

// Setup frequency casting .. needed to be called once per boot 
  vFCast_Setup();

#ifdef CRYSTAL
//XO Setup
  rXoSetup.fXoFreq	= 10000000.0;   // Frequency of the external crystal [Hz]
  rXoSetup.bLowCap	= 1;			// Capacitance setup of crystal 0 = above 14pf, 1 = below 14pf
  vFCast_XoSetup( &rXoSetup );
#endif

  // Read the battery voltage in mV, only once per boot to save power
  // Make loaded measurement .. 200 * 17 = 3.4ms wait after loading
  iBatteryMv = iMVdd_Measure( bBatteryWait_c );
  // Convert to RkeDemo format
  if (iBatteryMv > iMaxBatteryMv_c) 
  {
    iBatteryMv = iMaxBatteryMv_c;
  }
  bBatStatus = (iBatteryMv-1700)/50;
  if (iBatteryMv < iLowBatMv_c) 
  {
	bBatStatus |= 0xe0;
  }
  else
  {
	bBatStatus |= 0xa0;
  }
  
  // Setup the DMD temperature sensor for temperature mode 
  vDmdTs_RunForTemp( 3 ); // Skip first 3 samples 
  // Wait until there is a valid DMD temp sensor sample 
  while ( 0 == bDmdTs_GetSamplesTaken() );
  // Retrieve the temperature sample in internal units 
  //This temperature measurement has low absolute accuracy and not intended to be used for ambient temperature measurement. 
  iCurrentTemp = iDmdTs_GetLatestTemp();
  iCurrentTemp = iCurrentTemp / 20 + 250;		//Temperature in 0.1 degC units
//Read packet count from HVRAM
  ((BYTE *)&wPacketCount)[0] =bHvram_Read(bPacketCountAddr_c);
  ((BYTE *)&wPacketCount)[1] =bHvram_Read(bPacketCountAddr_c + 1);

 /*------------------------------------------------------------------------------
  *    TRANSMISSION LOOP PHASE
  *------------------------------------------------------------------------------
  */

// Application loop, including push button state analyzation and transmission. 
while(1)
{
        // Buttons analyzation 
        vButtonCheck();

        if (bButtonState)
        {
	        bRepeatCount = bRepeatCount_c;   		// Packet transmit repeat counter
            // Start to transmit. 
            vRepeatTxLoop();
            //Sync counter increment
	        vMtp_IncCount(0,6);		// increment Sync counter
	        EA=0;
	        bMtp_WrStatus=bMtp_Write(40);		// write MTP memory
	        EA=1;
        }
        else if( (lSys_GetMasterTime() >> 5) > bMaxWaitForPush_c )
        {
	        if ((PROT0_CTRL & M_NVM_BLOWN) > 1) //if part is burned to user or run mode.
  		{
                        //Disable all interrupts
                        EA = 0;
#ifdef CRYSTAL
                        // Disable XO
			bXO_CTRL = 0 ; 	
                        // Wait 20us for XO to stop
                        vSys_16BitDecLoop( 20 );
#endif
                        // Shutdown
                        vSys_Shutdown();      	
		}
        }
}
}


/*------------------------------------------------------------------------------
 *
 *    FUNCTION DESCRIPTION:
 *      This function contains the loop which consists of three procedures,
 *      tx setup, tx loop and tx shutdown in the application.
 *      During waiting for repeat transmission, check button state.
 *      Once any new push button is detected, then transmit the new packet 
 *      instead of the current packet.
 *
 *------------------------------------------------------------------------------ 
 */
void vRepeatTxLoop (void)

{ 
    unsigned char index = 0;

#if 0
        vFCast_Tune( fRkeDesiredFreq );
        //if (rOdsSetup.bModulationType == bModFSK_c)//add by jj
	{
	    vFCast_FskAdj( bRkeFskDev ); 
	}
        //  Now wait until there is a new temperature sample available
        while ( 0 == bDmdTs_GetSamplesTaken() )
        {
                //wait
        }
        //  Tune the PA with the current temperature as an argument 
        vPa_Tune( iDmdTs_GetLatestTemp());
        vStl_PreLoop();
#endif


#if 1
     while((P0&0xe)!= 0xe){
	    if (0 == index)
          fRkeDesiredFreq = f_433_RkeFreqFSK_b;
	    else if (1 == index)
          fRkeDesiredFreq = f_433_RkeFreqFSK_a;
	    else if (2 == index)
          fRkeDesiredFreq = f_433_RkeFreqFSK_c;

        if (index < 2) index++;
		else index = 0;

        vFCast_Tune( fRkeDesiredFreq );
        //if (rOdsSetup.bModulationType == bModFSK_c)//add by jj
	{
	    vFCast_FskAdj( bRkeFskDev ); 
	}
        //  Now wait until there is a new temperature sample available
        while ( 0 == bDmdTs_GetSamplesTaken() )
        {
                //wait
        }
        //  Tune the PA with the current temperature as an argument 
        vPa_Tune( iDmdTs_GetLatestTemp());
        vStl_PreLoop();

#endif
        do
        {
                // get current timestamp  
                lTimestamp = lSys_GetMasterTime();
	        if ((PROT0_CTRL & M_NVM_BLOWN) > 1) //if part is burned to user or run mode.
  	        {
    	                //turn LED on
	 	        GPIO_LED = 1; 
	        }	       
                while ( (lSys_GetMasterTime() - lTimestamp) < lLEDOnTime )
                {
                        //wait for LED ON time to expire
                }
	        GPIO_LED = 0;   //turn LED off
	        vRkePacketAssemble();

	        //Write packet count in frame
	        abFrame[bRke_FrameSize_c - 22] = ((BYTE *)&wPacketCount)[0];
	        abFrame[bRke_FrameSize_c - 21] = ((BYTE *)&wPacketCount)[1];
	        //update crc because of packet count change
	        vCalculateCrc();
	        //Convert whole frame before transmission
			rOdsSetup.bModulationType =bModFSK_c;//add by jj
	        vConvertPacket(rOdsSetup.bModulationType);
	        //Transmit packet
                vStl_SingleTxLoop(pbRke_FrameHead,bRke_FrameSize);
	        // Wait repeat interval. 
                while ( (lSys_GetMasterTime() - lTimestamp) < wRepeatInterval_c )
                {
                        //wait
                }
        }while(--bRepeatCount);
		bRepeatCount = 3;
	}
	    //Increment packet counter, used by PER measurement
	    wPacketCount++;
	    //Store packet count in HVRAM to keep it during shutdown
	    vHvram_Write(bPacketCountAddr_c,((BYTE *)&wPacketCount)[0]);
	    vHvram_Write(bPacketCountAddr_c + 1, ((BYTE *)&wPacketCount)[1]);

        vStl_PostLoop();

        // Clear time value for next button push detecting. 
        vSys_SetMasterTime(0);

        return;
} 

//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void isr_rtc (void) interrupt INTERRUPT_RTC using 1

{ 

  // Update the master time by 5 every time this isr is run.
  // clear the RTC_INT 
  RTC_CTRL &= ~M_RTC_INT;
  vSys_IncMasterTime(5); 
  bIsr_DebounceCount ++;
  if ((bIsr_DebounceCount % bDebounceInterval_c) == 0)
  {
    vBsr_Service();
  }
  return;
}
 
/*------------------------------------------------------------------------------
 *
 *    FUNCTION DESCRIPTION:
 *      Update bAp_ButtonState which indicates what to be transmitted.
 *      Check the elements on PTS (push tracking strcture) to see if any GPIO
 *      has been pressed or released.
 *      If any new pressed button has detected, the corresponding flag will be set and
 *      the associated information will be transmitted in
 *      application loop procedure.
 *
 *
 *------------------------------------------------------------------------------ 
 */

void vButtonCheck (void)
{ 
  // Disable RTC interrupt, or button state might be updated. 
  ERTC = 0;
  bButtonState = 0;     //comment this line out if autorepeat needed
  if (bBsr_GetPtsItemCnt())
     //Some buttons were pressed or released
  {
    
	bButtonState = wBsr_Pop() & 0xFF;
	if (bPrevBtn)
	{
		bPrevBtn = bButtonState;
		bButtonState = 0;
	}
	else
	{
		bPrevBtn = bButtonState;
	}
  }
  // Enable RTC interrupt 
  ERTC = 1;
  return;
}

/*
 *------------------------------------------------------------------------------ 
 */
void vRkePacketAssemble (void)
{ 
  BYTE i, j;
  long lSyncC;

  pbRke_FrameHead = abFrame;

#ifdef PACAPPIN
  if (PACAPPIN == 0)	
  {
	abFrame[0] = ((BYTE*)&wPA_CAP)[0] | 0xa0;
	abFrame[1] = ((BYTE*)&wPA_CAP)[1];
	bRke_FrameSize = 2;
	return;
  }
#endif

  bRke_FrameSize = bRke_FrameSize_c;

  //Sync counter read
  if (!bMtp_WrStatus)		//if last MTP memory write was successful
  {
  	pbMtp_Read();	//read MTP memory
  }
//get Sync counter value
  lSyncC = lMtp_GetDecCount(0,6);	

  for (i=0;i<bPreambleSize_c;i++)
  {
	abFrame[i] = bPreamble;
  }
        bStatus = 0;
//copy button bits from bButtonState to bStatus
        bStatus |= bButtonState & M_ButtonBits_c;

#ifdef AESUSED
  bStatus |= M_AesBit_c;	//set AES used bit
#endif
  if (bButtonState == bAssocButton_c)	//Association button pressed
  {
  	bStatus |= M_AssocBit_c;//set assoc bit
  	vCopyInputCode( abAssocKey, abAES2 );  //load assoc function key for AES
  }
  else
  {
  	vCopyInputCode( abBtnKey, abAES2 );	//load button function key for AES
  }

	abFrame[bRke_FrameSize_c - 29] = bSync1_c;
	abFrame[bRke_FrameSize_c - 28] = bSync2_c;
	abFrame[bRke_FrameSize_c - 27] = ((BYTE *)&lPartID)[0];
	abFrame[bRke_FrameSize_c - 26] = ((BYTE *)&lPartID)[1];
	abFrame[bRke_FrameSize_c - 25] = ((BYTE *)&lPartID)[2];
	abFrame[bRke_FrameSize_c - 24] = ((BYTE *)&lPartID)[3];
	abFrame[bRke_FrameSize_c - 23] = bStatus;
	abFrame[bRke_FrameSize_c - 22] = ((BYTE *)&wPacketCount)[0];
	abFrame[bRke_FrameSize_c - 21] = ((BYTE *)&wPacketCount)[1];

	abFrame[bRke_FrameSize_c - 20] = 0;				//CRC
	abFrame[bRke_FrameSize_c - 19] = 0;				//CRC

	abFrame[bRke_FrameSize_c - 18] = ((BYTE *)&iCurrentTemp)[0];//Temp field
	abFrame[bRke_FrameSize_c - 17] = ((BYTE *)&iCurrentTemp)[1];
	abFrame[bRke_FrameSize_c - 16] = bBatStatus;		//Battery field
	abFrame[bRke_FrameSize_c - 15] = ((BYTE *)&lSyncC)[0];
	abFrame[bRke_FrameSize_c - 14] = ((BYTE *)&lSyncC)[1];
	abFrame[bRke_FrameSize_c - 13] = ((BYTE *)&lSyncC)[2];
	abFrame[bRke_FrameSize_c - 12] = ((BYTE *)&lSyncC)[3];
	abFrame[bRke_FrameSize_c - 11] = bButtonState;
	abFrame[bRke_FrameSize_c - 10] = ((BYTE *)&wPA_CAP)[0];
	abFrame[bRke_FrameSize_c - 9] = ((BYTE *)&wPA_CAP)[1];
	abFrame[bRke_FrameSize_c - 8] = ((BYTE *)&lPartID)[0];
	abFrame[bRke_FrameSize_c - 7] = ((BYTE *)&lPartID)[1];
	abFrame[bRke_FrameSize_c - 6] = ((BYTE *)&lPartID)[2];
	abFrame[bRke_FrameSize_c - 5] = ((BYTE *)&lPartID)[3];
	abFrame[bRke_FrameSize_c - 4] = 0;
	abFrame[bRke_FrameSize_c - 3] = 0;

	abFrame[bRke_FrameSize_c - 2] = 0;	//CRC
	abFrame[bRke_FrameSize_c - 1] = 0;	//CRC


#ifdef AESUSED

  //----------------------------------------------------------------
  // AES encoding
  // Load data for session key generation
  abAES1[0] = ((BYTE *)&lPartID)[0];
  abAES1[1] = ((BYTE *)&lPartID)[1];
  abAES1[2] = ((BYTE *)&lPartID)[2];
  abAES1[3] = ((BYTE *)&lPartID)[3];
  for (i=4;i<16;i++)
  {
 	abAES1[i] = 0;
  }
  // Generate session key (into abAES1)
  vAes_Cipher( abAES1, abAES2 );
  // Copy data to encode in abAES2
  for (i=0,j=(bRke_FrameSize_c - 18) ;i<16;i++,j++)
  {
    abAES2[i] = abFrame[j];
  }
  // Encode data using session key in abAES1
  vAes_Cipher( abAES2, abAES1 );
  // Copy encoded data back into packet
  for (i=0,j=(bRke_FrameSize_c - 18) ;i<16;i++,j++)
  {
	abFrame[j] = abAES2[i];
  }
#endif
  return;
}
//--------------------------------------------------------------
//CRC calculation
void vCalculateCrc(void)
{
  BYTE i,j;
  WORD wCrc;
  wCrc = 0xffff;
  for(j=bPayloadStartIndex_c;j<(bRke_FrameSize_c - 20);j++)
  {
    wCrc = wCrc ^ ((WORD)abFrame[j]<<8);

    for (i = 8; i != 0; i--)
    {
      if (wCrc & 0x8000)
      {
	    wCrc = (wCrc << 1) ^ 0x8005;
      }
      //Bit pattern used (1)1000 0000 0000 0101, X16+X15+X2+1
      else
	  {
        wCrc <<= 1;
      }
    }
  }
  //-----------------------------------------------------------------
  //Write CRC in frame
  abFrame[bRke_FrameSize_c - 20] = ((BYTE*)&wCrc)[0];
  abFrame[bRke_FrameSize_c - 19] = ((BYTE*)&wCrc)[1];

  //--------------------------------------------------------------
  wCrc = 0xffff;
  for(j=bPayloadStartIndex_c;j < bPayloadStartIndex_c + bPayloadSize_c;j++)
  {
    wCrc = wCrc ^ ((WORD)abFrame[j]<<8);

    for (i = 8; i != 0; i--)
    {
      if (wCrc & 0x8000)
      {
	    wCrc = (wCrc << 1) ^ 0x8005;
      }
      //Bit pattern used (1)1000 0000 0000 0101, X16+X15+X2+1
      else
	  {
        wCrc <<= 1;
      }
    }
  }
  //-----------------------------------------------------------------
  //Write CRC in frame
  abFrame[bRke_FrameSize_c - 2] = ((BYTE*)&wCrc)[0];
  abFrame[bRke_FrameSize_c - 1] = ((BYTE*)&wCrc)[1];
  return;
}

  //-------------------------------------------------------------------
  //MSB first to LSB first conversion, and inversion if FSK used
void vConvertPacket (BYTE bModType)
{
  BYTE i,low,high;

  if (bModType)
  {
    bModType = 0xff;
  }
  for (i=0;i<(sizeof abFrame);i++)
  {
	low = abConvTable[(abFrame[i] & 0xf0) >> 4] & 0x0f;
	high = abConvTable[abFrame[i] & 0x0f] & 0xf0;
	abFrame[i] = (high | low) ^ bModType;
  }
  return;
}



//==================================================================
//     vCopyInputCode
//===================================================================
static void vCopyInputCode (
        BYTE const code *pbiSource,
        BYTE idata *pboData
        )
{
  BYTE          bInd;           // Loop index 
  // -- Copy data 
  for ( bInd = 16; bInd > 0; bInd-- )
  {
    *pboData = *pbiSource;
    pbiSource++;
    pboData++;
  }
  return;
}

//===============================================================================
//     vCustomFirstPowerUp
//===================================================================
void    vCustomFirstPowerUp   /* Function to be run if power_1st_time is 1 */
        (
        	BYTE bWaitTime
        )
/*------------------------------------------------------------------------------
 *
 *    FUNCTION DESCRIPTION:
 *      This function is to be run if the power_1st_time bit is 1 after boot.
 *------------------------------------------------------------------------------
 */
{
/*
 *------------------------------------------------------------------------------
 *
 *    VARIABLES:
 */
  BYTE xdata    bIndex;
/*
 *------------------------------------------------------------------------------
 */


//Initialize the UCHTIM to 0
  vSleepTim_SetCount( 0L );
//Waits for the bWaitTime
  	bIndex = 0;
//Each bit of bWaitTime is 40 ms
  	while ( bIndex < bWaitTime )
  	{
    	        vSys_16BitDecLoop( wSys_16Bit_40ms_c );
    	        bIndex++;
  	}

//Clear the HVRAM
  	while ( bIndex < 8 )
  	{
    	        vHvram_Write( bIndex, 0 );
    	        bIndex++;
	}
//Set the modulation mode 
	if (SetToOokGPIO==0)
	{
	        vHvram_Write( bModAddress_c, bModOOK_c );
	}
	else
	{
	        vHvram_Write( bModAddress_c, bModFSK_c );
	}

//Shutdown call to vSys_Shutdown()
	if ((PROT0_CTRL & M_NVM_BLOWN) > 1) //if part is burned to user or run mode.
  	{
  	        vSys_Shutdown();
	}
}

/*
 *------------------------------------------------------------------------------
 */
