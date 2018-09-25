;	CONVERT TO UPPER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_toupper
	.dcall	"3,0,f_toupper"
;
f_toupper:
	cp	a,#'a'
	jrult	paok
	cp	a,#'z'
	jrugt	paok
	sub	a,#'a'-'A'
paok:
	retf
;
	end
