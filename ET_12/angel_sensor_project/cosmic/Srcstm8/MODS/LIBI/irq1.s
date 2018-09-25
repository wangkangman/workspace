;	INTERRUPT LINE TESTING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_irq
	.dcall	"3,0,f_irq"
;
f_irq:
	clr	a		; prepare value
	jril	low		; line low, skip
	inc	a
low:
	retf			; and return
;
	end
