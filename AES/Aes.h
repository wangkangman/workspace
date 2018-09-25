#ifndef __AES_H
#define __AES_H
#include "tydef.h"


/**
* 参数 p: 明文的字符串数组。
* 参数 plen: 明文的长度,长度必须为16的倍数。
* 参数 key: 密钥的字符串数组。
*/
void aes(U08ET *p, U08ET plen, U08ET *key);

/**
* 参数 c: 密文的字符串数组。
* 参数 clen: 密文的长度,长度必须为16的倍数。
* 参数 key: 密钥的字符串数组。
*/
void deAes(U08ET *c, U08ET clen, U08ET *key);

void AES_test(void);

#endif
