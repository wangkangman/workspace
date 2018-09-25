/*	INTEGER DIVIDE WITH STRUCTURE RETURN
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdlib.h>

div_t div(int numer, int denom)
	{
	div_t res;

	res.quot = numer / denom;
	res.rem = numer % denom;
	return (res);
	}

