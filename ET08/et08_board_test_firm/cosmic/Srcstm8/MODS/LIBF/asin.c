/*	ARC SINE
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <math.h>

double asin(double x)
	{
	return (atan2(x, sqrt(1.0 - x * x)));
	}
