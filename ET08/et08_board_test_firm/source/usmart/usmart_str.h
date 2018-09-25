/**
  ******************************************************************************
  * @file    usmart_str.h
  * @author  Kimi
  * @version V0.0.1
  * @date    2018/3/24 15:45:06
  * @brief    This file declare for usmart_str          
  *           + process the string 
  *           + get information from rec buf
  *           + service for usmart
  *           + It's somthing about string conver & process
  *           
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

#ifndef __USMART_STR_H
#define __USMART_STR_H
#include "typedef.h"

u8 usmart_get_parmpos(u8 num);                                    //get the addr of the parameter.
u8 usmart_strcmp(u8 *str1, u8 *str2);                             //compare 2 strings.
u32 usmart_pow(u8 m, u8 n);                                       //M^N.
u8 usmart_str2num(u8 *str, u32 *res);                             //string2digit.
u8 usmart_get_cmdname(u8 *str, u8 *cmdname, u8 *nlen, u8 maxlen); //get command name and return the length.
u8 usmart_get_fname(u8 *str, u8 *fname, u8 *pnum, u8 *rval);      //get func name from string.
u8 usmart_get_aparm(u8 *str, u8 *fparm, u8 *ptype);               //get a parameter from string.
u8 usmart_get_fparam(u8 *str, u8 *parn);                          //get all parameter from string.
#endif
