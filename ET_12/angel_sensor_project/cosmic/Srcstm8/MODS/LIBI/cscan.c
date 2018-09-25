/*	FORMATTED SCAN
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FLSIZE	64	/* max significant chars for float input */
#define EOF	-1
#define FPP	0	/* integer version */

#if FPP
/*	states for goodflt
 *	<sign><digits><.><digits><e><sign><digits>
 *	states are SIGN's DIGIT's and DIGIT's but not acceptable yet
 */
#define S1	0x001
#define D1	0x002
#define D1N	0x102
#define D2	0x003
#define D2N	0x103
#define S2	0x004
#define D3	0x005
#define D3N	0x105
#define ACCEPT	0x006
#define REJECT	0x007
#define SSTART	S1

/*	character classes for goodflt
 */
#define CSIGN	0x010
#define CDIGIT	0x020
#define CDOT	0x030
#define	CE	0x040
#define CNONE	0x050
#endif

/*	constant tables used for conversion
 */
static const int base[] = {10, 1, 10, 8, 16, 16, 10};
static const char intfmt[] = {"diuoxXp"};
static const char tdigs[] = {"0123456789abcdefABCDEF"};
static char uncar = 0;		/* ungetted char */
static int nchars = 0;		/* character count */

/*	prototypes for local functions
 */
static int gtc(char **);
static void ungtc(int);

/*	"get" a char from a string or input
 */
static int gtc(char **p)
	{
	int c;

	++nchars;
	if (uncar)
		c = uncar, uncar = 0;
	else if (*p)
		{
		if (c = **p)
			++*p;
		else
			c = EOF;
		}
	else
		c = getchar();
#ifdef DEBUG
	printf("gtc: %c(%x)\n", c, c);
#endif
	return (c);
	}

#if FPP
/*	decide if input is part of a good floating number
 */
static int goodflt(int c, int *ps)
	{
	int ccl;

	if (isdigit(c))
		ccl = CDIGIT;
	else if (c == '.')
		ccl = CDOT;
	else if (c == 'e' || c == 'E')
		ccl = CE;
	else if (c == '-' || c == '+')
		ccl = CSIGN;
	else
		ccl = CNONE;
	switch (ccl | *ps)
		{
	case S1|CSIGN:
		*ps = D1N;
		break;
	case S1|CDIGIT:
		*ps = D1;
		break;
	case S1|CDOT:
		*ps = D2N;
		break;
	case D1N|CDIGIT:
		*ps = D1;
		break;
	case D1N|CDOT:
		*ps = D2N;
		break;
	case D1|CDIGIT:
		break;
	case D1|CDOT:
		*ps = D2;
		break;
	case D1|CE:
		*ps = S2;
		break;
	case D2N|CDIGIT:
		*ps = D2;
		break;
	case D2|CDIGIT:
		break;
	case D2|CE:
		*ps = S2;
		break;
	case S2|CSIGN:
		*ps = D3N;
		break;
	case S2|CDIGIT:
		*ps = D3;
		break;
	case D3N|CDIGIT:
		*ps = D3;
		break;
	case D3|CDIGIT:
		break;
	default:
		if (*ps == ACCEPT || *ps == D1 || *ps == D2 || *ps == D3)
			*ps = ACCEPT;
		else
			*ps = REJECT;
		return (0);
		}
	return (1);
	}
#endif

/*	library scan routine
 */
