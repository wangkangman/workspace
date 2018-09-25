;	CONVERT TO LOWER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_tolower
	.dcall	"2,0,_tolower"
;
_tolower:
	cp	a,#'A'
	jrult	paok
	cp	a,#'Z'
	jrugt	paok
	add	a,#'a'-'A'
paok:
	ret
;
	end
