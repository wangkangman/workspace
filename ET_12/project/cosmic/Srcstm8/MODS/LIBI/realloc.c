/*	HEAP MANAGEMENT FUNCTIONS
 *	Copyright (c) 1995 by COSMIC Software
 */
#include <stdlib.h>

#define SIZHDR	((unsigned int)&(((HEAP *)0)->x))
#define SIZPTR	(sizeof (char *))
#define MINELEM	(SIZHDR+SIZPTR)

#define max(a, b)	((a) > (b) ? (a) : (b))

/*	heap link structure
 */
typedef struct _heap
	{
	unsigned int hsize;
	union {
		double d;
		struct _heap *next;
		} x;
	} HEAP;

extern HEAP *_heap;

static HEAP **_above(HEAP *);
static HEAP **_below(HEAP *);
extern HEAP *_breakup(HEAP *, unsigned int);

/*	check for contiguous cell above current cell
 */
static HEAP **_above(HEAP *p)
	{
	HEAP *q;
	HEAP **qb;

	p = (HEAP *)((char *)p + p->hsize + MINELEM);
	for (qb = &_heap; q = *qb; qb = &q->x.next)
		if (q == p)
			return (qb);
	return (NULL);
	}

/*	check for contiguous cell below current cell
 */
static HEAP **_below(HEAP *p)
	{
	HEAP *q;
	HEAP **qb;

	p = (HEAP *)((char *)p - MINELEM);
	for (qb = &_heap; q = *qb; qb = &q->x.next)
		if (((char *)q + q->hsize) == (char *)p)
			return (qb);
	return (NULL);
	}

/*	realloc size bytes using ptr
 */
void *realloc(void *ptr, unsigned int size)
	{
	unsigned int newsize;
	HEAP *old, *q;
	HEAP **qb;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0)
		{
		free(ptr);
		return (NULL);
		}
	old = (HEAP *)((char *)ptr - SIZHDR);
	newsize = ((max(size, SIZPTR) + (SIZHDR-1)) & ~(SIZHDR-1));
	if (newsize + MINELEM <= size)
		return (NULL);
	for (;;)
		if ((newsize + MINELEM - SIZPTR) < old->hsize)
			{
			q = _breakup(old, newsize + SIZHDR);
			free(&q->x.next);
			return (&old->x.next);
			}
		else if (newsize <= (old->hsize + SIZPTR))
			return (&old->x.next);
		else if ((qb = _above(old)) &&
			(newsize <= (old->hsize + (*qb)->hsize + SIZPTR + MINELEM)))
			{
			q = *qb;
			*qb = q->x.next;
			old->hsize += q->hsize + MINELEM;
			}
		else if ((qb = _below(old)) &&
			(newsize <= (old->hsize + (*qb)->hsize + SIZPTR + MINELEM)))
			{
			q = *qb;
			*qb = q->x.next;
			q->hsize += old->hsize + MINELEM;
			memcpy(&q->x.next, &old->x.next,
				old->hsize + SIZPTR);
			old = q;
			}
		else
			{
			if (q = malloc(newsize))
				{
				memcpy(q, &old->x.next, old->hsize + SIZPTR);
				free(&old->x.next);
				}
			return (q);
			}
	}
