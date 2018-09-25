;	STRING CATENATE
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_strcat
	.dcall	"4,2,_strcat"
;
_strcat:
	pushw	x		; keep for result
bcls:
	ld	a,(x)		; look for a nul
	jreq	oks		; yes, skip
	incw	x		; next byte
	jra	bcls		; and loop back
oks:
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
