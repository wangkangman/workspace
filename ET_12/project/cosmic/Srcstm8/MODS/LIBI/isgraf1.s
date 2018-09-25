;	TEST FOR GRAPHIC
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isgraph
	.dcall	"3,0,f_isgraph"
;
f_isgraph:
	cp	a,#' '
	jrule	paok
	cp	a,#$7F
	jrult	ok
paok:
	clr	a
ok:
	retf
;
	end
