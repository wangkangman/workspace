/*	HYPERBOLIC COSINE
 *	Copyright (c) 1995 by COSMIC Software
 */
double cosh(double x)
	{
	extern double _exp(int, double *);
	double y;

	if (x < 0)
		x = -x;
	y = x;
	y = _exp(-1, &y);	/* y = 1/2 exp(x) */
	if (x < 19.4)	/* 19.4 ~ 28 ln 2 for 56 bits */
		return (y + 0.25 / y);
	else
		return (y);
	}
