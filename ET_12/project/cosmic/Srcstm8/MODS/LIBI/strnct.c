/*	BOUNDED STRING CATENATE
 *	Copyright (c) 1995 by COSMIC Software
 */
char *strncat(char *s1, char *s2, unsigned int n)
	{
	char *s = s1;

	while (*s)
		++s;
	while (n && *s2)
		{
		*s++ = *s2++;
		--n;
		}
	*s = '\0';
	return (s1);
	}
