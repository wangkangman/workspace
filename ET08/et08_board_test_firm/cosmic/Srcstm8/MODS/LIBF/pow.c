/*	COMPUTE X TO THE Y POWER
 *	Copyright (c) 1995 by COSMIC Software
 */
#define NATLOG2		0.693147180559945309

double pow(double x, double y)
	{
	extern double _exp(int, double *), _ln(double, double);
	extern int _ftrunc(double *);
	char isneg, isrecip;
	int iy;
	double ans, yi;

	isrecip = 0;
	if (y <= 0)
		{
		y = -y;
		++isrecip;
		}
	yi = y;
	iy = _ftrunc(&yi);
	if (x == 0 || (x < 0 && yi != y))
		return (0);	/* domain error */
	if (0 < x)
		isneg = 0;
	else
		{
		x = -x;
		isneg = iy & 1;	/* negate odd powers only */
		}
	if (yi == y && y < 32768.0)
		{
		if (isrecip)
			x = 1.0 / x;
		for (ans = 1.0; ; )		/* y is an integer */
			{
			if (iy & 1)
				ans *= x;	/* CAN OVERFLOW */
			iy >>= 1;
			if (iy == 0)
				break;
			x *= x;			/* CAN OVERFLOW */
			}
		}
	else
		{
		y *= _ln(x, NATLOG2);	/* CAN OVERFLOW */
		if (isrecip)
			y = -y;
		ans = _exp(0, &y);
		}
	return (isneg ? -ans : ans);
	}
