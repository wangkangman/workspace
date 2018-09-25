/*	GET CHARACTER FROM INPUT
 *	Copyright (c) 1998 by COSMIC Software
 */

/*	read a character with echo
 */
int getchar(void)
	{
	char c;

/* get c from hardware here */

	if (c == '\r')
		c = '\n';
	return (putchar(c));
	}
