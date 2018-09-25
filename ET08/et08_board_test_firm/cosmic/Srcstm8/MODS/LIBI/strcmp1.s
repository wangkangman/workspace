;	STRING COMPARE
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_strcmp
	.dcall	"3,2,f_strcmp"
;
f_strcmp:
	ldw	y,(4,sp)	; load source pointer
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
	retf			; and return
;
	end
