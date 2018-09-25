;	CHARACTER REVERSE SEARCH IN A STRING
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_strrchr
	.dcall	"3,1,f_strrchr"
;
f_strrchr:
	clrw	y		; prepare result
	decw	x
bcl:
	incw	x
	ld	a,(x)		; test nul
	jreq	fin		; nul reached
	cp	a,(4,sp)	; compare character
	jrne	bcl		; not found, continue
	ldw	y,x		; keep pointer
	jra	bcl		; and continue
fin:
	ldw	x,y		; set result
	retf			; and return
;
	end
