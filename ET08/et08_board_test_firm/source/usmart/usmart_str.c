/**
  ******************************************************************************
  * @file    usmart_str.c
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
  *           For the string process, the stack must more than 256byte.
  * 
  ******************************************************************************
  */

#include "usmart_str.h"
#include "usmart.h"

/**
  * @brief  compare two strings.
  * @param  
  *   str1 : string_1.
  *   str2 : string_2.
  * @retval 
  *   0 : equal.
  *   1 : unequal.
  *
  */
u8 usmart_strcmp(u8 *str1, u8 *str2)
{
  while (1)
  {
    if (*str1 != *str2)
      return 1; //unequal
    if (*str1 == '\0')
      break; //equal.
    str1++;
    str2++;
  }
  return 0; //equal
}

/**
  * @brief  copy string_1 to string_2.
  * @param  
  *   str1 : string_1.
  *   str2 : string_2.
  * @retval none
  *
  */
void usmart_strcopy(u8 *str1, u8 *str2)
{
  while (1)
  {
    *str2 = *str1; //copy
    if (*str1 == '\0')
      break; //finish.
    str1++;
    str2++;
  }
}

/**
  * @brief  get length of string.
  * @param  
  *   str : string.
  * @retval 
  *   length of string.
  *
  */
u8 usmart_strlen(u8 *str)
{
  u8 len = 0;
  while (1)
  {
    if (*str == '\0')
      break;
    len++;
    str++;
  }
  return len;
}

/**
  * @brief  power calculate.
  * @param  
  *   m : base value.
  *   n : exponent.
  * @retval 
  *   m^n.
  *
  */
u32 usmart_pow(u8 m, u8 n)
{
  u32 result = 1;
  while (n--)
    result *= m;
  return result;
}

/**
  * @brief  conver string to digit value.
  *         if value is hex, X must be capital letter.
  * @param  
  *   str : string.
  *   res : addr to store value.
  * @retval 
  *   0 : convert success.
  *   1 : format error.
  *   2 : hex format 0 error.
  *   3 : start format error.
  *   4 : dec format 0 error.
  *
  */
u8 usmart_str2num(u8 *str, u32 *res)
{
  u32 t;
  u8 bnum = 0; //length of value.
  u8 *p;
  u8 hexdec = 10; //default dec.
  p = str;
  *res = 0; //clean.
  while (1)
  {
    if ((*p <= '9' && *p >= '0') || (*p <= 'F' && *p >= 'A') || (*p == 'X' && bnum == 1)) //legal.
    {
      if (*p >= 'A')
        hexdec = 16; //change to hex format.
      bnum++;        //length.
    }
    else if (*p == '\0')
      break; //hit '\0' exit.
    else
      return 1; //not full dec or hex.
    p++;
  }
  p = str;          //remap to begining of string.
  if (hexdec == 16) //change to hex format.
  {
    if (bnum < 3)
      return 2;                         //length less than 3, exit.
    if (*p == '0' && (*(p + 1) == 'X')) //must start with '0X'.
    {
      p += 2;    //remap to begining of digit information.
      bnum -= 2; //sub offset.
    }
    else
      return 3; //format error.
  }
  else if (bnum == 0)
    return 4; //length == 0, exit.
  while (1)
  {
    if (bnum)
      bnum--;
    if (*p <= '9' && *p >= '0')
      t = *p - '0'; //char 2 digit <0 ~ 9>.
    else
      t = *p - 'A' + 10; //char 2 digit <A ~ F>.
    *res += t * usmart_pow(hexdec, bnum);
    p++;
    if (*p == '\0')
      break; //finish.
  }
  return 0; //convert success.
}

/**
  * @brief  get command name.
  * @param  
  *   str     : string.
  *   cmdname : command name.
  *   maxlen  : limit length.
  * @retval 
  *   0 : success.
  *   1 : fail.
  *
  */
u8 usmart_get_cmdname(u8 *str, u8 *cmdname, u8 *nlen, u8 maxlen)
{
  *nlen = 0;
  while (*str != ' ' && *str != '\0') //hit '\0' or ' ', end.
  {
    *cmdname = *str;
    str++;
    cmdname++;
    (*nlen)++; //calculate the length of the command.
    if (*nlen >= maxlen)
      return 1; //wrong instruction.
  }
  *cmdname = '\0'; //add '\0'.
  return 0;        //success
}

