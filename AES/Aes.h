#ifndef __AES_H
#define __AES_H
#include "tydef.h"


/**
* ���� p: ���ĵ��ַ������顣
* ���� plen: ���ĵĳ���,���ȱ���Ϊ16�ı�����
* ���� key: ��Կ���ַ������顣
*/
void aes(U08ET *p, U08ET plen, U08ET *key);

/**
* ���� c: ���ĵ��ַ������顣
* ���� clen: ���ĵĳ���,���ȱ���Ϊ16�ı�����
* ���� key: ��Կ���ַ������顣
*/
void deAes(U08ET *c, U08ET clen, U08ET *key);

void AES_test(void);

#endif
