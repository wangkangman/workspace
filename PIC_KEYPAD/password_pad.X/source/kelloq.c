#include "kelloq.h"

const u8 NLF[2][2][2][2][2] = {0, 1, 1, 1, 0, 1, 0, 0,
                               0, 0, 1, 0, 1, 1, 1, 0,
                               0, 0, 1, 1, 1, 0, 1, 0,
                               0, 1, 0, 1, 1, 1, 0, 0};

const char CUSTOMER_code[] = {0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01};

/* -----------------------------------------------------------------*/
/*  FUNC:  calculate new LRN                                        */
/*  PARA:                                                           */
/*    source:    source data        <8byte>                         */
/*    new_key:   new LRN            <8byte>                         */
/*  RERUEN:                                                         */
/*    pionter to new_key                                            */
/* -----------------------------------------------------------------*/
u8* LRN_MKEY(u8 *source, u8 *new_key)
{
  u8 ser[4];

  ser[3] = source[7];            
  ser[2] = source[6];
  ser[1] = source[5];
  ser[0] = source[4];
  ser[3] &= 0x0f;                
  ser[3] |= 0x20;
  DEcrypt(ser, CUSTOMER_code);  
  new_key[3] = ser[3];
  new_key[2] = ser[2];
  new_key[1] = ser[1];
  new_key[0] = ser[0];

  ser[3] = source[7];              
  ser[2] = source[6];
  ser[1] = source[5];
  ser[0] = source[4];
  ser[3] &= 0x0f;                  
  ser[3] |= 0x60;
  DEcrypt(ser, CUSTOMER_code);                       
  new_key[7] = ser[3];
  new_key[6] = ser[2];
  new_key[5] = ser[1];
  new_key[4] = ser[0];

  return new_key;
}

/* --------------------------------------------------------------------*/
/*    FUNC:   encrypt source data                                      */
/*    PARA:                                                            */
/*      source:     source data         <4byte>                        */
/*      _key:       LRN                 <8byte>                        */
/*    RETURN:                                                          */
/*      pionter to source                                              */
/* --------------------------------------------------------------------*/
u8* ENcrypt(u8 *source, u8* _key)
{
  u16 i;
  u8 c, nlf, y16, y0, k, result;
  c = 0;
  for (i = 0; i < 528; i++)
  {
    nlf = NLF[getBit(source, 31)][getBit(source, 26)][getBit(source, 20)][getBit(source, 9)][getBit(source, 1)];
    y16 = getBit(source, 16);
    y0 = getBit(source, 0);
    k = getBit(_key, i % 64);
    result = nlf^y16^y0^k;
    c = result ? 1 : 0;
    source = RRC(source, c, 4);
  }
  return source;
}


/* --------------------------------------------------------------------*/
/*    FUNC:   decrypt source data                                      */
/*    PARA:                                                            */
/*      source:     source data         <4byte>                        */
/*      _key:       LRN                 <8byte>                        */
/*    RETURN:                                                          */
/*      pionter to source                                              */
/* --------------------------------------------------------------------*/
u8 *  DEcrypt(u8 *source, u8* _key)
{
  u16 i;
  u8 c, nlf, y15, y31, k, result;
  c = 0;
  for (i = 528; i >0; i--)
  {
    nlf = NLF[getBit(source, 30)][getBit(source, 25)][getBit(source, 19)][getBit(source, 8)][getBit(source, 0)];
    y15 = getBit(source, 15);
    y31 = getBit(source, 31);
    k = getBit(_key, (i - 1) % 64);
    result = nlf^y15^y31^k;
    c = result ? 1 : 0;
    source = RLC(source, c, 4);
  }
  return source;
}


/* --------------------------------------------------------------------*/
/*    FUNC:   get bit value of str                                     */
/*    PARA:                                                            */
/*      source:     source data                                        */
/*      n:          bit position                                       */
/*    RETURN:                                                          */
/*      bit value                                                      */
/* --------------------------------------------------------------------*/
u8 getBit( u8* source, u16 n)
{
  return (source[n / 8] & ((u8)1 << (n % 8))) ? 1 : 0;
}



/* --------------------------------------------------------------------*/
/*    FUNC:  >> calculation                                            */
/*    PARA:                                                            */
/*      source:    source data                                         */
/*      c:         carry bit                                           */
/*      n:         length                                              */
/*    RETURN:                                                          */
/*      pointer to source                                              */
/* --------------------------------------------------------------------*/
u8 * RRC(u8 *source, u8 c, u8 n)
{
  u8 temp;
  for (; n>0; n--)
  {
    temp = source[n - 1];
    if (c) {
      source[n - 1] = (source[n - 1] >> 1) | 0x80;
    }
    else {
      source[n - 1] = (source[n - 1] >> 1) & 0x7f;
    }
    c = (temp & 0x01) ? 1 : 0;
  }
  return source;
}

/* --------------------------------------------------------------------*/
/*    FUNC:  << calculation                                            */
/*    PARA:                                                            */
/*      source:    source data                                         */
/*      c:         carry bit                                           */
/*      n:         length                                              */
/*    RETURN:                                                          */
/*      pointer to source                                              */
/* --------------------------------------------------------------------*/
u8 * RLC(u8 *source, u8 c, u8 n) {
  u8 temp, i;
  for (i = 0; i<n; i++) {
    temp = source[i];
    if (c != 0) {
      source[i] = (source[i] << 1) | 0x01;
    } else {
      source[i] = (source[i] << 1) & 0xfe;
    }
    c = (temp & 0x80) ? 1 : 0;
  }
  return source;
}

/* --------------------------------------------------------------------*/
/*    FUNC:  encrypt process                                           */
/*           calculate LRN by source byte4~byte7                       */
/*           then encrypt source byte0~byte3 by LRN                    */
/*    PARA:                                                            */
/*      source:    source data                                         */
/*    RETURN:                                                          */
/*      pointer to source                                              */
/* --------------------------------------------------------------------*/
u8* _keeloq_encrypt(char *source, nENCRYPTMODE EncryptMode) {
  char _key[8]; 
  if (NORMODE == EncryptMode){
	  LRN_MKEY(source, _key);
	  ENcrypt(source, _key);
	  return source;
  } else if (EZMODE == EncryptMode) {
	  ENcrypt(source, (u8*)CUSTOMER_code);
	  return source;
  }
  return source;
}

