;	ISLOWER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_islower
	.dcall	"2,0,_islower"
;
_islower:
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
