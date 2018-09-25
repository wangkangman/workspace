;	LOAD FAR POINTER FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by Y and extension
;	- result in Y and extension
;
	xdef	c_pytoy
	xref.b	c_y
	.dcall	"5,0,c_pytoy"
;
c_pytoy:
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
	ret			; and return
;
	end
