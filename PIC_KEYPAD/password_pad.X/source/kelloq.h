/* 
 * File:   kelloq.h
 * Author: Gilgamesh
 *
 * Created on May 22, 2018, 3:51 PM
 */

#ifndef KELLOQ_H
#define	KELLOQ_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "type.h"
    

    
    typedef enum{
        EZMODE = 0,
        NORMODE
    }nENCRYPTMODE;

    u8* ENcrypt(u8 *source, u8* _key);
    u8* DEcrypt(u8 *source, u8* _key);
    u8  getBit( u8 *source, u16 n);
    u8* RRC(u8 *source, u8 c, u8 n);
    u8* RLC(u8 *source, u8 c, u8 n);
    u8* _keeloq_encrypt(u8 *source, nENCRYPTMODE EncryptMode);
    u8* LRN_MKEY(u8 *source, u8 *new_key);



#ifdef	__cplusplus
}
#endif

#endif	/* KELLOQ_H */

