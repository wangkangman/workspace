/*	GET LINE FROM INPUT
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdio.h>

char *gets(is)
	char *is;
	{
	char *s;
	int c;

	s = is;
	for (;;)
		{
		if ((c = getchar()) == EOF)
			return (NULL);
		if (c == '\n')
			break;
		else
			*s++ = c;
		}
	if (is == s)
		return (NULL);
	*s = '\0';
	return (is);
	}
