/*	EXTRACT FRACTION AND INTEGER FROM DOUBLE
 *	Copyright (c) 1995 by COSMIC Software
 */
double modf(double d, double *pi)
	{
	*pi = d;
	_ftrunc(pi);
	return (d - *pi);
	}
