/*	EXTRACT FRACTION FROM EXPONENT PART
 *	Copyright (c) 1995 by COSMIC Software
 */
double frexp(double d, int *pi)
	{
	extern int _unpack(double *);
	*pi = _unpack(&d);
	return (d);
	}
