#include"pic.h"
#include"SPI.h"

VODET SPI_init(VODET){
  /* set SDO: RC2 */
  APFCON &= ~(1 << 6);
  /* set SS : RC3 */
  APFCON &= ~(1 << 5);   
  
  /* RC0<SCK> -> digit out */
//  ANSELC &= ~(1 << 0);
//  TRISC  &= ~(1 << 0);
  /* RC1<SDI> -> digit in  */
//  ANSELC &= ~(1 << 1);   
//  TRISC  |=  (1 << 0);
  /* RC2<SDO> -> digit out */
//  ANSELC &= ~(1 << 2);   
//  TRISC  &= ~(1 << 2);
  /* RC3<SS>  -> digit out */
//  ANSELC &= ~(1 << 3); 
  TRISC  &= ~(1 << 3);
  
  /* clear irq flag*/
  PIR1=0;
  /* CKE<bit6> = 1 */
  SSP1STAT = 0x0;//0x40;  
  /* bit4<ckp> = 0*/
  /* bit[3:0] = 1010  clock = FOSC/(4 * (SSP1ADD+1)) */
 // SSP1CON1 =0x21;//backup
  SSP1CON1 = 0x2a;
//  SSP1ADD = 0x1;
  SSP1ADD = 0x7;
  RC3 = 1;
}


VODET  SPI_send_byte(U08ET data)
{
  RC3 = 0;
  SSP1BUF = data;
  while(SSP1IF == 0);
  SSP1IF=0;
  RC3 = 1;
} 

VODET  SPI_send_string(U08ET* str, U08ET length){
    U08ET index, index_delay;
    for (index = 0; index < length; index++){
        SPI_send_byte(str[index]);
        for(index_delay = 0; index_delay < 30; index_delay++){}
    }
}