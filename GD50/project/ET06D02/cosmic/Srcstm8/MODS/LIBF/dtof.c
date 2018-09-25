/*	CONVERT DOUBLE TO STRING IN F-FORMAT
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <float.h>

int _dtof(char *is, double *pd, char p, char g)
	{
	extern int _norm(char *s, double *, char);
	extern int _round(char *s, char, char);
	char *q, *s, *m;
	int exp;
	char keep;
	double d;
	char buf[FLT_DIG+2];

	s = is;
	if ((d = *pd) < 0)
		{
		d = -d;
		*s++ = '-';
		}
	m = &buf[FLT_DIG];
	exp = _norm(q = buf, &d, FLT_DIG + 1);
	keep = exp + g;
	if (keep > FLT_DIG)
		keep = FLT_DIG;
	exp += _round(q, FLT_DIG + 1, keep);
	if (exp > p)
		{
		q += exp - p;
		exp = p;
		}
	if (exp <= 0)
		*s++ = '0';
	else
		{
		keep = exp;
		do
			*s++ = (q < m) ? *q++ : '0';
		while (--keep);
		}
	keep = g;
	if (keep > 0)
		*s++ = '.';
	for (; exp < 0 && keep > 0; ++exp, --keep)
		*s++ = '0';
	for (; keep > 0; --keep)
		*s++ = (q < m) ? *q++ : '0';
	keep = s - is;
	return (keep);
	}
