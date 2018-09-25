;	GET A WORD FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in X and extension
;	- result in X and extension
;
	xdef	c_getxfx
	xref.b	c_x
	.dcall	"3,0,c_getxfx"
;
c_getxfx:
	push	a		; save register
	ldw	c_x+1,x		; store address
	ldw	x,#1
	ldf	a,([c_x.e],x)	; get LSB
	ld	xl,a		; in place
	ldf	a,[c_x.e]	; get MSB
	ld	xh,a		; in place
	pop	a		; restore register
	ret			; and return
;
	end
