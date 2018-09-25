;	STRING LENGTH
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_strlen
	xref.b	c_x
	.dcall	"3,0,f_strlen"
;
f_strlen:
	ldw	c_x,x		; start pointer
	decw	x		; for efficiency
bcl:
	incw	x		; next byte
	tnz	(x)		; search nul byte
	jrne	bcl		; not found, loop back
	subw	x,c_x		; compute result
	retf			; and return
;
	end
