;	CONVERT TO LOWER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_tolower
	.dcall	"3,0,f_tolower"
;
f_tolower:
	cp	a,#'A'
	jrult	paok
	cp	a,#'Z'
	jrugt	paok
	add	a,#'a'-'A'
paok:
	retf
;
	end
