;	LOAD LONG ACCUMULATOR FROM FAR MEMORY
;	Copyright (c) 2001 by COSMIC Software
;	- operand pointed by X and extension
;
	xdef	c_fltor, c_fltor2
	xref.b	c_lreg, c_x
	.dcall	"2,0,c_fltor"
;
c_fltor:
	ldw	c_x+1,x		; set address LSB
c_fltor2:
	clrw	x
	ldf	a,([c_x.e],x)	; copy value
	ld	c_lreg,a
	incw	x		; next byte
	ldf	a,([c_x.e],x)
	ld	c_lreg+1,a
	incw	x		; next byte
	ldf	a,([c_x.e],x)
	ld	c_lreg+2,a
	incw	x		; next byte
	ldf	a,([c_x.e],x)
	ld	c_lreg+3,a
	ldw	x,c_x+1		; restore address
	ret			; and return
;
	end
