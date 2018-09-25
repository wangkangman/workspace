/*	SUBSTRING SEARCH
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdlib.h>

char *strstr(char *s1, char *s2)
	{
	char *s, *t;

	for ( ; *s1; ++s1)
		for (s = s1, t = s2; ; ++s, ++t)
			if (!*t)
				return (s1);
			else if (*s != *t)
				break;
	return (NULL);
	}
