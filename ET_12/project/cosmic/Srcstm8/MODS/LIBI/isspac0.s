;	TEST FOR SPACE
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isspace
	.dcall	"2,0,_isspace"
;
_isspace:
	cp	a,#9	; TAB
	jrult	paok
	cp	a,#13	; CR
	jrule	ok
	cp	a,#' '
	jrne	paok
ok:
	inc	a
	ret
paok:
	clr	a
	ret
;
	end
