/*	CONVERT DOUBLE TO STRING IN E-FORMAT
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <float.h>

int _dtoe(char *is, double *pd, char p, char g)
	{
	extern int _norm(char *, double *, char);
	extern int _round(char *, char, char);
	char i, j, rnd;
	char *s;
	int exp;
	union {
		double d;
		struct ix {
			unsigned int n;
			unsigned char dv;
			} v;
		} u;
	char buf[FLT_DIG+2];
	extern int _udiv(struct ix *);

	s = is;
	u.d = *pd;
	if (u.d < 0)
		{
		u.d = -u.d;
		*s++ = '-';
		}
	rnd = p + g;
	if (rnd > FLT_DIG)
		rnd = FLT_DIG;
	exp = _norm(buf, &u.d, rnd + 1);
	exp += _round(buf, rnd + 1, rnd);
	for (i = 0; i < rnd && i < p; ++i, --exp)
		*s++ = buf[i];
	for (; i < p; ++i, --exp)
		*s++ = '0';
	if (0 < g)
		{
		*s++ = '.';
		for (j = 0; j < g && i < rnd; ++i, ++j)
			*s++ = buf[i];
		for (; j < g; ++j)
			*s++ = '0';
		}
	*s++ = 'e';
	if (!u.d)
		exp = 0;
	if (exp < 0)
		{
		exp = -exp;
		*s++ = '-';
		}
	else
		*s++ = '+';
	u.v.n = exp;
	u.v.dv = 10;
	i = _udiv(&u.v);
	if (exp >= 100)
		{
		j = _udiv(&u.v);
		*s++ = u.v.n + '0';
		}
	else
		j = u.v.n;
	*s++ = j + '0';
	*s++ = i + '0';
	i = s - is;
	return (i);
	}
