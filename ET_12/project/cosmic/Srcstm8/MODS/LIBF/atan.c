/*	ARC TANGENT OF X AND Y
 *	Copyright (c) 1995 by COSMIC Software
 */
#define PI		3.141592653589793238
#define PIOVER2		1.570796326794896619

/*	1/8 intervals of atan(x)
 */
static const double atantab[] = {
	0.0,
	0.124354994546761435,
	0.244978663126864154,
	0.358770670270572220,
	0.463647609000806116,
	0.558599315343562436,
	0.643501108793284387,
	0.718829999621624505,
	0.785398163397448310
	};

/*	weighted atan coefficients
 */
static const double coeftab[] = {
	-0.089932528409257986,
	 0.111106342739529896,
	-0.142857131681271964,
	 0.199999999987267074,
	-0.333333333333327116,
	 0.999999999999999999
	};

/*	interpolate and correct error
 */
double atan2(double y, double x)
	{
	extern double _poly(double *, char, double);
	unsigned int i;
	char quad;
	double z;

	if (0 <= y)
		quad = 00;
	else
		{
		y = -y;
		quad = 04;
		}
	if (x < 0)
		{
		x = -x;
		quad ^= 03;
		}
	if (x < y)
		{
		z = x, x = y, y = z;
		quad ^= 01;
		}
	if (x == 0)
		z = PIOVER2;
	else if (y == 0)
		z = 0;
	else
		{
		x = y / x;
		i = 16.0 * x;
		i = (i + 1) >> 1;
		z = i * 0.125;
		x = (x - z) / (1.0 + x * z);
		z = atantab[i] + x * _poly(coeftab, 5, x * x);
		}
	switch (quad & 03)
		{
	case 1:
		z = PIOVER2 - z;
		break;
	case 2:
		z = PIOVER2 + z;
		break;
	case 3:
		z = PI - z;
		}
	return (quad & 04 ? -z : z);
	}

/*	atan
 */
double atan(double x)
	{
	return (atan2(x, 1.0));
	}
