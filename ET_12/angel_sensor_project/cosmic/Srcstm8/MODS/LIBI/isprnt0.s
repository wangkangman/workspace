;	TEST FOR PRINTABLE
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isprint
	.dcall	"2,0,_isprint"
;
_isprint:
	cp	a,#' '
	jrult	paok
	cp	a,#$7F
	jrult	ok
paok:
	clr	a
ok:
	ret
;
	end
