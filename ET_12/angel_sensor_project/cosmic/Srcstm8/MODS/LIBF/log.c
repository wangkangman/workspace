/*	LOG BASE 2 AND 10
 *	Copyright (c) 1995 by COSMIC Software
 */
#define HAFRAD2		0.707106781186547524
#define LOG2E		1.442695040888963407
#define NATLOG2		0.693147180559945309
#define LOG2		0.301029995663981195

#define	A0		-0.64124943423745581147E+2
#define A1		 0.16383943563021534222E+2
#define A2		-0.78956112887491257267E+0
#define B0		-0.76949932108494879777E+3
#define B1		 0.31203222091924532844E+3
#define B2		-0.35667977739034646171E+2

/*	evaluate log
 */
double _ln(double x, double scale)
	{
	extern const double _huge;
	extern int _unpack(double *);
	int n;
	double w, zn, zd;
	static const double half = 0.5;

	if (x <= 0)
		return (-_huge);	/* domain error */
	else
		{
		n = _unpack(&x);
		zn = x - half;
		if (HAFRAD2 <= x)
			{
			zn -= half;
			zd = x * half + half;
			}
		else
			{
			zd = zn * half + half;
			--n;
			}
		x = zn / zd;
		w = x * x;
		x += x * w * (A0 + w * (A1 + w * A2)) /
			(B0 + w * (B1 + w * (B2 + w)));
		x *= LOG2E * scale;
		if (n)
			x += (double)n * scale;
		return (x);
		}
	}

/*	natural log
 */
double log(double x)
	{
	return (_ln(x, NATLOG2));
	}

/*	common log
 */
double log10(double x)
	{
	return (_ln(x, LOG2));
	}
