/*	BUFFER COMPARE
 *	Copyright (c) 1998 by COSMIC Software
 */
int memcmp(unsigned char *s1, unsigned char *s2, unsigned int n)
	{
	for ( ; n; ++s1, ++s2, --n)
		if (*s1 != *s2)
			return (*s1 - *s2);
	return (0);
	}
