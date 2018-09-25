/*	CALCULATE CEILING (SMALLEST INTEGER NOT LESS THAN X)
 *	Copyright (c) 1995 by COSMIC Software
 */
double ceil(double x)
	{
	double yi;

	yi = x;
	_ftrunc((double *)&yi);
	return (yi < x ? yi + 1 : yi);
	}
