#include "dataflash.h"
#include "pfdl.h"
#include "Timer_Wdg.h"
#ifdef UART_DEBUG
#include "UartPrin.h"
#endif


/*------------------------------------------------------------------------*/
/*   FUNC: write data into flash_rom                                      */
/*   PAR:                                                                 */
/*      pbuf:     point to the data which shoud be save                   */
/*      index:    adress of dataflash_rom                                 */
/*      count:    length of data                                          */
/*   RETURN:                                                              */
/*      "0" is success   "1" is fail                                      */
/*------------------------------------------------------------------------*/
U08ET write_data_to_dataflash(U08ET *ram_buf, U16ET flash_adress, U16ET count)
{
   
   pfdl_descriptor_t    descriptor_pstr;
   pfdl_status_t	current_status;
   pfdl_request_t       current_pfdl_request_t;
   U16ET i;
/********open_status**********/     
   descriptor_pstr.fx_MHz_u08 = 24; 
   descriptor_pstr.wide_voltage_mode_u08 = 0x0;   
   DI();
   PFDL_Open(&descriptor_pstr);
/*************check block*********/
   current_pfdl_request_t.index_u16     = flash_adress;
   current_pfdl_request_t.bytecount_u16 = count;
   current_pfdl_request_t.command_enu   = PFDL_CMD_BLANKCHECK_BYTES;
   PFDL_Execute(&current_pfdl_request_t);
   current_status = PFDL_Handler();
   while(PFDL_BUSY ==  current_status){
   	current_status = PFDL_Handler();
   }
   wdtdog_feed();
   #ifdef UART_DEBUG
   PutString("CHECK:");
   puthex(current_status);
   PutString("\r\n");
   #endif
/*************erase_data**********/
   if (PFDL_ERR_MARGIN == current_status){
	   current_pfdl_request_t.index_u16     = (flash_adress < 0x400) ? 0:1;
	   current_pfdl_request_t.bytecount_u16 = count;
	   current_pfdl_request_t.command_enu   = PFDL_CMD_ERASE_BLOCK;
	   PFDL_Execute(&current_pfdl_request_t);   
	   current_status = PFDL_Handler();
           while(PFDL_BUSY ==  current_status){
   	         current_status = PFDL_Handler();
	   }
	   #ifdef UART_DEBUG
   	   PutString("ERASE:");
	   puthex(current_status);
   	   PutString("\r\n");
	   #endif
   }
   
/*************if preprocess success *******/   
   if(PFDL_OK != current_status){
	#ifdef UART_DEBUG
	PutString("flag1 \r\n");
	#endif
	wdtdog_feed();   
	PFDL_Close();
	EI();
   	return 1;
   }
   
   //for(i = 0; i<2000;i++);
/*************wrote data to flash **********/   
   current_pfdl_request_t.index_u16     = flash_adress;
   current_pfdl_request_t.bytecount_u16 = count;
   current_pfdl_request_t.data_pu08     = ram_buf;
   current_pfdl_request_t.command_enu   = PFDL_CMD_WRITE_BYTES;
   PFDL_Execute(&current_pfdl_request_t);
   current_status = PFDL_Handler();
   while(PFDL_BUSY ==  current_status){
         current_status = PFDL_Handler();
   }
   #ifdef UART_DEBUG
   PutString("WRITE:");
   puthex(current_status);
   PutString("\r\n");
   #endif
   
   
   if(PFDL_OK != current_status){
	#ifdef UART_DEBUG
	PutString("flag2 \r\n");
	#endif
	wdtdog_feed();   
	PFDL_Close();
	EI();
   	return 1;
   }
   
   current_pfdl_request_t.index_u16     = flash_adress;
   current_pfdl_request_t.bytecount_u16 = count;
   current_pfdl_request_t.data_pu08     = ram_buf;
   current_pfdl_request_t.command_enu   = PFDL_CMD_IVERIFY_BYTES;
   PFDL_Execute(&current_pfdl_request_t);   
   current_status = PFDL_Handler();
   while(PFDL_BUSY ==  current_status){
         current_status = PFDL_Handler();
   }
   #ifdef UART_DEBUG
   PutString("IVERIFY:");
   puthex(current_status);
   PutString("\r\n");
   #endif
   if(PFDL_OK != current_status){
	#ifdef UART_DEBUG
	PutString("flag3 \r\n");
	#endif
	wdtdog_feed();   
	PFDL_Close(); 
	EI();
   	return 1;
   }   

 /*  current_pfdl_request_t.index_u16     = index;   
   current_pfdl_request_t.command_enu   = PFDL_CMD_WRITE_BYTES;
   PFDL_Execute(&current_pfdl_request_t);
   while(PFDL_Handler());*/
   PFDL_Close();
   wdtdog_feed();
   EI();
   return 0;
}

/*------------------------------------------------------------------------*/
/*   FUNC: read data from flash_rom                                       */
/*   PAR:                                                                 */
/*      pbuf:     point to the store area                                 */
/*      index:    adress of dataflash_rom                                 */
/*      count:    length of data                                          */
/*   RETURN:                                                              */
/*      "0" is success   "1" is fail                                      */
/*------------------------------------------------------------------------*/
U08ET read_data_from_dataflash(U08ET* ram_buf, U16ET  flash_adress, U16ET  count)
{

   pfdl_descriptor_t    current_descriptor_t;
   pfdl_status_t	current_status;
   pfdl_request_t       current_pfdl_request_t;
   
   DI();
/********open_status**********/     
   current_descriptor_t.fx_MHz_u08 = 0x18;
   current_descriptor_t.wide_voltage_mode_u08 = 0x00;
   PFDL_Open(&current_descriptor_t); 


/*************read_data*********/
   current_pfdl_request_t.index_u16     = flash_adress;
   current_pfdl_request_t.data_pu08     = ram_buf;
   current_pfdl_request_t.bytecount_u16 = count;
   current_pfdl_request_t.command_enu   = PFDL_CMD_READ_BYTES;
   current_status = PFDL_Execute(&current_pfdl_request_t);
   while(PFDL_BUSY ==  current_status){
	      wdtdog_feed();
   	current_status = PFDL_Handler();
   } 
   wdtdog_feed();
   #ifdef UART_DEBUG
   if(PFDL_OK == current_status)
      	PutString("read success\r\n");
   else
        PutString("read fail\r\n");
   #endif
   wdtdog_feed();
   PFDL_Close();     
   EI();
   return 0;
}




