/*	BOUNDED STRING COMPARE
 *	Copyright (c) 1995 by COSMIC Software
 */
int strncmp(char *s1, char *s2, unsigned int n)
	{
	while (n && *s1 && *s1 == *s2)
		++s1, ++s2, --n;
	return (n ? *s1 - *s2 : 0);
	}
