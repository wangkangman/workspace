;	INTERRUPT MASK TESTING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_imask
	.dcall	"2,0,_imask"
;
_imask:
	clr	a		; prepare value
	jrnm	low		; mask low, skip
	inc	a
low:
	ret			; and return
;
	end
