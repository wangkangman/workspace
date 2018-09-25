;	ISUPPER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isupper
	.dcall	"2,0,_isupper"
;
_isupper:
	cp	a,#'A'
	jrult	paok
	cp	a,#'Z'+1
	jrult	ok
paok:
	clr	a
ok:
	ret
;
	end
