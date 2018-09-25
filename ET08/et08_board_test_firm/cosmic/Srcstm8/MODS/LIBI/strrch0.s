;	CHARACTER REVERSE SEARCH IN A STRING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_strrchr
	xref.b	c_x, c_y
	.dcall	"2,1,_strrchr"
;
_strrchr:
	clrw	y		; prepare result
	decw	x		; for efficiency
bcl:
	incw	x
	ld	a,(x)		; test nul
	jreq	fin		; nul reached
	cp	a,(3,sp)	; compare character
	jrne	bcl		; not found, continue
	ldw	y,x		; keep pointer
	jra	bcl		; and continue
fin:
	ldw	x,y		; set result
	ret			; and return
;
	end
