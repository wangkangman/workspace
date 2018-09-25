#include"pic.h"
#include"type.h"
#include"usart.h"

/*
 * @brief Inital uart_1 parameter
 *   para: None
 *   return : none
 * 
 */
void Init_Uart(){
    //SPBRGL = 0X1;
 //   APFCON0 &= ~(1<<7);
 //   APFCON0 &= ~(1<<2);
    APFCON0 &= ~(1<<7);
    APFCON0 |= (1<<2);
    //TXSTA
    CSRC = 0;
    TX9 = 1;
    TXEN = 1;
    SYNC=0;
    BRGH=1;
    BRG16 = 0;
    TRMT=1;
    //TX9D=1;
    //RCSTA
    SPEN = 1;
    RX9 = 1;
    SREN=1;
    CREN=1;
    ADDEN=0;
    FERR=0;
    OERR=0;
    RCIE = 1;
    TRISC |= (1<<5);
    BAUDCON = (1<<3);
    SPBRGH = 0x1;
    SPBRGL = 0xa1;
}
/*void Init_Uart(){
    //SPBRGL = 0X1;
    APFCON0 &= ~(1<<7);
    APFCON0 |= (1<<2);
    //TXSTA
    CSRC = 0;
    TX9 = 1;
    TXEN = 1;
    SYNC=0;
    BRGH=1;
    BRG16 = 0;
    TRMT=1;

    SPEN = 1;
    RX9 = 1;
    SREN=1;
    CREN=1;
    ADDEN=0;
    FERR=0;
    OERR=0;
    TRISC |= (1<<5);
    BAUDCON = (1<<3);
    SPBRGH = 0x1;
    SPBRGL = 0xa1;
}*/

/*
 * @brief send 1 byte by usart
 *   para
 *     Byte : the data should be sent
 *   return : none
 * 
 */
void Uart1SendByte(u8 Byte){   
    while(!TRMT);//?????????? ??
    TXREG = Byte ;
    TX9D = even_check(Byte);
}

/*
 * @brief send string by usart_1
 *   para : 
 *     a : piont to the string
 *     DatLong : length of the string
 *   return : None
 */
/*void Uart1SendString(u8 *a ,u8 DatLong) {   
  u8 i = 0 ;
  for( i = DatLong ; i > 0; i--, a++) {
    while(!TRMT);//?????????? ??
    TXREG = *a ;  
    TX9D = even_check(*a);
  }
}*/

/*
 * @brief even check
 *   para:
 *     data : the data which should be checked
 *   return:
 *     return the result <1 or 0>
 */
u8 even_check(u8 data){
    u8 res;
    while (data) {
      res = !res;
      data = data & (data - 1);
    }
    return res;
}

