;	TEST FOR PRINTABLE
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isprint
	.dcall	"3,0,f_isprint"
;
f_isprint:
	cp	a,#' '
	jrult	paok
	cp	a,#$7F
	jrult	ok
paok:
	clr	a
ok:
	retf
;
	end
