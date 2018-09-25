;	GET A WORD FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in X and extension
;	- result in Y
;
	xdef	c_getyfx
	xref.b	c_x
	.dcall	"3,0,c_getyfx"
;
c_getyfx:
	push	a		; save register
	ldw	c_x+1,x		; store address
	ldw	y,#1
	ldf	a,([c_x.e],y)	; get LSB
	ld	yl,a		; in place
	ldf	a,[c_x.e]	; get MSB
	ld	yh,a		; in place
	pop	a		; save register
	ret			; and return
;
	end
