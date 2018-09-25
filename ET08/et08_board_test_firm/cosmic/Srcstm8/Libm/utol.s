;	MOVE UNSIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- operand in X:A
;	- result in LONG ACCU
;
	xdef	d_uitol
	xref.b	c_lreg
	.dcall	"3,0,d_uitol"
;
d_uitol:
	ld	c_lreg+3,a	; store LSB
	exg	a,xl
	ld	c_lreg+2,a	; transfer MSB
	exg	a,xl
	clr	c_lreg+1	; complete
	clr	c_lreg		; result
	retf			; and return
;
	end
