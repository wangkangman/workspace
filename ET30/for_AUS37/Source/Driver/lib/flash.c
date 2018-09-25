#include "flash.h"
#include "fdl.h"
#include "fdl_types.h"




void flash_read(){
	fal_descriptor_t fal_descriptor_str;
	fal_status_t     my_fal_status_enu;
	fal_request_t    request;
	
	
	my_fal_status_enu = FAL_Init((__far fal_descriptor_t*)&fal_descriptor_str );
	if(my_fal_status_enu != FAL_OK) return;
	FAL_Open();
	
	request.index_u16 = 0x0000;
	request.command_enu = FAL_CMD_READ_WORD;
	FAL_Execute(&request);
	if(request.status_enu != FAL_OK) return;




}


