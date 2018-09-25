;	TEST FOR DIGIT
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isdigit
	.dcall	"2,0,_isdigit"
;
_isdigit:
	cp	a,#'0'
	jrult	paok
	cp	a,#'9'+1
	jrult	ok
paok:
	clr	a
ok:
	ret
;
	end
