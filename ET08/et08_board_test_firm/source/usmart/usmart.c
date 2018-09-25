/**
  ******************************************************************************
  * @file    usmart.c
  * @author  Kimi
  * @version V0.0.1
  * @date    2018/3/24 15:45:06
  * @brief    This file provides usmart functions to communicate with 
  *           other equipments.           
  *           + Interface with test board, pc or server. 
  *           + It's now Tx/Rx data by uart, you can redirect it by usb, ethernet, 
  *             or anyother interface.
  *           
  ******************************************************************************
  * @attention
  *           If func name is same as command name, the current command will be
  *           covered.
  *           + rec func should be performed in interface modlue, uart. spi or
  *             any other interface.
  *
  * 
  ******************************************************************************
  */

#include "usmart.h"
#include "uartct.h"
#include "stdio.h"

u8 *sys_cmd_tab[] = {
    "?",
    "help",
    "list",
    "id",
    "hex",
    "dec",
};





void usmart_init(void){
	uartct_init();
	usmart_dev.sptype = SP_TYPE_DEC;
}


/**
  * @brief  usmart system command.
  * @param  
  *   str : read from uart
  * @retval 
  *   0 : success
  *
  */
u8 usmart_sys_cmd_exe(u8 *str)
{
  u8 i;
  u8 sfname[MAX_FNAME_LEN]; //store function name
  u8 pnum;
  u8 rval;
  u32 res;
  res = usmart_get_cmdname(str, sfname, &i, MAX_FNAME_LEN); //get command and length
  if (res)
    return USMART_FUNCERR; //error command
  str += i;
  for (i = 0; i < sizeof(sys_cmd_tab) / 4; i++) //valid system command
  {
    if (usmart_strcmp(sfname, sys_cmd_tab[i]) == 0)
      break;
  }
  switch (i)
  {
  case 0:
  case 1: //help command
    printf("\r\n");
#if USMART_USE_HELP
    printf("------------------------USMART V0.0.1----------------------\r\n");
    printf("USMART is a interface of ET_equipments, you can call the\r\n");
    printf("interior fun by it, all of the parameters required to\r\n");
    printf("change as UTF-8 string,USMART have 6 commands hereinafter: \r\n");
    printf("\r\n");
    printf("?:      get help information\r\n");
    printf("help:   get help information\r\n");
    printf("list:   get function list\r\n\n");
    printf("id:     ID of all function\r\n\n");
    printf("hex:    recognize the paramter as hex\r\n\n");
    printf("dec:    recognize the paramter as dec\r\n\n");
    printf("please study the help information\r\n");
    printf("--------------------------ETdoor------------------------- \r\n");
#else
    printf("command fail\r\n");
#endif
    break;
  case 2: //search the command
    printf("\r\n");
    printf("-------------------------function list---------------------- \r\n");
    for (i = 0; i < usmart_dev.fnum; i++)
      printf("%s\r\n", usmart_dev.funs[i].name);
    printf("\r\n");
    break;
  case 3: //search the ID
    printf("\r\n");
    printf("-------------------------func ID --------------------------- \r\n");
    for (i = 0; i < usmart_dev.fnum; i++)
    {
      usmart_get_fname((u8 *)usmart_dev.funs[i].name, sfname, &pnum, &rval); //get local function name
      printf("%s id is:\r\n0X%08X\r\n", sfname, usmart_dev.funs[i].func);    //display ID
    }
    printf("\r\n");
    break;
  case 4: //hex command
    printf("\r\n");
    usmart_get_aparm(str, sfname, &i);
    if (i == 0) //dec
    {
      i = usmart_str2num(sfname, &res); //note the parameter
      if (i == 0)                       //convert enable
      {
        printf("HEX:0X%X\r\n", res); //convert to hex
      }
      else if (i != 4)
        return USMART_PARMERR; //parameter error
      else                     //display parameter
      {
        printf("hex display!\r\n");
        usmart_dev.sptype = SP_TYPE_HEX;
      }
    }
    else
      return USMART_PARMERR; //parameter error
    printf("\r\n");
    break;
  case 5: //dec
    printf("\r\n");
    usmart_get_aparm(str, sfname, &i);
    if (i == 0) //normal parameter
    {
      i = usmart_str2num(sfname, &res); //note the parameter
      if (i == 0)                       //convert disable
      {
        printf("DEC:%lu\r\n", res); //convert to dec
      }
      else if (i != 4)
        return USMART_PARMERR; //parameter error
      else                     //display parameter
      {
        printf("dec display!\r\n");
        usmart_dev.sptype = SP_TYPE_DEC;
      }
    }
    else
      return USMART_PARMERR; //parameter error
    printf("\r\n");
    break;
  default: //illegal command
    return USMART_FUNCERR;
  }
  return 0;
}

