#include "Aes.h"
#include "regdef.h"
#ifdef DBG_UART
#include "Uart.h"
#include "stdio.h"
#endif
/**
* S��
*/
static const U08ET S[16][16] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

/**
* ��S��
*/
static const U08ET S2[16][16] = { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

/**
* ��ȡ�������ݵĵ�8λ����4��λ
*/
static U08ET getLeft4Bit(U32ET num) {
	U08ET left = num & 0xf0;
	return left >> 4;
}

/**
* ��ȡ�������ݵĵ�8λ����4��λ
*/
static U08ET getRight4Bit(U32ET num) {
	return num & 0xf;
}
/**
* ������������S���л��Ԫ��
*/
static U08ET getNumFromSBox(U08ET index) {
	U08ET row, col;
	row = getLeft4Bit(index);
	col = getRight4Bit(index);
	return S[row][col];
}

/**
* ��һ���ַ�ת�������
*/
static U08ET getIntFromChar(U08ET c) {
	U08ET result = (U08ET)c;
	return result & 0xff;
}

/**
* ��16���ַ�ת���4X4�����飬
* �þ������ֽڵ�����˳��Ϊ���ϵ��£�
* �������������С�
*/
static void convertToIntArray(U08ET *str, U08ET pa[4][4]) {
	U08ET k = 0, i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			pa[j][i] = getIntFromChar(str[k]);
			k++;
		}
}

/**
* ��ӡ4X4������
*/
/*static void printArray(U32ET a[4][4]) {
	for (U32ET i = 0; i < 4; i++) {
		for (U32ET j = 0; j < 4; j++)
			printf("a[%d][%d] = 0x%x ", i, j, a[i][j]);
		printf("\n");
	}
	printf("\n");
}*/

/**
* ��ӡ�ַ�����ASSCI��
* ��ʮ��������ʾ��
*/
/*static void printASSCI(U08ET *str, U32ET len) {
	for (U32ET i = 0; i < len; i++)
		printf("0x%x ", getIntFromChar(str[i]));
	printf("\n");
}*/

/**
* ��������4���ַ��ϲ���һ��4�ֽڵ�����
*/
static U32ET getWordFromStr(U08ET *str) {
	return ((U32ET)getIntFromChar(str[0]) << 24) | \
		   ((U32ET)getIntFromChar(str[1]) << 16) | \
		   ((U32ET)getIntFromChar(str[2]) << 8)  | \
		    (U32ET)getIntFromChar(str[3]);
}

/**
* ��һ��4�ֽڵ����ĵ�һ�����������ĸ��ֽ�ȡ����
* ���һ��4��Ԫ�ص������������档
*/
static void splitIntToArray(U32ET num, U08ET array[4]) {
	array[0] = (num >> 24) & 0xff;
	array[1] = (num >> 16) & 0xff;
	array[2] = (num >> 8) & 0xff;
	array[3] = num & 0xff;
}

/**
* �������е�Ԫ��ѭ������stepλ
*/
static void leftLoop4int(U08ET array[4], U08ET step) {
	U08ET temp[4], i;
	U08ET index = step % 4;
	for (i = 0; i < 4; i++)
		temp[i] = array[i];

	for (i = 0; i < 4; i++) {
		array[i] = temp[index];
		index++;
		index = index % 4;
	}
}

/**
* �������еĵ�һ������������Ԫ�طֱ���Ϊ
* 4�ֽ����͵ĵ�һ�������������ֽڣ��ϲ���һ��4�ֽ�����
*/
static U32ET mergeArrayToInt(U08ET array[4]) {
	return (U32ET)array[0] << 24 | (U32ET)array[1] << 16 | (U32ET)array[2] << 8 | (U32ET)array[3];
}

