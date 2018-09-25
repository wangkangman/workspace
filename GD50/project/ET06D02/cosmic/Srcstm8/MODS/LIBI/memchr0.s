;	CHARACTER SEARCH IN A BUFFER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_memchr
	xref.b	c_x, c_y
	.dcall	"3,3,_memchr"
;
_memchr:
	ldw	y,(4,sp)	; count
	jreq	fin		; nul, exit
okc:
	ld	a,(3,sp)	; searched character
bcl:
	cp	a,(x)		; compare
	jreq	fnd		; equal, exit
	incw	x		; next 
	decw	y		; decrement count
	jrne	bcl		; and loop back
fin:
	clrw	x		; clear result
fnd:
	ret			; and return
;
	end
