/*	BUFFER COPY WITH OVERLAY
 *	Copyright (c) 1998 by COSMIC Software
 */
unsigned char *memmove(unsigned char *s1, unsigned char *s2, unsigned int n)
	{
	unsigned char *s = s1;

	if (s1 >= s2)
		for (s += n, s2 += n; n; --n)
			*--s = *--s2;
	else
		for ( ; n; --n)
			*s++ = *s2++;
	return (s1);
	}
