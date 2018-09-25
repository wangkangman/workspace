#include "keeloq.h"

/* ------------------------------------------*/
/*   table:NLF(x4,x3,x2,x1,x0) =             */
/*   x4x3x2^x4x3x1^x4x2x0^x4x1x0^x4x2^       */
/*   x4x0^x3x2^x3x0^x2x1^x1x0^x1^x0          */
/* ------------------------------------------*/
const u8 NLF[2][2][2][2][2]={0,1,1,1,0,1,0,0,
                             0,0,1,0,1,1,1,0,
                             0,0,1,1,1,0,1,0,
                             0,1,0,1,1,1,0,0};

// active const u8 CUSTOMER_code[8]={0x21, 0x18, 0xde, 0x7b, 0x0a, 0x9c, 0xd5, 0x51};
//const u8 CUSTOMER_code[8]={0x75, 0x31, 0x84, 0x83, 0xe9, 0x35, 0x9f, 0xfb};
//const u8 CUSTOMER_code[8]={0x51, 0xd5, 0x9c, 0x0a, 0x7b, 0xde, 0x18, 0x21};
// u8 CUSTOMER_code[8]={0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01};
/* -----------------------------------------------------------------*/
/*  FUNC:  calculate new LRN                                        */
/*  PARA:                                                           */
/*    source:    source data        <8byte>                        */
/*    new_key:   new LRN            <8byte>                        */
/*  RERUEN:                                                         */
/*    pionter to new_key                                            */
/* -----------------------------------------------------------------*/
/*u8* LRN_MKEY(u8 *source, u8 *new_key){
  new_key[3] = source[7];            
  new_key[2] = source[6];
  new_key[1] = source[5];
  new_key[0] = source[4];
  new_key[3] &= 0x0f;                
  new_key[3] |= 0x20;
  DEcrypt(new_key, (u8*)CUSTOMER_code);  

  new_key[7] = source[7];              
  new_key[6] = source[6];
  new_key[5] = source[5];
  new_key[4] = source[4];
  new_key[7] &= 0x0f;                  
  new_key[7] |= 0x60;
  DEcrypt(new_key + 4, (u8*)CUSTOMER_code);
  
  return new_key;
}*/

/* --------------------------------------------------------------------*/
/*    FUNC:   encrypt source data                                      */
/*    PARA:                                                            */
/*      source:     source data         <4byte>                        */
/*      _key:       LRN                 <8byte>                        */
/*    RETURN:                                                          */
/*      pionter to source                                              */
/* --------------------------------------------------------------------*/
/*u8* ENcrypt(u8 *source, u8* _key)
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
}*/

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
    U08ET bitk[4], res;
    U16ET index;
    for(index=0; index<528; index++) {
        bitk[0] = NLF[getBit(source,30)][getBit(source,25)][getBit(source,19)][getBit(source, 8)][getBit(source,0)];
        bitk[1] = getBit(source, 15);
        bitk[2] = getBit(source, 31);
        bitk[3] = getBit(_key, (527-index)%64);
        res = bitk[0]^bitk[1]^bitk[2]^bitk[3];
        source[3] = (source[3]<<1) | (source[2]>>7);
        source[2] = (source[2]<<1) | (source[1]>>7);
        source[1] = (source[1]<<1) | (source[0]>>7);
        source[0] = (source[0]<<1) | (res);
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
 //   U08ET byidx, btidx;

 //   byidx = n/8;
 //   btidx = n%8;

    return (source[n / 8] & (1 << (n % 8))) ? 1 : 0;
 //   return (source[byidx] & (1<<btidx)) ? 1 : 0;
/*    if( 0 == (source[byidx] & (1<<btidx)) ) {
        return 0;
    }
    else{
        return 1;
    }*/
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
/*u8 * RRC(u8 *source, u8 c, u8 n)
{
  u8 temp;
  for (; n>0; n--) {
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
}*/

/* --------------------------------------------------------------------*/
/*    FUNC:  << calculation                                            */
/*    PARA:                                                            */
/*      source:    source data                                         */
/*      c:         carry bit                                           */
/*      n:         length                                              */
/*    RETURN:                                                          */
/*      pointer to source                                              */
/* --------------------------------------------------------------------*/
/*u8 * RLC(u8 *source, u8 c, u8 n) {
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
}*/

/* --------------------------------------------------------------------*/
/*    FUNC:  encrypt process                                           */
/*           calculate LRN by source byte4~byte7                       */
/*           then encrypt source byte0~byte3 by LRN                    */
/*    PARA:                                                            */
/*      source:    source data                                         */
/*    RETURN:                                                          */
/*      pointer to source                                              */
/* --------------------------------------------------------------------*/
/*u8* _keeloq_encrypt(u8 *source, u8 EncryptMode) {
  u8 _key[8]; 
  if (NORMODE == EncryptMode){
	  LRN_MKEY(source, _key);
	  ENcrypt(source, _key);
  } else if (EZMODE == EncryptMode) {
	  ENcrypt(source, (u8*)CUSTOMER_code);
  }
  return source;
}*/

/* --------------------------------------------------------------------*/
/*    FUNC:  decrypt process                                           */
/*           calculate LRN_key by source byte4~byte7                   */
/*           then decrypt source byte0~byte3 by LRN                    */
/*    PARA:                                                            */
/*      source:    source data                                         */
/*    RETURN:                                                          */
/*      pointer to source                                              */
/* --------------------------------------------------------------------*/
/*void _keeloq_decrypt(u8 *source, u8* key) {
  /*u8 _key[8]; 
  LRN_MKEY(source, _key);
  DEcrypt(source, _key);
  return source;*/
//   DEcrypt((u8*)source, (u8*)key);
//}

