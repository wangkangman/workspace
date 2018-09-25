;	STORE FAR POINTER IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- source in Y and extension
;	- result pointed by X and pointer extension
;
	xdef	c_ytopx
	xref.b	c_x, c_y
	.dcall	"3,0,c_ytopx"
;
c_ytopx:
	ldw	c_x+1,x		; set address LSW
	push	a		; save register
	clrw	x
	ld	a,c_y		; copy value
	ldf	([c_x.e],x),a
	incw	x		; next byte
	ld	a,yh
	ldf	([c_x.e],x),a
	incw	x		; next byte
	ld	a,yl
	ldf	([c_x.e],x),a
	pop	a		; restore registers
	ret			; and return
;
	end
