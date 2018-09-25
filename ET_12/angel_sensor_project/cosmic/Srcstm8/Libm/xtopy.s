;	STORE FAR POINTER IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- source in X and extension
;	- result pointed by Y and pointer extension
;
	xdef	d_xtopy
	xref.b	c_x, c_y
	.dcall	"4,0,d_xtopy"
;
d_xtopy:
	ldw	c_y+1,y		; set address LSB
	push	a		; save register
	clrw	y
	ld	a,c_x		; copy value
	ldf	([c_y.e],y),a
	incw	y		; next byte
	ld	a,xh
	ldf	([c_y.e],y),a
	incw	y		; next byte
	ld	a,xl
	ldf	([c_y.e],y),a
	pop	a		; restore registers
	retf			; and return
;
	end
