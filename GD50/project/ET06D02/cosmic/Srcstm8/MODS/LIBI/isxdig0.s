;	TEST FOR HEXADECIMAL
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isxdigit
	.dcall	"2,0,_isxdigit"
;
_isxdigit:
	cp	a,#'0'
	jrult	paok
	cp	a,#'9'+1
	jrult	ok
	cp	a,#'A'
	jrult	paok
	cp	a,#'F'+1
	jrult	ok
	cp	a,#'a'
	jrult	paok
	cp	a,#'f'+1
	jrult	ok
paok:
	clr	a
ok:
	ret
;
	end