/**
  * @brief  sys task.
  * @param  none
  * @retval none
  *
  */
void usmart_task(void)
{
  static cnt = 0;
  cnt = (cnt < 10) ? (cnt + 1) : 0;
  if (cnt == 5)
    usmart_dev.scan(); //usmart scan
}

/**
  * @brief  get function name from str.
  * @param  
  *   *str : piont to string.
  * @retval 
  *   0 : success
  *   other : error
  *
  */
u8 usmart_cmd_rec(u8 *str)
{
  u8 sta, i, rval; //status.
  u8 rpnum, spnum;
  u8 rfname[MAX_FNAME_LEN];                           //buf, store func name <rec>.
  u8 sfname[MAX_FNAME_LEN];                           //buf, store func name <local>.
  sta = usmart_get_fname(str, rfname, &rpnum, &rval); //analysis func name, count of parameter <rec>.
  if (sta)
    return sta; //analysis error <rec>.
  for (i = 0; i < usmart_dev.fnum; i++)
  {
    sta = usmart_get_fname((u8 *)usmart_dev.funs[i].name, sfname, &spnum, &rval); //get func name, count of parameter <local>.
    if (sta)
      return sta;                           //analysis error <local>.
    if (usmart_strcmp(sfname, rfname) == 0) //func name : rec == local.
    {
      if (spnum > rpnum)
        return USMART_PARMERR; //parameter count error (rec less than local).
      usmart_dev.id = i;       //mark func ID.
      break;                   //match success.
    }
  }
  if (i == usmart_dev.fnum)
    return USMART_NOFUNCFIND;       //can't match local func.
  sta = usmart_get_fparam(str, &i); //get parameter count of rec func.
  if (sta)
    return sta;        //return error flag.
  usmart_dev.pnum = i; //mark count of parameter.
  return USMART_OK;
}

/**
  * @brief  execute rec func.
  * @param  none
  * @retval none
  *
  */
