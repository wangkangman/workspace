;	TEST FOR GRAPHIC
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isgraph
	.dcall	"2,0,_isgraph"
;
_isgraph:
	cp	a,#' '
	jrule	paok
	cp	a,#$7F
	jrult	ok
paok:
	clr	a
ok:
	ret
;
	end
