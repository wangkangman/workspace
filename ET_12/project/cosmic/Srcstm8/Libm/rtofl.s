;	STORE LONG ACCUMULATOR IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X and pointer extension
;
	xdef	d_rtofl, d_rtofl2
	xref.b	c_lreg, c_x
	.dcall	"4,0,d_rtofl"
;
d_rtofl:
	ldw	c_x+1,x		; set address LSB
d_rtofl2:
	push	a		; save register
	clrw	x
	ld	a,c_lreg	; copy value
	ldf	([c_x.e],x),a
	incw	x		; next byte
	ld	a,c_lreg+1
	ldf	([c_x.e],x),a
	incw	x		; next byte
	ld	a,c_lreg+2
	ldf	([c_x.e],x),a
	incw	x		; next byte
	ld	a,c_lreg+3
	ldf	([c_x.e],x),a
	pop	a		; restore registers
	ldw	x,c_x+1
	retf			; and return
;
	end