int _scan(char *pin, char *s, char **parg)
	{
	char notmatch, ok, dontset, neg;
	unsigned int w;
	double ld;	/* long double */
	int state, nitems, i, n, b, c;
	char flbuf[FLSIZE], size, typ, *pb, *p;
	unsigned long l, nl;

	nchars = 0;
	uncar = 0;
	nitems = 0;
	c = '\0';
	while (*s)
		if (*s != '%' || *++s == '%')
			{
#ifdef DEBUG
printf("matching literal \"%s\"\n", s);
#endif
			c = gtc(&pin);
			if (*s != ' ' && *s != '\t' && *s != '\n')
				{
				if (*s++ != c)
					{
					ungtc(c);
					break;
					}
				}
			else if (!isspace(c))
				{
				ungtc(c);
				break;
				}
			else
				{
				do
					c = gtc(&pin);
				while (isspace(c));
				ungtc(c);
				while (*s == ' ' || *s == '\t' || *s == '\n')
					++s;
				}
			}
		else
			{
			if ((dontset = *s) == '*')
				++s;
			else
				dontset = 0;
			for (w = 0; isdigit(*s); ++s)
				w = w * 10 + (char)(*s - '0');
			if (*s == 'l' || *s == 'L' || *s == 'h')
				size = *s++;
			else
				size = '\0';
			typ = *s++;
			if (w == 0)
				w = (typ == 'c') ? 1 : -1;
#ifdef DEBUG
printf("scan: parg: %x, dontset: %i, w: %i, size: %x, typ: %c\n",
	parg, dontset, w, size, typ);
#endif
			if (typ != 'n' && typ != 'c' && typ != '[')
				{
				do
					c = gtc(&pin);
				while (isspace(c));
				ungtc(c);
#ifdef DEBUG
printf("skipped white, last: %x\n", c);
#endif
				}
			ok = 0;
			if (typ == 'n')
				{
				if (!dontset)
					{
					*(int *)(*parg++) = nchars;
					++nitems;
					}
				ok = 1;
				}
			else if (typ == '[')
				{
				if ((notmatch = *s) == '^')
					++s;
				else
					notmatch = 0;
				if (!(p = (char *)strchr(s, ']')))
					break;
				i = p - s;
				while (w && (c = gtc(&pin)) != EOF &&
					(notmatch ?
						memchr(s, c, i) == NULL :
						memchr(s, c, i) != NULL))
					{
					ok = 1;
					if (!dontset)
						*(*(char **)parg)++ = c;
					--w;
					}
				if (w)
					ungtc(c);
				if (!dontset && ok)
					{
					*(*(char **)parg)++ = '\0';
					++parg, ++nitems;
					}
				s = p + 1;
				}
			else if (typ == 'c' || typ == 's')
				{
				for (; w && (c = gtc(&pin)) != EOF &&
					(typ == 'c' || !isspace(c)); --w)
					{
					ok = 1;
					if (!dontset)
						*(*(char **)parg)++ = c;
					}
				if (w)
					ungtc(c);
				if (!dontset && ok)
					{
					if (typ != 'c')
						*(*(char **)parg)++ = '\0';
					++parg, ++nitems;
					}
#ifdef DEBUG
printf("c/s format: w: %i, ok: %i, last: %c\n", w, ok, c);
#endif
				}
			else if (p = strchr(intfmt, typ))
				{
				b = base[p - intfmt];
				if ((c = gtc(&pin)) == '+' || c == '-')
					{
					--w;
					neg = (c == '-');
					}
				else
					{
					ungtc(c);
					neg = 0;
					}
				if (w == 0)
					break;
				else if (typ == 'i')
					if ((c = gtc(&pin)) != '0' || w <= 1)
						{
						b = 10;
						ungtc(c);
						}
					else if (((c = gtc(&pin)) != 'x' && c != 'X') || w <= 2)
						{
						b = 8;
						--w;
						ungtc(c);
						ok = 1;
						}
					else
						{
						b = 16;
						w -= 2;
						}
				else if (typ != 'p' || w <= 2)
					;
				else if ((c = gtc(&pin)) != '0')
					ungtc(c);
				else if ((c = gtc(&pin)) == 'x' || c == 'X')
					w -= 2;
				else
					{
					--w;
					ungtc(c);
					ok = 1;
					}
				n = (b < 16) ? b : b + 6;
#ifdef DEBUG
printf("%c format: neg: %i, b: %i, n: %i, w: %i, lastc: %x\n", typ, neg,
	b, n, w, c);
#endif
				for (l = 0; w && (c = gtc(&pin)) != EOF &&
					(p = memchr(tdigs, c, n)); --w)
					{
					ok = 1;
#ifdef DEBUG
printf("    c: %x, i: %i, l: %i, w: %i\n", c, i, l, w);
#endif
					nl = l * b;
					if (nl / b != l)
						w = 0;
					i = p - tdigs;
					l = nl + (i < b ? i : (i - 6));
					if (l < nl)
						w = 0;
					if (!w)
						{
						l = 0; /* modified */
						break;
						}
					}
				if (w)
					ungtc(c);
#ifdef DEBUG
printf("==> %i (0x%x, 0%o)\n", l, l, l);
#endif
				++nitems;
				if (dontset || !ok)
					--nitems;
				else if ((sizeof (int) == sizeof (long)) ?
					size != 'h' : size == 'l')
					**((long **)parg++) = neg ? -l : l;
				else if (l & 0xffff0000)
					**((int **)parg++) = 0; /* modified */
				else
					**((int **)parg++) = neg ? -l : l;
				}
#if FPP
			else if (strchr("eEfgG", typ))
				{
				if ((int)w == -1)
					w = FLSIZE;
				else if (FLSIZE < w)
					break;
#ifdef DEBUG
printf("%c format: w: %i, first loop count: %i\n", typ, w, i);
#endif
				pb = flbuf;
				state = SSTART;
				for (; w && (c = gtc(&pin)) != EOF && goodflt(c, &state); --w)
					*pb++ = c;
				if (w)
					ungtc(c);
				goodflt(EOF, &state);
				ok = (state == ACCEPT);
#ifdef DEBUG
printf("	ok: %i, state: %x, last: %x\n", ok, state, c);
#endif
				if (!dontset && ok)
					{
					++nitems;
					_btod(flbuf, pb - flbuf, &ld);
					if (size == 'l' || size == 'L')	/* long double == double */
						*(*(double **)parg++) = ld;
					else
						*(*(float **)parg++) = ld;
					}
				}
#endif
			else
				break;
			if (!ok)
				break;
			}
	return (nitems ? nitems : c == EOF ? EOF : 0);
	}

/*	"unget" a char to a string or FILE *
 */
static void ungtc(int c)
	{
#ifdef DEBUG
	printf("unget %c(%x)\n", c, c);
#endif
	if (c == EOF)
		return;
	--nchars;
	uncar = c;
	}
