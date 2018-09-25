;	CHARACTER SEARCH IN A STRING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_strchr
	xref.b	c_x
	.dcall	"3,1,f_strchr"
;
f_strchr:
bcl:
	ld	a,(x)		; test nul
	jreq	fini		; yes, exit
	cp	a,(4,sp)	; compare character
	jreq	fin		; found, exit
	incw	x		; next byte
	jra	bcl
fini:
	clrw	x		; complete result
fin:
	retf			; and return
;
	end
