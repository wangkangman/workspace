/**
  ******************************************************************************
  * @file    usmart.h
  * @author  Kimi
  * @version V0.0.1
  * @date    2018/3/24 15:45:06
  * @brief    This file declare for usmart          
  *           + Interface with test board
  *           + Interface with pc
  *           + Interface with server 
  *           + It's now Tx/Rx data by uart, you can redirect it by usb, ethernet, 
  *           + or anyother interface
  *           
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

#ifndef __USMART_H
#define __USMART_H
#include "usmart_str.h"
#include "typedef.h"
#define MAX_FNAME_LEN 30 //max length of function name.
#define MAX_PARM 10      //maxcount of parameter, combined with usmart_exe().
#define PARM_LEN 200     //maxcount of rec buf, stisfy with uart buf.

#define USMART_USE_HELP 1   //help information, open default.
#define USMART_USE_WRFUNS 1 //enable read&write function.
///////////////////////////////////////////////END///////////////////////////////////////////////////////////

#define USMART_OK 0         //success
#define USMART_FUNCERR 1    //func error
#define USMART_PARMERR 2    //parameter error
#define USMART_PARMOVER 3   //maxcount of parameter stack overflow
#define USMART_NOFUNCFIND 4 //can't match func

#define SP_TYPE_DEC 0 //dec printf
#define SP_TYPE_HEX 1 //hex

/**
  * @brief  execute rec func.
  * @param  
  *   func : func pionter.
  *   name : func name <string>.
  *
  */
struct _m_usmart_nametab
{
  void *func;
  const u8 *name;
};

/**
  * @brief  usmart control st.
  * @param  
  *
  */
struct _m_usmart_dev
{
  struct _m_usmart_nametab *funs; //fucn list

  void (*init)(void);       //init
  u8 (*cmd_rec)(u8 *str); //recognize func
  void (*exe)(void);      //execute
  void (*scan)(void);     //scan
  void (*task)(void);
  u8 fnum;                //count of functions
  u8 pnum;                //count of parameter
  u8 id;                  //function id
  u8 sptype;              //parameter display : 0-dec, 1-hex;
  u16 parmtype;           //parameter type
  u8 plentbl[MAX_PARM];   //length of every parameter
  u8 parm[PARM_LEN];      //parameter of function
};
extern struct _m_usmart_nametab usmart_nametab[]; //definition in usmart_config.c
extern struct _m_usmart_dev usmart_dev;           //definition in usmart_config.c
void usmart_init(void);                           //init
u8 usmart_cmd_rec(u8 *str);                       //recognize
void usmart_exe(void);                            //execute
void usmart_scan(void);                           //scan
void usmart_task(void);                           //task
u32 read_addr(u32 addr);                          //read value form addr
void write_addr(u32 addr, u32 val);               //write value to  addr

#endif
