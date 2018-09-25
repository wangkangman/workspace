;	CHARACTER SEARCH IN A STRING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_strchr
	.dcall	"2,1,_strchr"
;
_strchr:
bcl:
	ld	a,(x)		; test nul
	jreq	fini		; yes, exit
	cp	a,(3,sp)	; compare character
	jreq	fin		; found, exit
	incw	x		; next byte
	jra	bcl
fini:
	clrw	x		; complete result
fin:
	ret			; and return
;
	end