/**
* ������ֵ��
*/
static const U32ET Rcon[10] = { 0x01000000, 0x02000000,
0x04000000, 0x08000000,
0x10000000, 0x20000000,
0x40000000, 0x80000000,
0x1b000000, 0x36000000 };
/**
* ��Կ��չ�е�T����
*/
static U32ET T(U32ET num, U32ET round) {
	U08ET numArray[4], i;
	splitIntToArray(num, numArray);
	leftLoop4int(numArray, 1);//��ѭ��

	for (i = 0; i < 4; i++)
		numArray[i] = getNumFromSBox(numArray[i]);

	return mergeArrayToInt(numArray) ^ Rcon[round];
}

//��Կ��Ӧ����չ����
static U32ET w[44];

/**
* ��չ��Կ������ǰ�w[44]�е�ÿ��Ԫ�س�ʼ��
*/
static void extendKey(U08ET *key) {
	U08ET i, j;
	for (i = 0; i < 4; i++)
		w[i] = getWordFromStr(key + i * 4);

	for ( i = 4, j = 0; i < 44; i++) {
		if (i % 4 == 0) {
			w[i] = w[i - 4] ^ T(w[i - 1], j);
			j++;//��һ��
		}
		else {
			w[i] = w[i - 4] ^ w[i - 1];
		}
	}

}

/**
* ����Կ��
*/
static void addRoundKey(U08ET array[4][4], U32ET round) {
	U08ET warray[4], i, j;
	for (i = 0; i < 4; i++) {
		splitIntToArray(w[round * 4 + i], warray);
		for (j = 0; j < 4; j++) {
			array[j][i] = array[j][i] ^ warray[j];
		}
	}
}

/**
* �ֽڴ���
*/
static void subBytes(U08ET array[4][4]) {
	U08ET i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			array[i][j] = getNumFromSBox(array[i][j]);
}

/**
* ����λ
*/
static void shiftRows(U08ET array[4][4]) {
	U08ET rowTwo[4], rowThree[4], rowFour[4], i;
	//����״̬����ĵ�2,3,4��
	for (i = 0; i < 4; i++) {
		rowTwo[i] = array[1][i];
		rowThree[i] = array[2][i];
		rowFour[i] = array[3][i];
	}
	//ѭ��������Ӧ��λ��
	leftLoop4int(rowTwo, 1);
	leftLoop4int(rowThree, 2);
	leftLoop4int(rowFour, 3);

	//�����ƺ���и��ƻ�״̬������
	for (i = 0; i < 4; i++) {
		array[1][i] = rowTwo[i];
		array[2][i] = rowThree[i];
		array[3][i] = rowFour[i];
	}
}

/**
* �л��Ҫ�õ��ľ���
*/
static const U08ET colM[4][4] = { 2, 3, 1, 1,
1, 2, 3, 1,
1, 1, 2, 3,
3, 1, 1, 2 };

static U32ET GFMul2(U32ET s) {
	U32ET result = s << 1;
	U32ET a7 = result & 0x00000100;

	if (a7 != 0) {
		result = result & 0x000000ff;
		result = result ^ 0x1b;
	}

	return result;
}

static U32ET GFMul3(U32ET s) {
	return GFMul2(s) ^ s;
}

static U32ET GFMul4(U32ET s) {
	return GFMul2(GFMul2(s));
}

static U32ET GFMul8(U32ET s) {
	return GFMul2(GFMul4(s));
}

static U32ET GFMul9(U32ET s) {
	return GFMul8(s) ^ s;
}

static U32ET GFMul11(U32ET s) {
	return GFMul9(s) ^ GFMul2(s);
}

static U32ET GFMul12(U32ET s) {
	return GFMul8(s) ^ GFMul4(s);
}

static U32ET GFMul13(U32ET s) {
	return GFMul12(s) ^ s;
}

static U32ET GFMul14(U32ET s) {
	return GFMul12(s) ^ GFMul2(s);
}

