#include "Remote.h"
#ifdef UART_DEBUG
#include "UartPrin.h"
#endif 
/*-----------------------------------------------------------------*/
/* Const variable define                                           */
/*-----------------------------------------------------------------*/
const U08ET NLF[2][2][2][2][2] = 
{
    0,1,1,1,0,1,0,0,
    0,0,1,0,1,1,1,0,
    0,0,1,1,1,0,1,0,
    0,1,0,1,1,1,0,0,
};
const U08ET svid[8] = 
{
    0x02, 0x00, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB,
};

static ETST_REMOTE remoter;
static KEY_ST key_stored;

/*-----------------------------------------------------------------*/
/* Function declare                                                */
/*-----------------------------------------------------------------*/
static U08ET getsbit(U08ET *psrc, U16ET bidx);
static void decrypt(U08ET* pbuf, U08ET* nkey);
/*********************************************************************************
   FUNC: remote inital
     PARA :                                                          
       NONE                                                          
     RETN :                                                          
       NONE 
   inital the remote struct 
**********************************************************************************
**/

static void local_updata(void){
	U08ET err_count = 0;
   while (0 != write_data_to_dataflash((U08ET*)&key_stored, 0x400, sizeof(KEY_ST)) && err_count < 5) {
        err_count++;
   }
}


void remote_init(void){
	U08ET index;
	
	PORT_PMC1 &= ~(1<<4); /* digital port */
        PORT_PM1  |=  (1<<4); /* input mode */

	
	remoter.st_last = 0;
	remoter.st_curt = 0;
	remoter.st_remo = ST_REMOTE_TP;
	remoter.recvidx = 0;
	for (index = 0; index < RBUF_SIZE; index++) {
		remoter.rbuf[index] = 0;
		remoter.lbuf[index] = 0;
	}

}
/**
  * @brief  Read the stored key from dataflash, when sys open.
  * @param  None
  * @retval None
  *
  */
void RemoteKey_Read(void){
	U08ET index;
	wdtdog_feed();
	read_data_from_dataflash((U08ET*)&key_stored, 0x400, sizeof(KEY_ST));
	if (REMOTE_SAVE_FLAG != key_stored.save_flag){
		key_stored.save_flag = REMOTE_SAVE_FLAG;
		key_stored.store_step = 0;	
		key_stored.store_count = 0;
	}	
	wdtdog_feed();
	
	#ifdef UART_DEBUG
	PutString("----Remote stored key:\r\n");
	for (index = 0; index < KEY_AMOUNT; ++index) {
		wdtdog_feed();
		puthex(key_stored.store_key[index][0]);
		puthex(key_stored.store_key[index][1]);
		puthex(key_stored.store_key[index][2]);
		puthex(key_stored.store_key[index][3]);	
		PutString("\r\n");
	}
	wdtdog_feed();
	PutString("----Remote stored Step:");
	puthex(key_stored.store_step);
	PutString("\r\n");
	PutString("----Remote stored Count:");
	puthex(key_stored.store_count);	
	PutString("\r\n");	
	#endif
}



