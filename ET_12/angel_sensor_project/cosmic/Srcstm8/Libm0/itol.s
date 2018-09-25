;	MOVE SIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- integer in X:A
;	- result in LONG ACCU
;
	xdef	c_itol
	xref.b	c_lreg
	.dcall	"2,0,c_itol"
;
c_itol:
	ld	c_lreg+3,a	; store LSB
	ld	a,xl
	clrw	x
	ld	c_lreg+2,a	; transfer MSB
	jrpl	posit		; positive, clear
	decw	x		; otherwise sign extend
posit:
	ldw	c_lreg,x	; complete
	ret			; and return
;
	end
