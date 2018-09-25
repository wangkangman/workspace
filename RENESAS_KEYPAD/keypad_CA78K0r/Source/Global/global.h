#ifndef __GLOBAL_H
#define __GLOBAL_H

//#pragma sfr
//#pragma DI
//#pragma EI
//#pragma NOP
//#pragma HALT
//#pragma STOP

#define ET_TRUE (1)
#define ET_FALSE (0)


typedef signed   char  s8;
typedef signed   short s16;
typedef signed   long  s32;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

typedef void (*FUNC)(void);





typedef struct KEELOQ {
/*	u16 Encrypted_Sync_Counter : 16;
	u16 Encrypted_DISC : 10;
	u16 Encrypted_OVR : 2;
	u16 Encrypted_Button_Status : 4;*/
	u32 Encrypted_Data;
	u32 Fixed_Serial_Number_And_Button;
	u8  Fixed_Vlow : 1;
	u8  Fixed_Repeat : 1;
}KEELOQ;

typedef struct GLOBAL_CTRL{
    u8 channel;
    u8 sign_in_pw[4];
    u8 sign_in_step;
    
    u8 new_pw_1[4];
    u8 new_pw_2[4];
        
    KEELOQ Keeloq;
    
    u8  PassWord[4][4];
    u16 send_counter;
    u16 data_flag;
    u8  EndAddress;
}GLOBAL_CTRL;

extern GLOBAL_CTRL glb_ctr;
#define SEND_TIMES (6)
#define AUTO_CLOSE (4500)


#endif