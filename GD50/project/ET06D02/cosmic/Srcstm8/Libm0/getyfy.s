;	GET A WORD FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- result in Y
;
	xdef	c_getyfy
	xref.b	c_y
	.dcall	"3,0,c_getyfy"
;
c_getyfy:
	push	a		; save register
	ldw	c_y+1,y		; store address LSB
	ldw	y,#1
	ldf	a,([c_y.e],y)	; get LSB
	ld	yl,a		; in place
	ldf	a,[c_y.e]	; get MSB
	ld	yh,a		; in place
	pop	a		; registers
	ret			; and return
;
	end
