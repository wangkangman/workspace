;	CHARACTER FILL BUFFER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_memset
	.dcall	"5,3,f_memset"
;
f_memset:
	ldw	y,(5,sp)	; load count
	jreq	fin		; nul, exit
	ld	a,(4,sp)	; fill character
	pushw	x		; keep for result
bcl:
	ld	(x),a		; store character
	incw	x		; next location
	decw	y		; count down
	jrne	bcl		; and loop back
	popw	x		; set return value
fin:
	retf			; and return
;
	end
