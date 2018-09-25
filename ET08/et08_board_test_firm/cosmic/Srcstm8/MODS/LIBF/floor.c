/*	CALCULATE FLOOR (LARGEST INTEGER NOT GREATER THAN X)
 *	Copyright (c) 1995 by COSMIC Software
 */
double floor(double x)
	{
	double yi;

	yi = x;
	_ftrunc((double *)&yi);
	return (x < yi ? yi - 1.0 : yi);
	}
