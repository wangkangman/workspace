;	TEST FOR PUNCTUATION
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_ispunct
	.dcall	"3,0,f_ispunct"
;
f_ispunct:
	cp	a,#' '
	jrule	paok
	cp	a,#'0'
	jrult	ok
	cp	a,#'9'
	jrule	paok
	cp	a,#'A'
	jrult	ok
	cp	a,#'Z'
	jrule	paok
	cp	a,#'a'
	jrult	ok
	cp	a,#'z'
	jrule	paok
	cp	a,#$7F
	jrult	ok
paok:
	clr	a
ok:
	retf
;
	end
