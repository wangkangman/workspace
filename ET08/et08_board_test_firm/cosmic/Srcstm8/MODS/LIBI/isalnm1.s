;	TEST FOR ALPHANUMERIC
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isalnum
	.dcall	"3,0,f_isalnum"
;
f_isalnum:
	cp	a,#'0'
	jrult	paok
	cp	a,#'9'+1
	jrult	ok
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
