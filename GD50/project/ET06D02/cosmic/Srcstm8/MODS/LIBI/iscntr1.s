;	TEST FOR CONTROL
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_iscntrl
	.dcall	"3,0,f_iscntrl"
;
f_iscntrl:
	cp	a,#' '
	jrult	ok
	cp	a,#$7F
	jrne	paok
ok:
	inc	a
	retf
paok:
	clr	a
	retf
;
	end
