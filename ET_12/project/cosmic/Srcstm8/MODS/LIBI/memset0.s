;	CHARACTER FILL BUFFER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_memset
	xref.b	c_x
	.dcall	"4,3,_memset"
;
_memset:
	ldw	y,(4,sp)	; load count
	jreq	fin		; nul, exit
	ld	a,(3,sp)	; fill character
	pushw	x		; keep for result
bcl:
	ld	(x),a		; store character
	incw	x		; next location
	decw	y		; count down
	jrne	bcl		; and loop back
	popw	x		; set return value
fin:
	ret			; and return
;
	end
