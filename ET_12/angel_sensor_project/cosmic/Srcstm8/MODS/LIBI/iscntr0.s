;	TEST FOR CONTROL
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_iscntrl
	.dcall	"2,0,_iscntrl"
;
_iscntrl:
	cp	a,#' '
	jrult	ok
	cp	a,#$7F
	jrne	paok
ok:
	inc	a
	ret
paok:
	clr	a
	ret
;
	end
