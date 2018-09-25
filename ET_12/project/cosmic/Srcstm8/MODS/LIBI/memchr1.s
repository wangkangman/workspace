;	CHARACTER SEARCH IN A BUFFER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_memchr
	xref.b	c_x, c_y
	.dcall	"3,3,f_memchr"
;
f_memchr:
	ldw	y,(5,sp)	; count
	jreq	fin		; nul, exit
okc:
	ld	a,(4,sp)	; searched character
bcl:
	cp	a,(x)		; compare
	jreq	fnd		; equal, exit
	incw	x		; next 
	decw	y		; decrement count
	jrne	bcl
fin:
	clrw	x		; clear result
fnd:
	retf			; and return
;
	end
