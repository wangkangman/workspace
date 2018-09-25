/*	SQUARE ROOT
 *	Copyright (c) 1995 by COSMIC Software
 */
#define SQRT2		1.414213562373095049

/*	corrected Newtonian approximation
 */
double sqrt(double x)
	{
	extern double _addexp(int, double);
	extern int _unpack(double *);
	int n;
	double y;

	if (x <= 0)
		return (x);	/* domain error */
	else
		{
		n = _unpack(&x);
		y = 0.41731 + x * 0.59016;
		y += x / y;
		y = 0.25 * y + x / y;
		y += x / y;
		y = 0.25 * y + x / y;
		if (n & 1)
			y *= SQRT2;
		return (_addexp(n >> 1, y));
		}
	}