/**
* GF�ϵĶ�Ԫ����
*/
static U32ET GFMul(U32ET n, U32ET s) {
	U32ET result;

	if (n == 1)
		result = s;
	else if (n == 2)
		result = GFMul2(s);
	else if (n == 3)
		result = GFMul3(s);
	else if (n == 0x9)
		result = GFMul9(s);
	else if (n == 0xb)//11
		result = GFMul11(s);
	else if (n == 0xd)//13
		result = GFMul13(s);
	else if (n == 0xe)//14
		result = GFMul14(s);

	return result;
}
/**
* �л��
*/
static void mixColumns(U08ET array[4][4]) {

	U32ET tempArray[4][4], i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			tempArray[i][j] = array[i][j];

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			array[i][j] = GFMul(colM[i][0], tempArray[0][j]) ^ GFMul(colM[i][1], tempArray[1][j])
				^ GFMul(colM[i][2], tempArray[2][j]) ^ GFMul(colM[i][3], tempArray[3][j]);
		}
}
/**
* ��4X4����ת���ַ���
*/
static void convertArrayToStr(U08ET array[4][4], U08ET *str) {
	U08ET i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			*str++ = (char)array[j][i];
}
/**
* �����Կ����
*/
static U08ET checkKeyLen(U08ET len) {
	if (len == 16)
		return 1;
	else
		return 0;
}

/**
* ���� p: ���ĵ��ַ������顣
* ���� plen: ���ĵĳ��ȡ�
* ���� key: ��Կ���ַ������顣
*/
void aes(U08ET *p, U08ET plen, U08ET *key) {
	U08ET pArray[4][4], k, i;
	extendKey(key);//��չ��Կ
	for (k = 0; k < plen; k += 16) {
		convertToIntArray(p + k, pArray);
		addRoundKey(pArray, 0);//һ��ʼ������Կ��
		for (i = 1; i < 10; i++) {//ǰ9��
			subBytes(pArray);//�ֽڴ���
			shiftRows(pArray);//����λ
			mixColumns(pArray);//�л��
			addRoundKey(pArray, i);
		}
		//��10��
		subBytes(pArray);//�ֽڴ���
		shiftRows(pArray);//����λ
		addRoundKey(pArray, 10);
		convertArrayToStr(pArray, p + k);
	}
}
/**
* ������������S���л�ȡֵ
*/
static U32ET getNumFromS1Box(U32ET index) {
	U08ET row, col;
	row = getLeft4Bit(index);
	col = getRight4Bit(index);
	return S2[row][col];
}
/**
* ���ֽڱ任
*/
static void deSubBytes(U08ET array[4][4]) {
	U08ET i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			array[i][j] = getNumFromS1Box(array[i][j]);
}
/**
* ��4��Ԫ�ص�����ѭ������stepλ
*/
static void rightLoop4int(U08ET array[4], U08ET step) {
	U08ET temp[4];
	S08ET index, i;
	for (i = 0; i < 4; i++)
		temp[i] = array[i];

	index = step % 4;
	index = 3 - index;
	for (i = 3; i >= 0; i--) {
		array[i] = temp[index];
		index--;
		index = index == -1 ? 3 : index;
	}
}

/**
* ������λ
*/
static void deShiftRows(U08ET array[4][4]) {
	U08ET rowTwo[4], rowThree[4], rowFour[4], i;
	for (i = 0; i < 4; i++) {
		rowTwo[i] = array[1][i];
		rowThree[i] = array[2][i];
		rowFour[i] = array[3][i];
	}

	rightLoop4int(rowTwo, 1);
	rightLoop4int(rowThree, 2);
	rightLoop4int(rowFour, 3);

	for (i = 0; i < 4; i++) {
		array[1][i] = rowTwo[i];
		array[2][i] = rowThree[i];
		array[3][i] = rowFour[i];
	}
}
/**
* ���л���õ��ľ���
*/
static const U08ET deColM[4][4] = { 0xe, 0xb, 0xd, 0x9,
0x9, 0xe, 0xb, 0xd,
0xd, 0x9, 0xe, 0xb,
0xb, 0xd, 0x9, 0xe };

