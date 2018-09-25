;	LONG ADDITION FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	c_fladd
	xref.b	c_lreg, c_x
	.dcall	"2,0,c_fladd"
;
c_fladd:
	ldw	c_x+1,x		; store address LSW
	ldw	x,#3		; LSB index
	ldf	a,([c_x.e],x)	; start with LSB
	add	a,c_lreg+3	; add and
	ld	c_lreg+3,a	; store result
	decw	x		; next
	ldf	a,([c_x.e],x)	; byte and
	adc	a,c_lreg+2	; add and
	ld	c_lreg+2,a	; store result
	decw	x		; next
	ldf	a,([c_x.e],x)	; byte
	adc	a,c_lreg+1	; add and
	ld	c_lreg+1,a	; store result
	ldf	a,[c_x.e]	; MSB
	adc	a,c_lreg	; end with MSB
	ld	c_lreg,a	; store result
	ret			; and return
;
	end
