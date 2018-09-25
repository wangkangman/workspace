;	MOVE UNSIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- operand in X:A
;	- result in LONG ACCU
;
	xdef	c_uitol
	xref.b	c_lreg
	.dcall	"2,0,c_uitol"
;
c_uitol:
	ld	c_lreg+3,a	; store LSB
	exg	a,xl
	ld	c_lreg+2,a	; transfer MSB
	exg	a,xl
	clr	c_lreg+1	; complete
	clr	c_lreg		; result
	ret			; and return
;
	end
