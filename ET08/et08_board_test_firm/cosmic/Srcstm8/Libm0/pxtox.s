;	LOAD FAR POINTER FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X and extension
;	- result in X and extension
;
	xdef	c_pxtox
	xref.b	c_x
	.dcall	"5,0,c_pxtox"
;
c_pxtox:
	push	a		; save register
	ldw	c_x+1,x		; set address LSB
	ldw	x,#2
	ldf	a,([c_x.e],x)	; copy value
	push	a
	decw	x		; next byte
	ldf	a,([c_x.e],x)
	push	a
	ldf	a,[c_x.e]
	ld	c_x,a
	popw	x
	pop	a
	ret			; and return
;
	end
