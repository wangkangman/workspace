;	CONVERT TO UPPER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_toupper
	.dcall	"2,0,_toupper"
;
_toupper:
	cp	a,#'a'
	jrult	paok
	cp	a,#'z'
	jrugt	paok
	sub	a,#'a'-'A'
paok:
	ret
;
	end
