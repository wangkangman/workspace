;	STRING COMPARE
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_strcmp
	.dcall	"2,2,_strcmp"
;
_strcmp:
	ldw	y,(3,sp)	; load source pointer
bcl:
	ld	a,(x)		; load character
	jreq	fin1		; end reached on destination
	sub	a,(y)		; difference ?
	jrne	fin2		; yes exit
	incw	x		; next address
	incw	y
	jra	bcl		; and loop back
fin1:
	sub	a,(y)		; compute result
fin2:
	clrw	x
	ld	xh,a		; sign extent result
	ret			; and return
;
	end
