;	GET A WORD FROM FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in X and extension
;	- result in X:A
;
	xdef	c_getwfx
	xref.b	c_x
	.dcall	"3,0,c_getwfx"
;
c_getwfx:
	ldw	c_x+1,x		; store address
	ldw	x,#1
	ldf	a,([c_x.e],x)	; get LSB
	push	a		; save
	ldf	a,[c_x.e]	; get MSB
	ld	xl,a		; in place
	pop	a		; restore
	ret			; and return
;
	end
