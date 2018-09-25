;	FAR POINTER ADDITION IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	d_fpgadd
	xref.b	c_lreg, c_x
	.dcall	"3,0,d_fpgadd"
;
d_fpgadd:
	ldw	c_x+1,x		; store address LSW
	ldw	x,#2		; LSB index
	ldf	a,([c_x.e],x)	; start with LSB
	add	a,c_lreg+3	; add and
	ldf	([c_x.e],x),a	; store result
	decw	x		; next
	ldf	a,([c_x.e],x)	; byte and
	adc	a,c_lreg+2	; add and
	ldf	([c_x.e],x),a	; store result
	ldf	a,[c_x.e]	; byte
	adc	a,c_lreg+1	; add and
	ldf	[c_x.e],a	; store result
	retf			; and return
;
	end
