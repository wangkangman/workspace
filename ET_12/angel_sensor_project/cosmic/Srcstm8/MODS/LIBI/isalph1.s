;	TEST FOR ALPHA
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isalpha
	.dcall	"3,0,f_isalpha"
;
f_isalpha:
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
	retf
;
	end
