/*	RAISE DOUBLE TO A POWER OF 10
 *	Copyright (c) 1995 by COSMIC Software
 */
double _dtento(int exp, double *pd)
	{
	extern const int _ntens;
	extern const double _dtens[], _huge;
	unsigned int u;
	double *p = &_dtens[1];
	double *pnt = &_dtens[_ntens - 1];
	double d;

	if ((d = *pd) == 0)
		;
	else if (exp < 0)
		{
		for (u = -exp; u && p < pnt; u >>= 1, ++p)
			if (u & 1)
				d /= *p;
		for (; 0 < u && d != 0; --u)
			d /= *pnt;
		}
	else if (0 < exp)
		{
		for (u = exp; u && p < pnt; u >>= 1, ++p)
			if (u & 1)
				d *= *p;
		for (; 0 < u && d != _huge; --u)
			d *= *pnt;
		}
	return (d);
	}
