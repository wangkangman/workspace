/*	TANGENT 
 *	Copyright (c) 1995 by COSMIC Software
 */
#define PIOVER2	1.570796326794896619

double tan(double x)
	{
	char quad;
	double y;

	quad = _fmod((double *)&x, PIOVER2);
	y = x * x;
	x *= (34459425.0 + y *
		(-4729725.0 + y * (135135.0 + y * (-990.0 + y))))
		/ (34459425.0 + y * (-16216200.0 + y * (945945.0 + y *
		(-13860.0 + y * 45.0))));
	if (!(quad & 01) || x == 0)
		return (x);
	else
		return (-1.0 / x);
	}