/**
  * @brief  remap to next character.
  * @param  
  *   str     : string.
  * @retval 
  *   next character.
  *
  */
u8 usmart_search_nextc(u8 *str)
{
  str++;
  while (*str == ' ' && str != '\0')
    str++;
  return *str;
}

/**
  * @brief  get func name from string.
  * @param  
  *  *str     : string.
  *  *fname   : func name.
  *  *pnum    : count of parameter.
  *  *rval    : judge if need return value.
  * @retval 
  *   0 : success.
  *   other : error.
  *
  */
u8 usmart_get_fname(u8 *str, u8 *fname, u8 *pnum, u8 *rval)
{
  u8 res;
  u8 fover = 0; //deepth of ()
  u8 *strtemp;
  u8 offset = 0;
  u8 parmnum = 0;
  u8 temp = 1;
  u8 fpname[6];  //void+X+'/0'
  u8 fplcnt = 0; //1st cnt for length of parameter.
  u8 pcnt = 0;   //cnt of parameter
  u8 nchar;
  //judge if need return value
  strtemp = str;
  while (*strtemp != '\0')
  {
    if (*strtemp != ' ' && (pcnt & 0X7F) < 5) //maxcount == 5
    {
      if (pcnt == 0)
        pcnt |= 0X80;
      if (((pcnt & 0x7f) == 4) && (*strtemp != '*'))
        break;
      fpname[pcnt & 0x7f] = *strtemp;
      pcnt++;
    }
    else if (pcnt == 0X85)
      break;
    strtemp++;
  }
  if (pcnt)
  {
    fpname[pcnt & 0x7f] = '\0';
    if (usmart_strcmp(fpname, "void") == 0)
      *rval = 0;
    else
      *rval = 1;
    pcnt = 0;
  }

  //remap to the begining of func name.
  res = 0;
  strtemp = str;
  while (*strtemp != '(' && *strtemp != '\0')
  {
    strtemp++;
    res++;
    if (*strtemp == ' ' || *strtemp == '*')
    {
      nchar = usmart_search_nextc(strtemp); //get next char
      if (nchar != '(' && nchar != '*')
        offset = res; //ignore ' '  and '*'
    }
  }
  strtemp = str;
  if (offset)
    strtemp += offset + 1; //remap to the begining of func name.
  res = 0;
  nchar = 0; //0 : outside string. 1: inside string.
  while (1)
  {
    if (*strtemp == 0)
    {
      res = USMART_FUNCERR; //func error.
      break;
    }
    else if (*strtemp == '(' && nchar == 0)
      fover++; //() deepth++
    else if (*strtemp == ')' && nchar == 0)
    {
      if (fover)
        fover--;
      else
        res = USMART_FUNCERR; //error ,without '('
      if (fover == 0)
        break; //end.
    }
    else if (*strtemp == '"')
      nchar = !nchar;

    if (fover == 0) //in name frame.
    {
      if (*strtemp != ' ')
      {
        *fname = *strtemp; //get func name.
        fname++;
      }
    }
    else //func name over.
    {
      if (*strtemp == ',')
      {
        temp = 1; //add 1 parameter.
        pcnt++;
      }
      else if (*strtemp != ' ' && *strtemp != '(')
      {
        if (pcnt == 0 && fplcnt < 5) //avoid type "void".
        {
          fpname[fplcnt] = *strtemp;
          fplcnt++;
        }
        temp++;
      }
      if (fover == 1 && temp == 2)
      {
        temp++;    //avoid repetition
        parmnum++; //add 1
      }
    }
    strtemp++;
  }
  if (parmnum == 1) //amount 1.
  {
    fpname[fplcnt] = '\0'; //add '\0'
    if (usmart_strcmp(fpname, "void") == 0)
      parmnum = 0; //'void' none.
  }
  *pnum = parmnum; //amount of parmeter.
  *fname = '\0';   //add '\0'
  return res;      //return result
}

/**
  * @brief  get a parameter from string.
  *         + It's not the whole rec buf.
  *         + string[0] next to '(' .
  * @param  
  *  *str     : string.
  *  *fname   : parameter.
  *  *ptype   : the type of parameter.
  * @retval 
  *   0 : no more parameter.
  *   other : offset of next parameter.
  *
  */
