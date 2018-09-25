;	GET A WORD FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- result in Y
;
	xdef	d_getyfy
	xref.b	c_y
	.dcall	"4,0,d_getyfy"
;
d_getyfy:
	push	a		; save register
	ldw	c_y+1,y		; store address LSB
	ldw	y,#1
	ldf	a,([c_y.e],y)	; get LSB
	ld	yl,a		; in place
	ldf	a,[c_y.e]	; get MSB
	ld	yh,a		; in place
	pop	a		; registers
	retf			; and return
;
	end
