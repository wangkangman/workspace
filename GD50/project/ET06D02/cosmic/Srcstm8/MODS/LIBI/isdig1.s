;	TEST FOR DIGIT
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isdigit
	.dcall	"3,0,f_isdigit"
;
f_isdigit:
	cp	a,#'0'
	jrult	paok
	cp	a,#'9'+1
	jrult	ok
paok:
	clr	a
ok:
	retf
;
	end
