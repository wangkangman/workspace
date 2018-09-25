;	STRING COPY
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_strcpy
	.dcall	"5,2,f_strcpy"
;
f_strcpy:
	pushw	x		; keep for result
	ldw	y,(6,sp)	; load source pointer
bcl:
	ld	a,(y)		; load character
	incw	y		; next address
	ld	(x),a		; store character
	incw	x		; next byte
	tnz	a
	jrne	bcl		; loop back if not nul
	popw	x		; set return value
	retf			; and return
;
	end