u8 usmart_get_aparm(u8 *str, u8 *fparm, u8 *ptype)
{
  u8 i = 0;
  u8 enout = 0;
  u8 type = 0;   //default digit.
  u8 string = 0; //flag inside string
  while (1)
  {
    if (*str == ',' && string == 0)
      enout = 1; //delay to exit, should find next parameter.
    if ((*str == ')' || *str == '\0') && string == 0)
      break;       //exit
    if (type == 0) //digit
    {
      if ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F') || *str == 'X' || *str == 'x')
      {
        if (enout)
          break; //find next parameter, exit.
        if (*str >= 'a')
          *fparm = *str - 0X20; //small conver to capital.
        else
          *fparm = *str;
        fparm++;
      }
      else if (*str == '"') //find string flag.
      {
        if (enout)
          break;
        type = 1;
        string = 1; //rigister & reading.
      }
      else if (*str != ' ' && *str != ',') //illegal char, error.
      {
        type = 0XFF;
        break;
      }
    }
    else //string format
    {
      if (*str == '"')
        string = 0;
      if (enout)
        break;    //find next parameter, exit.
      if (string) //reading.
      {
        if (*str == '\\') //filter '\\'
        {
          str++;
          i++;
        }
        *fparm = *str;
        fparm++;
      }
    }
    i++; //offset ++
    str++;
  }
  *fparm = '\0'; //add '\0'
  *ptype = type; //parameter type.
  return i;      //length of parameter.
}

/**
  * @brief  get addr of parameter.
  * @param  
  *  num     : position of the parameter.
  * @retval 
  *  addr of the parameter.
  *
  */
u8 usmart_get_parmpos(u8 num)
{
  u8 temp = 0;
  u8 i;
  for (i = 0; i < num; i++)
    temp += usmart_dev.plentbl[i];
  return temp;
}

/**
  * @brief  get all parameter from string.
  * @param  
  *  *str     : string.
  *  *parn    : amount of parameters.
  *             + 0 : void.
  * @retval 
  *  0 : success.
  *  other : error.
  *
  */
u8 usmart_get_fparam(u8 *str, u8 *parn)
{
  u8 i, type;
  u32 res;
  u8 n = 0;
  u8 len;
  u8 tstr[PARM_LEN + 1]; //buf
  for (i = 0; i < MAX_PARM; i++)
    usmart_dev.plentbl[i] = 0; //clean para length list
  while (*str != '(')          //offset to start addr of para
  {
    str++;
    if (*str == '\0')
      return USMART_FUNCERR; //hit '\0'
  }
  str++; //offset next to '('
  while (1)
  {
    i = usmart_get_aparm(str, tstr, &type); //get 1st para
    str += i;                               //offset
    switch (type)
    {
    case 0: //digit
      if (tstr[0] != '\0')
      {
        i = usmart_str2num(tstr, &res); //note para
        if (i)
          return USMART_PARMERR;                                 //para error.
        *(u32 *)(usmart_dev.parm + usmart_get_parmpos(n)) = res; //note para.
        usmart_dev.parmtype &= ~(1 << n);                        //type digit
        usmart_dev.plentbl[n] = 4;                               //length = 4
        n++;                                                     //amonut++
        if (n > MAX_PARM)
          return USMART_PARMOVER; //too many para.
      }
      break;
    case 1:                                                          //string
      len = usmart_strlen(tstr) + 1;                                 //length++ for '\0'
      usmart_strcopy(tstr, &usmart_dev.parm[usmart_get_parmpos(n)]); //copy tstr to usmart_dev.parm[n]
      usmart_dev.parmtype |= 1 << n;                                 //type string
      usmart_dev.plentbl[n] = len;                                   //length = current len
      n++;
      if (n > MAX_PARM)
        return USMART_PARMOVER; //too many parameter
      break;
    case 0XFF:               //error.
      return USMART_PARMERR; //parameter error.
    }
    if (*str == ')' || *str == '\0')
      break; //hit ')' ot '\0', exit.
  }
  *parn = n;        //amount of parameters.
  return USMART_OK; //get parameter success.
}