void usmart_exe(void)
{
  u8 id, i;
  u32 res;
  u32 temp[MAX_PARM];       //parameter convert
  u8 sfname[MAX_FNAME_LEN]; //store local func name
  u8 pnum, rval;

  id = usmart_dev.id;
  if (id >= usmart_dev.fnum)
    return;                                                               //error.
  usmart_get_fname((u8 *)usmart_dev.funs[id].name, sfname, &pnum, &rval); //get local func name, count of parameter.
  printf("%s(", sfname);                                              //printf func name.
  for (i = 0; i < pnum; i++)                                              //printf parameter.
  {
    if (usmart_dev.parmtype & (1 << i)) //parameter is string
    {
      printf("\"");
      printf("%s", usmart_dev.parm + usmart_get_parmpos(i));
      printf("\"");
      temp[i] = (u32) & (usmart_dev.parm[usmart_get_parmpos(i)]);
    }
    else //parameter is digit
    {
      temp[i] = *(u32 *)(usmart_dev.parm + usmart_get_parmpos(i));
      if (usmart_dev.sptype == SP_TYPE_DEC)
        printf("%lu", temp[i]); //dec printf
      else
        printf("0X%X", temp[i]); //hex printf
    }
    if (i != pnum - 1)
      printf(",");
  }
  printf(")");
  //  usmart_reset_runtime(); //clear timer count
  switch (usmart_dev.pnum)
  {
  case 0: //parameter : void
    res = (*(u32(*)(void))usmart_dev.funs[id].func)();
    break;
  case 1: //parameter : 1
    res = (*(u32(*)(u32))usmart_dev.funs[id].func)(temp[0]);
    break;
  case 2: //parameter : 2
    res = (*(u32(*)(u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1]);
    break;
  case 3: //parameter : 3
    res = (*(u32(*)(u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2]);
    break;
  case 4: //parameter : 4
    res = (*(u32(*)(u32, u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3]);
    break;
  case 5: //parameter : 5
    res = (*(u32(*)(u32, u32, u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4]);
    break;
  case 6: //parameter : 6
    res = (*(u32(*)(u32, u32, u32, u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4],
                                                temp[5]);
    break;
  case 7: //parameter : 7
    res = (*(u32(*)(u32, u32, u32, u32, u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4],
                                                temp[5], temp[6]);
    break;
  case 8: //parameter : 8
    res = (*(u32(*)(u32, u32, u32, u32, u32, u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4],
                                                temp[5], temp[6], temp[7]);
    break;
  case 9: //parameter : 9
    res = (*(u32(*)(u32, u32, u32, u32, u32, u32, u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4],
                                                temp[5], temp[6], temp[7], temp[8]);
    break;
  case 10: //parameter : 10
    res = (*(u32(*)(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32))usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4],
                                                temp[5], temp[6], temp[7], temp[8], temp[9]);
    break;
  }
  //  usmart_get_runtime(); //get func execute time
  if (rval == 1) //return value.
  {
    if (usmart_dev.sptype == SP_TYPE_DEC)
      printf("=%lu;\r\n", res); //output result dec
    else
      printf("=0X%X;\r\n", res); //output result hex
  }
  else
    printf(";\r\n"); //void
  // if (usmart_dev.runtimeflag) //display execute time
  // {
  //   printf("Function Run Time:%d.%1dms\r\n", usmart_dev.runtime / 10, usmart_dev.runtime % 10); //printf execute time
  // }
}

/**
  * @brief  usmart sys scan, this func should be called per 100ms,
  *         to execute the rec func.
  * @param  none
  * @retval none
  *
  */
void usmart_scan(void)
{
  u8 sta, len;
  if (USART_RX_STA & 0x8000) //if rec over
  {
    len = USART_RX_STA & 0x3fff;            //get the length of buf.
    USART_RX_BUF[len] = '\0';               //add '\0' back.
    sta = usmart_dev.cmd_rec(USART_RX_BUF); //get information from buf
    if (sta == 0)
      usmart_dev.exe(); //execute command
    #if 0
    else
    {
      len = usmart_sys_cmd_exe(USART_RX_BUF);
      if (len != USMART_FUNCERR)
        sta = len;
      if (sta)
      {
        switch (sta)
        {
        case USMART_FUNCERR:
          printf("func error!\r\n");
          break;
        case USMART_PARMERR:
          printf("parameter error!\r\n");
          break;
        case USMART_PARMOVER:
          printf("parameter too many!\r\n");
          break;
        case USMART_NOFUNCFIND:
          printf("can't match func!\r\n");
          break;
        }
      }
    }
    #endif
    USART_RX_STA = 0; //reset rec buf & status
  }
}

#if USMART_USE_WRFUNS == 1 //if enable r/w func
/**
  * @brief  read value of addr.
  * @param  
  *   addr : direction.
  * @retval 
  *   current value.
  *
  */
u32 read_addr(u32 addr)
{
  return *(u32 *)addr;
}
/**
  * @brief  write value to addr.
  * @param  
  *   addr : direction.
  *   val  : value
  * @retval none
  *
  */
void write_addr(u32 addr, u32 val)
{
  *(u32 *)addr = val;
}
#endif
