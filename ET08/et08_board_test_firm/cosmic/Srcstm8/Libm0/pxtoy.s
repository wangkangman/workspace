;	LOAD FAR POINTER FROM FAR MEMORY
;	Copyright (c) 2007 by COSMIC Software
;	- operand pointed by X and extension
;	- result in Y and extension
;
	xdef	c_pxtoy
	xref.b	c_x, c_y
	.dcall	"3,0,c_pxtoy"
;
c_pxtoy:
	push	a		; save register
	ldw	c_x+1,x		; set address LSB
	clrw	x
	ldf	a,([c_x.e],x)	; copy value
	ld	c_y,a
	incw	x		; next byte
	ldf	a,([c_x.e],x)
	ld	yh,a
	incw	x		; next byte
	ldf	a,([c_x.e],x)
	ld	yl,a
	pop	a
	ldw	x,c_x+1
	ret			; and return
;
	end