/**
* ���л��
*/
static void deMixColumns(U08ET array[4][4]) {
	U08ET tempArray[4][4], i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			tempArray[i][j] = array[i][j];

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			array[i][j] = GFMul(deColM[i][0], tempArray[0][j]) ^ GFMul(deColM[i][1], tempArray[1][j])
				^ GFMul(deColM[i][2], tempArray[2][j]) ^ GFMul(deColM[i][3], tempArray[3][j]);
		}
}
/**
* ������4X4����������
*/
static void addRoundTowArray(U08ET aArray[4][4], U08ET bArray[4][4]) {
	U08ET i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			aArray[i][j] = aArray[i][j] ^ bArray[i][j];
}
/**
* ��4��32λ����Կ���л��4X4���飬
* ���ڽ������л��
*/
static void getArrayFrom4W(U08ET i, U08ET array[4][4]) {
	U08ET index = i * 4;
	U08ET colOne[4], colTwo[4], colThree[4], colFour[4], j;
	splitIntToArray(w[index], colOne);
	splitIntToArray(w[index + 1], colTwo);
	splitIntToArray(w[index + 2], colThree);
	splitIntToArray(w[index + 3], colFour);

	for (j = 0; j < 4; j++) {
		array[j][0] = colOne[j];
		array[j][1] = colTwo[j];
		array[j][2] = colThree[j];
		array[j][3] = colFour[j];
	}
}

/**
* ���� c: ���ĵ��ַ������顣
* ���� clen: ���ĵĳ��ȡ�
* ���� key: ��Կ���ַ������顣
*/
void deAes(U08ET *c, U08ET clen, U08ET *key) {
	U08ET cArray[4][4], wArray[4][4], i, k;

	extendKey(key);//��չ��Կ

	for (k = 0; k < clen; k += 16) {
		convertToIntArray(c + k, cArray);
		addRoundKey(cArray, 10);
		for (i = 9; i >= 1; i--) {
			deSubBytes(cArray);
			deShiftRows(cArray);
			deMixColumns(cArray);
			getArrayFrom4W(i, wArray);
			deMixColumns(wArray);
			addRoundTowArray(cArray, wArray);
		}
		deSubBytes(cArray);
		deShiftRows(cArray);
		addRoundKey(cArray, 0);
		convertArrayToStr(cArray, c + k);
	}
}


void AES_test(void){

	unsigned char key[] = { 0x2b, 0x7e, 0x15, 0x16,
													0x28, 0xae, 0xd2, 0xa6,
													0xab, 0xf7, 0x15, 0x88,
													0x09, 0xcf, 0x4f, 0x3c};

	unsigned char c[16] = { 0x3a, 0xd7, 0x7b, 0xb4,
							0x0d, 0x7a, 0x36, 0x60,
							0xa8, 0x9e, 0xca, 0xf3,
							0x24, 0x66, 0xef, 0x97, };
	unsigned char		a;
	GPIO_PC_ODR |=  (1<<1);

			/*				0xf5, 0xd3, 0xd5, 0x85,
							0x03, 0xb9, 0x69, 0x9d,
							0xe7, 0x85, 0x89, 0x5a,
							0x96, 0xfd, 0xba, 0xaf,

							0x43, 0xb1, 0xcd, 0x7f,
							0x59, 0x8e, 0xce, 0x23,
							0x88, 0x1b, 0x00, 0xe3,
							0xed, 0x03, 0x06, 0x88,

							0x7b, 0x0c, 0x78, 0x5e,
							0x27, 0xe8, 0xad, 0x3f,
							0x82, 0x23, 0x20, 0x71,
							0x04, 0x72, 0x5d, 0xd4 };*/
	deAes((U08ET *)c, 16, (U08ET *)key);
	GPIO_PC_ODR &= ~(1<<1);
	#ifdef DBG_UART
  for ( a = 0; a < 16; ++a) {
		printf("0x%2x \r\n",(U16ET)c[a]);
  }

  #endif
	
	
	
}