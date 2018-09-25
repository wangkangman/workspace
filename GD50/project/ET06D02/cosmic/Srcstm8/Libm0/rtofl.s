;	STORE LONG ACCUMULATOR IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X and pointer extension
;
	xdef	c_rtofl, c_rtofl2
	xref.b	c_lreg, c_x
	.dcall	"3,0,c_rtofl"
;
c_rtofl:
	ldw	c_x+1,x		; set address LSB
c_rtofl2:
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
	ret			; and return
;
	end
