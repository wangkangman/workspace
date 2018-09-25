;	STRING COPY
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_strcpy
	.dcall	"4,2,_strcpy"
;
_strcpy:
	pushw	x		; keep for result
	ldw	y,(5,sp)	; load source pointer
bcl:
	ld	a,(y)		; load character
	incw	y		; next address
	ld	(x),a		; store character
	incw	x		; next byte
	tnz	a
	jrne	bcl		; loop back if not nul
	popw	x		; set return value
	ret			; and return
;
	end
