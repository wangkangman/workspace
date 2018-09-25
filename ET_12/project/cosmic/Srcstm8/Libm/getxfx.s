;	GET A WORD FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in X and extension
;	- result in X and extension
;
	xdef	d_getxfx
	xref.b	c_x
	.dcall	"4,0,d_getxfx"
;
d_getxfx:
	push	a		; save register
	ldw	c_x+1,x		; store address
	ldw	x,#1
	ldf	a,([c_x.e],x)	; get LSB
	ld	xl,a		; in place
	ldf	a,[c_x.e]	; get MSB
	ld	xh,a		; in place
	pop	a		; restore register
	retf			; and return
;
	end
