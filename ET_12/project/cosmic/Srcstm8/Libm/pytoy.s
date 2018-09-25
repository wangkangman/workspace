;	LOAD FAR POINTER FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by Y and extension
;	- result in Y and extension
;
	xdef	d_pytoy
	xref.b	c_y
	.dcall	"6,0,d_pytoy"
;
d_pytoy:
	push	a		; save register
	ldw	c_y+1,y		; set address LSB
	ldw	y,#2
	ldf	a,([c_y.e],y)	; copy value
	push	a
	decw	y		; next byte
	ldf	a,([c_y.e],y)
	push	a
	ldf	a,[c_y.e]
	ld	c_y,a
	popw	y
	pop	a
	retf			; and return
;
	end
