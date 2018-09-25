;	TEST FOR HEXADECIMAL
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isxdigit
	.dcall	"3,0,f_isxdigit"
;
f_isxdigit:
	cp	a,#'0'
	jrult	paok
	cp	a,#'9'+1
	jrult	ok
	cp	a,#'A'
	jrult	paok
	cp	a,#'F'+1
	jrult	ok
	cp	a,#'a'
	jrult	paok
	cp	a,#'f'+1
	jrult	ok
paok:
	clr	a
ok:
	retf
;
	end
