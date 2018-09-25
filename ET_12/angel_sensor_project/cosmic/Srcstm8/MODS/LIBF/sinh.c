/*	HYPERBOLIC SINE
 *	Copyright (c) 1995 by COSMIC Software
 */
static const double sinhtab[] = {
	0.000000025084676126,
	0.000002755729445622,
	0.000198412698499514,
	0.008333333333333332,
	0.166666666666666675,
	1.000000000000000000,
	};

double sinh(double x)
	{
	extern double _exp(int, double *);
	extern double _poly(double *, char, double);
	char isneg;
	double y;

	isneg = 0;
	if (x < 0)
		{
		x = -x;
		++isneg;
		}
	if (x < 0.26)
		x *= _poly(sinhtab, 5, x * x);
	else
		{
		y = x;
		y = _exp(-1, &y);	/* y = 1/2 exp(|x|) */
		if (x < 19.4)	/* 19.4 ~ 28 ln 2 for 56 bits */
			x = (y - 0.25 / y);
		else
			x = y;
		}
	return (isneg ? -x : x);
	}
