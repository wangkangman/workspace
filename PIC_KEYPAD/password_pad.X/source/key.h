/* 
 * File:   key.h
 * Author: Gilgamesh
 *
 * Created on May 22, 2018, 1:19 PM
 */

#ifndef KEY_H
#define	KEY_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "pic.h"
    #include "type.h"

    #define KEY_ST_LOW   (0)
    #define KEY_ST_HIGH  (1)
    #define KEY_ST_MAX   (2) 
    
    #define KEY_ID_0 (0)
    #define KEY_ID_1 (1)
    #define KEY_ID_2 (2)
    #define KEY_ID_3 (3)
    #define KEY_ID_4 (4)
    #define KEY_ID_5 (5)
    #define KEY_ID_6 (6)
    #define KEY_ID_7 (7)
    #define KEY_ID_8 (8)
    #define KEY_ID_9 (9)
    #define KEY_ID_ASTERISK (10)
    #define KEY_ID_ENTER    (11)
    #define KEY_ID_MAX      (12)


   typedef  struct {
        u8 lstst:2;
        u8 curst:2;
        u8 stable:2;
        u8 active:2;
        u8 cycle:4;
        u8 counter:4;
    }ST_KEYS;


    void key_init(void);
    void key_task(void);
    u8 getkey(void);

#ifdef	__cplusplus
}
#endif

#endif	/* KEY_H */

