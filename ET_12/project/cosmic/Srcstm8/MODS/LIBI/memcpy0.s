;	BUFFER COPY
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_memcpy
	xref.b	c_x
	.dcall	"4,4,_memcpy"
;
_memcpy:
	ldw	y,(5,sp)	; load count
	jreq	fin		; nul, exit
	ldw	c_x,y		; save count
	inc	c_x		; for count down
	ldw	y,(3,sp)	; set src pointer
	pushw	x		; save pointer
bcl:
	ld	a,(y)		; load character
	ld	(x),a		; store character
	incw	y		; next
	incw	x		; address
	dec	c_x+1		; count down
	jrne	bcl		; and loop back
	dec	c_x
	jrne	bcl
	popw	x		; set return value
fin:
	ret			; and return
;
	end
