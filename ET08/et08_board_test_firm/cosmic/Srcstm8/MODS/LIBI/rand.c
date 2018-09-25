/*	RANDOM NUMBER GENERATOR
 *	Copyright (c) 1995 by COSMIC Software
 */
static long rnum = 1;

/*	starting seed value
 */
int rand(void)
	{
	rnum = rnum * 1103515245 + 12345;
	return ((int)(rnum >> 16) & 0x7fff);
	}

void srand(int seed)
	{
	rnum = seed;
	}
