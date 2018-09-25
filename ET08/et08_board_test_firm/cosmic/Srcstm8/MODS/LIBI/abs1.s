;	INTEGER ABSOLUTE VALUE
;	Copyright (c) 2001 by COSMIC Software
;
	xdef	f_abs
	.dcall	"3,0,f_abs"
;
f_abs:
	tnzw	x		; test sign
	jrpl	posit		; positive, ok
	negw	x		; negate
posit:
	retf			; and return
;
	end
