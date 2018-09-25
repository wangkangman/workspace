;	ISUPPER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isupper
	.dcall	"3,0,f_isupper"
;
f_isupper:
	cp	a,#'A'
	jrult	paok
	cp	a,#'Z'+1
	jrult	ok
paok:
	clr	a
ok:
	retf
;
	end
