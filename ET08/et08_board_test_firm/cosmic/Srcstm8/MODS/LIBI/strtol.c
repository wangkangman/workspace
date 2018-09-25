/*	STRING TO LONG CONVERSION
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <ctype.h>

long strtol(char *s, char **pe, int base)
	{
	char *p = s;
	long l = 0;
	char signe, n;

	while (*p == ' ' || *p == '\t')
		++p;
	signe = *p;
	if (signe == '+' || signe == '-')
		++p;
	if (base == 0)
		{
		if (*p == '0')
			{
			++p;
			if (*p == 'x' || *p == 'X')
				{
				++p;
				base = 16;
				}
			else
				base = 8;
			}
		else
			base = 10;
		}
	for (;;)
		{
		n = *p;
		if (isdigit(*p))
			n -= '0';
		else if (islower(*p))
			n -= 'a' - 10;
		else if (isupper(*p))
			n -= 'A' - 10;
		else
			break;
		if (n >= base)
			break;
		l = l * base + n;
		++p;
		}
	if (pe)
		*pe = p;
	return ((signe == '-') ? -l : l);
	}

