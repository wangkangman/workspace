;	LONG ADDITION WITH BYTE
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg in A
;
	xdef	d_ladc
	xref.b	c_lreg
	.dcall	"3,0,d_ladc"
;
d_ladc:
	add	a,c_lreg+3	; add and
	ld	c_lreg+3,a	; store result
	jrnc	ok		; no carry, exit
	inc	c_lreg+2	; propagate carry
	jrne	ok		; not zero, exit
	inc	c_lreg+1	; propagate carry
	jrne	ok		; not zero, exit
	inc	c_lreg		; propagate carry
ok:
	retf			; and return
;
	end
