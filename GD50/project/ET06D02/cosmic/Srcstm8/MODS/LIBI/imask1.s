;	INTERRUPT MASK TESTING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_imask
	.dcall	"3,0,f_imask"
;
f_imask:
	clr	a		; prepare value
	jrnm	low		; mask low, skip
	inc	a
low:
	retf			; and return
;
	end
