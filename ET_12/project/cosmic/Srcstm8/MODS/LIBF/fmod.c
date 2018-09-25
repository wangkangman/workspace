/*	DOUBLE MODULUS
 *	Copyright (c) 1995 by COSMIC Software
 */
double fmod(double x, double y)
	{
	double xf;

	if (y < 0)
		y = -y;
	xf = x;
	_fmod((double *)&xf, y);
	if (x < 0 && xf > 0)
		xf -= y;
	else if (x > 0 && xf < 0)
		xf += y;
	return (xf);
	}
