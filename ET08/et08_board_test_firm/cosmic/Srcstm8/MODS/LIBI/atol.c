/*	CONVERT BUFFER TO LONG
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <ctype.h>

long atol(char *nptr)
	{
	long num;
	char sig;

	while (isspace(*nptr))
		++nptr;
	sig = *nptr;
	if (sig == '+' || sig == '-')
		++nptr;
	for (num = 0; isdigit(*nptr); ++nptr)
		num = num * 10 + (char)(*nptr - '0');
	if (*nptr == 'l' || *nptr == 'L')
		++nptr;
	return ((sig == '-') ? -num : num);
	}
