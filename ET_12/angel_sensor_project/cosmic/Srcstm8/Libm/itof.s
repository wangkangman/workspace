;	INTEGER TO FLOAT CONVERSION
;	Copyright (c) 2006 by COSMIC Software
;	- integer in X
;	- result in FLOAT ACCU
;
	xdef	d_itof
	xref.b	c_lreg
	.dcall	"3,0,d_itof"
;
d_itof:
	clr	c_lreg+3	; prepare result
	ldw	c_lreg+1,x	; store value
	jrne	ok		; not nul, continue
	clr	c_lreg		; complete result
	retf			; and return
ok:
	mov	c_lreg,c_lreg+1	; save MSB
	jrpl	posit		; positive, ok
	negw	x		; else invert value
	ldw	c_lreg+1,x	; store value
posit:
	ld	a,#127+15	; start exponent
	tnzw	x		; test MSB
	jrmi	panor		; normalize ok
	tnz	c_lreg+1	; check MSB
	jrne	bcl		; not nul, continue
	ld	a,#127+7	; start exponent
	ldw	x,c_lreg+2	; load value
	jrmi	panor		; normalize ok
bcl:
	dec	a		; decrement exponent
	sllw	x		; shift value
	jrpl	bcl		; and loop again
panor:
	sllw	x		; remove hidden bit
	sll	c_lreg		; extract sign
	rrc	a		; exponent ok
	rrcw	x		; mantissa ok
	ldw	c_lreg+1,x	; store
	ld	c_lreg,a	; result
	retf			; and return
;
	end
