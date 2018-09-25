/*	ARC COSINE
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <math.h>

double acos(double x)
	{
	return (atan2(sqrt(1.0 - x * x), x));
	}
