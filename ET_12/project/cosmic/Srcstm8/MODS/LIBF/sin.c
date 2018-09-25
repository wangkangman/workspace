/*	SINE AND COSINE
 *	Copyright (c) 1995 by COSMIC Software
 */
#define PIOVER2	1.570796326794896619

/*	cos coefficients
 */
static const double costab[] = {
	 0.000000002063047029,
	-0.000000275552341524,
	 0.000024801578540308,
	-0.001388888886998399,
	 0.041666666666472379,
	-0.499999999999992511,
	 0.999999999999999953
	};

/*	sin coefficients
 */
static const double sintab[] = {
	 0.000000000158821503,
	-0.000000025050471633,
	 0.000002755731151152,
	-0.000198412698218075,
	 0.008333333333308121,
	-0.166666666666665226,
	 0.999999999999999976
	};

/*	find folded taylor expansion
 */
static double _sin(char quad, double *px)
	{
	extern double _poly(double *, char, double);
	double y;

	quad += _fmod(px, PIOVER2);
	y = *px * *px;
	if (quad & 01)
		y = _poly(costab, 6, y);
	else
		y = *px * _poly(sintab, 6, y);
	return ((quad & 02) ? -y : y);
	}

/*	sinus
 */
double sin(double x)
	{
	return (_sin(0, &x));
	}

/*	cosinus
 */
double cos(double x)
	{
	return (_sin(1, &x));
	}
