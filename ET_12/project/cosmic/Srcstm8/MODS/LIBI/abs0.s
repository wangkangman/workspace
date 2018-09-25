;	INTEGER ABSOLUTE VALUE
;	Copyright (c) 2001 by COSMIC Software
;
	xdef	_abs
	.dcall	"2,0,_abs"
;
_abs:
	tnzw	x		; test sign
	jrpl	posit		; positive, ok
	negw	x		; negate
posit:
	ret			; and return
;
	end
