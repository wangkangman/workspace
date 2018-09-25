;	INTERRUPT LINE TESTING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_irq
	.dcall	"2,0,_irq"
;
_irq:
	clr	a		; prepare value
	jril	low		; line low, skip
	inc	a
low:
	ret			; and return
;
	end
