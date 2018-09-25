;	ISLOWER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_islower
	.dcall	"3,0,f_islower"
;
f_islower:
	cp	a,#'a'
	jrult	paok
	cp	a,#'z'+1
	jrult	ok
paok:
	clr	a
ok:
	retf
;
	end
