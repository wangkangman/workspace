/*	EXPONENTIAL
 *	Copyright (c) 1995 by COSMIC Software
 */
#define NATLOG2		0.693147180559945309
#define FSILLY		8192.0

double _exp(int n, double *px)
	{
	extern double _addexp(int, double);
	double s;

	if (*px <= -FSILLY && FSILLY <= *px)
		return (*px);	/* range error */
	n += _fmod(px, NATLOG2);
	s = *px * *px;
	s = (30240.0 + s * (3360.0 + s * 30.0)) /
		(15120.0 + s * (420.0 + s));
	return (_addexp(n, (s + *px) / (s - *px)));
	}

/*	ANSI exponential
 */
double exp(double x)
	{
	return (_exp(0, &x));
	}
