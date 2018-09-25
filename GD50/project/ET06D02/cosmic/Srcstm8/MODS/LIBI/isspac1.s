;	TEST FOR SPACE
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isspace
	.dcall	"3,0,f_isspace"
;
f_isspace:
	cp	a,#9	; TAB
	jrult	paok
	cp	a,#13	; CR
	jrule	ok
	cp	a,#' '
	jrne	paok
ok:
	inc	a
	retf
paok:
	clr	a
	retf
;
	end
