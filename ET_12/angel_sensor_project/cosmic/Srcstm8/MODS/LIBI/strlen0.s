;	STRING LENGTH
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_strlen
	xref.b	c_x
	.dcall	"2,0,_strlen"
;
_strlen:
	ldw	c_x,x		; start address
	decw	x		; for efficiency
bcl:
	incw	x		; next byte
	tnz	(x)		; search nul byte
	jrne	bcl		; not found, loop back
	subw	x,c_x		; compute result
	ret			; and return
;
	end
