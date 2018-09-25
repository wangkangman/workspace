/*	BOUNDED STRING COPY
 *	Copyright (c) 1995 by COSMIC Software
 */
char *strncpy(char *s1, char *s2, unsigned int n)
	{
	char *s = s1;

	for (s = s1; n; ++s, --n)
		if (*s = *s2)
			++s2;
	return (s1);
	}
