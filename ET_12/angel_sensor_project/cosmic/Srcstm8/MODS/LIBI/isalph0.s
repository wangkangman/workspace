;	TEST FOR ALPHA
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isalpha
	.dcall	"2,0,_isalpha"
;
_isalpha:
	cp	a,#'A'
	jrult	paok
	cp	a,#'Z'+1
	jrult	ok
	cp	a,#'a'
	jrult	paok
	cp	a,#'z'+1
	jrult	ok
paok:
	clr	a
ok:
	ret
;
	end