/*********************************************************************************
   FUNC: remote code get
     PARA :                                                          
       NONE                                                          
     RETN :                                                          
       NONE 
   called by timer2   get remote status; 
**********************************************************************************
**/
void remote_timer_task(void){
	static U08ET lv_cnt, tp_cnt;
	U08ET  byidx,btidx;
	remoter.st_last = remoter.st_curt;
	remoter.st_curt = PORT_PD1& (0x1 << 4) ? 1 : 0;
    
	if (remoter.st_last == remoter.st_curt) {
		lv_cnt++;
	} else {
		if (ST_REMOTE_TP == remoter.st_remo) {
			if ((lv_cnt >= CNT_TE_MIN) && (lv_cnt <= CNT_TE_MAX)) {
				tp_cnt++;
				if (CNT_TP_TE == tp_cnt) {
					remoter.st_remo = ST_REMOTE_TH;
				}
			} else {
				tp_cnt = 0;
			}
			lv_cnt = 0;
	    	} else if (ST_REMOTE_TH == remoter.st_remo) {
			if ((1 == remoter.st_curt) && (lv_cnt>=CNT_TH_MIN) && (lv_cnt<=CNT_TH_MAX)) {
				remoter.st_remo = ST_REMOTE_DATA;
				remoter.recvidx = 0;
			} else {
				remoter.st_remo = ST_REMOTE_TP;
			}
			lv_cnt = 0;
	    	} else if (ST_REMOTE_DATA == remoter.st_remo) {
			if (1==remoter.st_curt) {
				byidx = remoter.recvidx/8;
				btidx = remoter.recvidx%8;
				if ((lv_cnt>=CNT_L_MIN) && (lv_cnt<=CNT_L_MAX)) {
					remoter.rbuf[byidx] &= (U08ET)(~(1<<btidx));
					remoter.recvidx++;
				} else if ((lv_cnt>=CNT_H_MIN) && (lv_cnt<=CNT_H_MAX)) {
					remoter.rbuf[byidx] |= (U08ET)(1<<btidx);
					remoter.recvidx++;
				} else {
					remoter.st_remo = ST_REMOTE_TP;
				}

				if(remoter.recvidx >= RBUF_SIZE*8) {
					remoter.st_remo = ST_REMOTE_OK;
				}
			}
			lv_cnt = 0;
		}	
	}
}


