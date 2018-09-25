/*	STRING END OF SPAN SEARCH
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdlib.h>

char *strpbrk(char *s1, char *s2)
	{
	char *s, *t;

	for (s = s1; *s; ++s)
		for (t = s2; *t; ++t)
			if (*t == *s)
				return (s);
	return (NULL);
	}
