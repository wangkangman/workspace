/*	PUT LINE TO OUTPUT
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdio.h>

int puts(char *s)
	{
	while (*s)
		if (putchar(*s++) == EOF)
			return (0);
	return (putchar('\n') != EOF);
	}
