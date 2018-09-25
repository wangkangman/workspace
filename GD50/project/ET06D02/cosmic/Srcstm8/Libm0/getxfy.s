;	GET A WORD FROM EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- result in X
;
	xdef	c_getxfy
	xref.b	c_y
	.dcall	"3,0,c_getxfy"
;
c_getxfy:
	push	a		; save register
	ldw	c_y+1,y		; store address
	ldw	x,#1
	ldf	a,([c_y.e],x)	; get LSB
	ld	xl,a		; in place
	ldf	a,[c_y.e]	; get MSB
	ld	xh,a		; in place
	pop	a		; restore register
	ret			; and return
;
	end
