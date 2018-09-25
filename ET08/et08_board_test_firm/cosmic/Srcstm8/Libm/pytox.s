;	LOAD FAR POINTER FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by Y and extension
;	- result in X and extension
;
	xdef	d_pytox
	xref.b	c_x, c_y
	.dcall	"4,0,d_pytox"
;
d_pytox:
	push	a		; save register
	ldw	c_y+2,y		; set address LSB
	clrw	y
	ldf	a,([c_y.e],y)	; copy value
	ld	c_x,a
	incw	y		; next byte
	ldf	a,([c_y.e],y)
	ld	xh,a
	incw	y		; next byte
	ldf	a,([c_y.e],y)
	ld	xl,a
	pop	a
	retf			; and return
;
	end
