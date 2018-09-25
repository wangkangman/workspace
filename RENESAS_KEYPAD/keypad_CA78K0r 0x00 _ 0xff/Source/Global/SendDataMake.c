

#pragma section @@CNSTL DFL_C AT 01F00H 
__far const unsigned char SN[]={0x00,0x00,0x00,0x00};

#include "global.h"
#include "keeloq.h"


//#pragma section @@CNSTL DFL_C AT 02CF0H




/*void get_source_code(void) {
     glb_ctr.Keeloq.Encrypted_Sync_Counter  = glb_ctr.send_counter;
     ++glb_ctr.send_counter;

     glb_ctr.Keeloq.Fixed_Serial_Number_And_Button = *((u32 *)SN);
     glb_ctr.Keeloq.Fixed_Serial_Number_And_Button &= 0x3fffffff;
     switch (glb_ctr.channel){
	     case 0:
	     	glb_ctr.Keeloq.Encrypted_Button_Status = 0x2;
		glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= ((u32)0x2) << 28;
		break;
	     case 1:
	     	glb_ctr.Keeloq.Encrypted_Button_Status = 0x4;
		glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= ((u32)0x4) << 28;
		break;
	     case 2:
	     	glb_ctr.Keeloq.Encrypted_Button_Status = 0x8;
		glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= ((u32)0x8) << 28;
		break;
	     case 3:
	     	glb_ctr.Keeloq.Encrypted_Button_Status = 0x1;
		glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= ((u32)0x1) << 28;	
		break;
	     default:
		break;
     }
     glb_ctr.Keeloq.Encrypted_DISC      = *((u32 *)SN);
     glb_ctr.Keeloq.Encrypted_OVR       = 0x0;
     _keeloq_encrypt((u8*)&glb_ctr.Keeloq, EZMODE);
}*/
#if 0
void get_source_code(void) {
	u32 LocalSN = *((u32 *)SN);
	glb_ctr.Keeloq.Encrypted_Data = 0;
	glb_ctr.Keeloq.Fixed_Serial_Number_And_Button = 0;
	glb_ctr.Keeloq.Fixed_Vlow = 0;
	glb_ctr.Keeloq.Fixed_Repeat = 0;
        ++glb_ctr.send_counter;
	
	glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= (LocalSN & 0x0fffffff);
	glb_ctr.Keeloq.Encrypted_Data |= (glb_ctr.send_counter & 0xffff);
	glb_ctr.Keeloq.Encrypted_Data |= ((LocalSN & 0x3ff) << 16);

     switch (glb_ctr.channel){
	     case 0:
		glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= (0x20000000);
		glb_ctr.Keeloq.Encrypted_Data                 |= (0x20000000);
		break;
	     case 1:
	     	glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= (0x40000000);
		glb_ctr.Keeloq.Encrypted_Data                 |= (0x40000000);
		break;
	     case 2:
	     	glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= (0x80000000);
		glb_ctr.Keeloq.Encrypted_Data                 |= (0x80000000);
		break;
	     case 3:
	     	glb_ctr.Keeloq.Fixed_Serial_Number_And_Button |= (0x10000000);
		glb_ctr.Keeloq.Encrypted_Data                 |= (0x10000000);	
		break;
	     default:
		break;
     }
     
     _keeloq_encrypt((u8*)&glb_ctr.Keeloq, NORMODE);
}
#endif

void get_source_code(void) {

     switch (glb_ctr.channel){
	    case 0:
			glb_ctr.Keeloq.Fixed_Serial_Number_And_Button = 0x0;
			glb_ctr.Keeloq.Encrypted_Data                 = 0x0;
		break;
	    
		case 1:
			glb_ctr.Keeloq.Fixed_Serial_Number_And_Button = 0xffffffff;
			glb_ctr.Keeloq.Encrypted_Data                 = 0xffffffff;
		break;
	    
		default:
		break;
     }

	 glb_ctr.Keeloq.Fixed_Vlow = 0;
	 glb_ctr.Keeloq.Fixed_Repeat = 0;
}


u8 get_cur_data(u8 step) {
	return (*(((unsigned int*)&glb_ctr.Keeloq) + (step/16))) & (1<<(step%16)) ? 0:1; 
}