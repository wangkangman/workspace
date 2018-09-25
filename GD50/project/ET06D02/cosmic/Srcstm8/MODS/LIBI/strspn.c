/*	STRING END OF SPAN SEARCH
 *	Copyright (c) 1995 by COSMIC Software
 */
int strspn(char *s1, char *s2)
	{
	char *s, *t;

	for (s = s1; *s; ++s)
		for (t = s2; ; ++t)
			if (!*t)
				return (s - s1);
			else if (*t == *s)
				break;
	return (s - s1);
	}
