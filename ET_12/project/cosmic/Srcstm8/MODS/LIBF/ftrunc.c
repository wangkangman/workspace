/*	TRUNCATE A DOUBLE NUMBER
 *	Copyright (c) 1995 by COSMIC Software
 */
/*	Cheat version. Doesn't work if 2^31 < *pd
 */
int _ftrunc(double *pd)
	{
	long lo;

	lo = *pd;
	*pd = lo;

	return (lo);
	}
