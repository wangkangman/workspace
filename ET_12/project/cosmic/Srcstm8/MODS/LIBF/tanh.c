/*	HYPERBOLIC TANGENT
 *	Copyright (c) 1995 by COSMIC Software
 */
double tanh(double x)
	{
	extern double _exp(int, double *);
	char isneg;
	double y;

	isneg = 0;
	if (x < 0)
		{
		x = -x;
		++isneg;
		}
	if (x < 0.347)	/* 0.347 ~ 1/2 ln 2 */
		{
		y = x + x;
		y = y * y;	/* y = (2 x) ** 2 */
		x *= 2.0 / (2.0 + y * (55440.0 + y * (756.0 + y))
			/ (332640.0 + y * (10080.0 + y * 42.0)));
		}
	else if (x < 19.4)	/* 19.4 ~ 28 ln 2 for 56 bits */
		{
		y = x + x;
		y = _exp(0, &y);
		x = (y - 1.0) / (y + 1.0);
		}
	else
		x = 1.0;
	return (isneg ? -x : x);
	}