/*********************************************************************************
   FUNC: Get remote controller key 
     PARA :                                                          
       pbuf : buff to store key value                                                        
     RETN :                                                          
       return ET_TURE if receive a key code, else return ET_FALSE  
**********************************************************************************
**/
U08ET remote_gkey(U08ET *pbuf)
{
    U08ET index;

    if (ST_REMOTE_OK != remoter.st_remo) {
        return ET_FALSE;
    }

    if ((0x00 == remoter.rbuf[4]) && (0x00 == remoter.rbuf[5]) && (0x00 == remoter.rbuf[6]) && (0x00 == (remoter.rbuf[7]&0x0F))) {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    if ((0xFF == remoter.rbuf[4]) && (0xFF == remoter.rbuf[5]) && (0xFF == remoter.rbuf[6]) && (0x0F == (remoter.rbuf[7]&0x0F))) {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    /* check if the remote controller is keep pressed */
    for (index = 0; index < ENCY_SIZE; index++) {
        if (remoter.rbuf[index] != remoter.lbuf[index]) {
            break;
        }
    }
    if (ENCY_SIZE == index) {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }

    for (index = 0; index < RBUF_SIZE; index++) {
        remoter.lbuf[index] = remoter.rbuf[index];
    }
    
    /* decrypt hopping code */
    wdtdog_feed();
    decrypt(remoter.rbuf, svid);

    /* check decryption result */
    if( (remoter.rbuf[2] != remoter.rbuf[4]) || ((remoter.rbuf[3]&0xF0) != (remoter.rbuf[7]&0xF0)) ) {
        remoter.st_remo = ST_REMOTE_TP;
        return ET_FALSE;
    }    
    
    pbuf[0] = remoter.rbuf[4];
    pbuf[1] = remoter.rbuf[5];
    pbuf[2] = remoter.rbuf[6];
    pbuf[3] = remoter.rbuf[7];
    
   

    remoter.st_remo = ST_REMOTE_TP;

    return ET_TRUE;
}


/*********************************************************************************
 *  FUNC: Get a saved remote key
 *    PARA :                                                          
 *      NONE                                                        
 *    RETN :                                                          
 *      return ET_TURE if password is right, else return ET_FALSE  
**********************************************************************************
**/
U08ET remote_hits(void)
{
    U08ET key[4], index;

    if (ET_FALSE == remote_gkey(key)) {
        return ET_FALSE;
    }
    for (index = 0; index < key_stored.store_count; ++index) {
	if (key[0] == key_stored.store_key[index][0] && \
    	    key[1] == key_stored.store_key[index][1] && \
	    key[2] == key_stored.store_key[index][2] && \
	    key[3] == key_stored.store_key[index][3]) {
		return ET_TRUE;
	}
    }   
    
    return ET_FALSE;
}
/**
  * @brief  Insert 1 key into DataFlash.
  * @param  
  *	pbuf: piont to the new key(4 byte msb -> lsb);
  * @retval None
  *
  */
void  remote_insert_key(U08ET *pbuf){
	U08ET index;
	for (index = 0; index < key_stored.store_count; ++index){
		if (pbuf[0] == key_stored.store_key[index][0] && \
	            pbuf[1] == key_stored.store_key[index][1] && \
		    pbuf[2] == key_stored.store_key[index][2] && \
		   (pbuf[3] & 0x0f) == (key_stored.store_key[index][3] & 0x0f)) {
   			    key_stored.store_key[index][3] = pbuf[3];
//			    write_data_to_dataflash((U08ET*)&key_stored, 0x400, sizeof(KEY_ST));
			    local_updata();
			    return;
		}
	}
	key_stored.store_key[key_stored.store_step][0] = pbuf[0];
	key_stored.store_key[key_stored.store_step][1] = pbuf[1];
	key_stored.store_key[key_stored.store_step][2] = pbuf[2];
	key_stored.store_key[key_stored.store_step][3] = pbuf[3];
	key_stored.store_step = (key_stored.store_step == KEY_AMOUNT - 1) ? 0 : (key_stored.store_step + 1);
	if(key_stored.store_count < KEY_AMOUNT)
		++key_stored.store_count;
		#ifdef UART_DEBUG
			PutString("Remote flag_2: ");
			PutString("\r\n");
		    	#endif 
//	write_data_to_dataflash((U08ET*)&key_stored, 0x400, sizeof(KEY_ST));
        local_updata();
}
/**
  * @brief  Clear the Dataflash.
  * @param  None
  * @retval None
  *
  */
void  remote_clrs(void){
/*	U08ET index;
	for (index = 0; index <= KEY_AMOUNT; ++index){
		key_stored.store_key[index][0] = 0x0;
		key_stored.store_key[index][1] = 0xff;
		key_stored.store_key[index][2] = 0xff;
		key_stored.store_key[index][3] = 0xff;
	}*/
	key_stored.save_flag = REMOTE_SAVE_FLAG;
	key_stored.store_step = 0;	
	key_stored.store_count = 0;
//	write_data_to_dataflash((U08ET*)&key_stored, 0x400, sizeof(KEY_ST));
	local_updata();	
}

/*-----------------------------------------------------------------*/
/* FUNC : Get one bit of from array                                */
/* PARA :                                                          */
/*   psrc : array data buff                                        */
/*   bidx : target bit index                                       */
/* RETN :                                                          */
/*   return bit value                                              */
/*-----------------------------------------------------------------*/
static U08ET getsbit(U08ET *psrc, U16ET bidx)
{
    U08ET byidx, btidx;

    byidx = (U08ET)(bidx/8);
    btidx = (U08ET)(bidx%8);

    if( 0 == (psrc[byidx] & (1<<btidx)) ) {
        return 0;
    }
    else{
        return 1;
    }
}

/*-----------------------------------------------------------------*/
/* FUNC : Simple mode KEELOQ decryption                            */
/* PARA :                                                          */
/*   pbuf : orginal encrypted data                                 */
/*   nkey : nlf key                                                */
/* RETN :                                                          */
/*   NONE                                                          */
/*-----------------------------------------------------------------*/
static void decrypt(U08ET pbuf[4], U08ET nkey[8])
{
    U08ET bitk[4], res;
    U16ET index;

    for(index=0; index<528; index++) {
        bitk[0] = NLF[getsbit(pbuf,30)][getsbit(pbuf,25)][getsbit(pbuf,19)][getsbit(pbuf, 8)][getsbit(pbuf,0)];
        bitk[1] = getsbit(pbuf, 15);
        bitk[2] = getsbit(pbuf, 31);
        bitk[3] = getsbit(nkey, (527-index)%64);
        res = bitk[0]^bitk[1]^bitk[2]^bitk[3];
        pbuf[3] = (pbuf[3]<<1) | (pbuf[2]>>7);
        pbuf[2] = (pbuf[2]<<1) | (pbuf[1]>>7);
        pbuf[1] = (pbuf[1]<<1) | (pbuf[0]>>7);
        pbuf[0] = (pbuf[0]<<1) | (res);
    }
}


